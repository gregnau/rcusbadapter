/* Keyboard output functions, only for 32u4 based boards
 *  
 * This file is part of the Wireless RC Adapter.
 */

#if defined(KEY_ENABLE)

  // Default USB keyboard library of Arduino
  #include <Keyboard.h>


  // Global variables
  bool key_flags[CHANNELS] = {false};


  // Keyboard.begin wrap function
  void startKeyboard() { Keyboard.begin(); }


  // TODO improve with typedef to trinary operator
  
  // Function to send the key stroke, but only once when channel changes
  void sendKey(uint8_t chid, char keytosend) {
    if (!key_flags[chid] && rc_values[chid] > CHAN_CENTER + CHAN_THRESHOLD) {
      Keyboard.write(keytosend);
      key_flags[chid] = true;  // Set the flag when key sent once to prevent flooding
    }
    else if (key_flags[chid] && rc_values[chid] < CHAN_CENTER - CHAN_THRESHOLD) { 
      Keyboard.write(keytosend);
      key_flags[chid] = false;  // Set the flag when key sent once to prevent flooding
    }
  }

#endif  // defined(KEY_ENABLE)
