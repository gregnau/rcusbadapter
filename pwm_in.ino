/* Pulse Width Modulation functions
 *  
 * This file is part of the Wireless RC Adapter.
 */

#if defined(PWM_RECEIVER)

  // Use EnableInterrupt library from GreyGnome
  #include "src/EnableInterrupt/EnableInterrupt.h"

  // Input pin(s) configuration
  #if defined(ARDUINO_AVR_PROMICRO)
    const uint8_t RC_PINS[6] = {15, 14, 16, 10, 9, 8};
  #elif defined(ARDUINO_AVR_MICRO) || defined(ARDUINO_AVR_LEONARDO)
    const uint8_t RC_PINS[6] = {11, 10, 9, 8, PB2, PB1};  // Last 2 is located on the ICSP header
  #elif defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_DUEMILANOVE)
    const uint8_t RC_PINS[6] = {A1, A2, A3, A4, A5, A6};
  #elif defined(ARDUINO_AVR_ADK) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)
    const uint8_t RC_PINS[6] = {A10, A11, A12, A13, A14, A15};
  #endif


  // Global variable(s)
  volatile uint32_t rc_shared_ts[CHANNELS];  // Long-array to store TimeSequences


  void rcProcessPwm(uint8_t id) {
    if (isHigh(RC_PINS[id])) {
      rc_shared_ts[id] = micros();
    }
    else {
        rc_values[id] = (uint16_t) (micros() - rc_shared_ts[id]);
        tx_shared_flags |= FLAGS[id];  // Set the 'new data to process' flag
    }
  }

  
  void rcProcessCh1() { rcProcessPwm(0); }
  void rcProcessCh2() { rcProcessPwm(1); }
  void rcProcessCh3() { rcProcessPwm(2); }
  void rcProcessCh4() { rcProcessPwm(3); }
  void rcProcessCh5() { rcProcessPwm(4); }
  void rcProcessCh6() { rcProcessPwm(5); }

  
  void rcSetupPwm(uint8_t numofchs) {
    for (uint8_t o = 0; o < numofchs; o++) {
      switch (o) {
        case 0:
          enableInterrupt(RC_PINS[o], rcProcessCh1, CHANGE);
          break;
        case 1:
          enableInterrupt(RC_PINS[o], rcProcessCh2, CHANGE);
          break;
        case 2:
          enableInterrupt(RC_PINS[o], rcProcessCh3, CHANGE);
          break;
        case 3:
          enableInterrupt(RC_PINS[o], rcProcessCh4, CHANGE);
          break;
        case 4:
          enableInterrupt(RC_PINS[o], rcProcessCh5, CHANGE);
          break;
        case 5:
          enableInterrupt(RC_PINS[o], rcProcessCh6, CHANGE);
          break;
}
    }
  }


  #if defined(TRIM_CH1) || defined(TRIM_CH2) || defined(TRIM_CH3) || defined(TRIM_CH4) || defined(TRIM_CH5) || defined(TRIM_CH6)
    uint16_t adjust(uint16_t diff, uint8_t chan) {
        switch (chan) {
          #if defined(TRIM_CH1)
            case 0: return diff + TRIM_CH1;
          #endif
          
          #if defined(TRIM_CH2)
            case 1: return diff + TRIM_CH2;
          #endif
  
          #if defined(TRIM_CH3)
            case 2: return diff + TRIM_CH3;
          #endif
  
          #if defined(TRIM_CH4)
            case 3: return diff + TRIM_CH4;
          #endif
  
          #if defined(TRIM_CH5)
            case 4: return diff + TRIM_CH5;
          #endif
  
          #if defined(TRIM_CH6)
            case 5: return diff + TRIM_CH6;
          #endif
        }
      
      return diff;
    }
  #endif  // defined(TRIM_CH#)

#endif  // defined(PWM_RECEIVER)
