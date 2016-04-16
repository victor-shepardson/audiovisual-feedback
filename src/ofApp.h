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

private:
    class ofxFboAllocatorBin{
    public:
        unordered_set<ofFbo*> available;
        unordered_set<ofFbo*> unavailable;
    };

    unordered_map<uint64_t, ofxFboAllocatorBin> bins;

    uint64_t keyFromSettings(ofFbo::Settings);
    uint64_t keyFromFbo(ofFbo*);
    uint64_t keyFromDim(uint64_t,uint64_t,uint64_t);
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

    ofParameterGroup &getParameterGroup();
    ofParameterGroup& getParameter(string path);
    void setParameter(string, float);
    void setParameter(string, int);

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

    //set all nodes to dirty
    //call update on each root node
    void update();

    //construct a graph from XML
    void buildFromXml(ofXml x);

    ofFbo &getFbo(string);
    void swapFbos(string, string);

    ofxShaderGraph(ofxFboAllocator *a, ofFbo::Settings s);

    //void forwardParameter(const void* alias_param, auto val);
    //void forwardParameter(auto &val);
};

//================================================================================

//should replace aux with a collection of buffers and save()/restore() with push()/pop()
//then lazily allocate new buffers to aux
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

/*template <class T>
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
    void update(float dt);
    ofxPingPongFbo &getState();
private:
    int ticks, tocks;
    ofxPingPongFbo y, ytemp; //y stores the state at each time step; ytemp stores updated states for intermediate computations
    vector<ofxPingPongFbo> k;
    T *app;
    void (T::*derivative)(float, ofxPingPongFbo&);
    void rkUpdate(float dt, int i);
    void rkUpdate(float dt);
    void rkStep(float t, float dt, int i);
};
*/
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
        void setupParameters();
        void setupConstants();
        void setupGL();
        void setupGraph();
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

        /*void dynDerivative(float t, ofxPingPongFbo &yprime);
        void lpDerivative(float t, ofxPingPongFbo &yprime);

        void resampleToWindow(ofxPingPongFbo &src);
        void resample(ofxPingPongFbo &src, ofxPingPongFbo &dest);

        void processingAtScale(float t, ofxPingPongFbo &src, ofxPingPongFbo &m, ofxPingPongFbo &dest, float scale, float mod);
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
        void magnitudes(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void displacement(ofxPingPongFbo &src, ofxPingPongFbo &dest);
        void multilateral_filter(ofxPingPongFbo &src, ofxPingPongFbo &aux, ofxPingPongFbo &dest);
*/
        void mov(ofFbo &src, ofFbo &dest);

        //void beginShader(string);
        //void endShader();

        //map<string, ofShader> shaders;
        //ofShader *cur_shader;

        //wrap shader.setUniformxx
        void setShaderParam(const string&);
        void setShaderParam(const string&, float);
        void setShaderParam(const string&, float, float);
        void setShaderParam(const string&, float, float, float);
        void setShaderParam(const string&, float, float, float, float);
        void setShaderParam(const string&, ofVec2f&);
        void setShaderParam(const string&, ofVec3f&);
        void setShaderParam(const string&, ofVec4f&);
        void setShaderParam(const string&, int);
        void setShaderParam(const string&, int, int);
        void setShaderParam(const string&, int, int, int);
        void setShaderParam(const string&, int, int, int, int);
        void setShaderParam(const string&, ofMatrix4x4);
        void setShaderParam(const string&, ofTexture&, int loc=0);

        ofParameterGroup params;
        ofxOscParameterSync sync;
        int local_port, remote_port;
        string remote_host;
        ofxPanel gui;

        ofxFboAllocator fbo_allocator;
        ofxShaderGraph *forward_graph;//, *integrator_graph;

        ofParameter<int> seed; //seed for filling matrices below

        //ofMatrix4x4 grad_proj, color_proj;

        ofFbo::Settings fbo_params;

        //ofxDynamicalTexture<ofApp> *dyn, *lp;

        //vector<ofxPingPongFbo> y_pyramid, yprime_pyramid;
        //ofxPingPongFbo  agent_fbo, display_fbo, readback_fbo, render_fbo;
        double frame;

        int disp_buf, disp_mode, disp_scale, audio_file_size, oversample_waveform, num_scales,
            integrator, cycle_disp_mode;

        int window_width, window_height, render_width, render_height, realtime_width, realtime_height, readback_width, readback_height;

        double frame_rate, scale_factor;

        ofVec2f draw_offset;

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

        int audio_device, audio_channels, audio_buffer_size, audio_sample_rate;
        double audio_delay;
        ofSoundStream ss;
};

