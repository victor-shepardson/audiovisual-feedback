# audiovisual-feedback
realtime audiovisual feedback experiments with openFrameworks

## dependencies
[ofxVideoWaveTerrain](https://github.com/victor-shepardson/ofxVideoWaveTerrain)

[ofxFastFboReader](https://github.com/neilmendoza/ofxFastFboReader)

[ofxVideoRecorder](https://github.com/DomAmato/ofxVideoRecorder), or [my fork for win7/cb/of0.8.4](https://github.com/victor-shepardson/ofxVideoRecorder)

[ffmpeg](https://www.ffmpeg.org/)

## build

note that audio device and ffmpeg location are currently hardcoded in setup()

builds with Win 7 / Code::Blocks / oF 0.8.4, needs support for opengl 3

## use

see params.txt for names of parameters which can be controlled by OSC messages

or, use max for live device to send OSC

compile rtaudio with ASIO support (using msys) to use w/ jackaudio on windows

## notes, ideas, todo

shaders todo:

fix crash on exit (lol)

try to disable all alpha and do 4 channel feedback?
look into abstracting away number of input/output channels? shader generation?

ideas for whole-image features:
	- motion vector between frames using phase correlation
	- color principal components
	- mean+stddev gradient direction
	- mean+stddev color

think in terms of temporal and spatial filters
	- bandpass filter in time: approach a short time average, flee a longer average

	how best to make temporal filter spatially varying? (or color dependent)

TODO:
 - get m4L device automation curves working right
 - general purpose speedhack parameter?
 - too many buffers for laptop at 1080p (fix, test on workstation) (why is the lazy buffer allocation not working right?)
 - 