#include <cstdlib>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h>

using namespace std;

#include <RF24/RF24.h>
#include "NRF24MiLightRadio.h"
#include "MiLightRadioConfig.h"

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ);

//cmd line arguments
string room, type, button;

class MiRemote {
public:
  MiRemote(NRF24MiLightRadio* radio, uint16_t deviceId){
    this->deviceId = deviceId;
    this->radio = radio;
    this->radio->begin();
  }
  ~MiRemote(){
    delete this->radio;
  }
  void press(string button){
    cout << "button '" << button << "' pressed on remote:" << deviceId << endl;
    if(button == "on"){
      turnOn();
    }
    else if(button == "off"){
      turnOff();
    }
    else if(button == "brightnessUp"){
      brightnessUp();
    }
    else if(button == "brightnessDown"){
      brightnessDown();
    }
    else{
      cout << "button '" << button << "' is not supported by remote " << deviceId << endl;
    }
  }

  void turnOn(){
    this->sendPacket(0x05);
  }
  void turnOff(){
    this->sendPacket(0x09);
  }
  void brightnessUp(){
    this->sendPacket(0x0C);
  }
  void brightnessDown(){
    this->sendPacket(0x04);
  }

private:
  void sendPacket(uint8_t command){
    size_t packetPtr = 0;
    uint8_t packet[7]; //[config.packetLength];
    
    packet[packetPtr++] = 0x5A; //CCT_PROTOCOL_ID;
    packet[packetPtr++] = deviceId >> 8; // Byte 2 and 3: Device ID
    packet[packetPtr++] = deviceId & 0xFF;
    packet[packetPtr++] = 0x00; // Byte 4: Zone
    packet[packetPtr++] = command; // Byte 5: Bulb command
    packet[packetPtr++] = (uint8_t)time(NULL); //sequenceNum++; // Byte 6: Packet sequence number 0..255
    packet[packetPtr++] = 0; // Byte 7: checksum over previous bytes, including packet length = 7

    uint8_t checksum;
    checksum = 7; // Packet length is not part of packet
    for (uint8_t i = 0; i < 6; i++) {
      checksum += packet[i];
    }
    packet[6] = checksum;

    for (int i = 0; i < 40; i++) {
      radio->write(packet, 7);
      usleep(50);
    }
  }
  NRF24MiLightRadio* radio;
  uint16_t deviceId;
};

MiRemote getRemote(string room, string type){
  MiLightRadioConfig* radioConfig = NULL;
  if(type == "white") {
    radioConfig = &MiLightRadioConfig::ALL_CONFIGS[1];
  } 
  else if (type == "rgb") {
    radioConfig = &MiLightRadioConfig::ALL_CONFIGS[0];
  }
  else {
    cout << "error! unsupported type:" << type << endl;
    exit(-3);
  }

  NRF24MiLightRadio* mlr = new NRF24MiLightRadio(radio, *radioConfig);

  uint16_t deviceId = 0x14 << 8 | 0x41;
  if(room == "living-room") {
    deviceId = 0x14 << 8 | 0x41;
  } 
  else if (room == "bedroom") {
    deviceId = 0x87 << 8 | 0x88;
  }
  else {
    cout << "error! unknown device id for room:" << room << endl;
    exit(-4);
  }
  return MiRemote(mlr, deviceId);
}

void parseArgs(int argc, char **argv)
{
  int opt;
  while ((opt = getopt(argc,argv,"r:t:b:")) != EOF){
    switch(opt)
    {
        case 'r': room = optarg; break;
        case 't': type = optarg; break;
        case 'b': button = optarg; break;
        default: abort();
    }
  }
}

void validateArgs()
{
  if(room.empty() || type.empty() || button.empty()){
    cout << "Arguments -r -t -b are required!" << endl;
    exit(-2);
  }
  cout << "room:" << room << endl;
  cout << "type:" << type << endl;
  cout << "button:" << button << endl;
}

int main(int argc, char **argv)
{
  parseArgs(argc, argv);
  validateArgs();

  MiRemote remote = getRemote(room, type);
  remote.press(button);

  return 0;
}
