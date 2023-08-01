#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <stdio.h>

int pin = 0;

long leftButton = 157635573;
long centerButton = 157635541;
long rightButton = 157635573;

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

  if (ELECHOUSE_cc1101.getCC1101()) {  // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  } else {
    Serial.println("Connection Error");
  }

  ELECHOUSE_cc1101.Init();          // must be set to initialize the cc1101!
  ELECHOUSE_cc1101.setMHZ(433.92);  // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  mySwitch.enableReceive(pin);      // Receiver on
  ELECHOUSE_cc1101.SetRx();         // set Receive on
}
void loop() {
  delay(1000);
  if (mySwitch.available()) {
    if (mySwitch.getReceivedValue() == centerButton) {
      Serial.print("RSSI: ");
      Serial.println(ELECHOUSE_cc1101.getRssi());
    }
    mySwitch.resetAvailable();
  }
}
