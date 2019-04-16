/* Serial output functions for various boards
 *  
 * This file is part of the Wireless RC Adapter.
 */

#if defined(SER_ENABLE)

  // Protocol properties
  #define FRAME_HEADER  16416
  #define FRAME_DELAY    7700


  // Global variables
  uint8_t tx_buffer[32];
  uint16_t tx_shared_chksum;
  uint32_t serial_timer;  // Transmission timer


  // Returns the checksum of dataframe data in integer
  void chksumTxBuf(uint16_t txshd) {
    txshd = 65535 - txshd;
    tx_buffer[30] = (uint8_t) (txshd & 0xFF);  // Write LOW byte first
    tx_buffer[31] = (uint8_t) (txshd >> 8);  // Then HIGH byte next
  }


  //
  void processSerial() {
    for (uint8_t x=0; x<CHANNELS; x++) {
      writeTxBuf((x*2+2), rc_values[x]);
    }

    chksumTxBuf(tx_shared_chksum);
  }
  
  
  // Timing and output contents of tx buffer
  void outputSerial() {
    uint32_t curtime = micros();

    if (curtime >= serial_timer) {
      Serial.write(tx_buffer, 32);
//      for (uint8_t h=0; h<sizeof(tx_buffer); h++) Serial.print(tx_buffer[h], HEX);
//      Serial.println();
      serial_timer = (uint32_t) curtime + FRAME_DELAY;
    }
    else {
      processSerial();
    }
  }
  

  // Function to store int values in the tx buffer
  inline void writeTxBuf(uint8_t pos, uint16_t intval) {
    // Remove the old values from the checksum
    tx_shared_chksum -= tx_buffer[pos];
    tx_shared_chksum -= tx_buffer[pos+1];
    
    // Write new values to tx buffer
    tx_buffer[pos] = (uint8_t) (intval & 0xFF);  // Write LOW byte first
    tx_buffer[pos+1] = (uint8_t) (intval >> 8);  // Then HIGH byte next

    // Add the new values to checksum
    tx_shared_chksum += tx_buffer[pos];
    tx_shared_chksum += tx_buffer[pos+1];
  }
  

  // Function to initialize serial comm and tx buffer
  void initSerial() {
    Serial.begin(115200, SERIAL_8N1);
    delay(10);  // Wait for serial to begin
  
    // Initialize the tx buffer with default values
    writeTxBuf(0, FRAME_HEADER);  // Fixed data frame header
    for (uint8_t p=2; p<30; p+=2) writeTxBuf(p, CHAN_CENTER);
    chksumTxBuf(tx_shared_chksum);  // Calculate checksum and put in the footer

//    Serial.print(tx_buffer[30], HEX);Serial.println(tx_buffer[31], HEX);
  }

#endif  // defined(SER_ENABLE)
