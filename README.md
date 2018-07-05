# smart-home
This project is about home automation. I use this in my home to control the lighting with my phone or voice commands.

I run these modules on my Raspberry Pi 3 and Android phone.

## Modules

### milight-remote
This is a low level modul that can control my MiLight led-strips. It is a basic reimplementation of the original remote controller of the leds.

### speech-recognition
This module uses a microphone to listen to the environment constantly and trigger events when certain hotwords are detected. For example it can call the `milight-remote` module to turn on the lights when it hears the hotword "turn the lights on"

### web
A web application that exposes the functionality of the `milight-remote` module to the network.

### android-client
An Android client that is communicating with the `web` module.