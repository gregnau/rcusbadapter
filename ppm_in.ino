/* Pulse Position modulation functions
 *  
 * This file is part of the Wireless RC Adapter.
 */

/* Code to read PPM taken from:
 * http://github.com/timonorawski/RCPPMJoystick
 * by Timon Orawski (11-04-2016)
 * Copyright (c) Timon Orawski
 * 
 * Added by Bayu Laksono (27-03-2018)
 * http://github.com/sblaksono
 * 
 * Updated by GregNau (29-03-2019)
 * http://gregnau.github.io
 */

#if defined(PPM_RECEIVER)

  #include <avr/interrupt.h>

  // PPM packets configuration
  #define NEWFRAME_PULSE_WIDTH 3000
  #define TIMER_COUNT_DIVIDER 2

  // Input pin(s) configuration
  #if defined(ARDUINO_PROMICRO) || defined(ARDUINO_LEONARDO) || defined(ARDUINO_AVR_MICRO)
    const uint8_t RC_PINS[1] = {4};
  #else
    const uint8_t RC_PINS[1] = {8};
  #endif
  

  void initTimer(void) {
    // Input Capture setup
    // ICNC1: =0 Disable Input Capture Noise Canceler to prevent delay in reading
    // ICES1: =1 for trigger on rising edge
    // CS11: =1 set prescaler to 1/8 system clock (F_CPU)
    TCCR1A = 0;
    TCCR1B = (0 << ICNC1) | (1 << ICES1) | (1 << CS11);
    TCCR1C = 0;
  
    // Interrupt setup
    // ICIE1: Input capture
    TIFR1 = (1 << ICF1);  // clear pending
    TIMSK1 = (1 << ICIE1);  // and enable
  }


  uint16_t adjust(uint16_t diff, uint8_t chan) {
    #if defined(TRIM_CH1) || defined(TRIM_CH2) || defined(TRIM_CH3) || defined(TRIM_CH4) || defined(TRIM_CH5) || defined(TRIM_CH6) || defined(TRIM_CH7) || defined(TRIM_CH8)
      switch (chan) {
        #if defined(TRIM_CH1)
          case 0:
            diff += TRIM_CH1;
            break;
        #endif
        
        #if defined(TRIM_CH2)
          case 1:
            diff += TRIM_CH2;
            break;
        #endif

        #if defined(TRIM_CH3)
          case 2:
            diff += TRIM_CH3;
            break;
        #endif

        #if defined(TRIM_CH4)
          case 3:
            diff += TRIM_CH4;
            break;
        #endif

        #if defined(TRIM_CH5)
          case 4:
            diff += TRIM_CH5;
            break;
        #endif

        #if defined(TRIM_CH6)
          case 5:
            diff += TRIM_CH6;
            break;
        #endif

        #if defined(TRIM_CH7)
          case 6:
            diff += TRIM_CH7;
            break;
        #endif

        #if defined(TRIM_CH8)
          case 7:
            diff += TRIM_CH8;
            break;
        #endif
      }
    #endif
    
    // Convert to microseconds (because of timer prescaler usage)
    diff = diff / TIMER_COUNT_DIVIDER;
    
    return diff;
  }

  
  void setupPins(void) {
    pinAsInputPullUp(RC_PINS[0]);  // Set-up the input capture pin with internal pull-up resistor
  }

  
  void rcSetupPpm() {
    setupPins();
    initTimer();
  }
  
  
  // Interrupt Service Routine function
  ISR(TIMER1_CAPT_vect) {
    union twoBytes {
      uint16_t word;
      uint8_t  byte[2];
    } timeValue;
  
    uint16_t now, diff;
    static uint16_t last = 0;
    static uint8_t chan = 0;
  
    timeValue.byte[0] = ICR1L;  // Grab captured timer value (low byte)
    timeValue.byte[1] = ICR1H;  // Grab captured timer value (high byte)
  
    now = timeValue.word;
    diff = now - last;
    last = now;
  
    // All numbers are microseconds multiplied by TIMER_COUNT_DIVIDER (as prescaler is set to 1/8 of 16 MHz)
    if (diff > (NEWFRAME_PULSE_WIDTH * TIMER_COUNT_DIVIDER)) {
      chan = 0;  // New data frame detected, start again
    }
    else {
      if (diff > (MIN_PULSE_WIDTH * TIMER_COUNT_DIVIDER - CHAN_THRESHOLD)
          && diff < (MAX_PULSE_WIDTH * TIMER_COUNT_DIVIDER + CHAN_THRESHOLD)
          && chan < CHANNELS) {

        rc_values[chan] = adjust(diff, chan);  // Store detected value
        tx_shared_flags |= FLAGS[chan];  // Set the 'new data to process' flag
      }
  
      chan++;  // No value detected within expected range, move to next channel
    }
  }
  
#endif  // defined(PPM_RECEIVER)
