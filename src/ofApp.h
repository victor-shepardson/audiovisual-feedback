#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "ofxVideoWaveTerrain.h"

//wav file writing is janked. why???

//would like to switch between realtime and render modes
//in realtime mode: asynchronous video+audio, real time audio
//in render mode: precise number of audio samples per frame (compute audio in draw() and store/stream to disk)
//should be able to dynamically change resolution

//todo:
// - move most drawing and audio into auxiliary functions
// - bring in machinery to save lossless images and audio files (will composite later w/ aftereffects)
// - put some kind of visual indication of render mode into draw loop

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

        ofParameterGroup params;
        ofParameter<float> target_sat;
        ofParameter<float> target_mean;
        ofParameter<float> target_mix;
        ofParameter<float> time_scale;
        ofParameter<float> bound_gauss;
        ofParameter<float> bound_clip;
        ofParameter<float> rot;
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

        vector<ofFbo> k_fbos, aux_fbos;
        ofFbo *y_fbo, *agent_fbo, *display_fbo, *scratch_fbo, *readback_fbo, *render_fbo;
        float frame;
        ofShader shader_rkupdate, shader_rkderivative, shader_display, shader_blur, shader_grad, shader_test;

        int disp_mode, channels, audio_file_size, oversample_waveform, undersample_terrain;

        int window_width, window_height, render_width, render_height, realtime_width, realtime_height;

        double sample_rate, frame_rate, audio_delay;

        bool drawing, //false in setup, true once draw loop begins
            realtime, use_camera, fullscreen;

        ofVideoGrabber camera;

        ofxVideoWaveTerrain *vwt;

        ofstream audio_file;

        ofDirectory cur_save_dir;
};

