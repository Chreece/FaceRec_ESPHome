# FaceRec (BETA) - Local, Fast, Reliable Face Recognition project.

# DISCLAIMER
The module can be easily manipulated to erase and add faces with physical buttons. Please make sure you don't expose them.
DON'T USE THIS PROJECT TO CONTROL SECURITY MEASURES AS LOCKS etc.

# Description
It utilizes the [TX510](https://www.hlktech.net/index.php?id=1077&cateid=761) kit module that contains a 3d Camera, a display and a flash.
This All-In-One kit can autonomously, register and then recognise faces.
*In the link there is only the chip, you need the KIT that has everything*
It communicates via Serial to an ESP and via ESPHome can be controllable from Home Assistant.

![20230523_095931](https://github.com/Chreece/FaceRec_ESPHome/assets/68458228/92994688-31d8-4c0b-8c53-ebfb890ee00a)

## Connection Diagram:
TX510 | ESP | 
-------- | -------- |
5V | VCC | 
GND | GND |
P2: USI1_SD0 | RX |
P2: USI1_SCLK | TX  | 

Entities exposed:
*  Switch for turning on/off: 
   * Diplay
   * Backlight
   * Onboard LED Flash
*  Buttons: 
   * Registration and Identification of faces
   * Query the number of registered faces
   * Delete a UserID # needs an input_number helper in HA with name: `input_number.hx510_input`
   * Clear all the registered faces
   * Query the TX510 version
   * Restart MCU (ESP)
   * Reboot TX510
*  Text Sensors:
   * User ID that recognised or registered. The UserIds start from 0 (first face registered = 0)
   * Responce from the TX510 on every action
*  Binary Sensor:
   * Presence sensor that turns on when the identification process was successful
    
## Usage
  * Register a UserID (Face)
    * Look in front the camera and press the Register User Button in HA or S1 button on the TX510 kit
    * You get on screen and ha responce: `Failed` with a reason or `Success` with the UserID that has been saved (starting with 0)
  * Identify a User
    * Look in front the camera and press the Start Identify Button in HA or S2 button on the TX510 kit
    * You get on screen and ha responce: `Failed` with a reason or `Success` with the UserID that has been recognised
  * Delete a UserID
    * Select the UserID (0-999) from the entity `input_number.hx510_input` in HA
    * Press the Delete UserID button on HA
    * You get on screen and ha responce: `Failed` or `Success`
  * Query Sum and UserIDs saved on TX510:
    * Press Query Num. Reg. UserIDs Button on HA
    * You get on screen and ha responce: `Sum:` with the summary of all registered users and the UserIDs that have been registered

This version runs on ESPHome firmware, you can check the version running on [BTHome](https://github.com/Chreece/FaceRec_BTHome) that could potientially run on battery

# In folder [resources](https://github.com/Chreece/FaceRec_BTHome/tree/master/resources) you can find the English firmware + manuals and test tools
Please read the comments in the code.
