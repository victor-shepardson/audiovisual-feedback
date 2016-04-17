#include "ofApp.h"
#include "math.h"

uint64_t ofxFboAllocator::keyFromSettings(ofFbo::Settings s){
    uint64_t w = s.width;
    uint64_t h = s.height;
    uint64_t c = s.numColorbuffers;
    return keyFromDim(w,h,c);
}

uint64_t ofxFboAllocator::keyFromFbo(ofFbo *fbo){
    uint64_t w = fbo->getWidth();
    uint64_t h = fbo->getHeight();
    uint64_t c = fbo->getNumTextures();
    return keyFromDim(w,h,c);
}

uint64_t ofxFboAllocator::keyFromDim(uint64_t w, uint64_t h, uint64_t c){
    uint64_t mask = (1<<25)-1;
    w &= mask;
    h &= mask;
    c &= mask;
    return h | (w<<24) | (c<<48);
}

ofFbo* ofxFboAllocator::allocate(ofFbo::Settings s){
    uint64_t k = keyFromSettings(s);

    ofxFboAllocatorBin &bin = bins[k];

    //cout<<bin.available.size()<<endl;

    ofFbo * ret;
    if(bin.available.size()){
        auto i = bin.available.begin();
        ret = *i;
        bin.available.erase(i);
    }
    else{
        cout<<"ofxFboAllocator: making new ("<<s.width<<"x"<<s.height<<"x"<<s.numColorbuffers<<") fbo with key = "<<k<<endl;
        ret = new ofFbo();
        ret->allocate(s);
        if(keyFromFbo(ret)!=k)
            cout<<"ofxFboAllocator warning: keys do not match"<<endl;
    }
    bin.unavailable.insert(ret);

    //cout<<"ofxFboAllocator: allocated ("<<s.width<<"x"<<s.height<<"x"<<s.numColorbuffers<<") key = "<<k<<" ptr = "<<ret<<endl;

    return ret;
}

void ofxFboAllocator::release(ofFbo *fbo){
    if(!fbo){
        cout<<"ofxFboAllocator warning: attempt to release null fbo"<<endl;
        return;
    }
    uint64_t k = keyFromFbo(fbo);

    ofxFboAllocatorBin &bin = bins[k];
    auto i = bin.unavailable.find(fbo);
    if(i == bin.unavailable.end()){
        cout<<"ofxFboAllocator warning: attempt to release fbo not owned by allocator"<<endl;
        return;
    }
    bin.available.insert(fbo);
    bin.unavailable.erase(i);

    // cout<<"ofxFboAllocator: released ("<<fbo->getWidth()<<"x"<<fbo->getHeight()<<"x"<<fbo->getNumTextures()<<") key = "<<k<<" ptr = "<<fbo<<endl;
}

//fbos deleted here are still being allocated. WHYYY
void ofxFboAllocator::destroy_available(){
    for(auto &i:bins){
        auto &bin = i.second.available;
        for(auto fbo:bin){
            if(fbo){
                //fbo.clear();
                delete fbo;
                cout<<"ofxFboAllocator: destroyed fbo "<<fbo<<endl;
            }
        }
        bin.clear();
    }
}

uint32_t ofxBaseShaderNode::node_count = 0;
ofxBaseShaderNode::ofxBaseShaderNode(ofxFboAllocator *a, ofFbo::Settings s, string n, ofShader * sh){
    allocator = a;
    settings = s;
    shader = sh;
    num_dependents = 0;
    node_count++;
    output = NULL;
    hasSizeParameter = false;

    if(n==""){
        char temp[100];
        snprintf(temp, 100, "node%04d", node_count);
        name = string(temp);
    }
    else
        name = n;

    if(shader)
        setupParameters();
}

ofxBaseShaderNode::~ofxBaseShaderNode(){
    delete shader;
    if(output)
        releaseBuffer();
}

void ofxBaseShaderNode::setupParameters(){
    params.setName(name);

    string src = shader->getShaderSource(GL_FRAGMENT_SHADER);

    uint32_t output_count = 0;

    vector<string> src_lines = ofSplitString(src, "\n");
    for(string line:src_lines){
        ofStringReplace(line, "\t", " ");
        vector<string> tokens = ofSplitString(line, " ", true);
        if(tokens.size() < 3)
            continue;
        string keyword = tokens[0];
        string param_type = tokens[1];
        string param_name = tokens[2].substr(0, tokens[2].length()-1);//ofSplitString(tokens[2], ";", true, true)[0];
        if(keyword=="uniform"){
            //skip special parameters which are handled explicitly in draw()
            if(param_name == "size" && param_type == "ivec2"){
                hasSizeParameter = true;
                cout<<name<<": special uniform \"size\" detected"<<endl;
                continue;
            }
            cout<<"<"<<name<<"> <"<<param_type<<"> <"<<param_name<<">"<<endl;
            setupParameter(param_type, param_name);
        }
        else if(keyword=="out"){
            if(param_type!="vec4")
                cout<<"warning: only vec4 outs supported"<<endl;
            output_count++;
        }
    }

    cout<<params<<endl;

    //set number of output textures based on number of "out vec4"s in shader
    cout<<"detected "<<output_count<<" output buffers"<<endl;
    settings.numColorbuffers = output_count;
}

//construct an ofParameterGroup representing a vector type
//or is param_type is a sampler, put it in textureInputs
void ofxBaseShaderNode::setupParameter(string param_type, string param_name){
    bool isIntegral = true;
    uint32_t dim = 0;

    if(param_type == "int"){
        dim = 1;
    }else if(param_type == "ivec2"){
        dim = 2;
    }else if(param_type == "ivec3"){
        dim = 3;
    }else if(param_type == "ivec4"){
        dim = 4;
    }else if(param_type == "float"){
        isIntegral = false;
        dim = 1;
    }else if(param_type == "vec2"){
        isIntegral = false;
        dim = 2;
    }else if(param_type == "vec3"){
        isIntegral = false;
        dim = 3;
    }else if(param_type == "vec4"){
        isIntegral = false;
        dim = 4;
    }

    if(param_type == "sampler2D"){
        textureInputs.push_back(param_name);
    }
    else{
        ofParameterGroup g;
        g.setName(param_name);
        for(uint32_t i = 0; i<dim; i++){
            int initial = 0;
            /*if(param_name=="size" && isIntegral && dim==2)
                if(i)
                    initial=settings.height;
                else
                    initial=settings.width;*/
            stringstream ss;
            ss<<i;
            if(isIntegral)
                ss<<"i";
            else
                ss<<"f";
            string elem_name = ss.str();
            if(isIntegral){
                g.add(ofParameter<int>().set(elem_name, initial));
            }else{
                g.add(ofParameter<float>().set(elem_name, initial));
            }
        }
        params.add(g);
    }
}

void ofxBaseShaderNode::update(){
    //if not dirty, nothing to do
    if(!getDirty())
        return;

    //cout<<"drawing node "<<name<<endl;

    //first, recursively make sure all dependencies are up to date
    for(auto node:inputs){
        node->update();
    }
    //get a buffer and draw to it
    if(!output){
        requestBuffer();
    }
    draw();
    setDirty(false);

    //now that we've drawn, dependents can start to draw, so set the counter
    dependent_count = num_dependents;

    //update inputs so they know when to free resources
    for(auto node:inputs){
        node->decrementDependents();
    }
}

//request from the allocator an appropriate fbo to draw to
void ofxBaseShaderNode::requestBuffer(){
    //cout<<"node "<<name<<" requesting buffer"<<endl;
    if(output)
        cout<<"ofxBaseShaderNode warning: requesting new buffer without releasing old one"<<endl;
    output = allocator->allocate(settings);
    //activate all drawbuffers
    vector<int> db;
    for(size_t i=0; i<getNumOutputTextures(); i++)
        db.push_back(i);
    output->setActiveDrawBuffers(db);
}

//release output fbo to the allocator
void ofxBaseShaderNode::releaseBuffer(){
    //if(output)
        allocator->release(output);
    output = NULL;
}

bool ofxBaseShaderNode::getDirty(){
    return dirty;
}
void ofxBaseShaderNode::setDirty(bool b){
    dirty = b;
}

//recursively dirty the whole graph
/*void ofxBaseShaderNode::dirtyAll(){
    for(auto i = inputs.begin(); i!=inputs.end(); i++){
        ofxBaseShaderNode *node = *i;
        node->dirtyAll();
    }
    setDirty(true);
}*/

uint32_t ofxBaseShaderNode::getNumOutputTextures(){
    return settings.numColorbuffers;
}

//note that getNumInputs() and getNumInputTextures() require that inputs has been populated already
uint32_t ofxBaseShaderNode::getNumInputTextures(uint32_t i){
    if(i<inputs.size())
        return inputs[i]->getNumOutputTextures();
    return 0;
}
uint32_t ofxBaseShaderNode::getNumInputs(){
    return inputs.size();
}

ofParameterGroup& ofxBaseShaderNode::getParameterGroup(){
    return params;
}

//note that this gets a Parameter group with ofParameter elements indexed "0", "1", ...
//should maybe have a name for that
ofParameterGroup& ofxBaseShaderNode::getParameter(string path){
    vector<string> tokens = ofSplitString(path, "/", true);
    ofParameterGroup* g = &params;
    for(string token:tokens){
        try{
            ofAbstractParameter &p = g->get(token);
            g = &static_cast<ofParameterGroup&>(p);
        }
        catch(exception e){
            cout<<"warning: parameter "<<path<<" does not exist (specifically at \""<<token<<"\")"<<endl;
            break;
        }    
    }
    return *g;
}

/*template<typename T>
void ofxBaseShaderNode::setParameterAlias(string path, ofParameter<T>& mom){
    //cout<<"setParameterAlias: "<<path<<", "<<mom<<endl;
    try{
        vector<string> tokens = ofSplitString(path, "/", true);
        if(tokens.size()<2)
            throw exception();
        string elem = tokens.back();
        tokens.pop_back();
        ofParameter<T> &p = getParameter(ofJoinString(tokens, "/")).get<T>(elem);
        p.makeReferenceTo(mom);
    }
    catch(exception e){
        cout<<"warning: set alias failed, bad parameter path "<<path<<endl;
    }
}*/

void ofxBaseShaderNode::setParameter(string path, float v){
    try{
        vector<string> tokens = ofSplitString(path, "/", true);
        string elem = tokens.back();
        tokens.pop_back();
        getParameter(ofJoinString(tokens, "/")).getFloat(elem) = v;
    }
    catch(exception e){
        cout<<"warning: bad parameter path "<<path<<endl;
    }
}

void ofxBaseShaderNode::setParameter(string path, int v){
    try{
        vector<string> tokens = ofSplitString(path, "/", true);
        string elem = tokens.back();
        tokens.pop_back();
        getParameter(ofJoinString(tokens, "/")).getInt(elem) = v;
    }
    catch(exception e){
        cout<<"warning: bad parameter path "<<path<<endl;
    }
}

void ofxBaseShaderNode::scaleResolution(float ws, float hs){
    settings.width*=ws;
    settings.height*=hs;
    if(output){
        releaseBuffer();
        requestBuffer();
    }
}

/*bool ofxBaseShaderNode::verifyInputShapes(){
    bool b = true;
    for(auto i = inputs.begin(); i<input.end(); i++){
        ofxBaseShaderNode &node = **i;
        b = b && 
    }
}*/

//when no more dependents, release resources
void ofxBaseShaderNode::decrementDependents(){
    dependent_count--;
    //cout<<"node "<<name<<" dependents = "<<dependent_count<<endl;
    if(dependent_count==0)
        releaseBuffer();
}

//infer uniform type from structure of ofParameterGroup g
//and set in shader from values in g.
//assumes shader->begin() has been called already
void ofxBaseShaderNode::setShaderParam(ofParameterGroup &g){
    size_t dim = g.size();

    //need to infer the uniform type from size of, and types in, this group
    //unfortunately g.getType(0) is weird and compiler dependent, so this.
    //weep for me, weep for c++
    string type = "";
    if(g.contains("0i"))
        type = "int";
    else if(g.contains("0f"))
        type = "float";

    string param_name = g.getName();

    // cout<<"node "<<name<<" setting "<<type<<" "<<dim<<" uniform "<<param_name<<endl;

    if(type=="float"){
        // cout<<g.getFloat("0f")<<endl;
        switch(dim){
            case 1:
                shader->setUniform1f(param_name, g.getFloat("0f"));
                break;
            case 2:
                shader->setUniform2f(param_name, g.getFloat("0f"), g.getFloat("1f"));
                break;
            case 3:
                shader->setUniform3f(param_name, g.getFloat("0f"), g.getFloat("1f"), g.getFloat("2f"));
                break;
            case 4:
                shader->setUniform4f(param_name, g.getFloat("0f"), g.getFloat("1f"), g.getFloat("2f"), g.getFloat("3f"));
        }
    }
    else if(type=="int"){
        switch(dim){
            case 1:
                shader->setUniform1i(param_name, g.getInt("0i"));
                break;
            case 2:
                shader->setUniform2i(param_name, g.getInt("0i"), g.getInt("1i"));
                break;
            case 3:
                shader->setUniform3i(param_name, g.getInt("0i"), g.getInt("1i"), g.getInt("2i"));
                break;
            case 4:
                shader->setUniform4i(param_name, g.getInt("0i"), g.getInt("1i"), g.getInt("2i"), g.getInt("3i"));
        }
    }
}

void ofxBaseShaderNode::draw(){
    if(!shader || !output)
        return;
    //cout<<"node "<<name<<" drawing"<<endl;
    int w = output->getWidth(), h = output->getHeight();
    shader->begin();
    //loop over params and bind to uniforms
    for(auto uncast_ptr:params){
        //ofParameterGroup &g = uncast_ptr->castGroup();
        auto ptr = static_pointer_cast<ofParameterGroup>(uncast_ptr);
        ofParameterGroup &g = *ptr;
        setShaderParam(g);
    }
    //set size parameter
    if(hasSizeParameter){
         // cout<<"node "<<name<<": autosetting size parameter to "<<w<<", "<<h<<endl;
        shader->setUniform2i("size", w, h);
    }
    //loop over inputs, then over textures and bind to uniforms
    size_t tex_count = 0;
    for(auto input_node:inputs){
        string param_name = textureInputs[tex_count];
        //loop over textures in each input
        for(size_t j=0; j<input_node->getNumOutputTextures(); j++){
            //cout<<"node "<<name<<": setting texture input "<<tex_count<<" from texture "<<j<<" of "<<(input_node->name)<<endl;
            ofTexture &tex = input_node->output->getTexture(j);
            shader->setUniformTexture(param_name, tex, tex_count);
            tex_count++;            
        }
    }
    //draw
    output->begin();    
    ofDrawRectangle(0,0,w,h);
    output->end();
    shader->end();
}

//override getDirty() to always return false
bool ofxConstantShaderNode::getDirty(){
    return false;
}
//override constructor to request a buffer
ofxConstantShaderNode::ofxConstantShaderNode(ofxFboAllocator *a, ofFbo::Settings s, string n)
: ofxBaseShaderNode(a, s, n, NULL){
    requestBuffer();
}
//override decrementDependents() to not release buffer
void ofxConstantShaderNode::decrementDependents(){
    dependent_count--;
}

//override to copy fbo content
void ofxConstantShaderNode::scaleResolution(float ws, float hs){
    settings.width*=ws;
    settings.height*=hs;
    if(output){
        ofFbo *output_old = output;
        releaseBuffer();
        requestBuffer();
        if(output!=output_old){
            output->begin();
            output_old->draw(0,0,output->getWidth(), output->getHeight());
            output->end();
        }
    }
}

ofxShaderGraph::ofxShaderGraph(ofxFboAllocator *a, ofFbo::Settings s){
    allocator = a;
    fbo_settings = s;
}

//delete all nodes
ofxShaderGraph::~ofxShaderGraph(){
    for(auto i:nodes){
        delete i.second;
    }
}

//unsafe
ofFbo& ofxShaderGraph::getFbo(string s){
    ofxBaseShaderNode *node = nodes.at(s);
    return *(node->output);
}

void ofxShaderGraph::swapFbos(string a, string b){
    ofxBaseShaderNode *a_node = nodes.at(a);
    ofxBaseShaderNode *b_node = nodes.at(b);
    ofFbo *temp = a_node->output;
    a_node->output = b_node->output;
    b_node->output = temp;
}

void ofxShaderGraph::setResolution(int w, int h){
    float ws = float(w)/fbo_settings.width;
    float hs = float(h)/fbo_settings.height;

    fbo_settings.width = w;
    fbo_settings.height = h;

    cout<<"scale all nodes by factor of "<<ws<<" horizontal, "<<hs<<" vertical"<<endl;

    for(auto i:nodes)
        (i.second)->scaleResolution(ws,hs);
}

/*template<typename T>
//void ofxShaderGraph::forwardParameter(const void *alias_param, T val){
void ofxShaderGraph::forwardParameter(T &val){
    //get name of alias param
    //look up name(s) of aliased param(s)
    //set aliased param(s) with value of alias param
}*/

void ofxShaderGraph::buildFromXml(ofXml x){
    unordered_set<string> root_names;

    x.reset();
    unsigned long n = x.getNumChildren();

    params.setName(x.getName());

    //create alias parameters
   /* if(x.setTo("aliases") && x.setToChild(0)){
        aliases.setName("aliases");
        do{
            string type = x.getName();
            string name = x.getAttribute("name");
            if(name=="")
                continue;
            if(type=="int"){
                const int init = x.getIntValue();
                ofParameter<int> p(name, init);
                p.addListener(this, &ofxShaderGraph::forwardParameter<int>);
                aliases.add(p);
            }
            else if(type=="float"){
                const float init = x.getFloatValue();
                ofParameter<float> p(name, init);
                p.addListener(this, &ofxShaderGraph::forwardParameter<float>);
                aliases.add(p);
            }else{
                cout<<"ofxShaderGraph warning: type "<<type<<" of alias parameter "<<name<<" not supported"<<endl;
            }
        }while(x.setToSibling());
        params.add(aliases);
    }*/

    //first pass, create all nodes and set defaults+aliases
    for(unsigned long i=0; i<n; i++){
        x.reset();
        x.setToChild(i);
        if(x.getName()!="node")
            continue;
        string name = x.getAttribute("name");
        if(name==""){
            cout<<"ofxShaderGraph warning: anonymous node in XML"<<endl;
            continue;
        }
        ofxBaseShaderNode *node;
        ofFbo::Settings node_fbo_settings = fbo_settings;
        if(x.setTo("scale")){
            float scale = x.getFloatValue();
            node_fbo_settings.width*=scale;
            node_fbo_settings.height*=scale;
            x.setToParent();
        }
        string shader_name = x.getAttribute("shader");
        if(shader_name == ""){
            //shaderless node
            node = new ofxConstantShaderNode(allocator, node_fbo_settings, name);
        }
        else{
            ofShader *shader = new ofShader();
            stringstream shader_path;
            shader_path<<ofToDataPath("../../src/shader/")<<shader_name;
            shader->load(shader_path.str());
            if(!shader->isLoaded()){
                cout<<"ofxShaderGraph warning: shader "<<shader_name<<" failed to load from path "<<shader_path.str()<<endl;
            }
            else{
                cout<<"ofxShaderGraph: loaded shader "<<shader_name<<endl;
            }
            node = new ofxBaseShaderNode(allocator, node_fbo_settings, name, shader);
        }
        //grab all of the new node's parameters
        params.add(node->getParameterGroup());
        //put it in the map from names to nodes
        nodes.insert(pair<string, ofxBaseShaderNode*>(name, node));
        //everything goes in root_names, to be removed when is appears as an input
        root_names.insert(name);

        cout<<"ofxShaderGraph: inserted node "<<name<<endl;

        //set defaults
        if(x.setTo("defaults") && x.setToChild(0)){
            do{
                string param_type = x.getName();
                string param_name = x.getAttribute("name");
                if(param_type=="float"){
                    float v = x.getFloatValue();
                    node->setParameter(param_name, v);
                }else if(param_type=="int"){
                    int v = x.getIntValue();
                    node->setParameter(param_name, v);
                }else{
                    cout<<"ofxShaderGraph warning: parameter type "<<param_type<<" unsupported"<<endl;
                }
            }while(x.setToSibling());
            x.setToParent();
            x.setToParent();
        }

        //set aliases
        /*if(x.setTo("aliases") && x.setToChild(0)){
            do{
                string param_type = x.getName();
                string param_name = x.getAttribute("name");
                string alias_name = x.getValue();
                if(param_type=="float"){
                    auto alias_param = aliases.getFloat(alias_name);
                    node->setParameterAlias(param_name, alias_param);
                }else if(param_type=="int"){
                    auto alias_param = aliases.getInt(alias_name);
                    node->setParameterAlias(param_name, alias_param);
                }else{
                    cout<<"ofxShaderGraph warning: aliased parameter type "<<param_type<<" unsupported"<<endl;
                }
            }while(x.setToSibling());
            x.setToParent();
            x.setToParent();
        }*/
    }

    //second pass, fill in inputs for each node and remove all referenced nodes from root_names
    for(unsigned long i=0; i<n; i++){
        x.reset();
        x.setToChild(i);
        if(x.getName()!="node")
            continue;
        string name = x.getAttribute("name");
        if(name=="")
            continue;
        ofxBaseShaderNode *node = nodes[name];

        //set inputs
        if(!x.setTo("inputs"))
            continue;
        if(!x.setToChild(0))
            continue;
        do{
            string input_name = x.getValue();
            try{
                ofxBaseShaderNode *input = nodes.at(input_name);
                node->inputs.push_back(input);
                input->num_dependents++;
                root_names.erase(input_name);
                cout<<"ofxShaderGraph: connected node "<<input_name<<" to node "<<name<<endl;
            }
            catch(exception e){
                cout << "ofxShaderGraph warning: node "<<name<<" lists input "<<input_name<<" which does not exist"<<endl;
            }
        }while(x.setToSibling());
    }

    //populate roots
    for(string root_name:root_names){
        ofxBaseShaderNode *root_node = nodes.at(root_name);
        roots.insert(root_node);
        cout<<"ofxShaderGraph: node "<<root_name<<" is a root"<<endl;
    }

}

//set all nodes to dirty
//call update on each node with no dependents
void ofxShaderGraph::update(){
    for(auto i:nodes){
        ofxBaseShaderNode *node = i.second;
        node->setDirty(true);
    }
    for(auto node:roots){
        node->update();
    }
}

//=============================================================================

void ofApp::setupConstants()
{
    ofxXmlSettings s;
    s.loadFile(ofToDataPath("../../src/settings.xml"));

    //OSC
    local_port = s.getValue("osc_local_port", 6666);
    remote_host = s.getValue("osc_remote_host", "localhost");
    remote_port = s.getValue("osc_remote_port", 6667);

    //constants for multi-scale processing
    num_scales = 1;
    scale_factor = 2;
    discard_largest_scale = false;

    //target video refresh rate
    frame_rate = s.getValue("video_frame_rate", 60);

    //buffer sizes
    int base_width = s.getValue("base_width", 1920);
    int base_height = s.getValue("base_height", 1080);
    oversample_waveform = s.getValue("oversample_waveform", 1);
    int undersample_terrain = s.getValue("undersample_terrain", 10);

    fullscreen = false;
    if(fullscreen){
        //if fullscreen is initially true, adapt to screen size
        window_width = ofGetScreenWidth();
        window_height = ofGetScreenHeight();
    }
    else{
        window_width = base_width/4;
        window_height = base_height/4;
    }

    //initial master resolution
    realtime_width = window_width;
    realtime_height = window_height;

    //size of readback buffer for video streaming, also master resolution during render mode
    render_width = base_width;
    render_height = base_height;

    //size of high-precision buffer read back to video volume
    readback_width = base_width/undersample_terrain;
    readback_height = base_height/undersample_terrain;

    //audio constants
    audio_channels = 2;
    audio_sample_rate = s.getValue("audio_sample_rate", 48000);
    audio_delay = s.getValue("audio_delay", .5);
    audio_device = s.getValue("audio_device", 3);
    audio_buffer_size = s.getValue("audio_buffer_size", 256);

}
void ofApp::setupParameters()
{
 //set up OSC controllable parameters from params.txt (which is in Max's goofy coll format for now)
    params.setName("params");
    string params_text = ofBufferFromFile(ofToDataPath("../../src/params.txt")).getText();
    vector<string> params_lines = ofSplitString(params_text, "\n");
    if(params_lines.size()==1) params_lines = ofSplitString(params_text, "\r");
    for(auto line:params_lines){
        vector<string> param_tokens = ofSplitString(line, " ");
        if(param_tokens.size()<3) break;
        string param_name = param_tokens[1];
        float param_val = atof(param_tokens[2].c_str());
        params.add(ofParameter<float>().set(param_name, param_val));

        cout<<"added OSC parameter "<<param_name<<" with initial value "<<param_val<<endl;
    }

    //set up parameters which are not floats and/or have callbacks separately for now
	//params.add(seed.set("seed",0));
	//seed.addListener(this, &ofApp::initParams);

    params.add(forward_graph->params);

    //I don't really understand the role of ofxGui in managing ofParameters/ofParameterGroups, something to return to
    gui.setup(params);
	sync.setup((ofParameterGroup&)gui.getParameter(),local_port,remote_host,remote_port);
}

void ofApp::setupGraph(ofxShaderGraph *&g, string path){
    g = new ofxShaderGraph(&fbo_allocator, fbo_params);
    ofXml x;
    x.load(path);
    g->buildFromXml(x);
}

void ofApp::setupGL(){

    ofDisableArbTex();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableDepthTest();
    ofDisableAntiAliasing();
    ofSetTextureWrap(GL_REPEAT, GL_REPEAT);

    fbo_params = ofFbo::Settings(); //needs to come after the ofDisable* above
    fbo_params.width = realtime_width;
    fbo_params.height = realtime_height;
    fbo_params.internalformat = GL_RGBA16F;
    fbo_params.useDepth = false;
    fbo_params.useStencil = false;
    fbo_params.wrapModeHorizontal = GL_REPEAT;
    fbo_params.wrapModeVertical = GL_REPEAT;
    fbo_params.numSamples = 0;

    ofSetVerticalSync(true); //does not appear to be working on windows w/ of 0.8.4

    ofSetFullscreen(fullscreen);
    ofSetWindowShape(window_width, window_height);
}

void ofApp::setupGlobals(){
    mute = true;

    realtime = true;
    use_camera = false;
    recording = false;    

    //integrator = 0;

    frame = 0;


    cycle_disp_mode = 0;

    disp_mode = 0;

     //set the sequence of display modes to cycle through
    display_sequence.push_back(AVFBDM_Color);
    //display_sequence.push_back(AVFBDM_Monochrome);
    display_sequence.push_back(AVFBDM_Agents);
    //display_sequence.push_back(AVFBDM_Displacement);
    display_sequence.push_back(AVFBDM_Filter);
  
}

void ofApp::setupAudio(){
    int frames_to_keep = 48;
    vwt = new ofxVideoWaveTerrain(frames_to_keep, audio_sample_rate, audio_delay);

    ofSoundStreamListDevices();
    ss.setDeviceID(audio_device);
    ss.setup(this, audio_channels, 0, audio_sample_rate, audio_buffer_size, 4);
}

void ofApp::setup(){
    ofEnableDataPath();

    setupConstants();

    setupGlobals();

    setupGL();

    setupGraph(forward_graph, "../../src/graph/forward.xml");

    setupParameters();

    cout<<params<<endl;

    //allocateFbos();

    //fill(agent_fbo, ofFloatColor(0,0,0,1));

    initRandom(forward_graph->getFbo("state00"), 0);
    mov(forward_graph->getFbo("state00"), forward_graph->getFbo("state00_lp"));

    setupAudio();

    if(use_camera){
        camera.setVerbose(true);
        camera.listDevices();
        camera.initGrabber(render_width, render_height);
    }

    cout<<"setup complete"<<endl;
}

void ofApp::close(){
    ss.close();
    delete forward_graph;
    delete vwt;
    if(audio_file_size>0)
        endRenderMode();
}

void ofApp::setResolution(int w, int h){
    fbo_params.width = w;
    fbo_params.height = h;
    forward_graph->setResolution(w,h);
    fbo_allocator.destroy_available();
}

void ofApp::mov(ofFbo &src, ofFbo &dest){
    dest.begin();
    src.draw(0,0,dest.getWidth(), dest.getHeight());
    dest.end();
}

void ofApp::fill(ofFbo &dest, ofFloatColor c, ofBlendMode mode){
    ofPushStyle();
    ofFill();
    ofEnableBlendMode(mode);
    ofSetColor(c);
    dest.begin();
    ofDrawRectangle(0, 0, dest.getWidth(), dest.getHeight());
    dest.end();
    ofPopStyle();
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

    //get input nodes
    ofFbo &y_fbo = forward_graph->getFbo("state00");
    ofFbo &lp_fbo = forward_graph->getFbo("state00_lp");
    ofFbo &agent_fbo = forward_graph->getFbo("agent_state");
    ofFbo &agent_input_fbo = forward_graph->getFbo("agent_input");

    int w = y_fbo.getWidth();
    int h = y_fbo.getHeight();

    vwt->setAgentRate(params.getFloat("agent_rate"));
    vwt->setMomentumTime(params.getFloat("momentum_time"));
    vwt->setPathJitter(params.getFloat("path_jitter"));
    vwt->setAgentCombFrequency(params.getFloat("comb_freq"));
    vwt->getVideoVolume()->setAspectRatio(double(w)/h);

    //draw agent path
    int aw = agent_fbo.getWidth(), ah = agent_fbo.getHeight();

    fill(agent_input_fbo, ofFloatColor(0,0,0,0), OF_BLENDMODE_DISABLED);
    agent_input_fbo.begin();
    vwt->draw(0, 0, aw, ah);
    agent_input_fbo.end();//InPlace();


    //main video process
    forward_graph->update();

    //feedback
    forward_graph->swapFbos("state00_new", "state00");
    forward_graph->swapFbos("state00_lp_new", "state00_lp");
    forward_graph->swapFbos("agent_state_new", "agent_state");

    //get outputs
    ofFbo &readback_fbo = forward_graph->getFbo("state00_readback");
    ofFbo &display_fbo = forward_graph->getFbo("state00_display");

    //branch on disp_mode and draw to screen
    int ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    
    switch(display_sequence[disp_mode]){
        case AVFBDM_Color:
            display_fbo.draw(0,0,ww,wh);
            //mov(y_fbo, display_fbo);
            //b2u(display_fbo, display_fbo);
            break;
        case AVFBDM_Monochrome:
            /*beginShader("display");
            setShaderParam("size", display_fbo.getWidth(), display_fbo.getHeight());
            setShaderParam("state", y_fbo.getTextureReference(), 0);
            display_fbo.begin();
            ofDrawRectangle(0, 0, display_fbo.getWidth(), display_fbo.getHeight());
            display_fbo.end();
            endShader();*/
            break;
        case AVFBDM_Agents:
            agent_fbo.draw(0,0,ww,wh);
            //resample(agent_fbo, display_fbo);
            break;
        case AVFBDM_Pyramid:
            /*if(!disp_scale)
                mov(y_pyramid[disp_scale], display_fbo);
            else
                resample(y_pyramid[disp_scale], display_fbo);
            b2u(display_fbo, display_fbo);*/
            break;
        case AVFBDM_Displacement:
            /*if(!disp_scale)
                mov(yprime_pyramid[disp_scale], display_fbo);
            else
                resample(yprime_pyramid[disp_scale], display_fbo);
            b2u(display_fbo, display_fbo);*/
            break;
        case AVFBDM_Filter:
            lp_fbo.draw(0,0,ww,wh);
            /*mov(lp->getState(), display_fbo);
            b2u(display_fbo, display_fbo);*/
            break;
    }
/*
    beginShader("torus_shift");
    setShaderParam("state", display_fbo.getTextureReference(0), 0);
    setShaderParam("shift", draw_offset);
    setShaderParam("size", ww, wh);
    display_fbo.draw(0,0,ww,wh);
    endShader();
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
*/

    ofFloatPixels *pix = new ofFloatPixels();
    readback_fbo.readToPixels(*pix,0);
    vwt->insert_frame(pix);


    //if in render mode, compute audio in the draw loop
    //delay should be equal to frame duration
    /*if(!realtime){
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
*/
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

    //}

    frame++;
    drawing = true;
}

void ofApp::initRandom(ofFbo &target, int seed){
    printf("init random %d\n", seed);
    ofSeedRandom(seed);
    ofFloatPixels newState;
    int w = target.getWidth();
    int h = target.getHeight();
    newState.allocate(w, h, OF_PIXELS_RGB);
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
    int w = render_width, h = render_height;

    stringstream bitrate;
    bitrate<<int(h*w*frame_rate*24/1000)<<"k";
    cout<<bitrate.str()<<endl;
    vr.setVideoBitrate(bitrate.str());

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
    //with bit rate set as above, slow memory growth + spike at end, decent quality

    //vr.setVideoCodec("libx264"); //video likes to freeze part way through, lossy, memory fills fairly quickly
    //vr.setVideoCodec("libx264rgb"); //same

    //vr.setVideoCodec("jpeg2000"); //memory fills extremely fast, hangs at end, video broken
    //vr.setVideoCodec("libopenjpeg"); //even more garbage


    vr.setFfmpegLocation("\"C:\\Program Files (x86)\\ffmpeg\\bin\\ffmpeg.exe\"");

    string fname = ofGetTimestampString();

    vr.setup(fname, w, h, frame_rate, audio_sample_rate, audio_channels);

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
    for(size_t i=0; i<nsamps; i++){
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
    stringstream file_name;
    cur_save_dir = ofDirectory(ofGetTimestampString());
    cur_save_dir.create();
    file_name << cur_save_dir.path()<<ofToDataPath("/audio.wav");

    openAudioFile(file_name.str());
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
        /*cout<<"reload shaders"<<endl;
        loadShaders();
        */
    }
    if(key=='p'){
        disp_scale = ofWrap(disp_scale+1, 0, num_scales);
        cout<<"display scale: "<<disp_scale<<endl;
    }
    if(key=='s'){
        /*ofPixels p;
        display_fbo.readToPixels(p);
        ofImage screenshot = ofImage(p);
        stringstream fname;
        fname<<"ss-"<<ofGetTimestampString()<<".png";
        screenshot.saveImage(fname.str());*/
    }
    if(key=='r'){
        initRandom(forward_graph->getFbo("state00"), frame);
        mov(forward_graph->getFbo("state00"), forward_graph->getFbo("state00_lp"));
        //initRandom(lp->getState(), seed+1);
        //initRandom(dyn->getState(), seed);
        vwt->scramble();
        //ofPushStyle();
        //ofSetColor(0,0,0);
        //agent_fbo.begin();
        //ofDrawRectangle(0,0,agent_fbo.getWidth(),agent_fbo.getHeight());
        //agent_fbo.end();
        //ofPopStyle();
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
        "d - change display mode"<<endl<<
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
