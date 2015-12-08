#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "ofxVideoWaveTerrain.h"

//ok, it turns out you can't magically sample/render to the same texture in an fbo;
//at least not across blocks computed on the GPU, it looks like. whoops!
//need a ping-ponging fbo with two textures
//the main blur is fixed for now, inelegantly
//agent blur should be wrong but I haven't noticed any artifacts
//the only direct feedback in the runge-kutta seems ok since it is the update step, which is local to each pixel

//why does the blurred state display upside down??

//should maybe wrap utility stuff like blur and gradient in a class

//should look into writing own shaders for agent drawing instead of relying on blending

//look into kalieidoscoping/symmetries

//build abstractions for multi-scale processes:
// - downsample, upsample shaders
// - accumulation of derivatives:
// - for each scale:[downsample image -> compute derivative -> upsample derivative -> accumulate] -> integrate

class ofxPingPongFbo{
public:
    ofxPingPongFbo();
    ofxPingPongFbo copy();
    void destroy();
    void allocate(ofFbo::Settings);
    void draw(int,int,int,int);
    ofTexture& getTextureReference();
    ofTexture& getTextureReference(int);
    int getWidth();
    int getHeight();
    void save();
    void restore();
    void begin();
    void end();
    void beginInPlace();
    void endInPlace();
    void readToPixels(ofPixels&, int) const;
    void readToPixels(ofFloatPixels&, int) const;
    int getNumTextures();
    void setActiveDrawBuffer(int);
	void setActiveDrawBuffers(const vector<int>&);
	void activateAllDrawBuffers();
    ofFbo::Settings settings;
private:
    ofFbo *ping, *pong, *aux;
};

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

        void allocateFbos();
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
        void initRandom(ofxPingPongFbo &target, int mode);
        void initParams(int &seed);
        void toggleRecord(int& r);

        void rkUpdate(float dt, ofxPingPongFbo &y, ofxPingPongFbo &k, ofxPingPongFbo &new_y);
        void rkUpdate(float dt, ofxPingPongFbo &y, vector<ofxPingPongFbo> &k, ofxPingPongFbo &new_y);
        void rkDerivative(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime);
        void rkStep(float t, float dt, ofxPingPongFbo &y, ofxPingPongFbo &k, ofxPingPongFbo &yprime);
        void rungeKutta(float t, float dt, ofxPingPongFbo &y, vector<ofxPingPongFbo> &k);

        void resample(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void derivativeAtScale(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime, float scale);
        void derivativePost(float t, ofxPingPongFbo &y, ofxPingPongFbo &yprime, ofxPingPongFbo &new_yprime);

        void b2u(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void fill(ofxPingPongFbo &dest, ofFloatColor c, ofBlendMode mode = OF_BLENDMODE_ALPHA);
        void blur(ofxPingPongFbo &src, ofxPingPongFbo &dest, float radius);
        void sub(ofxPingPongFbo &pos, ofxPingPongFbo& neg, ofxPingPongFbo &dest);
        void mov(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void blend(ofxPingPongFbo &src, ofxPingPongFbo &dest, ofBlendMode mode);
        void gradients(ofxPingPongFbo &src);

        ofParameterGroup params;

        ofParameter<float> lf_bleed; //amount of low frequency information left behind in high-passed bins
        ofParameter<float> blur_initial; //radius of a blur applied before frequency decomposition, in pixels
        ofParameter<float> blur_scale; //scale the radius of blur used for low pass filter (determined by fixed scale factor)
        ofParameter<float> drive; //not in use; shape nonlinearity applied to sum of scales

        ofParameter<float> disp_exponent; //how to scale displacements to compensate for downsampling
        ofParameter<float> xdrift; //horizontal displacement
        ofParameter<float> ydrift; //vertical displacement
        ofParameter<float> zoom; //radial displacement
        ofParameter<float> warp_grad; //control magnitude of gradient-based displacement
        ofParameter<float> warp_color; //control magnitude of color-based displacement
        ofParameter<float> warp_agent; //control magnitude of agent-based displacement
        ofParameter<float> agent_drive; //shape nonlinearity used to squash agent values

        ofParameter<float> target_sat; //target saturation for normalization
        ofParameter<float> target_mean; //target mean for normalization
        ofParameter<float> target_mix; //how much to approach the target color when normalizing
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

        vector<ofxPingPongFbo> k_fbos, y_pyramid, yprime_pyramid;//blur_fbos, grad_fbos;
        ofxPingPongFbo y_fbo, agent_fbo, display_fbo, readback_fbo, render_fbo;
        float frame;
        ofShader shader_blur, shader_resample, shader_rkupdate, shader_display, shader_scale_derivative, shader_post_derivative, shader_grad;
        //ofShader shader_rkderivative, shader_test,,;

        int disp_buf, disp_mode, disp_scale, channels, audio_file_size, oversample_waveform, undersample_terrain, num_scales;

        int window_width, window_height, render_width, render_height, realtime_width, realtime_height;

        double sample_rate, frame_rate, audio_delay, scale_factor;

        bool drawing, //false in setup, true once draw loop begins
            realtime, use_camera, fullscreen, mute;

        ofVideoGrabber camera;

        ofxVideoWaveTerrain *vwt;

        ofstream audio_file;

        ofDirectory cur_save_dir;
};

