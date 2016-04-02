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
