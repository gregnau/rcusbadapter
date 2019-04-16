/* Futaba Serial Bus protocol functions
 *  
 * This file is part of the Wireless RC Adapter.
 */

/* Code to read SBUS taken from:
 * http://github.com/fdivitto/sbus
 * by Fabrizio Di Vittorio (29-08-2018)
 * Copyright (c) Fabrizio Di Vittorio
 * 
 * Updated by Anthony Chiam (27-03-2018)
 * http://github.com/ultimate1112
 * 
 * Updated by GregNau (05-04-2019)
 * http://gregnau.github.io
 */

#if defined(SBUS_RECEIVER)

  // Flag fields
  #define FLAG_CHANNEL_17  1
  #define FLAG_CHANNEL_18  2
  
  // Channel input calibration
  #define CHANNEL_MIN 173
  #define CHANNEL_MAX 1812

  // Input pin(s) configuration
  const uint8_t RC_PINS[1] = {10};
    
  // Global variables
  static volatile uint8_t * s_pin;
  static uint8_t s_pinMask;
  static uint8_t s_PCICRMask;
  static volatile uint8_t s_receivingWordIndex = 0;  // Contains which word is currently receiving (0..24)
  static volatile uint8_t s_frame[23];  // Received frames array
  static volatile bool s_hasSignal = false;  // Connection status boolean
  
  
  // Channel information stucture
  struct chinfo_t {
    uint8_t idx;
    uint8_t shift1;
    uint8_t shift2;
    uint8_t shift3; // 11 = byte 3 ignored
  };
  
  // Channel information register
  static const chinfo_t CHINFO[16] PROGMEM = {
    { 0, 0, 8, 11}, { 1, 3, 5, 11}, { 2, 6, 2, 10}, { 4, 1, 7, 11}, { 5, 4, 4, 11},
    { 6, 7, 1,  9}, { 8, 2, 6, 11}, { 9, 5, 3, 11}, {11, 0, 8, 11}, {12, 3, 5, 11},
    {13, 6, 2, 10}, {15, 1, 7, 11}, {16, 4, 4, 11}, {17, 7, 1,  9}, {19, 2, 6, 11},
    {20, 5, 3, 11}
  };
  
  
  // Handle pin change interrupt here
  static void handleInterrupt();
  #define ISRN(N) ISR(PCINT ## N ## _vect) { if (s_PCICRMask == 1 << (N)) handleInterrupt(); }
  
  // Define Pin Change Interrupts for specific boards
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega328P__)
    ISRN(0) ISRN(1) ISRN(2)
  #else
    // Fall back to PCINT[0-7]
    ISRN(0)
  #endif
  
  
  static void handleInterrupt() {
    // Start-Bit?
    if (pinGet(s_pin, s_pinMask)) {
      TCNT0 = 0;  // Reset Timer-0 counter
      PCICR &= ~s_PCICRMask;  // Disable interrupt for the group
      uint8_t receivingWord = 0;  // Start-bit received
      TIFR0 |= 1 << OCF0A;  // Reset OCF0A flag by writing 1
      uint8_t parity = 0xFF;    
  
      // Receive other bits (including parity bit, ignore stop bits)
      for (uint8_t receivedBitIndex = 0; receivedBitIndex < 9; ++receivedBitIndex) {
        
        // WARN: inside this loop interrupts are re-enabled to allow other libraries to work correctly (ie Servo library)
        interrupts();
        while (!(TIFR0 & (1 << OCF0A)));  // Wait for TCNT0 == OCR0A
        noInterrupts();
  
        TIFR0 |= 1 << OCF0A;  // Reset OCF0A flag by writing 1
        
        // Sample current bit  
        if (pinGet(s_pin, s_pinMask)) receivingWord |= 1 << receivedBitIndex;  // Parity shift here as >7 bit, so it is just discarded
        else parity = ~parity;
      }
  
      // Check parity
      if (!parity) {  // Parity Error
        s_receivingWordIndex = 0;
      }
      else {  // Parity OK
        receivingWord = ~receivingWord;
    
        if (s_receivingWordIndex == 0) {
          //  Check start word (must be 0x0F)
          if (receivingWord == 0x0F) {
            ++s_receivingWordIndex;  // Bypass this word
          }
        }
        else if (s_receivingWordIndex == 24) {
          // Bypass this word
          if (receivingWord == 0x00) ++s_receivingWordIndex;
        }
        else {
          // Save channels and flags and last ending word
          s_frame[s_receivingWordIndex - 1] = receivingWord;
          s_hasSignal = true;
  
          // Next word          
          ++s_receivingWordIndex;
        }
      }
      
      PCIFR |= s_PCICRMask;  // Reset pin change interrupt flag
      PCICR |= s_PCICRMask;  // Enable interrupt for the group
  
      // Last word, restart word count...
      if (s_receivingWordIndex == 25) s_receivingWordIndex = 0;
    }
  }
  
  
  // Process configured number of channels from sbus
  void rcProcessSbus() {
    for (uint8_t q = 0; q < CHANNELS; q++) {
      rc_values[q] = getChSbus(q + 1);
    }
  }
  
  
  // Returns channel values between 173..1812 (as received by SBUS)
  uint16_t getChRawSbus(uint8_t channelIndex) {
    if (channelIndex >= 1 && channelIndex <= 16) {
      chinfo_t chinfo;
      memcpy_P(&chinfo, CHINFO + channelIndex - 1, sizeof(chinfo_t));
    
      uint8_t idx = chinfo.idx;
    
      noInterrupts();
      uint8_t b1 = s_frame[idx++];
      uint8_t b2 = s_frame[idx++];
      uint8_t b3 = s_frame[idx];
      interrupts();
    
      return ((b1 >> chinfo.shift1) | (b2 << chinfo.shift2) | (b3 << chinfo.shift3)) & 0x7FF;
    }
    else if (channelIndex == 17 || channelIndex == 18) {
      if (channelIndex == 17) {
        return s_frame[22] & FLAG_CHANNEL_17 ? CHANNEL_MAX : CHANNEL_MIN;
      }
      else {
        return s_frame[22] & FLAG_CHANNEL_18 ? CHANNEL_MAX : CHANNEL_MIN;
      }
    }
    else {
      return 0; // Error
    }
  }
  
  
  // Returns channel values between 988..2012 (CleanFlight friendly)
  uint16_t getChSbus(uint8_t channelIndex) { return 5 * getChRawSbus(channelIndex) / 8 + 880; }
  
  
  // An interrupt is always generated for every frame received. 
  void rcSetupSbus() {
    s_pin = pinOfPin(RC_PINS[0]);
    s_pinMask = pinMask(RC_PINS[0]);
    s_PCICRMask = 1 << digitalPinToPCICRbit(RC_PINS[0]);
    
    // Setup TIMER 0 CTC (Clear Timer on Compare)
    TCCR0A = 1 << WGM01; 
    
    // No prescaling
    TCCR0B = 1 << CS00; 
  
    // Set TOP timer 0 value
    /* With no-prescaler 1/16000000=0.00000000625,
     * each bit requires 10us, so reset timer 2 every 10us,
     * so reset timer every 10/0.0625 = 160 ticks. 
     */
    OCR0A = F_CPU / 1000000 * 10 - 1; // for 16MHz = 159;  
  
    // Setup pin change interrupt
    pinAsInput(RC_PINS[0]);
    *digitalPinToPCMSK(RC_PINS[0]) |= 1 << digitalPinToPCMSKbit(RC_PINS[0]);  // Enable pin
    PCIFR |= s_PCICRMask;  // Clear any outstanding interrupt
    PCICR |= s_PCICRMask;  // Enable interrupt for the group
  }

#endif  //defined(SBUS_RECEIVER)
