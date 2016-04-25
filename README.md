# audiovisual-feedback
realtime audiovisual feedback experiments with openFrameworks

## dependencies
[ofxVideoWaveTerrain](https://github.com/victor-shepardson/ofxVideoWaveTerrain)

neilmendoza's fork of [ofxFastFboReader](https://github.com/neilmendoza/ofxFastFboReader) for float formats

my fork of [ofxVideoRecorder](https://github.com/victor-shepardson/ofxVideoRecorder) for high-quality video recording

[ffmpeg](https://www.ffmpeg.org/)

## build

`make`

known to build with Win 7 / msys2 / oF 0.9.3

graphics driver must support opengl 3.2

## use

set audio device and ffmpeg location in `settings.xml`

define a shader graph in `forward.xml`

currently, the names of certain nodes are hardcoded in ofApp.cpp. In the future it will be possible to annotate the graph with the names of nodes to be swapped after each cycle or conditionally drawn to the screen.

press `h` for a listing of single-key commands

OSC control is currently in transition. Some OSC controllable parameters can be found in `params.txt`. All shader uniform are exposed to control at address `params/nodename/uniformname/elementindexandtype` e.g. `params/state00_disp/warp_color/0f`

fb-control.amxd is a basic GUI for sending OSC from Max 7 or Ableton Live.
