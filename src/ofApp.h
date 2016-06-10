#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOscParameterSync.h"
#include "ofxVideoWaveTerrain.h"
#include "ofxVideoRecorder.h"
#include "ofxFastFboReader.h"

#include <unordered_set>
#include <unordered_map>

//class to distribute fbos for space+time efficient drawing
//for example in a drawing graph we don't need an fbo for every node,
//we can free a node's resources once all its dependencies have drawn
//however we don't want to be allocating and freeing GPU memory every frame
//so assuming that the shapes of buffers needed don't change much/often/at all between frames,
//we can just allocate once and hand out pointers to requesting nodes
    //ofFbo::Settings objects should hash to sets of free and allocated fbos with those settings
    //those sets should have constant time insert and remove
class ofxFboAllocator{
public:
    //produce an fbo pointer with the specified settings
    ofFbo* allocate(ofFbo::Settings s);

    //release an fbo pointer back to the pool of available fbos
    void release(ofFbo *fbo);

    //destroy all fbos not currently in use
    void destroy_available();

    ~ofxFboAllocator();

private:
    class ofxFboAllocatorBin{
    public:
        unordered_set<ofFbo*> available;
        unordered_set<ofFbo*> unavailable;
    };

    unordered_map<uint64_t, ofxFboAllocatorBin> bins;

    uint64_t keyFromSettings(ofFbo::Settings);
    uint64_t keyFromFbo(ofFbo*);
    uint64_t keyFromDim(uint64_t,uint64_t,uint64_t,uint64_t);
    uint64_t fboFormat(ofFbo *fbo);

};

class ofxBaseShaderNode{
public:
    //incremented by constructor
    static uint32_t node_count;

    string name;

    //initialized each cycle to the number of nodes depending on this one, 
    //and decremented by those nodes after drawing.
    uint32_t dependent_count;

    //track the number of nodes dependent on this one
    uint32_t num_dependents;

    //fbo settings for this node's output
    ofFbo::Settings settings;

    //list of nodes upon which this one depends (ordered list of inputs)
    vector<ofxBaseShaderNode*> inputs;

    //list of sampler uniforms in shader
    vector<string> textureInputs;

    //pointer to resource allocator
    ofxFboAllocator *allocator;

    //pointer to output fbo
    ofFbo *output;

    ofxBaseShaderNode(ofxFboAllocator *a, ofFbo::Settings s, string n, ofShader *sh);
    virtual ~ofxBaseShaderNode();

    //black-box drawing function to be differentiated by subclasses
    //dummy draw() so class is not abstract
    void draw();
    //virtual void draw();

    //graph-traversing update function
    void update();

    //request from the allocator an appropriate fbo to draw to
    void requestBuffer();

    //release output fbo to the allocator
    void releaseBuffer();

    virtual bool getDirty();
    void setDirty(bool b);

    //recursively dirty the whole graph
    //void dirtyAll();

    uint32_t getNumOutputTextures();
    uint32_t getNumInputTextures(uint32_t i);
    uint32_t getNumInputs();

    ofParameterGroup& getParameterGroup();
    ofParameterGroup& getParameter(string path);
    void setParameter(string, float);
    void setParameter(string, int);

    virtual void scaleResolution(float, float);

   // void setParameterAlias(string, ofParameter<auto>&);


    //bool verifyInputShapes();

    //when no more dependents, release resources
    //note that if a node starts without dependents, this will not be called
    virtual void decrementDependents();

private:
    //flag noting whether this node has been drawn yet this cycle
    bool dirty;

    bool hasSizeParameter;

    //expose parameters
    ofParameterGroup params;

    //
    ofShader *shader;

    //parse shader for uniforms, add to params
    void setupParameters();
    void setupParameter(string type, string name);

    void setShaderParam(ofParameterGroup &g);
};

//class representing a node whose output is not updated within a cycle
// (i.e., may be updated externally by an integrator or other drawing subsystem)
class ofxConstantShaderNode : public ofxBaseShaderNode{
public:
     //override constructor to request a buffer
    ofxConstantShaderNode(ofxFboAllocator *a, ofFbo::Settings s, string n);
    //override getDirty() to always return false
    bool getDirty();
    //override decrementDependents() to not release buffer
    void decrementDependents();
    //override scaleResolution() to copy contents
    void scaleResolution(float, float);
};

//class managing a DAG of ofxBaseShaderNodes
class ofxShaderGraph{
public:
     //fbo settings (stores resolution and type; numColorBuffers will be set by each node)
    ofFbo::Settings fbo_settings;

    //pointer to resource allocator
    ofxFboAllocator *allocator;

    ofParameterGroup params;
    //ofParameterGroup aliases;

    unordered_map<string, ofxBaseShaderNode*> nodes;
    unordered_set<ofxBaseShaderNode*> roots;

    unordered_map<string, string> swap;
    unordered_map<string, float> init;

    //set all nodes to dirty
    //call update on each root node
    void update();

    //construct a graph from XML
    void buildFromXml(ofXml x);

    void setResolution(int w, int h);

    ofFbo &getFbo(string);
    void swapFbos(string, string);
    void swapFbos();

    void initFbos();

    ofxShaderGraph(ofxFboAllocator *a, ofFbo::Settings s);
    ~ofxShaderGraph();
    //void forwardParameter(const void* alias_param, auto val);
    //void forwardParameter(auto &val);
};

//================================================================================

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
        void writeAudioSamps(float *samps, size_t nsamps);
        void toggleVideoRecord();
        void beginVideoRecord();
        void endVideoRecord();

        void loadShaders();
        void setupParameters();
        void setupConstants();
        void setupGL();
        void setupGraph(ofxShaderGraph*&, string);
        void setupGlobals();
        void setupAudio();

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
        void initRandom(ofFbo &target, int mode); //todo: replace with GPU noise in modular style
        void initParams(int &seed);

        void mov(ofFbo &src, ofFbo &dest);
        void fill(ofFbo &dest, ofFloatColor c, ofBlendMode mode = OF_BLENDMODE_ALPHA);


        ofParameterGroup params;
        ofxOscParameterSync sync;
        int local_port, remote_port;
        string remote_host;
        ofxPanel gui;

        string ffmpeg_path, forward_graph_path;

        ofxFboAllocator fbo_allocator;
        ofxShaderGraph *forward_graph;//, *integrator_graph;

        //ofParameter<int> seed; //seed for filling matrices below

        ofFbo::Settings fbo_params;

        double frame;

        int disp_buf, disp_mode, disp_scale, audio_file_size, oversample_waveform, num_scales,
            integrator, cycle_disp_mode, num_agents;

        int window_width, window_height, render_width, render_height, realtime_width, realtime_height, readback_width, readback_height;

        double frame_rate_limit, frame_rate_record, scale_factor;

        ofVec2f draw_offset;

        bool drawing, //false in setup, true once draw loop begins
            realtime, use_camera, fullscreen, mute, discard_largest_scale, recording, save_frame;

        vector<string> display_sequence;

        ofVideoGrabber camera;

        ofxVideoRecorder vr;

        ofxVideoWaveTerrain *vwt;

        ofstream audio_file;

        ofDirectory cur_save_dir;

        int audio_device, audio_channels, audio_buffer_size, audio_sample_rate;
        double audio_delay;
        ofSoundStream ss;

        ofxFastFboReader record_reader, vwt_reader;
};

