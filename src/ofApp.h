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
        void initRandom(ofFbo &target, int mode);
        void initParams(int &seed);
        void toggleRecord(int& r);

        void rkUpdate(float dt, ofFbo &y, ofFbo &k, ofFbo &new_y);
        void rkUpdate(float dt, ofFbo &y, vector<ofFbo> &k, ofFbo &new_y);
        void rkDerivative(float t, ofFbo &y, ofFbo &yprime);
        void rkStep(float t, float dt, ofFbo &y, ofFbo &k, ofFbo &yprime);
        void rungeKutta(float t, float dt, ofFbo &y, vector<ofFbo> &k);

        void resample(ofFbo &src, ofFbo &dest);
        void derivativeAtScale(float t, ofFbo &y, ofFbo &yprime, float scale);
        void derivativePost(float t, ofFbo &y, ofFbo &yprime, ofFbo &new_yprime);

        void blur(ofFbo &src, ofFbo &dest, float radius);
        void sub(ofFbo &pos, ofFbo& neg, ofFbo &dest);
        void mov(ofFbo &src, ofFbo &dest);
        void gradients(ofFbo &src);

        ofParameterGroup params;
        ofParameter<float> target_sat;
        ofParameter<float> target_mean;
        ofParameter<float> target_mix;
        ofParameter<float> time_scale;
        ofParameter<float> kernel_width;
        ofParameter<float> bound_clip;
        ofParameter<float> rot;
        ofParameter<float> zoom;
        ofParameter<float> warp;
        ofParameter<float> blur_size;
        ofParameter<float> agent_rate;
        ofParameter<float> momentum_time;
        ofParameter<float> path_jitter;
        ofParameter<float> fade_time;
        ofParameter<float> path_blur;
        ofParameter<int> seed;

        ofMatrix4x4 grad_proj, color_proj;

        ofxOscParameterSync sync;

        ofxPanel gui;

        ofFbo::Settings fbo_params;

        vector<ofFbo> k_fbos, y_pyramid, yprime_pyramid;//blur_fbos, grad_fbos;
        ofFbo *y_fbo, *agent_fbo, *display_fbo, *scratch_fbo, *readback_fbo, *render_fbo;
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

