# speech-recognition
This module uses a microphone to listen to the environment constantly and trigger events when certain hotwords are detected. For example it can call the `milight-remote` 
module to turn on the lights when it hears the hotword "turn the lights on" It uses Snowboy to detect hotwords, the _snowboydetect.so library is precompiled for Raspberry 
Pi, and can be downloaded from [here](http://docs.kitt.ai/snowboy/). I tested it only on the Raspberry Pi 3 model B.
### Requirements
* PyAudio 0.2.9
