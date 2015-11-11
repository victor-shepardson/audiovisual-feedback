#include "ofApp.h"
#include "math.h"

void ofApp::setup(){
    params.setName("params");
	params.add(target_sat.set("target_sat",.7));
	params.add(target_mean.set("target_mean",-.5));
	params.add(target_mix.set("target_mix",.1));
	params.add(time_scale.set("time_scale",.2));
	params.add(rot.set("rot",.66));
	params.add(blur_size.set("blur_size",2));
	params.add(bound_gauss.set("bound_gauss",3));
	params.add(bound_clip.set("bound_clip",1.5));
	params.add(seed.set("seed",0));
	params.add(warp.set("warp",0));
	params.add(agent_rate.set("agent_rate",30.));
	params.add(momentum_time.set("momentum_time",.00003));
	params.add(path_jitter.set("path_jitter", 0));
	params.add(fade_time.set("fade_time", .5));
	params.add(path_blur.set("path_blur", 2.));
	seed.addListener(this, &ofApp::initParams);

    grad_proj(0,0) = 1;
    grad_proj(1,1) = 1;
    grad_proj(2,2) = 1;
    color_proj(0,0) = 1;
    color_proj(1,1) = 1;
    color_proj(2,2) = -1;

	gui.setup(params);
	sync.setup((ofParameterGroup&)gui.getParameter(),6666,"localhost",6667);

    realtime = true;
    use_camera = false;

    oversample_waveform = 2;
    undersample_terrain = 2;

    float oversample_all = 1;
    fullscreen = false;
    if(fullscreen){
        window_width = ofGetScreenWidth();
        window_height = ofGetScreenHeight();
    }
    else{
        window_width = 960;
        window_height = 540;
    }

    realtime_width = window_width*oversample_all;
    realtime_height = window_height*oversample_all;

    render_width = 1920*oversample_all;
    render_height = 1080*oversample_all;

    ofSetFullscreen(fullscreen);
    ofSetWindowShape(window_width, window_height);

    ofEnableDataPath();
    ofDisableArbTex();
    //ofDisableAlphaBlending();
    ofDisableDepthTest();
    //ofDisableAntiAliasing();
    ofSetTextureWrap(GL_REPEAT, GL_REPEAT);

    shader_rkderivative.load(ofToDataPath("../../src/shader/rkderivative"));
    shader_blur.load(ofToDataPath("../../src/shader/blur"));
    shader_rkupdate.load(ofToDataPath("../../src/shader/rkupdate"));
    shader_grad.load(ofToDataPath("../../src/shader/grad"));
    shader_display.load(ofToDataPath("../../src/shader/display"));
    shader_test.load(ofToDataPath("../../src/shader/test"));

    fbo_params = ofFbo::Settings(); //needs to come after the ofDisable* above
    fbo_params.width = realtime_width;
    fbo_params.height = realtime_height;
    fbo_params.internalformat = GL_RGB32F;
    fbo_params.useDepth = false;
    fbo_params.useStencil = false;
    fbo_params.wrapModeHorizontal = GL_REPEAT;
    fbo_params.wrapModeVertical = GL_REPEAT;
    fbo_params.numSamples = 0;

    frame = 0;

    disp_mode = 0;

    allocateFbos();

    agent_fbo->begin();
    ofClear(0,0,0,1);
    agent_fbo->end();

    initRandom(*y_fbo, 0);

    channels = 2;
    frame_rate = 30;
    sample_rate = 48000;
    audio_delay = .25;
    int frames_to_keep = 100;
    double time_to_keep = 10;

    vwt = new ofxVideoWaveTerrain(frames_to_keep, time_to_keep, sample_rate, audio_delay);

    ofSoundStreamListDevices();
    ofSoundStreamSetup(2, 0, this, sample_rate, 256, 4);

    if(use_camera){
        camera.setVerbose(true);
        camera.listDevices();
        camera.initGrabber(render_width, render_height);
    }
    printf("setup complete\n");
}

void ofApp::close(){
    if(audio_file_size>0)
        endRenderMode();
    ofSoundStreamClose();
}

void ofApp::allocateFbos(){
    for(int i=0; i<4; i++){
        ofFbo fbo;
        fbo.allocate(fbo_params);
        k_fbos.push_back(fbo);
    }
    for(int i=0; i<3; i++){
        ofFbo fbo;
        fbo.allocate(fbo_params);
        aux_fbos.push_back(fbo);
    }
    y_fbo = new ofFbo();
    y_fbo->allocate(fbo_params);
    display_fbo = new ofFbo();
    display_fbo->allocate(fbo_params);
    scratch_fbo = new ofFbo();
    scratch_fbo->allocate(fbo_params);
    render_fbo = new ofFbo();
    render_fbo->allocate(fbo_params);

    ofFbo::Settings agent_fbo_params = fbo_params;
    //agent_fbo_params.numSamples = oversample_waveform;
    agent_fbo_params.width *= oversample_waveform;
    agent_fbo_params.height *= oversample_waveform;
    agent_fbo = new ofFbo();
    agent_fbo->allocate(agent_fbo_params);

    ofFbo::Settings readback_fbo_params = fbo_params;
    readback_fbo_params.width /= undersample_terrain;
    readback_fbo_params.height /= undersample_terrain;
    readback_fbo = new ofFbo();
    readback_fbo->allocate(readback_fbo_params);
}

void ofApp::setResolution(int x, int y){

    ofFbo *temp_y_fbo = y_fbo;
    ofFbo *temp_agent_fbo = agent_fbo;

    k_fbos.clear();
    aux_fbos.clear();

    delete scratch_fbo;
    delete display_fbo;
    delete readback_fbo;

    fbo_params.width = x;
    fbo_params.height = y;

    allocateFbos();

    y_fbo->begin();
    temp_y_fbo->draw(0,0,y_fbo->getWidth(),y_fbo->getHeight());
    y_fbo->end();
    delete temp_y_fbo;

    agent_fbo->begin();
    temp_agent_fbo->draw(0,0,agent_fbo->getWidth(),agent_fbo->getHeight());
    agent_fbo->end();
    delete temp_agent_fbo;
}

//get next y from y,dt,k0,k1,k2,k3 and store in new_y
void ofApp::rkUpdate(float dt, ofFbo &y, vector<ofFbo> &k, ofFbo &new_y){
    int w = y.getWidth();
    int h = y.getHeight();
    new_y.begin();
    shader_rkupdate.begin();
    shader_rkupdate.setUniform1i("mode", 1);
    shader_rkupdate.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkupdate.setUniformTexture("k0", k[0].getTextureReference(),1);
    shader_rkupdate.setUniformTexture("k1", k[1].getTextureReference(),2);
    shader_rkupdate.setUniformTexture("k2", k[2].getTextureReference(),3);
    shader_rkupdate.setUniformTexture("k3", k[3].getTextureReference(),4);
    shader_rkupdate.setUniform1f("dt", dt);
    ofRect(0, 0, w, h);
    shader_rkupdate.end();
    new_y.end();
}

//get next y from y,dt,k and store in new_y
void ofApp::rkUpdate(float dt, ofFbo &y, ofFbo &k, ofFbo &new_y){
    int w = y.getWidth();
    int h = y.getHeight();
    new_y.begin();
    shader_rkupdate.begin();
    shader_rkupdate.setUniform1i("mode", 0);
    shader_rkupdate.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkupdate.setUniformTexture("k0", k.getTextureReference(),1);
    shader_rkupdate.setUniform1f("dt", dt);
    ofRect(0, 0, w, h);
    shader_rkupdate.end();
    new_y.end();
}

//the meat: compute y' as f(t, y) and store in yprime
void ofApp::rkDerivative(float t, ofFbo &y, ofFbo &yprime){
    int w = y.getWidth();
    int h = y.getHeight();

    //convolution pass using aux_fbos[0]
    if(!use_camera){
        aux_fbos[0].begin();
        shader_blur.begin();
        shader_blur.setUniformTexture("state", y.getTextureReference(),0);
        shader_blur.setUniform2i("size", w, h);
        shader_blur.setUniform2f("dir",0,blur_size);
        ofRect(0, 0, w, h);
        shader_blur.setUniformTexture("state", aux_fbos[0].getTextureReference(),0);
        shader_blur.setUniform2f("dir",blur_size,0);
        ofRect(0, 0, w, h);
        shader_blur.end();
        aux_fbos[0].end();
    }

    //horizontal and vertical gradient passes
    aux_fbos[1].begin();
    shader_grad.begin();
    shader_grad.setUniformTexture("state", y.getTextureReference(),0);
    shader_grad.setUniform2f("dir", 1, 0);
    shader_grad.setUniform2i("size",w,h);
    ofRect(0,0,w,h);
    //shader_grad.end();
    aux_fbos[1].end();
    aux_fbos[2].begin();
    //shader_grad.begin();
    //shader_grad.setUniformTexture("state", y.getTextureReference(),0);
    shader_grad.setUniform2f("dir", 0, 1);
    //shader_grad.setUniform2i("size",w,h);
    ofRect(0,0,w,h);
    shader_grad.end();
    aux_fbos[2].end();

    ofTexture * blur_tex;
    if(use_camera)
        blur_tex = &camera.getTextureReference();
    else
        blur_tex = &aux_fbos[0].getTextureReference();

    yprime.begin();
    shader_rkderivative.begin();
    shader_rkderivative.setUniform1f("t",t);
    shader_rkderivative.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkderivative.setUniformTexture("blur", *blur_tex,1);
    shader_rkderivative.setUniformTexture("xgrad", aux_fbos[1].getTextureReference(),2);
    shader_rkderivative.setUniformTexture("ygrad", aux_fbos[2].getTextureReference(),3);
    shader_rkderivative.setUniformTexture("agents", agent_fbo->getTextureReference(),4);
    shader_rkderivative.setUniform2i("size", w, h);
    shader_rkderivative.setUniform1f("target_sat", target_sat);
    shader_rkderivative.setUniform1f("target_mean", target_mean);
    shader_rkderivative.setUniform1f("target_mix", target_mix);
    shader_rkderivative.setUniform1f("time_scale", time_scale);
    shader_rkderivative.setUniform1f("rot", rot);
    shader_rkderivative.setUniform1f("warp", warp);
    shader_rkderivative.setUniform1f("bound_gauss", bound_gauss);
    shader_rkderivative.setUniform1f("bound_clip", bound_clip);
    shader_rkderivative.setUniformMatrix4f("color_proj", color_proj);
    shader_rkderivative.setUniformMatrix4f("grad_proj", grad_proj);
    ofRect(0, 0, w, h);
    shader_rkderivative.end();
    yprime.end();
}

//compute f(t+dt, y+dt*k) and store in yprime
void ofApp::rkStep(float t, float dt, ofFbo &y, ofFbo &k, ofFbo &yprime){
    if(dt>0){
        rkUpdate(dt, y, k, *scratch_fbo); //using scratch_fbo to store y+k*dt
        rkDerivative(t+dt, *scratch_fbo, yprime);
    }
    else{
        rkDerivative(t, y, yprime);
    }
}
//use runge-kutta algorithm to approximate y(t+dt). k must contain at least 4 scratch fbos
void ofApp::rungeKutta(float t, float dt, ofFbo &y, vector<ofFbo> &k){
    rkStep(t, 0, y, k[0], k[0]); //first k[0] is a dummy argument
    rkStep(t, .5*dt, y, k[0], k[1]);
    rkStep(t, .5*dt, y, k[1], k[2]);
    rkStep(t, dt, y, k[2], k[3]);
    rkUpdate(dt, y, k, y); //just compute y in-place for now
}

void ofApp::update(){
    sync.update();
    if(use_camera) camera.update();
    ofSetFrameRate(frame_rate);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::draw(){
    double cur_frame_rate = frame_rate;
    if(realtime) cur_frame_rate = ofGetFrameRate();

    int w = y_fbo->getWidth();
    int h = y_fbo->getHeight();

    vwt->setAgentRate(agent_rate);
    vwt->setMomentumTime(momentum_time);
    vwt->setPathJitter(path_jitter);
    vwt->setAspectRatio(double(w)/h);

    //draw agent path
    int aw = agent_fbo->getWidth(), ah = agent_fbo->getHeight();
    float alpha = 1 - pow(2, -1./(fade_time*cur_frame_rate));
    agent_fbo->begin();
    ofPushStyle();
    ofFill();
    ofSetColor(ofFloatColor(0,0,0,alpha));
    ofRect(0, 0, aw, ah);
    ofPopStyle();
    vwt->draw(0, 0, aw, ah);
    shader_blur.begin();
    shader_blur.setUniformTexture("state", agent_fbo->getTextureReference(),0);
    shader_blur.setUniform2i("size", aw, ah);
    shader_blur.setUniform2f("dir",0,path_blur);
    ofRect(0, 0, aw, ah);
    shader_blur.setUniform2f("dir",path_blur,0);
    ofRect(0, 0, aw, ah);
    shader_blur.end();
    agent_fbo->end();

    rungeKutta(frame, 1, *y_fbo, k_fbos);

    display_fbo->begin();
    shader_display.begin();
    shader_display.setUniformTexture("state", y_fbo->getTextureReference(),0);
    ofRect(0, 0, display_fbo->getWidth(), display_fbo->getHeight());
    shader_display.end();
    display_fbo->end();

    readback_fbo->begin();
    display_fbo->draw(0,0,readback_fbo->getWidth(),readback_fbo->getHeight());
    readback_fbo->end();

    //read pixels back from video memory and put a new frame in the VWT
    ofFloatPixels pix;
    readback_fbo->readToPixels(pix,0);
    vwt->insert_frame(pix);

    //if in render mode, compute audio in the draw loop
    //delay should be equal to frame duration
    if(!realtime){
        int frame_duration_samps = sample_rate/frame_rate;
        int nsamps = frame_duration_samps*channels;
        float * samps = (float*)malloc(nsamps*sizeof(float));
        vwt->audioOut(samps, frame_duration_samps, channels);
        for(int i=0; i<nsamps; i++){
            union{
                int32_t word;
                char bytes[4];
            } v;
            v.word = double(1<<30)*double(samps[i]);
            audio_file.write(v.bytes+1,3);
            audio_file_size+=3;
        }
        free((void*)samps);

        string timestamp = ofGetTimestampString();

        //save frame to disk
        //render_fbo->begin();
        //display_fbo->draw(0,0,render_fbo->getWidth(), render_fbo->getHeight());
        //render_fbo->end();
        ofImage img;
        img.allocate(display_fbo->getWidth(), display_fbo->getHeight(), OF_IMAGE_COLOR);
        display_fbo->readToPixels(img.getPixelsRef());
        stringstream feedback_fname;
        feedback_fname<<cur_save_dir.path()<<ofToDataPath("/feedback-")<<frame<<".png";
        img.saveImage(feedback_fname.str());

        /*render_fbo->begin();
        agent_fbo->draw(0,0,render_fbo->getWidth(), render_fbo->getHeight());
        render_fbo->end();
        render_fbo->readToPixels(pix);
        stringstream waveform_fname;
        waveform_fname<<cur_save_dir.path()<<ofToDataPath("/waveform-")<<frame<<".png";
        ofImage(pix).saveImage(waveform_fname.str());

        if(use_camera){
            render_fbo->begin();
            camera.draw(0,0,render_fbo->getWidth(), render_fbo->getHeight());
            render_fbo->end();
            render_fbo->readToPixels(pix);
            stringstream camera_fname;
            camera_fname<<cur_save_dir.path()<<ofToDataPath("/camera-")<<frame<<".png";
            ofImage(pix).saveImage(camera_fname.str());
        }*/

    }

    switch(disp_mode){
        case 0:
            display_fbo->draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case 1:
            agent_fbo->draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case 2:
            if(use_camera) camera.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
            break;
    }

    frame++;
    drawing = true;
}

void ofApp::initRandom(ofFbo &target, int seed){
    printf("init random %d\n", seed);
    ofSeedRandom(seed);
    ofFloatPixels newState;
    int w = target.getWidth();
    int h = target.getHeight();
    newState.allocate(w, h, OF_IMAGE_COLOR);
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            float r = ofSignedNoise(x+1,y+11,frame,0);
            float g = ofSignedNoise(x+111,y+1111,frame,.5);
            float b = ofSignedNoise(x+11111,y+111111,frame,1);
            newState.setColor(x,y,ofFloatColor(r,g,b));
        }
    }
    target.begin();
    ofImage(newState).draw(0,0,w,h);
    target.end();
}

void ofApp::initParams(int &seed){
    ofSeedRandom(seed);
    for(int i=0; i<3; i++)
    for(int j=0; j<3; j++){
        color_proj(i,j) = 2*ofRandom(-1,1);
        grad_proj(i,j) = 2*ofRandom(-1,1);
    }
}

void ofApp::toggleRenderMode(){
    realtime = !realtime;
    if(realtime)
        endRenderMode();
    else
        beginRenderMode();
}

void ofApp::beginRenderMode(){
    cout<<"render mode"<<endl;

    setResolution(render_width, render_height);

    vwt->setAudioDelay(1./frame_rate + 1./sample_rate);
    //open a new wav file and write header
    audio_file_size = 0;
    stringstream ss;
    cur_save_dir = ofDirectory(ofGetTimestampString());
    cur_save_dir.create();
    ss << cur_save_dir.path()<<ofToDataPath("/audio.wav");
    char write_buf[44];
    audio_file.open(ss.str().c_str(), ofstream::binary);

    strncpy(write_buf,"RIFF",4);
    *((int32_t*)(write_buf+4)) = 0; //dummy size
    strncpy(write_buf+8,"WAVEfmt ",8);
    *((int32_t*)(write_buf+16)) = 16;
    *((int16_t*)(write_buf+20)) = 1; //PCM
    *((int16_t*)(write_buf+22)) = channels;
    *((int32_t*)(write_buf+24)) = sample_rate; //PCM
    *((int32_t*)(write_buf+28)) = sample_rate*channels*3; //byte rate
    *((int16_t*)(write_buf+32)) = channels*3; //block_align
    *((int16_t*)(write_buf+34)) = 24; //bits per sample
    strncpy(write_buf+36,"data",4);
    *((int32_t*)(write_buf+40)) = 0; //dummy size

    audio_file.write(write_buf, 44);
}

void ofApp::endRenderMode(){
    cout<<"realtime mode"<<endl;

    setResolution(realtime_width, realtime_height);

    vwt->setAudioDelay(audio_delay);
    //write size of current wav file and close
    union{
        int32_t word;
        char bytes[4];
    } write_buf;
    audio_file.seekp(4);
    write_buf.word = audio_file_size+36;
    audio_file.write(write_buf.bytes,4);
    audio_file.seekp(40);
    write_buf.word = audio_file_size;
    audio_file.write(write_buf.bytes,4);
    audio_file.close();
    audio_file_size = 0;
}

void ofApp::keyPressed(int key){
    if(key=='d'){
        disp_mode = ofWrap(disp_mode+1, 0, 3);
        cout<<disp_mode<<endl;
    }
    if(key=='f'){
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    if(key=='h'){
        usage();
    }
    if(key=='s'){
        ofPixels p;
        display_fbo->readToPixels(p);
        ofImage ss = ofImage(p);
        stringstream fname;
        fname<<"ss-"<<ofGetTimestampString()<<".png";
        ss.saveImage(fname.str());
    }
    if(key=='r'){
        initRandom(*y_fbo, seed);
    }
    if(key==']'){
        setResolution(fbo_params.width*2, fbo_params.height*2);
        cout<<"new resolution: "<<fbo_params.width<<" by "<<fbo_params.height<<endl;
    }
    if(key=='['){
        if(fbo_params.width>1 && fbo_params.height>1)
            setResolution(fbo_params.width*.5, fbo_params.height*.5);
        cout<<"new resolution: "<<fbo_params.width<<" by "<<fbo_params.height<<endl;
    }
    if(key=='n'){ //for testing purposes
        setResolution(fbo_params.width, fbo_params.height);
        cout<<"call setResolution() with current resolution"<<endl;
    }
    if(key=='m'){
        toggleRenderMode();
    }
}

void ofApp::usage(){
    cout<<
        "d - change display mode (0=feedback, 1=waveform, 2=camera"<<endl<<
        "f - toggle fullscreen"<<endl<<
        "h - print this message"<<endl<<
        "m - toggle render mode"<<endl<<
        "n - reinitialize FBOs (for testing purposes)"<<endl<<
        "r - randomize video"<<endl<<
        "s - save current frame as png"<<endl<<
        "[ - halve resolution"<<endl<<
        "] - double resolution"<<endl;
}

void ofApp::keyReleased(int key){
}
void ofApp::mouseMoved(int x, int y ){
}
void ofApp::mouseDragged(int x, int y, int button){
}
void ofApp::mousePressed(int x, int y, int button){
}
void ofApp::mouseReleased(int x, int y, int button){
}
void ofApp::windowResized(int w, int h){
}
void ofApp::gotMessage(ofMessage msg){
}
void ofApp::dragEvent(ofDragInfo dragInfo){
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    if(drawing && realtime)
        vwt->audioOut(output, bufferSize, nChannels);
}
