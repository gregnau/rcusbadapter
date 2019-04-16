/* Led functions for various boards
 * 
 * This file is part of the Wireless RC Adapter.
 */

// Global variables
bool led_mode = false;
uint32_t led_timer = 0;


#if defined(ARDUINO_AVR_PROMICRO)
  void initLed() {
    TXLED0;  // Just turn off initially
    RXLED0;  // Both of them
  }


  void ledOn() {
    TXLED1;
    RXLED1;
  }

  
  void ledOff() {
    TXLED0;
    RXLED0;
  }

  
  void blinkLed(uint8_t times, uint8_t dly) {
    for (uint8_t u = 0; u < times; u++) {
      TXLED1;
      RXLED1;
      delay(dly);
      TXLED0;
      RXLED0;
      delay(dly);
    }
  }

  
  void waveLed(uint16_t dly) {
    uint32_t curtime = millis();

    if (curtime - led_timer >= dly) {
      if (!led_mode) {
        TXLED1;
        RXLED0;
      }
      else {
        TXLED0;
        RXLED1;
      }

      led_mode = !led_mode;
      led_timer = curtime;
    }
  }

  
#elif defined(ARDUINO_AVR_MICRO) || defined(ARDUINO_AVR_LEONARDO)
  void initLed() {
    #define RXLED 17  // RXLED is on pin 17
    #define TXLED 30  // TXLED is on pin 30

    pinAsOutput(RXLED);
    pinAsOutput(TXLED);
    
    digitalLow(TXLED);
    digitalLow(RXLED);
  }

  
  void ledOn() {
    digitalHigh(TXLED);
    digitalHigh(RXLED);
  }

  
  void ledOff() {
    digitalLow(TXLED);
    digitalLow(RXLED);
  }

  
  void blinkLed(uint8_t times, uint8_t dly) {
    for (uint8_t u = 0; u < times; u++) {
      digitalHigh(TXLED);
      digitalHigh(RXLED);
      delay(dly);
      digitalLow(TXLED);
      digitalLow(RXLED);
      delay(dly);
    }
  }

  
  void waveLed(uint16_t dly) {
    uint32_t curtime = millis();

    if (curtime - led_timer >= dly) {
      if (!led_mode) {
        digitalLow(TXLED);
        digitalHigh(RXLED);
      }
      else {
        digitalHigh(TXLED);
        digitalLow(RXLED);
      }

      led_mode = !led_mode;
      led_timer = curtime;
    }
  }

  
#elif defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_DUEMILANOVE)
  void initLed() {
    #define LEDPIN 13

    pinAsOutput(LEDPIN);
    
    digitalLow(LEDPIN);
  }

  
  void ledOn() {
    digitalHigh(LEDPIN);
  }

  
  void ledOff() {
    digitalLow(LEDPIN);
  }

  
  void blinkLed(uint8_t times, uint8_t dly) {
    for (uint8_t u = 0; u < times; u++) {
      digitalHigh(LEDPIN);
      delay(dly);
      digitalLow(LEDPIN);
      delay(dly);
    }
  }

  
  void waveLed(uint16_t dly) {
    uint32_t curtime = millis();

    if (curtime - led_timer >= dly) {
      if (!led_mode) {
        digitalLow(LEDPIN);
      }
      else {
        digitalHigh(LEDPIN);
      }
      
      led_mode = !led_mode;
      led_timer = curtime;
    }
  }
  
#elif defined(ARDUINO_AVR_ADK) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)
  // ToDo - support for MEGA ADK/2560 boards
  void initLed() {
    ;
  }
  
  
  void ledOn() {
    ;
  }
  
  
  void ledOff() {
    ;
  }
  
  
  void blinkLed(uint8_t times, uint8_t dly) {
    for (uint8_t u = 0; u < times; u++) {
      ;
    }
  }
  
  
  void waveLed(uint16_t dly) {
    uint32_t curtime = millis();

    if (curtime - led_timer >= dly) {
      if (!led_mode) {}

    led_mode = !led_mode;
    led_timer = curtime;
    }
  }
  
#endif
