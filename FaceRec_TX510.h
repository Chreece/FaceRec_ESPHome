#include "esphome.h"

using namespace esphome;

#define SYNCWORD1 0xEF
#define SYNCWORD2 0xAA
// Commands:
#define IDENT 0x12  // Identify
#define REGIS 0x13  // Register Face
#define DEL 0x20  // Delete Face
#define CLR 0x21  // Clear All Faces
#define BACKLIT 0xC0  // Backlight control
#define DISPL 0xC1  // Display Control
#define FLSH 0xC2 // Flash control
#define VERS 0x30 // Query Version
#define REBT 0xC3 // Reboot
#define BAUDRT 0x51 // Change Baudrate
#define NUMBERREC 0xC4  // Query number recorded entries
#define TURNON 0x01
#define TURNOFF 0x00
// Baudrates:
#define BAUD9600 0x00
#define BAUD19200 0x01
#define BAUD38400 0x02
#define BAUD57600 0x03
#define BAUD115200 0x04
// Responds from TX510
#define SUCC 0x00 // Successful
#define FAIL 0x01 // Failed
#define ANGL 0x03 // Angle failure
#define FAIL2D 0x06 // 2D failure
#define FAIL3D 0x07 // 3D failure
#define NOMATCH 0x08  // No mactching entry
#define EXIST 0x09  // Entry already exist

class UARTSensor : public Component, public UARTDevice {
  public:
  UARTSensor(UARTComponent *parent) : UARTDevice(parent) {}
  TextSensor* presence_sensor = new TextSensor();
  TextSensor* text_userid = new TextSensor();
  TextSensor* text_reply = new TextSensor();

  void setup() {
  }

std::vector<int> bytes;

void loop() override
{
  static int MAX_PACKET_SIZE = 45;
  static int packetIndex = 0;
  static bool receivingData = false;
  static byte receivedParity = 0;

  while (available()) {

    bytes.push_back(read());

    if (receivingData) {
      if (bytes[packetIndex] == receivedParity && packetIndex > 7) {
        processPacket();
        receivedParity = 0;
        packetIndex = 0;
        receivingData = false;
        bytes.clear();
      } else {
        receivedParity += bytes[packetIndex];
        packetIndex++;
      }
      
    } else if (packetIndex <= MAX_PACKET_SIZE) {

      if (bytes[0] == SYNCWORD1 && packetIndex == 0) {
        packetIndex = 1;
      } else if (bytes[1] == SYNCWORD2 && packetIndex == 1) {
        packetIndex++;
        receivingData = true;
      }
      else {
        receivingData = false;
        packetIndex = 0;
        bytes.clear();
      }
    } else {
      packetIndex = 0;
      receivedParity = 0;
      receivingData = false;
      bytes.clear();
    }
  }
}

void processPacket() {
  switch(bytes[7]){
    case IDENT:
    case REGIS:
    case DEL:
    case CLR:
    case DISPL:
    case BACKLIT:
    case FLSH:
    case REBT:
    case NUMBERREC:
      switch(bytes[8]){
        case SUCC:
          id(text_reply).publish_state("Success");
          if(bytes[7] == IDENT || bytes[7] == REGIS || bytes[7] == NUMBERREC) {
            byte userId;
            userId = bytes[9] + bytes[10];
            if(bytes[7] != NUMBERREC){ 
              id(text_userid).publish_state(String(userId).c_str());
              id(presence_sensor).publish_state("on");
            } else {
              id(text_reply).publish_state(String(userId).c_str());
            }
          }
          break;
        default:
          if(bytes[7] == IDENT || bytes[7] == REGIS) {
            id(text_userid).publish_state("None");
            id(presence_sensor).publish_state("off");
          }
          switch(bytes[8]){
            case FAIL:
              if (bytes[7] == DISPL ||
                    bytes[7] == BACKLIT ||
                    bytes[7] == FLSH){
                id(text_reply).publish_state("Turned OFF");
              } else {
                id(text_reply).publish_state("Failed");
              }
              break;
            case FAIL2D:
              id(text_reply).publish_state("Failed 2D");
              break;
            case FAIL3D:
              id(text_reply).publish_state("Failed 3D");
              break;
            case ANGL:
              id(text_reply).publish_state("Failed Angle");
              break;
            case NOMATCH:
              id(text_reply).publish_state("Failed No Match");
              break;
          }
          break;
      }
      break;
    case VERS:
      byte version[20];
      int indx = 0;
      for(int i = 16; i < bytes.size()-1; i++){
        version[indx] = bytes[i];
        indx++;
      }
      String ver = byteArrayToAsciiString(version, indx);
      id(text_reply).publish_state(ver.c_str());
  }

}

  String byteArrayToAsciiString(byte* byteArray, int arraySize) {
  String result = "";

  for (int i = 0; i < arraySize; i++) {
    result += char(byteArray[i]);
  }

  return result;
}

};
