#include <ELECHOUSE_CC1101.h>

// These examples are from the Electronics Cookbook by Simon Monk
// Connections (for an Arduino Uno)
// Arduino          CC1101
// GND              GND
// 3.3V             VCC
// 10               CSN/SS   **** Must be level shifted to 3.3V
// 11               SI/MOSI  **** Must be level shifted to 3.3V
// 12               SO/MISO
// 13               SCK      **** Must be level shifted to 3.3V
// 2                GD0

const int n = 61;
byte buffer[n] = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Set line ending to New Line in Serial Monitor.");
  Serial.println("Enter Message");
  ELECHOUSE_cc1101.Init(F_433); // set frequency - F_433, F_868, F_965 MHz
}

void loop() {
  if (Serial.available()) {
    int len = Serial.readBytesUntil('\n', buffer, n);
    buffer[len] = '\0';
    Serial.println((char *)buffer);
    ELECHOUSE_cc1101.SendData(buffer, len);
    Serial.println("Sent");
  }
}



/* 
 #include <ELECHOUSE_CC1101_SRC_DRV.h>

byte transmitt_byte[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmitt_char = "Hello World";

void setup() {

    Serial.begin(9600);
    
    if (ELECHOUSE_cc1101.getCC1101()){        // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
    }else{
    Serial.println("Connection Error");
    }
 
    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
 // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setCrc(1);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

    Serial.println("Tx Mode");
}

void loop() {

//3 different methods to send data without gdo
//When sending, we give a little time to completely transmit the message (time in millis).
//You can shorten the time. It depends on the data rate and the packet length. Just try it out for fine tuning.

//Transmitt "Hello World" from byte format.
ELECHOUSE_cc1101.SendData(transmitt_byte, 11, 100);
delay(2000);

//Transmitt "Hello World" from char format.
ELECHOUSE_cc1101.SendData(transmitt_char, 100);
delay(2000);

//Transmitt "Hello World" from char format directly.
ELECHOUSE_cc1101.SendData("Hello World", 100);
delay(2000);
}
*/
