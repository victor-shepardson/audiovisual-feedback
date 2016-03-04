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
/*ofxPingPongFbo ofxPingPongFbo::copy(){
    ofxPingPongFbo daughter;
    daughter.allocate(settings);
    return daughter;
}*/
ofxPingPongFbo::ofxPingPongFbo(const ofxPingPongFbo &parent){
    ping = parent.ping;
    pong = parent.pong;
    aux = parent.aux;
    settings = parent.settings;
}
ofxPingPongFbo& ofxPingPongFbo::operator=(const ofxPingPongFbo &parent){
    ping = parent.ping;
    pong = parent.pong;
    aux = parent.aux;
    settings = parent.settings;
    return *this;
}
void ofxPingPongFbo::destroy(){
    delete ping;
    delete pong;
    delete aux;
}
//lazy allocation so that pong+aux won't hog memory when unused
//broken for pong, why?
void ofxPingPongFbo::allocate(ofFbo::Settings s){
    ping->allocate(s);
    pong->allocate(s);
    //aux->allocate(s);
    settings = s;
}
void ofxPingPongFbo::swap(){
    ofFbo *temp = ping;
    ping = pong;
    pong = temp;
}
void ofxPingPongFbo::save(){
    if(!aux->isAllocated()) aux->allocate(settings);
    aux->begin();
    ping->draw(0,0);
    aux->end();
}
void ofxPingPongFbo::restore(){
    ofFbo *temp = ping;
    ping = aux;
    aux = temp;
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
    return ping->getWidth();
}
int ofxPingPongFbo::getHeight(){
    return ping->getHeight();
}
void ofxPingPongFbo::begin(){
    if(!pong->isAllocated()) pong->allocate(settings);
    pong->begin();
}
void ofxPingPongFbo::end(){
    pong->end();
    swap();
}
void ofxPingPongFbo::beginInPlace(){
    ping->begin();
}
void ofxPingPongFbo::endInPlace(){
    ping->end();
}
void ofxPingPongFbo::readToPixels(ofPixels & pixels, int attachmentPoint = 0) {
    reader.readToPixels(*ping, pixels);
    //ping->readToPixels(pixels, attachmentPoint);
}
void ofxPingPongFbo::readToPixels(ofFloatPixels & pixels, int attachmentPoint = 0) {
    reader.readToFloatPixels(*ping, pixels);
    //ping->readToPixels(pixels, attachmentPoint);
}
int ofxPingPongFbo::getNumTextures(){
    return ping->getNumTextures();
}
// !!!
// unsure if these will work with beginInPlace() and endInPlace()
void ofxPingPongFbo::setActiveDrawBuffer(int i){
    ping->setActiveDrawBuffer(i);
    //pong->setActiveDrawBuffer(i);
    //aux->setActiveDrawBuffer(i);
}
void ofxPingPongFbo::setActiveDrawBuffers(const vector<int>& i){
    ping->setActiveDrawBuffers(i);
    //pong->setActiveDrawBuffers(i);
    //aux->setActiveDrawBuffers(i);
}
void ofxPingPongFbo::activateAllDrawBuffers(){
    ping->activateAllDrawBuffers();
    //pong->activateAllDrawBuffers();
    //aux->activateAllDrawBuffers();
}

//====================================================================

template <class T>
ofxDynamicalTexture<T>::ofxDynamicalTexture(T *p, void (T::*f)(float, ofxPingPongFbo&)){
    app = p;
    derivative = f;
    //maybe should test if already loaded, or something better
    shader_rkupdate.load(ofToDataPath("../../src/shader/rkupdate"));
    ticks = tocks = 0;
}

template <class T>
ofShader ofxDynamicalTexture<T>::shader_rkupdate = ofShader();

template <class T>
void ofxDynamicalTexture<T>::allocate(ofFbo::Settings s){
    y.allocate(s);
    ytemp.allocate(s);
    for(int i=0; i<4; i++){
        ofxPingPongFbo fbo;
        fbo.allocate(s);
        k.push_back(fbo);
    }
}
template <class T>
void ofxDynamicalTexture<T>::destroy(){
    y.destroy();
    ytemp.destroy();
    for(int i=0; i<k.size(); i++)
        k[i].destroy();
    k.clear();
}

//return the current working state (so multiple dynamical textures can integrate in sync)
template <class T>
ofxPingPongFbo& ofxDynamicalTexture<T>::getState(){
    return ytemp;
}

//ytemp := y + dt*(k0 + 2*k1 + 2*k2 + k3)/6
template <class T>
void ofxDynamicalTexture<T>::rkUpdate(float dt){
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
    ytemp.beginInPlace();
    ofRect(0, 0, w, h);
    ytemp.endInPlace();
    shader_rkupdate.end();
}

// ytemp := y+k[i]*dt
template <class T>
void ofxDynamicalTexture<T>::rkUpdate(float dt, int i){
    int w = y.getWidth();
    int h = y.getHeight();
    shader_rkupdate.begin();
    shader_rkupdate.setUniform1i("mode", 0);
    shader_rkupdate.setUniformTexture("y", y.getTextureReference(),0);
    shader_rkupdate.setUniformTexture("k0", k[i].getTextureReference(),1);
    shader_rkupdate.setUniform1f("dt", dt);
    ytemp.beginInPlace();
    ofRect(0, 0, w, h);
    ytemp.endInPlace();
    shader_rkupdate.end();
}

// k[i] := f(t+dt, ytemp)
template <class T>
void ofxDynamicalTexture<T>::rkStep(float t, float dt, int i){
    (app->*derivative)(t+dt, k[i]);
}
//use runge-kutta algorithm to approximate y(t+dt). k must contain 4 scratch fbos
template <class T>
void ofxDynamicalTexture<T>::tock(float t, float dt, int i){
    //on each tock, set the derivative
    switch(i){
        case 0:
            rkStep(t, 0, 0);
            break;
        case 1:
            rkStep(t, .5*dt, 1);
            break;
        case 2:
            rkStep(t, .5*dt, 2);
            break;
        case 3:
            rkStep(t, dt, 3);
    }
    tocks++;
}
template <class T>
void ofxDynamicalTexture<T>::tick(float t, float dt, int i){
    switch(i){
        case 0:
            //first tick, set the state at t to the working state
            //this way any changes made to the working state outside of draw() will be kept
            y.beginInPlace();
            ytemp.draw(0,0,y.getWidth(),y.getHeight());
            y.endInPlace();
            break;
            //on each other tick, update the working state
        case 1:
            rkUpdate(.5*dt, 0);
            break;
        case 2:
            rkUpdate(.5*dt, 1);
            break;
        case 3:
            rkUpdate(dt, 2);
    }
    ticks++;
}

//infer from number of ticks/tocks how to update, warn if not valid
template <class T>
void ofxDynamicalTexture<T>::update(float dt){
    if(ticks==1 && tocks==1)
        rkUpdate(dt, 0); //euler
    else if(ticks==4 && tocks==4)
        rkUpdate(dt); //rk4
    else
        cout<<"ofxDynamicalTexture error: attempt to update after "<<ticks<<" ticks and "<<tocks<<" tocks"<<endl;
    ticks=tocks=0;
}

//============================================================

void ofApp::setupConstants()
{
    //OSC
    local_port = 6666;
    remote_host = "localhost";
    remote_port = 6667;

    //constants for multi-scale processing
    num_scales = 6;
    scale_factor = 2;

    //target video refresh rate
    frame_rate = 60;

    //audio constants
    audio_channels = 2;
    audio_sample_rate = 48000;
    audio_delay = .5;
    audio_device = 0;
    audio_buffer_size = 256;

}
void ofApp::setupParameters()
{
 //set up OSC controllable parameters from params.txt (which is in Max's goofy coll format for now)
    params.setName("params");
    string params_text = ofBufferFromFile(ofToDataPath("../../src/params.txt")).getText();
    vector<string> params_lines = ofSplitString(params_text, "\n");
    if(params_lines.size()==1) params_lines = ofSplitString(params_text, "\r");
    for(auto i = params_lines.begin(); i!=params_lines.end(); i++){
        vector<string> param_tokens = ofSplitString(*i, " ");
        if(param_tokens.size()<3) break;
        string param_name = param_tokens[1];
        float param_val = atof(param_tokens[2].c_str());
        params.add(ofParameter<float>().set(param_name, param_val));

        cout<<"added OSC parameter "<<param_name<<" with initial value "<<param_val<<endl;
    }

    //set up parameters which are not floats and/or have callbacks separately for now
	params.add(seed.set("seed",0));
	seed.addListener(this, &ofApp::initParams);

    //I don't really understand the role of ofxGui in managing ofParameters/ofParameterGroups, something to return to
    gui.setup(params);
	sync.setup((ofParameterGroup&)gui.getParameter(),local_port,remote_host,remote_port);
}

void ofApp::setup(){

    setupConstants();

    setupParameters();

    grad_proj(0,0) = 1;
    grad_proj(1,1) = 1;
    grad_proj(2,2) = 1;
    color_proj(0,0) = 1;
    color_proj(1,1) = 1;
    color_proj(2,2) = -1;

    mute = true;

    realtime = true;
    use_camera = false;
    recording = false;

    oversample_waveform = 1;
    int undersample_terrain = 10;

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

    render_width = 1920;//*oversample_all;
    render_height = 1080;//*oversample_all;

    readback_width = 1920/undersample_terrain;
    readback_height = 1080/undersample_terrain;

    ofSetVerticalSync(true); //does not appear to be working on windows w/ of 0.8.4

    ofSetFullscreen(fullscreen);
    ofSetWindowShape(window_width, window_height);

    ofEnableDataPath();

    ofDisableArbTex();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableDepthTest();
    ofDisableAntiAliasing();
    ofSetTextureWrap(GL_REPEAT, GL_REPEAT);

    loadShaders();

    fbo_params = ofFbo::Settings(); //needs to come after the ofDisable* above
    fbo_params.width = realtime_width;
    fbo_params.height = realtime_height;
    fbo_params.internalformat = GL_RGB32F;
    fbo_params.useDepth = false;
    fbo_params.useStencil = false;
    fbo_params.wrapModeHorizontal = GL_REPEAT;
    fbo_params.wrapModeVertical = GL_REPEAT;
    fbo_params.numSamples = 0;

    integrator = 0;

    frame = 0;

    cycle_disp_mode = 0;

    disp_mode = 0;
    disp_scale = 0;
    disp_buf = 0;

    //set the sequence of display modes to cycle through
    display_sequence.push_back(AVFBDM_Color);
    display_sequence.push_back(AVFBDM_Monochrome);
    display_sequence.push_back(AVFBDM_Agents);
    //display_sequence.push_back(AVFBDM_Displacement);
    //display_sequence.push_back(AVFBDM_Filter);


    discard_largest_scale = true;

    allocateFbos();

    fill(agent_fbo, ofFloatColor(0,0,0,1));
    initRandom(dyn->getState(), 0);

    int frames_to_keep = 48;
    vwt = new ofxVideoWaveTerrain(frames_to_keep, audio_sample_rate, audio_delay);

    ofSoundStreamListDevices();
    ss.setDeviceID(audio_device);
    ss.setup(this, audio_channels, 0, audio_sample_rate, audio_buffer_size, 4);

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

void ofApp::loadShaders(){
    cout<<"loading shaders..."<<endl;
    shader_resample.load(ofToDataPath("../../src/shader/resample"));
    shader_blur.load(ofToDataPath("../../src/shader/blur"));
    shader_grad.load(ofToDataPath("../../src/shader/grad"));
    shader_display.load(ofToDataPath("../../src/shader/display"));
    shader_multiscale.load(ofToDataPath("../../src/shader/multiscale"));
    shader_post_derivative.load(ofToDataPath("../../src/shader/post_derivative"));
    //shader_test.load(ofToDataPath("../../src/shader/test"));
    shader_scale_add.load(ofToDataPath("../../src/shader/scale_add"));
    shader_lp_filter.load(ofToDataPath("../../src/shader/lp_filter"));
    shader_warp.load(ofToDataPath("../../src/shader/warp"));
    shader_multi_warp.load(ofToDataPath("../../src/shader/multi_warp"));
    shader_edge_aware.load(ofToDataPath("../../src/shader/edge_aware_filter"));
    shader_torus_shift.load(ofToDataPath("../../src/shader/torus_shift"));
    cout<<"done loading shaders"<<endl;
}

//only happens once, in setup()
void ofApp::allocateFbos(){

    reallocateFbos();

    readback_fbo = ofxPingPongFbo();
    ofFbo::Settings readback_fbo_params = fbo_params;
    readback_fbo_params.width = readback_width;
    readback_fbo_params.height = readback_height;
    readback_fbo.allocate(readback_fbo_params);

    render_fbo = ofxPingPongFbo();
    ofFbo::Settings render_fbo_params = fbo_params;
    render_fbo_params.internalformat = GL_RGB8;
    render_fbo_params.width = render_width;
    render_fbo_params.height = render_height;
    render_fbo.allocate(render_fbo_params);

}

//allocate all size-changing fbos
void ofApp::reallocateFbos(){
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
        fbo.allocate(yprime_pyramid_params);
        yprime_pyramid.push_back(fbo);
        yprime_pyramid_params.width/=scale_factor;
        yprime_pyramid_params.height/=scale_factor;
    }

    dyn = new ofxDynamicalTexture<ofApp>(this, &ofApp::dynDerivative);
    dyn->allocate(fbo_params);

    lp = new ofxDynamicalTexture<ofApp>(this, &ofApp::lpDerivative);
    lp->allocate(fbo_params);

    display_fbo = ofxPingPongFbo();

    display_fbo.allocate(fbo_params);

    agent_fbo = ofxPingPongFbo();
    ofFbo::Settings agent_fbo_params = fbo_params;
    agent_fbo_params.width *= oversample_waveform;
    agent_fbo_params.height *= oversample_waveform;
    agent_fbo_params.numColorbuffers = 3;
    agent_fbo.allocate(agent_fbo_params);

}

//note that render_fbo, readback_fbo do not currently change size
void ofApp::setResolution(int x, int y){
    //for all to-be-resized buffers, either destroy or back up for later destruction
    ofxDynamicalTexture<ofApp> *dyn_old = dyn;
    ofxDynamicalTexture<ofApp> *lp_old = lp;

    ofxPingPongFbo agent_fbo_old = agent_fbo;
    agent_fbo = ofxPingPongFbo();

    for(int i=0; i<y_pyramid.size(); i++)
        y_pyramid[i].destroy();
    y_pyramid.clear();
    for(int i=0; i<yprime_pyramid.size(); i++)
        yprime_pyramid[i].destroy();
    yprime_pyramid.clear();

    display_fbo.destroy();
    //display_fbo = ofxPingPongFbo();

    //allocate buffers at new size
    fbo_params.width = x;
    fbo_params.height = y;
    reallocateFbos();

    //resample all feedback buffers to new sizes, then free resources
    resample(dyn_old->getState(), dyn->getState());
    dyn_old->destroy();
    delete dyn_old;

    resample(lp_old->getState(), lp->getState());
    lp_old->destroy();
    delete lp_old;

    resample(agent_fbo_old, agent_fbo);
    agent_fbo_old.destroy();
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
        /*mov(src, dest);
        blur(dest, dest, 2./sf);
    }*/
        shader_resample.begin();
        setShaderParam(shader_resample, "src0", src.getTextureReference(0), 0);
        setShaderParam(shader_resample, "dest_size", dest.getWidth(), dest.getHeight());
        setShaderParam(shader_resample, "num_textures", 1);
        dest.beginInPlace();
        ofRect(0,0,dest.getWidth(), dest.getHeight());
        dest.endInPlace();
        shader_resample.end();
    }
}
void ofApp::resampleToWindow(ofxPingPongFbo &src){
    int ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    //resample src to dest with truncated gaussian kernel
    float sf = float(src.getWidth())/ww;
    if(sf>1){ //downsample
        src.save();
        blur(src, src, 2.*sf);
        src.draw(0,0,ww,wh);
        src.restore();
    }
    else{ //upsample
 /*       mov(src, dest);
        blur(dest, dest, 2./sf);
    }
*/
        shader_resample.begin();
        shader_resample.setUniformTexture("src0", src.getTextureReference(0), 0);
        shader_resample.setUniform2i("dest_size", ww,wh);
        shader_resample.setUniform1i("num_textures", 1);
        ofRect(0,0, ww,wh);
        shader_resample.end();
    }
}
void ofApp::b2u(ofxPingPongFbo &src, ofxPingPongFbo &dest){
    if(&src!=&dest)
        mov(src, dest);
    fill(dest, ofFloatColor(1,1,1,1), OF_BLENDMODE_ADD);
    fill(dest, ofFloatColor(0,0,0,.5));
}
void ofApp::fill(ofxPingPongFbo &dest, ofFloatColor c, ofBlendMode mode){
    ofPushStyle();
    ofFill();
    ofEnableBlendMode(mode);
    ofSetColor(c);
    dest.beginInPlace();
    ofRect(0, 0, dest.getWidth(), dest.getHeight());
    dest.endInPlace();
    ofPopStyle();
}
void ofApp::blur(ofxPingPongFbo &src, ofxPingPongFbo &dest, float radius){
    if(radius<=0){
        if(&dest != &src)
            mov(src, dest);
        return;
    }
    int w = dest.getWidth(), h = dest.getHeight();
    shader_blur.begin();
    shader_blur.setUniformTexture("state", src.getTextureReference(0),0);
    shader_blur.setUniform2i("size", w, h);
    shader_blur.setUniform2f("dir",0,radius);
    dest.beginInPlace();
    ofRect(0, 0, w, h);
    dest.endInPlace();
    shader_blur.setUniformTexture("state", dest.getTextureReference(0),0);
    shader_blur.setUniform2f("dir",radius,0);
    dest.begin();
    ofRect(0, 0, w, h);
    dest.end();
    shader_blur.end();
}
void ofApp::edge_aware_filter(ofxPingPongFbo &src, ofxPingPongFbo &dest){
    int w = dest.getWidth(), h = dest.getHeight();
    shader_edge_aware.begin();
    shader_edge_aware.setUniformTexture("state", src.getTextureReference(0),0);
    shader_edge_aware.setUniform2i("size", w, h);
    dest.begin();
    ofRect(0, 0, w, h);
    dest.end();
}
void ofApp::sub(ofxPingPongFbo &pos, ofxPingPongFbo& neg, ofxPingPongFbo &dest){
    /*ofPushStyle();
    if(&dest!=&pos)
        mov(pos,dest);
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    dest.beginInPlace();
    neg.draw(0,0,dest.getWidth(), dest.getHeight());
    dest.endInPlace();
    ofPopStyle();*/
    scale_add(1,pos,-1,neg,dest);
}
void ofApp::scale_add(float a, ofxPingPongFbo &x, float b, ofxPingPongFbo &y, ofxPingPongFbo &dest){
    int w = dest.getWidth(), h = dest.getHeight();
    shader_scale_add.begin();
    shader_scale_add.setUniformTexture("x",x.getTextureReference(0),0);
    shader_scale_add.setUniformTexture("y",y.getTextureReference(0),1);
    shader_scale_add.setUniform2i("size", w, h);
    shader_scale_add.setUniform1f("a",a);
    shader_scale_add.setUniform1f("b",b);
    dest.beginInPlace();
    ofRect(0,0,w,h);
    dest.endInPlace();
    shader_scale_add.end();
}

void ofApp::mix(float m, ofxPingPongFbo &x, ofxPingPongFbo &y, ofxPingPongFbo &dest){
    scale_add(1-m, x, m, y, dest);
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
    shader_grad.setUniform2i("size", src.getWidth(), src.getHeight());
    src.beginInPlace();
    vector<int> grad_bufs(2); grad_bufs[0] = 1; grad_bufs[1] = 2;
    src.setActiveDrawBuffers(grad_bufs);
    ofRect(0,0,src.getWidth(), src.getHeight());
    src.setActiveDrawBuffer(0); //clean up before calling end; active draw buffer assumed to be 0 elsewhere
    src.endInPlace();
    shader_grad.end();
}

void ofApp::setShaderParam(ofShader &s, const string& p){
    s.setUniform1f(p, params.getFloat(p));
}
void ofApp::setShaderParam(ofShader &s, const string& p, float v){
    s.setUniform1f(p, v);
}
void ofApp::setShaderParam(ofShader &s, const string& p, float v1, float v2){
    s.setUniform2f(p, v1, v2);
}
void ofApp::setShaderParam(ofShader &s, const string& p, float v1, float v2, float v3){
    s.setUniform3f(p, v1, v2, v3);
}
void ofApp::setShaderParam(ofShader &s, const string& p, float v1, float v2, float v3, float v4){
    s.setUniform4f(p, v1, v2, v3, v4);
}
void ofApp::setShaderParam(ofShader &s, const string& p, ofVec2f &v){
    s.setUniform2f(p, v.x, v.y);
}
void ofApp::setShaderParam(ofShader &s, const string& p, ofVec3f &v){
    s.setUniform3f(p, v.x, v.y, v.z);
}
void ofApp::setShaderParam(ofShader &s, const string& p, ofVec4f &v){
    s.setUniform4f(p, v.x, v.y, v.z, v.w);
}
void ofApp::setShaderParam(ofShader &s, const string& p, int v){
    s.setUniform1i(p, v);
}
void ofApp::setShaderParam(ofShader &s, const string& p, int v1, int v2){
    s.setUniform2i(p, v1, v2);
}
void ofApp::setShaderParam(ofShader &s, const string& p, int v1, int v2, int v3){
    s.setUniform3i(p, v1, v2, v3);
}
void ofApp::setShaderParam(ofShader &s, const string& p, int v1, int v2, int v3, int v4){
    s.setUniform4i(p, v1, v2, v3, v4);
}
void ofApp::setShaderParam(ofShader &s, const string& p, ofTexture& t, int loc){
    s.setUniformTexture(p, t, loc);
}

void ofApp::filtering(float t, ofxPingPongFbo &src, ofxPingPongFbo &dest){
    float blur_post = params.getFloat("blur_post");
    int filter_steps = params.getFloat("filter_steps");

    if(!filter_steps){
            blur(src, dest, blur_post);
    }
    else{
        edge_aware_filter(src, dest);
        for(int i=1; i<filter_steps; i++)
            edge_aware_filter(dest, dest);
        blur(dest, dest, blur_post);
    }
}

void ofApp::multiscaleProcessing(float t, ofxPingPongFbo &src, ofxPingPongFbo &dest){
    int w = src.getWidth();
    int h = src.getHeight();

    float blur_initial = params.getFloat("blur_initial");
    float blur_scale = params.getFloat("blur_scale");
    float lf_bleed = params.getFloat("lf_bleed");

    //sub(y, f, yprime); //yprime as scratch

    blur(src, y_pyramid[0], blur_initial);
   // mov(src, y_pyramid[0]);

    //compute pyramid + derivatives of y and accumulate to yprime
    //float amt_blurred = 0; //keep track of accumulated blur to keep downsampling consistent
    for(int i=0; i<y_pyramid.size()-1; i++){
        //blur(y_pyramid[i], y_pyramid[i], blur_initial);
        blur(y_pyramid[i], yprime_pyramid[i], scale_factor*blur_scale);
        //float blur_amt = blur_scale;//max(0., blur_scale*scale_factor - amt_blurred);
       // blur(y_pyramid[i], yprime_pyramid[i], blur_amt); //using yprime_pyramid[i] as scratch
        //fill(yprime_pyramid[i], ofFloatColor(0,0,0,lf_bleed), OF_BLENDMODE_ALPHA);
        //amt_blurred = (amt_blurred + blur_amt)/scale_factor; //divide by scale_factor since coordinate system gets scaled
        //sub(y_pyramid[i], yprime_pyramid[i], y_pyramid[i]);
        scale_add(1, y_pyramid[i], lf_bleed-1, yprime_pyramid[i], y_pyramid[i]);
        mov(yprime_pyramid[i], y_pyramid[i+1]);
    }
    int scales_to_process = y_pyramid.size();
    if(discard_largest_scale) scales_to_process-=1;
    for(int i=scales_to_process-1; i>=0; i--){
        float scale = pow(scale_factor,i);
        gradients(y_pyramid[i]);
        int mod_idx = i;
        float mod = 0.;
        if(discard_largest_scale){
            mod_idx +=1;
            mod = 1.;
        }
        processingAtScale(t, y_pyramid[i], yprime_pyramid[mod_idx], yprime_pyramid[i], scale, mod);
    }
    //for(int i=0; i<scales_to_process; i++){
    //    if(!i){
//-------            mov(yprime_pyramid[0], dest);
    /*    }
        else{
            resample(yprime_pyramid[i], y_pyramid[0]); //using y_pyramid[0] as scratch
            //float m = 1./(i+1);
            //mix(m, dest, y_pyramid[0], dest);
            blend(y_pyramid[0], dest, OF_BLENDMODE_ADD);
        }
    }*/

    /*shader_warp.begin();
    shader_warp.setUniformTexture("src", src.getTextureReference(), 0);
    shader_warp.setUniformTexture("disp", dest.getTextureReference(), 1);
    shader_warp.setUniform2i("size",w,h);
    dest.begin();
    ofRect(0,0,w,h);
    dest.end();
    shader_warp.end();
    */
    shader_multi_warp.begin();
    shader_multi_warp.setUniformTexture("src", src.getTextureReference(), 0);
    for(int i=0; i<scales_to_process; i++){
        stringstream ss;
        ss<<"disp"<<i;
        setShaderParam(shader_multi_warp, ss.str(), yprime_pyramid[i].getTextureReference(), 1+i);
    }
    setShaderParam(shader_multi_warp, "num_tex", scales_to_process);
    setShaderParam(shader_multi_warp, "size", w,h);

    dest.begin();
    ofRect(0,0,w,h);
    dest.end();
}

void ofApp::processingAtScale(float t, ofxPingPongFbo &y, ofxPingPongFbo &m, ofxPingPongFbo &yprime, float scale, float mod){
    int w = y.getWidth(), h = y.getHeight();
shader_multiscale.begin();
    setShaderParam(shader_multiscale, "t", t);
    setShaderParam(shader_multiscale, "y", y.getTextureReference(0), 0);
    setShaderParam(shader_multiscale, "xgrad", y.getTextureReference(1), 1);
    setShaderParam(shader_multiscale, "ygrad", y.getTextureReference(2), 2);
    setShaderParam(shader_multiscale, "modulation", m.getTextureReference(0), 3);
    setShaderParam(shader_multiscale, "modulate", mod);
    setShaderParam(shader_multiscale, "size", w, h);
    setShaderParam(shader_multiscale, "scale", scale);
    setShaderParam(shader_multiscale, "disp_exponent");
    setShaderParam(shader_multiscale, "warp_color");
    setShaderParam(shader_multiscale, "warp_grad");
    //setShaderParam(shader_multiscale, "color_proj", color_proj);
    //setShaderParam(shader_multiscale, "grad_proj", grad_proj);
    yprime.begin();
    ofRect(0, 0, w, h);
    yprime.end();
    shader_multiscale.end();
}

void ofApp::derivativePost(float t, ofxPingPongFbo &y, ofxPingPongFbo &new_y, ofxPingPongFbo &lp, ofxPingPongFbo &new_yprime){
    int w = y.getWidth(), h = y.getHeight();

    shader_post_derivative.begin();
    setShaderParam(shader_post_derivative, "t", t);
    setShaderParam(shader_post_derivative, "y", y.getTextureReference(0), 0);
    setShaderParam(shader_post_derivative, "new_y", new_y.getTextureReference(0), 1);
    setShaderParam(shader_post_derivative, "lp", lp.getTextureReference(0), 2);
    setShaderParam(shader_post_derivative, "agents", agent_fbo.getTextureReference(0), 3);
    setShaderParam(shader_post_derivative, "agradx", agent_fbo.getTextureReference(1), 4);
    setShaderParam(shader_post_derivative, "agrady", agent_fbo.getTextureReference(2), 5);
    setShaderParam(shader_post_derivative, "size", w, h);
    setShaderParam(shader_post_derivative, "warp_agent");
    setShaderParam(shader_post_derivative, "agent_drive");
    setShaderParam(shader_post_derivative, "drive");
    setShaderParam(shader_post_derivative, "time_scale");
    setShaderParam(shader_post_derivative, "rot");
    setShaderParam(shader_post_derivative, "bound_clip");
    setShaderParam(shader_post_derivative, "num_scales", num_scales);
    setShaderParam(shader_post_derivative, "saturate");
    setShaderParam(shader_post_derivative, "bias");
    setShaderParam(shader_post_derivative, "gen");
    setShaderParam(shader_post_derivative, "compress");
    setShaderParam(shader_post_derivative, "zoom");
    setShaderParam(shader_post_derivative, "suck");
    setShaderParam(shader_post_derivative, "swirl");
    setShaderParam(shader_post_derivative, "xdrift");
    setShaderParam(shader_post_derivative, "ydrift");
    setShaderParam(shader_post_derivative, "mirror_amt");
    setShaderParam(shader_post_derivative, "mirror_shape");

    new_yprime.begin();
    ofRect(0,0,w,h);
    new_yprime.end();
    shader_post_derivative.end();
}

//the meat: compute y' as f(t, y) and store in yprime
void ofApp::dynDerivative(float t, ofxPingPongFbo &yprime){
    multiscaleProcessing(t, dyn->getState(), yprime);
    filtering(t, yprime, yprime);
    derivativePost(t, dyn->getState(), yprime, lp->getState(), yprime);
}

void ofApp::lpDerivative(float t, ofxPingPongFbo &yprime){
    /*float epsilon = 1 - pow(2, -1./lp_frames);
    float damp = 1-pow(.001,lp_frames+1);
    blur(y, yprime, lp_radius);
    mix(epsilon, yprime, dyn->y, yprime);
    scale_add(damp, yprime, -1, y, yprime);*/

    float time_scale = params.getFloat("time_scale");
    float lp_frames = params.getFloat("lp_frames");
    int w = yprime.getWidth(), h = yprime.getHeight();
    float alpha = pow(2, -lp_frames);
    float beta = params.getFloat("lp_radius");
    float epsilon = pow(.001,lp_frames+1);
    shader_lp_filter.begin();
    setShaderParam(shader_lp_filter, "x", dyn->getState().getTextureReference(0), 0);
    setShaderParam(shader_lp_filter, "y", lp->getState().getTextureReference(0), 1);
    setShaderParam(shader_lp_filter, "size", w, h);
    setShaderParam(shader_lp_filter, "alpha", alpha);
    setShaderParam(shader_lp_filter, "beta", beta);
    setShaderParam(shader_lp_filter, "epsilon", epsilon);
    setShaderParam(shader_lp_filter, "time_scale");
    yprime.beginInPlace();
    ofRect(0,0,w,h);
    yprime.endInPlace();
    shader_lp_filter.end();
}

void ofApp::update(){
    sync.update();
    if(use_camera) camera.update();
    ofSetFrameRate(frame_rate);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    if(cycle_disp_mode && !(int(frame)%cycle_disp_mode)) disp_mode = (disp_mode+1)%display_sequence.size();
    draw_offset = ofVec2f( ofWrap(draw_offset.x+(params.getFloat("xshift"))/frame_rate, -.5, .5),
        ofWrap(draw_offset.y+(params.getFloat("yshift"))/frame_rate, -.5, .5)
                          );
}

void ofApp::draw(){
    double cur_frame_rate = frame_rate;
    if(realtime) cur_frame_rate = ofGetFrameRate();

    ofxPingPongFbo &y_fbo = dyn->getState();
    int w = y_fbo.getWidth();
    int h = y_fbo.getHeight();

    float fade_time = params.getFloat("fade_time");
    float path_blur = params.getFloat("path_blur");
    vwt->setAgentRate(params.getFloat("agent_rate"));
    vwt->setMomentumTime(params.getFloat("momentum_time"));
    vwt->setPathJitter(params.getFloat("path_jitter"));
    vwt->setAgentCombFrequency(params.getFloat("comb_freq"));
    vwt->getVideoVolume()->setAspectRatio(double(w)/h);

    //draw agent path
    int aw = agent_fbo.getWidth(), ah = agent_fbo.getHeight();
    float alpha = 1 - pow(2, -1./(fade_time*cur_frame_rate));
    fill(agent_fbo, ofFloatColor(.0,.0,.0,alpha));
    agent_fbo.beginInPlace();
    vwt->draw(0, 0, aw, ah);
    agent_fbo.endInPlace();
    blur(agent_fbo, agent_fbo, path_blur);
    gradients(agent_fbo);

    //main video process
    //integrator = 0 for euler, 1 for rk4
    int steps = 1 + 3*integrator;
    for(int i=0; i<steps; i++){
        dyn->tick(frame, 1, i);
        lp->tick(frame, 1, i);
        dyn->tock(frame, 1, i);
        lp->tock(frame, 1, i);
    }
    dyn->update(1);
    lp->update(1);

//    blur(dyn->y, dyn->y, blur_initial);

    //branch on disp_mode and draw to screen
    int ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    switch(display_sequence[disp_mode]){
        case AVFBDM_Color:
            mov(y_fbo, display_fbo);
            b2u(display_fbo, display_fbo);
            break;
        case AVFBDM_Monochrome:
            shader_display.begin();
            setShaderParam(shader_display, "size", display_fbo.getWidth(), display_fbo.getHeight());
            setShaderParam(shader_display, "state", y_fbo.getTextureReference(), 0);
            display_fbo.begin();
            ofRect(0, 0, display_fbo.getWidth(), display_fbo.getHeight());
            display_fbo.end();
            shader_display.end();
            break;
        case AVFBDM_Agents:
            resample(agent_fbo, display_fbo);
            break;
        case AVFBDM_Pyramid:
            if(!disp_scale)
                mov(y_pyramid[disp_scale], display_fbo);
            else
                resample(y_pyramid[disp_scale], display_fbo);
            b2u(display_fbo, display_fbo);
            break;
        case AVFBDM_Displacement:
            if(!disp_scale)
                mov(yprime_pyramid[disp_scale], display_fbo);
            else
                resample(yprime_pyramid[disp_scale], display_fbo);
            b2u(display_fbo, display_fbo);
            break;
        case AVFBDM_Filter:
            mov(lp->getState(), display_fbo);
            b2u(display_fbo, display_fbo);
            break;
    }

    shader_torus_shift.begin();
    setShaderParam(shader_torus_shift, "state", display_fbo.getTextureReference(0), 0);
    setShaderParam(shader_torus_shift, "shift", draw_offset);
    setShaderParam(shader_torus_shift, "size", ww, wh);
    display_fbo.draw(0,0,ww,wh);
    shader_torus_shift.end();
    //resampleToWindow(display_fbo);

    if(recording){
        uint64_t time;

        //ofFloatPixels pix;
        ofPixels pix;

        mov(display_fbo, render_fbo);

        time = ofGetSystemTimeMicros();
        render_fbo.readToPixels(pix,0);
        //display_fbo.readToPixels(pix,0);
        cout<<"readback time: "<<ofGetSystemTimeMicros()-time<<" us"<<endl;

        time = ofGetSystemTimeMicros();
        bool success = vr.addFrame(pix);
        cout<<"frame add time: "<<ofGetSystemTimeMicros()-time<<" us"<<endl;
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    //else
    //    mov(display_fbo, render_fbo);
    //render_fbo.draw(0,0,ww,wh);

    //read pixels back from video memory and put a new frame in the VWT
    if(readback_fbo.getWidth() != y_fbo.getHeight()){
        resample(y_fbo, readback_fbo);
        b2u(readback_fbo, readback_fbo);
    }
    else{
        b2u(y_fbo, readback_fbo);
    }
    ofFloatPixels pix;
    readback_fbo.readToPixels(pix,0);
    vwt->insert_frame(pix);


    //if in render mode, compute audio in the draw loop
    //delay should be equal to frame duration
    if(!realtime){
        int frame_duration_samps = audio_sample_rate/frame_rate;
        int nsamps = frame_duration_samps*audio_channels;
        float * samps = (float*)malloc(nsamps*sizeof(float));
        vwt->audioOut(samps, frame_duration_samps, audio_channels);
        writeAudioSamps(samps, nsamps);
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

void ofApp::toggleVideoRecord(){
    recording = !recording;
    if(recording)
        beginVideoRecord();
    else
        endVideoRecord();
}

void ofApp::beginVideoRecord(){
    int w = render_fbo.getWidth(), h = render_fbo.getHeight();

    stringstream ss;
    ss<<int(h*w*frame_rate*24/1000)<<"k";
    cout<<ss.str()<<endl;
    vr.setVideoBitrate(ss.str());

   // vr.setVideoCodec("ffv1"); //black or frozen video in vlc, eats memory
    //vr.setVideoCodec("ffvhuff"); //works but memory fills quickly and crashes
    //vr.setVideoCodec("huffyuv"); //works, memory still fills quickly
    //vr.setVideoCodec("ljpeg"); //video frozen, eats memory
    //vr.setVideoCodec("qtrle"); //eats memory, sound plays but no image
    //vr.setVideoCodec("snow"); //eats memory, vlc can't decode
    //vr.setVideoCodec("libwebp"); //eats memory, sound but no image in vlc
    //vr.setVideoCodec("zlib"); //same
    //vr.setMovFileExtension(".avi");

    //vr.setVideoCodec("dirac"); vr.setAdditionalVideoFlags("-strict -1"); //ffmpeg crashes immediately
    //vr.setVideoCodec("vc2"); vr.setAdditionalVideoFlags("-strict -1"); //same as dirac
    //vr.setVideoCodec("libschroedinger"); //ffmpeg crashes at end, video broken

    vr.setVideoCodec("mpeg4"); //flat memory use, works great, very lossy;
    //with bit rate set as above, slow memory growth + spike at end, good quality

    //vr.setVideoCodec("libx264"); //video likes to freeze part way through, lossy, memory fills fairly quickly
    //vr.setVideoCodec("libx264rgb"); //same

    //vr.setVideoCodec("jpeg2000"); //memory fills extremely fast, hangs at end, video broken
    //vr.setVideoCodec("libopenjpeg"); //even more garbage


    vr.setFfmpegLocation("\"C:\\Program Files (x86)\\ffmpeg\\bin\\ffmpeg.exe\"");

    string fname = ofGetTimestampString();

    //vr.setup(fname, w, h, frame_rate, sample_rate, channels);
    vr.setup(fname, w, h, frame_rate, audio_sample_rate, audio_channels);
    //vr.setup(fname, w, h, frame_rate, 0, 0);

    vr.start();
}

void ofApp::endVideoRecord(){
    vr.close();
}

void ofApp::openAudioFile(string fname){
    audio_file_size = 0;

    char write_buf[44];
    audio_file.open(fname.c_str(), ofstream::binary);

    strncpy(write_buf,"RIFF",4);
    *((int32_t*)(write_buf+4)) = 0; //dummy size
    strncpy(write_buf+8,"WAVEfmt ",8);
    *((int32_t*)(write_buf+16)) = 16;
    *((int16_t*)(write_buf+20)) = 1; //PCM
    *((int16_t*)(write_buf+22)) = audio_channels;
    *((int32_t*)(write_buf+24)) = audio_sample_rate; //PCM
    *((int32_t*)(write_buf+28)) = audio_sample_rate*audio_channels*3; //byte rate
    *((int16_t*)(write_buf+32)) = audio_channels*3; //block_align
    *((int16_t*)(write_buf+34)) = 24; //bits per sample
    strncpy(write_buf+36,"data",4);
    *((int32_t*)(write_buf+40)) = 0; //dummy size

    audio_file.write(write_buf, 44);
}

void ofApp::closeAudioFile(){
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

void ofApp::writeAudioSamps(float *samps, int nsamps){
    for(int i=0; i<nsamps; i++){
        union{
        int32_t word;
            char bytes[4];
        } v;
        v.word = double(1<<30)*double(samps[i]);
        audio_file.write(v.bytes+1,3);
        audio_file_size+=3;
    }
}

void ofApp::beginRenderMode(){
    cout<<"render mode"<<endl;

    setResolution(render_width, render_height);

    //set minimal lag of audio behind video
    vwt->setAudioDelay(1./frame_rate + 1./audio_sample_rate);

    //open a new wav file and write header
    stringstream ss;
    cur_save_dir = ofDirectory(ofGetTimestampString());
    cur_save_dir.create();
    ss << cur_save_dir.path()<<ofToDataPath("/audio.wav");

    openAudioFile(ss.str());
}

void ofApp::endRenderMode(){
    cout<<"realtime mode"<<endl;

    setResolution(realtime_width, realtime_height);

    vwt->setAudioDelay(audio_delay);

    closeAudioFile();
}

void ofApp::keyPressed(int key){
    if(key>='0' && key<='9'){
        cycle_disp_mode = int(key)-48;
        cout<<"cycle display mode: "<<cycle_disp_mode<<endl;
    }
    if(key=='a'){
        mute = !mute;
    }
/*    if(key=='b'){
        disp_buf = ofWrap(disp_buf+1, 0, 3);
        cout<<"display buffer: "<<disp_buf<<endl;
    }*/
    if(key=='c'){
        draw_offset.x = draw_offset.y = 0;
    }
    if(key=='d'){
        disp_mode = (disp_mode+1)%display_sequence.size();
        cout<<"display mode: "<<disp_mode<<endl;
    }
    if(key=='f'){
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    if(key=='h'){
        usage();
    }
    if(key=='i'){
        integrator = 1-integrator;
        cout<<"integrator: "<<integrator<<endl;
    }
    if(key=='l'){
        cout<<"reload shaders"<<endl;
        loadShaders();
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
        initRandom(lp->getState(), seed+1);
        initRandom(dyn->getState(), seed);
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
    if(key=='v'){
        toggleVideoRecord();
    }
}

void ofApp::usage(){
    cout<<
        "0-9 - cycle display mode every n frames"<<endl<<
        "a - audio on/off"<<endl<<
       // "b - change display buffer for pyramid"<<endl<<
        "c - recenter draw_offset"<<endl<<
        "d - change display mode (monochrome->agents->pyramid->lp->color)"<<endl<<
        "f - toggle fullscreen"<<endl<<
        "h - print this message"<<endl<<
        "i - toggle integrator (0=euler, 1=rk4)"<<endl<<
        "l - reload shaders"<<endl<<
        "m - toggle render mode"<<endl<<
        "n - reinitialize FBOs (for testing purposes)"<<endl<<
        "p - change display scale for pyramid"<<endl<<
        "r - randomize video"<<endl<<
        "s - save current frame as png"<<endl<<
        "v - toggle video recorder"<<endl<<
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
    if(drawing && realtime){
        vwt->audioOut(output, bufferSize, nChannels);
        if(recording)
            vr.addAudioSamples(output,bufferSize, nChannels);
        if(mute)
            memset((void *)output, 0, bufferSize*nChannels*sizeof(float));
    }
}
