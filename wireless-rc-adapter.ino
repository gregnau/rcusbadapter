/* Wireless RC Adapter (DEVELOPMENT VERSION)
 *  http://wireless-rc-adapter.github.io
 * 
 * Connects an RC receiver as a HID compatible
 * joystick to almost any kind of device, with
 * just a simple USB cable.
 * 
 *  GregNau  @  2015-2019
 */

/******************************************************************/
/* >>> Configuration Options <<<                                  */
/* More information about coniguration is available in the Wiki:  */
/* http://github.com/wireless-rc-adapter/wireless-rc-adapter/wiki */
/******************************************************************/

/*----------------------------------------------*/
/* >>> 1. Input Settings (enable only one!) <<< */
/*----------------------------------------------*/
//#define  PWM_RECEIVER   // Enable Pulse Width Modulation receiver
//#define  PPM_RECEIVER   // Enable Pulse Position Modulation receiver (default)
//#define  SBUS_RECEIVER  // Enable sBus protocol serial receiver
//#define  IBUS_RECEIVER  // Enable iBus protocol serial receiver

/*-----------------------------------*/
/* >>> 2. Channels Configuration <<< */
/*-----------------------------------*/
//#define  CHANNELS          8  // Override the default 6 channels (PWM max: 6, PPM max: 8, SBUS max: 10)
//#define  CHAN_CENTER    1530  // Configure custom stick center value (default: 1500)
//#define  CHAN_HALFWAY    450  // Define custom halfway value (default: 500)
//#define  CHAN_THRESHOLD  200  // Customize channel value detection threshold (default: 100)

/*-----------------------------------------------*/
/* >>> 3. Output Settings (multiple allowed) <<< */
/*-----------------------------------------------*/
//#define  JOYSTICK_ENABLE  // Enable native Joystick output (default)
//#define  MULTI_ENABLE     // Enabled Multi-Axis contoller output
//#define  SERIAL_ENABLE    // Enable Serial iBus protocol output
//#define  KEYBOARD_ENABLE  // Enabled native Keyboard output
//#define  MOUSE_ENABLE     // Enable native Mouse output

/*--------------------------*/
/* >>> 4. Channel Trims <<< */
/*--------------------------*/
//#define TRIM_CH1  -15  // Adjust channels (for example when TX have no trims
//#define TRIM_CH2  +15  // Values can be both positive and negative (default: off)
//#define TRIM_CH3   +0
//#define TRIM_CH4   +0
//#define TRIM_CH5   +0
//#define TRIM_CH6   +0
//#define TRIM_CH7   +0
//#define TRIM_CH8   +0

/*-------------------------------------*/
/* >>> 5. Keyboard Output Settings <<< */
/*-------------------------------------*/
//#define CH1_LOW_KEY    ''  //
//#define CH1_HIGH_KEY   ''  // 
//#define CH2_LOW_KEY    ''
//#define CH2_HIGH_KEY   ''
//#define CH3_LOW_KEY    ''
//#define CH3_HIGH_KEY   ''
//#define CH4_LOW_KEY    ''
//#define CH4_HIGH_KEY   ''
//#define CH5_LOW_KEY    ''
//#define CH5_HIGH_KEY   ''
//#define CH6_LOW_KEY    ''
//#define CH6_HIGH_KEY   ''
//#define CH7_LOW_KEY   'c'
//#define CH7_HIGH_KEY  'c'
//#define CH8_LOW_KEY   'u'
//#define CH8_HIGH_KEY  'i'

/********************************/
/* End of Configuration options */
/********************************/


/**************** DO NOT EDIT BELOW THIS LINE, UNLESS YOU KNOW HOW TO FIX IT ****************/


/* Faster low-level I/O functions */
#define portOfPin(P)        portOutputRegister(digitalPinToPort(P))
#define ddrOfPin(P)         portModeRegister(digitalPinToPort(P))
#define pinOfPin(P)         portInputRegister(digitalPinToPort(P))
#define pinMask(P)          digitalPinToBitMask(P)
#define pinAsInput(P)       (*(ddrOfPin(P)) &= ~pinMask(P))
#define pinAsInputPullUp(P) (*(ddrOfPin(P)) &= ~pinMask(P)); digitalHigh(P)
#define pinAsOutput(P)      (*(ddrOfPin(P)) |= pinMask(P))
#define digitalLow(P)       (*(portOfPin(P)) &= ~pinMask(P))
#define digitalHigh(P)      (*(portOfPin(P)) |= pinMask(P))
#define isHigh(P)           ((*(pinOfPin(P)) & pinMask(P)) > 0)
#define isLow(P)            ((*(pinOfPin(P)) & pinMask(P)) == 0)
#define pinGet(pin, mask)   (((*(pin)) & (mask)) > 0)
#define digitalState(P)     ((uint8_t)isHigh(P))


/* Different error checkings of the configuration */

// Error checking for channels configuration
#if defined(PWM_RECEIVER) && CHANNELS > 6
  #line 27
  #error Too much channels are configured, the maximum is 6 with PWM modulation!
#elif defined(PPM_RECEIVER) && CHANNELS > 8
  #line 27
  #error Too much channels are configured, the maximum is 8 with PPM modulation!
#elif defined(SBUS_RECEIVER) && CHANNELS > 8
  #line 27
  #error Too much channels are configured, the maximum is 8 with sBus protocol!
#elif !defined(CHANNELS)
  #define CHANNELS 6  // Default number of channels
#endif


// Default channel configuration (CleanFlight compatible)
#if !defined(CHAN_THRESHOLD)
  #define CHAN_THRESHOLD 100
#endif

#if !defined(CHAN_CENTER)
  #define CHAN_CENTER 1500
#endif

#if !defined(CHAN_HALFWAY)
  #define CHAN_HALFWAY 500
#endif

#define MIN_PULSE_WIDTH (CHAN_CENTER - CHAN_HALFWAY - 12)
#define MAX_PULSE_WIDTH (CHAN_CENTER + CHAN_HALFWAY + 12)


// Keyboard configuration error check
#if defined(KEY_ENABLE)
  
  // Channel 1 keys check
  #if defined(CH1_LOW_KEY) && defined(CH1_HIGH_KEY)
    #define KEY1_ENABLE  // Enable CH1 keys
  #endif

  // Channel 2 keys check
  #if defined(CH2_LOW_KEY) && defined(CH2_HIGH_KEY)
    #define KEY2_ENABLE  // Enable CH2 keys
  #endif

  // Channel 3 keys check
  #if defined(CH3_LOW_KEY) && defined(CH3_HIGH_KEY)
    #define KEY3_ENABLE  // Enable CH3 keys
  #endif

  // Channel 4 keys check
  #if defined(CH4_LOW_KEY) && defined(CH4_HIGH_KEY)
    #define KEY4_ENABLE  // Enable CH4 keys
  #endif

  // Channel 5 keys check
  #if defined(CH5_LOW_KEY) && defined(CH5_HIGH_KEY)
    #define KEY5_ENABLE  // Enable CH5 keys
  #endif

  // Channel 6 keys check
  #if defined(CH6_LOW_KEY) && defined(CH6_HIGH_KEY)
    #define KEY6_ENABLE  // Enable CH6 keys
  #endif

  // Channel 7 keys check
  #if defined(CH7_LOW_KEY) && defined(CH7_HIGH_KEY)
    #define KEY7_ENABLE  // Enable CH7 keys
  #endif

  // Channel 8 keys check
  #if defined(CH8_LOW_KEY) && defined(CH8_HIGH_KEY)
    #define KEY8_ENABLE  // Enable CH8 keys
  #endif
  
#endif  // defined(KEY_ENABLE)

/* End of Configuration error checkings */


// Global variables
const uint8_t FLAGS[8] = {1, 2, 4, 8, 16, 32, 64, 128};  // Channel value position flags
volatile uint16_t rc_values[CHANNELS] = {0};  // Actual channel values
uint8_t tx_shared_flags = 0;


void setup() {
  // Initialize led(s) control for various boards
  initLed();  

  // Setup the enabled input and attach interrupts
  #if defined(PPM_RECEIVER)
    rcSetupPpm();
  #elif defined(PWM_RECEIVER)
    rcSetupPwm(CHANNELS);
  #elif defined(SBUS_RECEIVER)
    rcSetupSbus();
  #elif defined(IBUS_RECEIVER)
    rcSetupIbus();
  #endif

  delay(250);  // Give signals some time to settle...
  
  // Start serial output (if enabled)
  #if defined(SER_ENABLE)
    initSerial();
  #endif
  
  // Setup and start joystick output (if enabled)
  #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
    startJoystick();  // Start joystick output
  #endif

  // Start keyboard output (if enabled)
  #if defined(KEY_ENABLE)
    startKeyboard();
  #endif
}


void loop() {
  txProcessOutput();
}


// Function to output flagged/updated values
inline void txProcessOutput() {
  static uint8_t localflags;

  if (tx_shared_flags) {  // If there are new values to process
    #if defined(JOY_ENABLE) || defined(MULTI_ENABLE) || defined(KEY_ENABLE)
      localflags = tx_shared_flags;
  
      // Process every channel with a flag
      for (uint8_t q = 0; q < CHANNELS; q++) {
        if (localflags & FLAGS[q]) {
          switch (q) {
            case 0:  // CHANNEL 1
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyX(rc_values[q]);
              #endif
  
              #if defined(KEY1_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH1_LOW_KEY : CH1_HIGH_KEY);
              #endif
              break;
            case 1:  // CHANNEL 2
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyY(rc_values[q]);
              #endif
  
              #if defined(KEY2_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH2_LOW_KEY : CH2_HIGH_KEY);
              #endif
              break;
            case 2:  // CHANNEL 3
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyZ(rc_values[q]);
              #endif
  
              #if defined(KEY3_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH3_LOW_KEY : CH3_HIGH_KEY);
              #endif
              break;
            case 3:  // CHANNEL 4
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyRz(rc_values[q]);
              #endif
  
              #if defined(KEY4_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH4_LOW_KEY : CH4_HIGH_KEY);
              #endif
              break;
            case 4:  // CHANNEL 5
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyRy(rc_values[q]);
              #endif
  
              #if defined(KEY5_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH5_LOW_KEY : CH5_HIGH_KEY);
              #endif
              break;
            case 5:  // CHANNEL 6
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyRx(rc_values[q]);
              #endif
  
              #if defined(KEY6_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH6_LOW_KEY : CH6_HIGH_KEY);
              #endif
              break;
            case 6:  // CHANNEL 7
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyBtn(0, rc_values[q] < CHAN_CENTER ? LOW : HIGH);
              #endif
  
              #if defined(KEY7_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH7_LOW_KEY : CH7_HIGH_KEY);
              #endif
              break;
            case 7:  // CHANNEL 8
              #if defined(JOY_ENABLE) || defined(MULTI_ENABLE)
                sendJoyBtn(1, rc_values[q] < CHAN_CENTER ? LOW : HIGH);
              #endif
  
              #if defined(KEY8_ENABLE)
                sendKey(q, rc_values[q] < CHAN_CENTER ? CH8_LOW_KEY : CH8_HIGH_KEY);
              #endif
              break;
          }
        }
      }
    #endif // defined(JOY_ENABLE) || defined(MULTI_ENABLE) || defined(KEY_ENABLE)

    #if defined(SER_ENABLE)
      outputSerial();
    #endif
    
    tx_shared_flags = 0;  // Reset output flags
  }

  localflags = 0;
}
