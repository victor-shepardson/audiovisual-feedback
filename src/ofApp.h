#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "ofxVideoWaveTerrain.h"
#include "ofxVideoRecorder.h"
#include "ofxFastFboReader.h"

//
class ofxPingPongFbo{
public:
    ofxPingPongFbo();
    ofxPingPongFbo& operator=(const ofxPingPongFbo &);
    ofxPingPongFbo(const ofxPingPongFbo &);
    //ofxPingPongFbo copy();
    void destroy();
    void allocate(ofFbo::Settings);
    void draw(int,int,int,int);
    ofTexture& getTextureReference();
    ofTexture& getTextureReference(int);
    int getWidth();
    int getHeight();
    void swap();
    void save();
    void restore();
    void begin();
    void end();
    void beginInPlace();
    void endInPlace();
    void readToPixels(ofPixels&, int) ;
    void readToPixels(ofFloatPixels&, int) ;
    int getNumTextures();
    void setActiveDrawBuffer(int);
	void setActiveDrawBuffers(const vector<int>&);
	void activateAllDrawBuffers();
    ofFbo::Settings settings;
private:
    ofFbo *ping, *pong, *aux;
    ofxFastFboReader reader;
};

template <class T>
class ofxDynamicalTexture{
public:
    static ofShader shader_rkupdate;
    ofxDynamicalTexture(T *app, void (T::*derivative)(float, ofxPingPongFbo&));
    void allocate(ofFbo::Settings);
    void destroy();
    //to update, tick() all mutually dependent ofxDynamicalTextures, then tock() them;
    //do this once for euler, 4 times for rk4; call tick and tock with i=loop index (0-3)
    //then call update with mode = 0 for euler, 1 for rk4
    void tick(float t, float dt, int i);
    void tock(float t, float dt, int i);
    void update(float dt, int mode);
    ofxPingPongFbo &getState();
private:
    ofxPingPongFbo y, ytemp;
    vector<ofxPingPongFbo> k;
    T *app;
    void (T::*derivative)(float, ofxPingPongFbo&);
    void rkUpdate(float dt, int i, ofxPingPongFbo &dest);
    void rkUpdate(float dt);
    void rkStep(float t, float dt, int i);
};

//manages a numerical integration algorithm with derivatives set by a black box
//should manage any ancillary buffers needed; who should own the state buffers?
//sleep on this for now
/*template <typename T>
class ofxRKIntegrator{
public:
    void step(float t, float dt);
    void add(ofxDynamicalTexture<T> *p);
private:
    vector<ofxDynamicalTexture<T> *> v;
};*/

class ofApp : public ofBaseApp{

    public:
        void close();
        void setup();
        void update();
        void draw();
        void audioOut(float * output, int bufferSize, int nChannels);

        void usage();
        void toggleRenderMode();
        void beginRenderMode();
        void endRenderMode();
        void openAudioFile(string fname);
        void closeAudioFile();
        void writeAudioSamps(float *samps, int nsamps);
        void toggleVideoRecord();
        void beginVideoRecord();
        void endVideoRecord();

        void loadShaders();

        void allocateFbos();
        void reallocateFbos();
        void setResolution(int x, int y);

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void initRandom(ofxPingPongFbo &target, int mode); //todo: replace with GPU noise in modular style
        void initParams(int &seed);

        void dynDerivative(float t, ofxPingPongFbo &yprime);
        void lpDerivative(float t, ofxPingPongFbo &yprime);

        void resampleToWindow(ofxPingPongFbo &src);
        void resample(ofxPingPongFbo &src, ofxPingPongFbo &dest);

        void processingAtScale(float t, ofxPingPongFbo &src, ofxPingPongFbo &m, ofxPingPongFbo &dest, float scale);
        void multiscaleProcessing(float t, ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void filtering(float t, ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void derivativePost(float t, ofxPingPongFbo &y, ofxPingPongFbo &new_y, ofxPingPongFbo &lp, ofxPingPongFbo &new_yprime);

        void scale_add(float a, ofxPingPongFbo &x, float b, ofxPingPongFbo &y, ofxPingPongFbo &dest);
        void mix(float m, ofxPingPongFbo &x, ofxPingPongFbo &y, ofxPingPongFbo &dest);
        void b2u(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void fill(ofxPingPongFbo &dest, ofFloatColor c, ofBlendMode mode = OF_BLENDMODE_ALPHA);
        void blur(ofxPingPongFbo &src, ofxPingPongFbo &dest, float radius);
        void sub(ofxPingPongFbo &pos, ofxPingPongFbo& neg, ofxPingPongFbo &dest);
        void edge_aware_filter(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void mov(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void blend(ofxPingPongFbo &src, ofxPingPongFbo &dest, ofBlendMode mode);
        void gradients(ofxPingPongFbo &src);

        ofParameterGroup params;

        ofParameter<float> filter_steps; //truncated to int: number of times to apply edge-aware filter
        ofParameter<float> blur_post; //radius of gaussian blur applied after warping
        ofParameter<float> lf_bleed; //amount of low frequency information left behind in high-passed bins
        ofParameter<float> blur_initial; //radius of a blur applied before frequency decomposition, in pixels
        ofParameter<float> blur_scale; //scale the radius of blur used for low pass filter (determined by fixed scale factor)
        ofParameter<float> drive; //gain parameter on main derivative
        ofParameter<float> lp_frames; //set the time constant for the temporal filter in frames
        ofParameter<float> lp_radius; //set the radius of spatial diffusion on the temporal filter

        ofParameter<float> disp_exponent; //how to scale displacements to compensate for downsampling
        ofParameter<float> xdrift; //horizontal displacement
        ofParameter<float> ydrift; //vertical displacement
        ofParameter<float> zoom; //scale around center
        ofParameter<float> suck; //radial displacement
        ofParameter<float> swirl; //perpendicular to suck
        ofParameter<float> mirror_shape; //interpolate horizontal/radial
        ofParameter<float> mirror_amt; //mix amount
        ofParameter<float> warp_grad; //control magnitude of gradient-based displacement
        ofParameter<float> warp_color; //control magnitude of color-based displacement
        ofParameter<float> warp_agent; //control magnitude of agent-based displacement
        ofParameter<float> agent_drive; //shape nonlinearity used to squash agent values

        ofParameter<float> saturate; //color saturating component of derivative
        ofParameter<float> bias; //bias derivative
        ofParameter<float> gen; //scale magnitude of spatial component of derivative
        ofParameter<float> compress; //at 1, normalize derivate at each scale. at 0, do nothing
        ofParameter<float> rot; //how much weird color rotation
        ofParameter<float> time_scale; //scale derivatives/time
        ofParameter<float> bound_clip; //clamp derivatives to prevent blow up

        ofParameter<float> agent_rate; //image widths in Hz if traveling at max velocity in a horizontal line
        ofParameter<float> momentum_time; //time constant for momentum, in seconds
        ofParameter<float> path_jitter; //random jitter in agent position, in image widths
        ofParameter<float> fade_time; //time constant for agent buffer fade, in seconds
        ofParameter<float> path_blur; //radius of agent buffer blur, in pixels

        ofParameter<float> test_param; //general purpose parameter for convenience

        ofParameter<int> seed; //seed for filling matrices below

        ofMatrix4x4 grad_proj, color_proj;

        ofxOscParameterSync sync;

        ofxPanel gui;

        ofFbo::Settings fbo_params;

        ofxDynamicalTexture<ofApp> *dyn, *lp;

        vector<ofxPingPongFbo> y_pyramid, yprime_pyramid;
        ofxPingPongFbo  agent_fbo, display_fbo, readback_fbo, render_fbo;
        float frame;

        ofShader shader_blur, shader_resample, shader_display,
            shader_multiscale, shader_post_derivative, shader_grad,
            shader_scale_add, shader_lp_filter, shader_warp, shader_edge_aware,
            shader_multi_warp;

        int disp_buf, disp_mode, disp_scale, channels, audio_file_size, oversample_waveform, num_scales,
            integrator, cycle_disp_mode;

        int window_width, window_height, render_width, render_height, realtime_width, realtime_height, readback_width, readback_height;

        double sample_rate, frame_rate, audio_delay, scale_factor;

        bool drawing, //false in setup, true once draw loop begins
            realtime, use_camera, fullscreen, mute, discard_largest_scale, recording;

        enum AVFBDisplayMode{
            AVFBDM_Color,
            AVFBDM_Monochrome,
            AVFBDM_Agents,
            AVFBDM_Pyramid,
            AVFBDM_Displacement,
            AVFBDM_Filter
        };
        vector<AVFBDisplayMode> display_sequence;

        ofVideoGrabber camera;
        ofxVideoRecorder vr;

        ofxVideoWaveTerrain *vwt;

        ofstream audio_file;

        ofDirectory cur_save_dir;

        ofSoundStream ss;
};

