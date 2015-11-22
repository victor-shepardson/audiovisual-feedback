#include "ofApp.h"
#include "math.h"

//always read the current state from ping
//draw to pong when using begin() and end(), then automatically swap with ping
//note that end() must be called after each self-draw for this to work
//if using blending, draw in place to ping using beginInPlace() and endInPlace()
//might ultimately be cleaner to just avoid blending, emulating with shaders
ofxPingPongFbo::ofxPingPongFbo(){
    ping = new ofFbo();
    pong = new ofFbo();
    aux = new ofFbo();
}
ofxPingPongFbo ofxPingPongFbo::copy(){
    ofxPingPongFbo daughter;
    daughter.allocate(settings);
    return daughter;
}
void ofxPingPongFbo::destroy(){
    delete ping;
    delete pong;
    delete aux;
}
void ofxPingPongFbo::allocate(ofFbo::Settings s){
    ping->allocate(s);
    pong->allocate(s);
    aux->allocate(s);
    settings = s;
}
void ofxPingPongFbo::save(){
    aux->begin();
    ping->draw(0,0);
    aux->end();
}
void ofxPingPongFbo::restore(){
    ping = aux;
}
void ofxPingPongFbo::draw(int x,int y,int w,int h){
    ping->draw(x,y,w,h);
}
ofTexture& ofxPingPongFbo::getTextureReference(){
    return ping->getTextureReference();
}
ofTexture& ofxPingPongFbo::getTextureReference(int i){
    return ping->getTextureReference(i);
}
int ofxPingPongFbo::getWidth(){
    return settings.width;
}
int ofxPingPongFbo::getHeight(){
    return settings.height;
}
void ofxPingPongFbo::begin(){
    pong->begin();
}
void ofxPingPongFbo::end(){
    pong->end();
    ofFbo *temp = ping;
    ping = pong;
    pong = temp;
}
void ofxPingPongFbo::beginInPlace(){
    ping->begin();
}
void ofxPingPongFbo::endInPlace(){
    ping->end();
}
void ofxPingPongFbo::readToPixels(ofPixels & pixels, int attachmentPoint = 0) const{
    ping->readToPixels(pixels, attachmentPoint);
}
void ofxPingPongFbo::readToPixels(ofFloatPixels & pixels, int attachmentPoint = 0) const{
    ping->readToPixels(pixels, attachmentPoint);
}
int ofxPingPongFbo::getNumTextures(){
    return settings.numColorbuffers;
}
// !!!
// unsure if these will work with beginInPlace() and endInPlace()
void ofxPingPongFbo::setActiveDrawBuffer(int i){
    ping->setActiveDrawBuffer(i);
    pong->setActiveDrawBuffer(i);
}
void ofxPingPongFbo::setActiveDrawBuffers(const vector<int>& i){
    ping->setActiveDrawBuffers(i);
    pong->setActiveDrawBuffers(i);
}
void ofxPingPongFbo::activateAllDrawBuffers(){
    ping->activateAllDrawBuffers();
    pong->activateAllDrawBuffers();
}

void ofApp::setup(){
    params.setName("params");
	params.add(target_sat.set("target_sat",.7));
	params.add(target_mean.set("target_mean",-.5));
	params.add(target_mix.set("target_mix",.1,0,1));
	params.add(time_scale.set("time_scale",.2));
	params.add(rot.set("rot",.66));
	params.add(blur_size.set("blur_size",2));
	params.add(kernel_width.set("kernel_width",3));
	params.add(bound_clip.set("bound_clip",1.5));
	params.add(seed.set("seed",0));
	params.add(warp.set("warp",0));
	params.add(zoom.set("zoom",0));
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

    mute = true;

    realtime = true;
    use_camera = false;

    oversample_waveform = 2;
    undersample_terrain = 2;

    float oversample_all = 1.;
    fullscreen = false;
    if(fullscreen){
        window_width = ofGetScreenWidth();
        window_height = ofGetScreenHeight();
    }
    else{
        window_width = 1920/4;
        window_height = 1080/4;
    }

    realtime_width = window_width*oversample_all;
    realtime_height = window_height*oversample_all;

    render_width = 1920*oversample_all;
    render_height = 1080*oversample_all;

    ofSetFullscreen(fullscreen);
    ofSetWindowShape(window_width, window_height);

    ofEnableDataPath();

    ofDisableArbTex();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableDepthTest();
    ofDisableAntiAliasing();
    ofSetTextureWrap(GL_REPEAT, GL_REPEAT);

    shader_resample.load(ofToDataPath("../../src/shader/resample"));
    shader_blur.load(ofToDataPath("../../src/shader/blur"));
    shader_rkupdate.load(ofToDataPath("../../src/shader/rkupdate"));
    shader_grad.load(ofToDataPath("../../src/shader/grad"));
    shader_display.load(ofToDataPath("../../src/shader/display"));
    //shader_rkderivative.load(ofToDataPath("../../src/shader/rkderivative"));
    shader_scale_derivative.load(ofToDataPath("../../src/shader/scale_derivative"));
    shader_post_derivative.load(ofToDataPath("../../src/shader/post_derivative"));
    //shader_test.load(ofToDataPath("../../src/shader/test"));

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
    disp_scale = 0;
    disp_buf = 0;

    num_scales = 4;
    scale_factor = 3;

    allocateFbos();

    fill(agent_fbo, ofFloatColor(0,0,0,1));
    initRandom(y_fbo, 0);

    channels = 2;
    frame_rate = 24;
    sample_rate = 48000;
    audio_delay = .5;
    int frames_to_keep = 100;

    vwt = new ofxVideoWaveTerrain(frames_to_keep, sample_rate, audio_delay);

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
    ofSoundStreamClose();
    if(audio_file_size>0)
        endRenderMode();
}

void ofApp::allocateFbos(){
    for(int i=0; i<4; i++){
        ofxPingPongFbo fbo;
        fbo.allocate(fbo_params);
        k_fbos.push_back(fbo);
    }

    ofFbo::Settings y_pyramid_params = fbo_params;
    y_pyramid_params.numColorbuffers = 3; //store gradients
    for(int i=0; i<num_scales; i++){
        ofxPingPongFbo fbo;
        fbo.allocate(y_pyramid_params);
        y_pyramid.push_back(fbo);
        y_pyramid_params.width/=scale_factor;
        y_pyramid_params.height/=scale_factor;
    }

    ofFbo::Settings yprime_pyramid_params = fbo_params;
    for(int i=0; i<num_scales; i++){
        ofxPingPongFbo fbo;
        fbo.allocate(yprime_pyramid_params  );
        yprime_pyramid.push_back(fbo);
        yprime_pyramid_params.width/=scale_factor;
        yprime_pyramid_params.height/=scale_factor;
    }

    y_fbo = ofxPingPongFbo();
    y_fbo.allocate(fbo_params);
    display_fbo = ofxPingPongFbo();
    display_fbo.allocate(fbo_params);
    scratch_fbo = ofxPingPongFbo();
    scratch_fbo.allocate(fbo_params);
    render_fbo = ofxPingPongFbo();
    render_fbo.allocate(fbo_params);

    ofFbo::Settings agent_fbo_params = fbo_params;
    agent_fbo_params.width *= oversample_waveform;
    agent_fbo_params.height *= oversample_waveform;
    agent_fbo = ofxPingPongFbo();
    agent_fbo.allocate(agent_fbo_params);

    ofFbo::Settings readback_fbo_params = fbo_params;
    readback_fbo_params.width /= undersample_terrain;
    readback_fbo_params.height /= undersample_terrain;
    readback_fbo = ofxPingPongFbo();
    readback_fbo.allocate(readback_fbo_params);
}

void ofApp::setResolution(int x, int y){

    ofxPingPongFbo temp_y_fbo = y_fbo;
    ofxPingPongFbo temp_agent_fbo = agent_fbo;

    for(int i=0; i<k_fbos.size(); i++)
        k_fbos[i].destroy();
    k_fbos.clear();
    for(int i=0; i<y_pyramid.size(); i++)
        y_pyramid[i].destroy();
    y_pyramid.clear();
    for(int i=0; i<yprime_pyramid.size(); i++)
        yprime_pyramid[i].destroy();
    yprime_pyramid.clear();

    scratch_fbo.destroy();
    display_fbo.destroy();
    readback_fbo.destroy();

    fbo_params.width = x;
    fbo_params.height = y;

    allocateFbos();

    resample(temp_y_fbo, y_fbo);
    temp_y_fbo.destroy();

    resample(temp_agent_fbo, agent_fbo);
    temp_agent_fbo.destroy();
}
void ofApp::resample(ofxPingPongFbo &src, ofxPingPongFbo &dest){
    //resample src to dest with truncated gaussian kernel
    float sf = float(src.getWidth())/dest.getWidth();
    if(sf>1){ //downsample
        src.save();
        blur(src, src, 2.*sf);
        mov(src, dest);
        src.restore();
    }
    else{ //upsample
 /*       mov(src, dest);
        blur(dest, dest, 2./sf);
    }
*/
        shader_resample.begin();
        shader_resample.setUniformTexture("src0", src.getTextureReference(0), 0);
        shader_resample.setUniform2i("dest_size", dest.getWidth(), dest.getHeight());
        shader_resample.setUniform1i("num_textures", 1);
        dest.begin();
        ofRect(0,0,dest.getWidth(), dest.getHeight());
        dest.end();
        shader_resample.end();
    }
}
void ofApp::fill(ofxPingPongFbo &dest, ofFloatColor c){
    ofPushStyle();
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(c);
    dest.beginInPlace();
    ofRect(0, 0, dest.getWidth(), dest.getHeight());
    dest.endInPlace();
    ofPopStyle();
}
void ofApp::blur(ofxPingPongFbo &src, ofxPingPongFbo &dest, float radius){
    int w = dest.getWidth(), h = dest.getHeight();
    shader_blur.begin();
    shader_blur.setUniformTexture("state", src.getTextureReference(),0);
    shader_blur.setUniform2i("size", w, h);
    shader_blur.setUniform2f("dir",0,radius);
    dest.begin();
    ofRect(0, 0, w, h);
    dest.end();
    shader_blur.setUniformTexture("state", dest.getTextureReference(),0);
    shader_blur.setUniform2f("dir",radius,0);
    dest.begin();
    ofRect(0, 0, w, h);
    dest.end();
    shader_blur.end();
}
void ofApp::sub(ofxPingPongFbo &pos, ofxPingPongFbo& neg, ofxPingPongFbo &dest){
    ofPushStyle();
    if(&dest!=&pos)
        mov(pos,dest);
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    dest.beginInPlace();
    neg.draw(0,0,dest.getWidth(), dest.getHeight());
    dest.endInPlace();
    ofPopStyle();
}
//mov is style-agnostic
void ofApp::mov(ofxPingPongFbo &src, ofxPingPongFbo &dest){
    dest.beginInPlace();
    src.draw(0,0,dest.getWidth(), dest.getHeight());
    dest.endInPlace();
}
void ofApp::blend(ofxPingPongFbo &src, ofxPingPongFbo &dest, ofBlendMode mode){
    ofPushStyle();
    ofEnableBlendMode(mode);
    mov(src, dest);
    ofPopStyle();
}
void ofApp::gradients(ofxPingPongFbo &src){
    //render x and y color gradients of texture 0 into textures 1 and 2 of the fbo
    shader_grad.begin();
    shader_grad.setUniformTexture("state", src.getTextureReference(0),0);
    shader_grad.setUniform2i("size", src.getWidth(),src.getHeight());
    src.beginInPlace();
    vector<int> grad_bufs(2); grad_bufs[0] = 1; grad_bufs[1] = 2;
    src.setActiveDrawBuffers(grad_bufs);
    ofRect(0,0,src.getWidth(), src.getHeight());
    src.setActiveDrawBuffer(0); //clean up before calling end; active draw buffer assumed to be 0
    src.endInPlace();
    shader_grad.end();
}

void ofApp::derivativeAtScale(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime, float scale){
    int w = y.getWidth(), h = y.getHeight();
    shader_scale_derivative.begin();
    shader_scale_derivative.setUniform1f("t",t);
    shader_scale_derivative.setUniformTexture("y", y.getTextureReference(0),0);
    shader_scale_derivative.setUniformTexture("xgrad", y.getTextureReference(1),1);
    shader_scale_derivative.setUniformTexture("ygrad", y.getTextureReference(2),2);
    shader_scale_derivative.setUniform2i("size", w, h);
    shader_scale_derivative.setUniform1f("scale", scale);
    shader_scale_derivative.setUniform1f("warp", warp);
    shader_scale_derivative.setUniform1f("zoom", zoom);
    yprime.begin();
    ofRect(0, 0, w, h);
    yprime.end();
    shader_scale_derivative.end();
}
void ofApp::derivativePost(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime, ofxPingPongFbo &new_yprime){
    int w = y.getWidth(), h = y.getHeight();
    shader_post_derivative.begin();
    shader_post_derivative.setUniform1f("t",t);
    shader_post_derivative.setUniformTexture("y", y.getTextureReference(),0);
    shader_post_derivative.setUniformTexture("yprime", yprime.getTextureReference(),1);
    shader_post_derivative.setUniformTexture("agents", agent_fbo.getTextureReference(),2);
    shader_post_derivative.setUniform2i("size", w, h);
    shader_post_derivative.setUniform1f("time_scale",time_scale);
    shader_post_derivative.setUniform1f("rot",rot);
    shader_post_derivative.setUniform1f("bound_clip",bound_clip);
    shader_post_derivative.setUniform1f("num_scales", num_scales);
    shader_post_derivative.setUniform1f("target_sat",target_sat);
    shader_post_derivative.setUniform1f("target_mean",target_mean);
    shader_post_derivative.setUniform1f("target_mix",target_mix);
    new_yprime.begin();
    ofRect(0,0,w,h);
    new_yprime.end();
    shader_post_derivative.end();
}

//get next y from y,dt,k0,k1,k2,k3 and store in new_y
void ofApp::rkUpdate(float dt, ofxPingPongFbo &y, vector<ofxPingPongFbo> &k, ofxPingPongFbo &new_y){
    int w = y.getWidth();
    int h = y.getHeight();
    shader_rkupdate.begin();
    shader_rkupdate.setUniform1i("mode", 1);
    shader_rkupdate.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkupdate.setUniformTexture("k0", k[0].getTextureReference(),1);
    shader_rkupdate.setUniformTexture("k1", k[1].getTextureReference(),2);
    shader_rkupdate.setUniformTexture("k2", k[2].getTextureReference(),3);
    shader_rkupdate.setUniformTexture("k3", k[3].getTextureReference(),4);
    shader_rkupdate.setUniform1f("dt", dt);
    new_y.begin();
    ofRect(0, 0, w, h);
    new_y.end();
    shader_rkupdate.end();
}

//get next y from y,dt,k and store in new_y
void ofApp::rkUpdate(float dt, ofxPingPongFbo &y, ofxPingPongFbo &k, ofxPingPongFbo &new_y){
    int w = y.getWidth();
    int h = y.getHeight();
    shader_rkupdate.begin();
    shader_rkupdate.setUniform1i("mode", 0);
    shader_rkupdate.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkupdate.setUniformTexture("k0", k.getTextureReference(),1);
    shader_rkupdate.setUniform1f("dt", dt);
    new_y.begin();
    ofRect(0, 0, w, h);
    new_y.end();
    shader_rkupdate.end();
}

//the meat: compute y' as f(t, y) and store in yprime
void ofApp::rkDerivative(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime){
    int w = y.getWidth();
    int h = y.getHeight();

    mov(y, y_pyramid[0]);

    //compute pyramid + derivatives of y and accumulate to yprime
    for(int i=0; i<y_pyramid.size()-1; i++){
        blur(y_pyramid[i], yprime_pyramid[i], 2.*scale_factor); //using yprime_pyramid[i] as scratch
        sub(y_pyramid[i], yprime_pyramid[i], y_pyramid[i]);
        mov(yprime_pyramid[i], y_pyramid[i+1]);
    }
   for(int i=0; i<y_pyramid.size(); i++){
        float scale = pow(scale_factor,i);
        gradients(y_pyramid[i]);
        derivativeAtScale(t, y_pyramid[i], yprime_pyramid[i], scale);
        if(!i){
            mov(yprime_pyramid[i], yprime);
        }
        else{
            resample(yprime_pyramid[i], y_pyramid[0]); //using y_pyramid[0] as scratch
            blend(y_pyramid[0], yprime, OF_BLENDMODE_ADD);
        }
    }

    //further (local) processing of derivative
    derivativePost(t, y, yprime, yprime);
}

//compute f(t+dt, y+dt*k) and store in yprime
void ofApp::rkStep(float t, float dt, ofxPingPongFbo &y, ofxPingPongFbo &k, ofxPingPongFbo &yprime){
    if(dt>0){
        rkUpdate(dt, y, k, scratch_fbo); //using scratch_fbo to store y+k*dt
        rkDerivative(t+dt, scratch_fbo, yprime);
    }
    else{
        rkDerivative(t, y, yprime);
    }
}
//use runge-kutta algorithm to approximate y(t+dt). k must contain at least 4 scratch fbos
void ofApp::rungeKutta(float t, float dt, ofxPingPongFbo &y, vector<ofxPingPongFbo> &k){
    rkStep(t, 0, y, k[0], k[0]); //first k[0] is a dummy argument
    rkStep(t, .5*dt, y, k[0], k[1]);
    rkStep(t, .5*dt, y, k[1], k[2]);
    rkStep(t, dt, y, k[2], k[3]);
    rkUpdate(dt,y,k,y); //in-place works because the update operation is completely parallel
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

    int w = y_fbo.getWidth();
    int h = y_fbo.getHeight();

    vwt->setAgentRate(agent_rate);
    vwt->setMomentumTime(momentum_time);
    vwt->setPathJitter(path_jitter);
    vwt->getVideoVolume()->setAspectRatio(double(w)/h);

    //draw agent path
    int aw = agent_fbo.getWidth(), ah = agent_fbo.getHeight();
    float alpha = 1 - pow(2, -1./(fade_time*cur_frame_rate));
    fill(agent_fbo, ofFloatColor(0,0,0,alpha));
    agent_fbo.beginInPlace();
    vwt->draw(0, 0, aw, ah);
    agent_fbo.endInPlace();
    blur(agent_fbo, agent_fbo, path_blur);

    rungeKutta(frame, 1, y_fbo, k_fbos);

    display_fbo.begin();
    shader_display.begin();
    shader_display.setUniform2i("size", display_fbo.getWidth(), display_fbo.getHeight());
    shader_display.setUniformTexture("state", y_fbo.getTextureReference(),0);
    ofRect(0, 0, display_fbo.getWidth(), display_fbo.getHeight());
    shader_display.end();
    display_fbo.end();

    //read pixels back from video memory and put a new frame in the VWT
    mov(display_fbo, readback_fbo);
    ofFloatPixels pix;
    readback_fbo.readToPixels(pix,0);
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
        //display_fbo.draw(0,0,render_fbo->getWidth(), render_fbo->getHeight());
        //render_fbo->end();
        ofImage img;
        img.allocate(display_fbo.getWidth(), display_fbo.getHeight(), OF_IMAGE_COLOR);
        display_fbo.readToPixels(img.getPixelsRef());
        stringstream feedback_fname;
        feedback_fname<<cur_save_dir.path()<<ofToDataPath("/feedback-")<<frame<<".png";
        img.saveImage(feedback_fname.str());

        /*render_fbo->begin();
        agent_fbo.draw(0,0,render_fbo->getWidth(), render_fbo->getHeight());
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

    int ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    switch(disp_mode){
        case 0:
            display_fbo.draw(0,0,ww,wh);
            break;
        case 1:
            agent_fbo.draw(0,0,ww,wh);
            break;
        case 2:
            if(use_camera) camera.draw(0,0,ww,wh);
            else{
                if(!disp_scale)
                    mov(yprime_pyramid[disp_scale], display_fbo);
                else
                    resample(yprime_pyramid[disp_scale], display_fbo);
                shader_display.begin();
                shader_display.setUniform2i("size",ww,wh);
                shader_display.setUniformTexture("state", display_fbo.getTextureReference(),0);
                ofRect(0,0,ww,wh);
                shader_display.end();
            }
            break;
        case 3:
            shader_display.begin();
            shader_display.setUniform2i("size",ww,wh);
            shader_display.setUniformTexture("state", k_fbos[0].getTextureReference(),0);
            ofRect(0,0,ww,wh);
            shader_display.end();
            break;
        case 4:
            shader_display.begin();
            shader_display.setUniform2i("size",ww,wh);
            shader_display.setUniformTexture("state", y_pyramid[disp_scale].getTextureReference(1),0);
            ofRect(0,0,ww,wh);
            shader_display.end();
            break;
        case 5:
            shader_display.begin();
            shader_display.setUniform2i("size",ww,wh);
            shader_display.setUniformTexture("state", y_pyramid[disp_scale].getTextureReference(2),0);
            ofRect(0,0,ww,wh);
            shader_display.end();
            break;
    }

    frame++;
    drawing = true;
}

void ofApp::initRandom(ofxPingPongFbo &target, int seed){
    printf("init random %d\n", seed);
    ofSeedRandom(seed);
    ofFloatPixels newState;
    int w = target.getWidth();
    int h = target.getHeight();
    newState.allocate(w, h, OF_IMAGE_COLOR);
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            float r = ofSignedNoise(x,y,frame);
            float g = ofSignedNoise(x+11111,y+11111,frame);
            float b = ofSignedNoise(x+37283,y+37283,frame);
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
    if(key=='a'){
        mute = !mute;
    }
/*    if(key=='b'){
        disp_buf = ofWrap(disp_buf+1, 0, 3);
        cout<<"display buffer: "<<disp_buf<<endl;
    }*/
    if(key=='d'){
        disp_mode = ofWrap(disp_mode+1, 0, 6);
        cout<<"display mode: "<<disp_mode<<endl;
    }
    if(key=='f'){
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    if(key=='h'){
        usage();
    }
    if(key=='p'){
        disp_scale = ofWrap(disp_scale+1, 0, num_scales);
        cout<<"display scale: "<<disp_scale<<endl;
    }
    if(key=='s'){
        ofPixels p;
        display_fbo.readToPixels(p);
        ofImage ss = ofImage(p);
        stringstream fname;
        fname<<"ss-"<<ofGetTimestampString()<<".png";
        ss.saveImage(fname.str());
    }
    if(key=='r'){
        initRandom(y_fbo, seed);
        vwt->scramble();
        ofPushStyle();
        ofSetColor(0,0,0);
        agent_fbo.begin();
        ofRect(0,0,agent_fbo.getWidth(),agent_fbo.getHeight());
        agent_fbo.end();
        ofPopStyle();
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
        "a - audio on/off"<<endl<<
       // "b - change display buffer for pyramid"<<endl<<
        "d - change display mode (0=feedback, 1=waveform, 2=derivative"<<endl<<
        "f - toggle fullscreen"<<endl<<
        "h - print this message"<<endl<<
        "m - toggle render mode"<<endl<<
        "n - reinitialize FBOs (for testing purposes)"<<endl<<
        "p - change display scale for pyramid"<<endl<<
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
    if(mute) output=0;
    if(drawing && realtime)
        vwt->audioOut(output, bufferSize, nChannels);
}
