#include <Hologram800.h>
#include <SoftwareSerial.h>

const int RX_PIN = 7; //SIM800 RX connected to pin D7
const int TX_PIN = 8; //SIM800 TX connected to pin D8
const char HOLOGRAM_ID[] = "****";  //replace w/your SIM id
const char HOLOGRAM_KEY[] = "****"; //replace w/your SIM key

Hologram800 hologram(TX_PIN, RX_PIN, HOLOGRAM_ID, HOLOGRAM_KEY);

void setup() {
  Serial.begin(9600);
  while(!Serial);

  if(hologram.init()) {

    // Send data to Hologram's Cloud
    if (hologram.sendData("This is some awesome data!")) {
      Serial.println(F("Data Sent"));
    }

    // Send data to Hologram's Cloud through SMS
    // Not preferred, slower transmittals and costs more
    if (hologram.sendSMStoCloud("This is some awesome SMS data!")) {
      Serial.println(F("Data Sent by SMS"));
    }

    // Send soft SMS through TCP
    // Charges as data, not SMS (save $$)
    if (hologram.sendCloudSMS("+18001235555", "Soft SMS, how cool!")) {
      Serial.println(F("Cloud SMS Sent"));
    }

    // Send SMS
    // Not preferred, slower transmittals and costs more
    if (hologram.sendSMS("+18001235555", "Oldschool SMS")) {
      Serial.println(F("Cloud SMS Sent"));
    }

    delay(4000);
  }

}

void loop() {
  // Interact directly with the SIM800 module
  // Send AT commands and see Serial output from module
  hologram.serialDebug();
}
