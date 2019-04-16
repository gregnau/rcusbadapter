/* Joystick output functions, only for 32u4 based boards
 *  
 * This file is part of the Wireless RC Adapter.
 */
 
#if defined(JOY_ENABLE) || defined(MULTI_ENABLE)

  // Modified version of the library to be compatible with Linux & Unity games
  #include "src/ArduinoJoystickLibrary/Joystick.h"


  #if defined(JOY_ENABLE)
    #define JOYSTICK_TYPE 0x04  // JOYSTICK_TYPE_JOYSTICK
  #elif defined(MULTI_ENABLE)
    #define JOYSTICK_TYPE 0x08  // MULTI_AXIS_CONTROLLER
  #endif


  // Set the joystick's unique HID report ID
  #define HID_REPORT_ID 0x03  // Do not use 0x01 or 0x02 as they are used by the built-in Keyboard and Mouse libraries


  // Configure joystick features according to number of available channels
  #if CHANNELS == 1
    #define INCLUDE_X_AXIS   true  // Indicates if the X Axis is available on the joystick
    #define INCLUDE_Y_AXIS  false  // Indicates if the Y Axis is available on the joystick
    #define INCLUDE_Z_AXIS  false  // Indicates if the Z Axis (in some situations this is the right X Axis) is available on the joystick
    #define INCLUDE_RX_AXIS false  // Indicates if the X Axis Rotation (in some situations this is the right Y Axis) is available on the joystick
    #define INCLUDE_RY_AXIS false  // Indicates if the Y Axis Rotation is available on the joystick
    #define INCLUDE_RZ_AXIS false  // Indicates if the Z Axis Rotation is available on the joystick
    #define BUTTON_COUNT 0  // Indicates how many buttons will be available on the joystick (0-32)
  
  #elif CHANNELS == 2
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS  false
    #define INCLUDE_RX_AXIS false
    #define INCLUDE_RY_AXIS false
    #define INCLUDE_RZ_AXIS false
    #define BUTTON_COUNT 0
  
  #elif CHANNELS == 3
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS false
    #define INCLUDE_RY_AXIS false
    #define INCLUDE_RZ_AXIS false
    #define BUTTON_COUNT 0
  
  #elif CHANNELS == 4
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS  true
    #define INCLUDE_RY_AXIS false
    #define INCLUDE_RZ_AXIS false
    #define BUTTON_COUNT 0
  
  #elif CHANNELS == 5
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS  true
    #define INCLUDE_RY_AXIS  true
    #define INCLUDE_RZ_AXIS false
    #define BUTTON_COUNT 0
  
  #elif CHANNELS == 6
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS  true
    #define INCLUDE_RY_AXIS  true
    #define INCLUDE_RZ_AXIS  true
    #define BUTTON_COUNT 0
  
  #elif CHANNELS == 7
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS  true
    #define INCLUDE_RY_AXIS  true
    #define INCLUDE_RZ_AXIS  true
    #define BUTTON_COUNT 1
  
  #elif CHANNELS == 8
    #define INCLUDE_X_AXIS   true
    #define INCLUDE_Y_AXIS   true
    #define INCLUDE_Z_AXIS   true
    #define INCLUDE_RX_AXIS  true
    #define INCLUDE_RY_AXIS  true
    #define INCLUDE_RZ_AXIS  true
    #define BUTTON_COUNT 2
  #endif

  #define HAT_SWITCH_COUNT        0  // Indicates how many hat switches will be available on the joystick. (0-2)
  #define INCLUDE_RUDDER      false  // Indicates if the Rudder is available on the joystick.
  #define INCLUDE_THROTTLE    false  // Indicates if the Throttle is available on the joystick.
  #define INCLUDE_ACCELERATOR false  // Indicates if the Accelerator is available on the joystick.
  #define INCLUDE_BRAKE       false  // Indicates if the Brake is available on the joystick.
  #define INCLUDE_STEERING    false  // Indicates if the Steering is available on the joystick.


  // Joystick configuration (manual in src/ArduinoJoystickLibrary/README.md)
  Joystick_ Joystick(HID_REPORT_ID, JOYSTICK_TYPE, BUTTON_COUNT, HAT_SWITCH_COUNT,
                     INCLUDE_X_AXIS, INCLUDE_Y_AXIS, INCLUDE_Z_AXIS, INCLUDE_RX_AXIS,
                     INCLUDE_RY_AXIS, INCLUDE_RZ_AXIS, INCLUDE_RUDDER, INCLUDE_THROTTLE,
                     INCLUDE_ACCELERATOR, INCLUDE_BRAKE, INCLUDE_STEERING);

  
  // Initialize and start joystick interface
  void startJoystick() {
    initJoystickAxes(CHANNELS);

    Joystick.begin();
  }


  // Joystick.setXAxis wrap function
  void sendJoyX(uint16_t xstate) { Joystick.setXAxis(xstate); }

  // Joystick.setYAxis wrap function
  void sendJoyY(uint16_t ystate) { Joystick.setYAxis(ystate); }

  // Joystick.setZAxis wrap function
  void sendJoyZ(uint16_t zstate) { Joystick.setZAxis(zstate); }

  // Joystick.setRzAxis wrap function
  void sendJoyRz(uint16_t rzstate) { Joystick.setRzAxis(rzstate); }

  // Joystick.setRyAxis wrap function
  void sendJoyRy(uint16_t rystate) { Joystick.setRyAxis(rystate); }

  // Joystick.setRxAxis wrap function
  void sendJoyRx(uint16_t rxstate) { Joystick.setRxAxis(rxstate); }

  // Joystick.setButton wrap function
  void sendJoyBtn(uint8_t btnid, uint8_t btnstate) { Joystick.setButton(btnid, btnstate); }


  // Initialize available joystick axes
  void initJoystickAxes(uint8_t numofchs) {
    for (uint8_t w = 0; w < numofchs; w++) {
      switch (w) {
        case 0:  // Channel 1
          Joystick.setXAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
        case 1:  // Channel 2
          Joystick.setYAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
        case 2:  // Channel 3
          Joystick.setZAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
        case 3:  // Channel 4
          Joystick.setRzAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
        case 4:  // Channel 5
          Joystick.setRyAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
        case 5:  // Channel 6
          Joystick.setRxAxisRange(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
          break;
      }
    }
  }

#endif  // defined(JOY_ENABLE)
