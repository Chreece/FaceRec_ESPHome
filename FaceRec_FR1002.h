#include "esphome.h"

using namespace esphome;

#define SYNCWORD1 0xEF
#define SYNCWORD2 0xAA
// Commands:
#define IDENT 0x12  // Identify
#define REGIS 0x13  // Register User
#define DEL 0x20  // Delete UserID
#define CLR 0x21  // Clear All UserIDs
#define VERS 0x30 // Query Version
#define REBT 0xC3 // Reboot
#define NUMBERREC 0xC4  // Query number recorded entries
#define TURNON 0x01
#define TURNOFF 0xED
// Responds from FR1002
//#define SUCC 0x00 // Successful
//#define FAIL 0x01 // Failed
//#define ANGL 0x03 // Angle failure
//#define FAIL2D 0x06 // 2D failure
//#define FAIL3D 0x07 // 3D failure
//#define NOMATCH 0x08  // No mactching entry
//#define EXIST 0x09  // Entry already exist
#define SUCC 0x00 // Success
#define FAIL 0x01 // The module rejected the command
#define ALGO 0x02 // The entry / matching algorithm has been terminated
#define ERROR 0x03 // Send message error
#define CAMFAIL 0x04 // The camera failed to open
#define UNKN 0x05 // Unknown error
#define INVALID 0x06 // Invalid parameters
#define MEMOUT 0x07 // Run out of memory
#define NOUSER 0x08 // No entered users
#define MAXUSER 0x09 // Entry exceeds the maximum number of users
#define FACEENTERED 0x0A // The face has been entered
#define VIVIAN 0x0C // Vivian detection failed
#define TIMEOUT 0x0D // Enter or unlock the timeout
#define ENCFAIL 0x0E // Encryption chip licensing failed
#define READFAIL 0x13 // Failed to read the file
#define WRITEFAIL 0x14 // Failed to write files
#define NOTENC 0x15 // The communication protocol is not encrypted
#define RGBNOTRED 0x17 // The RGB images are not ready
#define INVALIDID 0xFD // Invalid information ID, key is not written
#define DETECTERROR 0xFE // Detection error
#define CODEERR 0xFF // Coding error


class UARTSensor : public Component, public UARTDevice {
  public:
  UARTSensor(UARTComponent *parent) : UARTDevice(parent) {}
  TextSensor* presence_sensor = new TextSensor();
  TextSensor* text_userid = new TextSensor();
  TextSensor* text_reply = new TextSensor();

  void setup() {
    ESP_LOGD("custom", "UARTSensor setup...");
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
  ESP_LOGD("custom", "Processing packet...");
  switch(bytes[7]){
    case IDENT:
    case REGIS:
    case DEL:
    case CLR:
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
              String sum = String(userId);
              String text = "Sum: " + String(userId);
              if( sum != "0"){
                for(int i = 11; i < bytes.size() - 2; i+=2){
                  byte id = bytes[i] + bytes[i+1];
                  text+= ", User ID: " + String(id);
                }
              }
              id(text_reply).publish_state(text.c_str());
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
              id(text_reply).publish_state("The module rejected the command");
              break;
            case ALGO:
              id(text_reply).publish_state("The entry / matching algorithm has been terminated");
              break;
            case ERROR:
              id(text_reply).publish_state("Send message error");
              break;
            case CAMFAIL:
              id(text_reply).publish_state("The camera failed to open");
              break;
            case UNKN:
              id(text_reply).publish_state("Unknown error");
              break;
            case INVALID:
              id(text_reply).publish_state("Invalid parameters");
              break;
            case MEMOUT:
              id(text_reply).publish_state("Run out of memory");
              break;
            case NOUSER:
              id(text_reply).publish_state("No entered users");
              break;
            case MAXUSER:
              id(text_reply).publish_state("Entry exceeds the maximum number of users");
              break;
            case VIVIAN:
              id(text_reply).publish_state("Vivian detection failed");
              break;
            case TIMEOUT:
              id(text_reply).publish_state("Enter or unlock the timeout");
              break;
            case ENCFAIL:
              id(text_reply).publish_state("Encryption chip licensing failed");
              break;
            case READFAIL:
              id(text_reply).publish_state("Failed to read the file");
              break;
            case WRITEFAIL:
              id(text_reply).publish_state("Failed to write files");
              break;
            case NOTENC:
              id(text_reply).publish_state("The communication protocol is not encrypted");
              break;
            case RGBNOTRED:
              id(text_reply).publish_state("The RGB images are not ready");
                break;
            case INVALIDID:
              id(text_reply).publish_state("Invalid information ID, key is not written");
                break;
            case DETECTERROR:
              id(text_reply).publish_state("Detection error");
                break;
            case CODEERR:
              id(text_reply).publish_state("Coding error");
                break;
          }
          break;
      }
      break;
        case VERS: {
          std::vector<byte> version;
          for (int i = 16; i < bytes.size() - 1; i++) {
              version.push_back(bytes[i]);
          }
          String ver = byteArrayToAsciiString(version.data(), version.size());
          id(text_reply).publish_state(ver.c_str());
          break;
        }
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