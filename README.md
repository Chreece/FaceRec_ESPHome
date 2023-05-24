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
*Some ESP32 boards can't power the TX510 when in Identification process so if you have strange behaviours make sure TX510 gets it's 5v or else try with another board*
TX510 | ESP | 
-------- | -------- |
5V | VCC | 
GND | GND |
P2: USI1_SD0 | RX |
P2: USI1_SCLK | TX  | 

Entities exposed:
*  Switch for turning on/off: 
   * `Diplay`
   * `Backlight`
   * Onboard LED `Flash`
*  Buttons: 
   * `Registration` and `Identification` of Users
   * `Query the number of UserIDs`
   * `Delete a UserID` *needs an input_number helper in HA with name: `input_number.hx510_input`*
   * `Clear all` the registered faces
   * `Query the TX510 version`
   * `Baud rate change`
   * `Restart MCU` (ESP)
   * `Reboot TX510`
*  Text Sensors:
   * `User ID` that recognised or registered. The UserIds start from 0 (first face registered = 0)
   * `Responce` from the TX510 on every action
*  Binary Sensor:
   * `Presence sensor` that turns on when the identification process was successful
* Number:
  * `User Input` where you can input UserIDs to delete or to change the Baud Rate from TX510
    
## Initial Setup
* Copy the .yaml and .h files in your ESPHome config folder
* Edit the .yaml file for your ESP settings
* Compile & Upload to the ESP

## How to:
  * Register a UserID (Face)
    * Look in front the camera and press the Register User Button in HA or S1 button on the TX510 kit
    * You get on screen and ha responce: `Failed` with a reason or `Success` with the UserID that has been saved (starting with 0)
  * Identify a User
    * Look in front the camera and press the Start Identify Button in HA or S2 button on the TX510 kit
    * You get on screen and ha responce: `Failed` with a reason or `Success` with the UserID that has been recognised
  * Delete a UserID
    * Input the UserID (0-999) in number entity `User Input`
    * Press the Delete UserID button on HA
    * You get on screen and ha responce: `Success` always (even if the UserID doesn't exit, hardware limitation)
  * Query Sum and UserIDs saved on TX510:
    * Press Query Num. Reg. UserIDs Button on HA
    * You get a ha responce: `Sum:` with the summary of all registered users and the UserIDs that have been registered
  * Baud Rate change on TX510:
    * Input the corresponding number for the Baud Rate from the list below, in number entity `User Input`
      * 0 = 9600
      * 1 = 19200
      * 2 = 38400
      * 3 = 57600
      * 4 = 115200
    * Press the Change Baud Rate button on HA
    * You get on screen and ha responce: `Success`

This version runs on ESPHome firmware, you can check the version running on [BTHome](https://github.com/Chreece/FaceRec_BTHome) that could potientially run on battery

# In folder [resources](https://github.com/Chreece/FaceRec_BTHome/tree/master/resources) you can find the English firmware + manuals and test tools
Please read the comments in the code.
