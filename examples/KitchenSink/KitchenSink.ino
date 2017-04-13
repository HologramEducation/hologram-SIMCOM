#include <HologramSIMCOM.h>

#define RX_PIN 8 //SIM800 RX connected to pin D8
#define TX_PIN 7 //SIM800 TX connected to pin D7
#define RESET_PIN 10 //SIM800 reset connected to pin D10
#define HOLO_ID "****"  //replace w/your SIM id
#define HOLO_KEY "****" //replace w/your SIM key

HologramSIMCOM Hologram(TX_PIN, RX_PIN, RESET_PIN, HOLO_ID, HOLO_KEY); // Instantiate Hologram

bool runOnce = true;

void setup() {

  Serial.begin(19200);
  while(!Serial);

  // begin(baud, serverPort) starts modem, syncs baud and opens incoming port
  // begin(baud) would start the modem but not open an incoming server port
  Hologram.begin(19200, 8888);

}

void loop() {

  // debug() allows you to send commands directly to the module and see responses
  Hologram.debug();

  // cellService() returns a boolean specifying if oyu re connected or not
  if(runOnce && Hologram.cellService()) {

    // Send string data to Hologram's Cloud
    Hologram.send("Hello World");

    // Send JSON data to Hologram's Cloud
    Hologram.send("{\"message\": \"this is a JSON message\"}");

    // Send SMS through TCP - must be fully qualified international format
    Hologram.sendSMS("+13125556666", "SMS from Hologram with love");

    // cellStrength() returns an int specifying the quality of your connection [0-none,1-poor,2-good,3-great]
    switch (Hologram.cellStrength()) {
        case 0:
            Serial.println("Connection is non-existent");
        case 1:
            Serial.println("Connection is poor");
        case 2:
            Serial.println("Connection is good");
        case 3:
            Serial.println("Connection is great");
    }

    runOnce = false;

  }

  // availableMessage() returns the length of any unread messages
  if(Hologram.availableMessage() > 0) {

    // readMessage() returns both incoming TCP data or incoming SMS messages
    // once you read a message the buffer is wiped, if called again nothing will be there
    Serial.print("Incoming Message: ");
    Serial.println(Hologram.readMessage());

  }

}
