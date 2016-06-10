# audiovisual-feedback
realtime audiovisual feedback experiments with openFrameworks

## dependencies

openFrameworks 0.9.3

[ofxVideoWaveTerrain](https://github.com/victor-shepardson/ofxVideoWaveTerrain)

neilmendoza's fork of [ofxFastFboReader](https://github.com/neilmendoza/ofxFastFboReader) for float formats

my fork of [ofxVideoRecorder](https://github.com/victor-shepardson/ofxVideoRecorder) for high-quality video recording

[ffmpeg](https://www.ffmpeg.org/)

## build

just `make`

known to build with Win 7 / msys2 / oF 0.9.3

## use

graphics driver must support opengl 3.2

set audio device and ffmpeg location in `settings.xml`

define a shader graph in `forward.xml`

press `h` for a listing of single-key commands

OSC control is currently in transition. Some OSC controllable parameters can be found in `params.txt`. All shader uniforms are automatically exposed to control at address `params/nodename/uniformname/elementindexandtype` e.g. `params/state00_disp/warp_color/0f`

fb-control.amxd is a basic GUI for sending OSC from Max 7 or Ableton Live.

note that ofxOscParameterSync can crash when it get an OSC message which doesn't match any ofParameter.