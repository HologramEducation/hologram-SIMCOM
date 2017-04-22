#include <HologramSIMCOM.h>

#define RX_PIN 8 //SIM800 RX connected to pin D8
#define TX_PIN 7 //SIM800 TX connected to pin D7
#define RESET_PIN 10 //SIM800 reset connected to pin D10
#define HOLO_KEY "********" //replace w/your SIM key
#define LED_PIN 13 //led
#define BTN_PIN 12 //push button

int ledState = 0; //this variable tracks the state of the LED, negative if off, positive if on
int ledSwitch = 0;
unsigned ledStart;
unsigned ledLongest = 0;
unsigned lastSend = millis();
int sendInterval = 120; // send stats every 2 minutes

HologramSIMCOM Hologram(TX_PIN, RX_PIN, RESET_PIN, HOLO_KEY); // Instantiate Hologram

void setup() {
    Serial.begin(19200);
    while(!Serial);

    // Start modem and connect to Hologram's global network
    Hologram.debug();
    bool cellConnected = Hologram.begin(19200, 8888); // set baud to 19200 and start server on port 8888
    if(cellConnected) {
        Serial.println(F("Cellular is connected"));
    } else {
        Serial.println(F("Cellular connection failed"));
    }

    //set modes for used pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT);

    Serial.println(F("Setup complete"));
}

void loop() {
    Hologram.debug();

    // Toggle LED with physical button press
    if(digitalRead(BTN_PIN) == HIGH) {
        toggleLed();
        while(digitalRead(BTN_PIN) == HIGH); // wait until user stops pressing button
    }

    // Check for inbound messages
    // Toggle LED through inbound messages
    if(Hologram.availableMessage() > 0) {
        String message = Hologram.readMessage();

        if(message == "LED") { // send message "LED" to toggle led remotely
            toggleLed();
        } else {
            Serial.print(F("Unknown inbound message: "));
            Serial.println(message);
        }
    }

    // Send collected data of LED stats
    sendLedData();
}

void toggleLed() {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    ledSwitch++; // count switches

    // Check for new logest record
    if(ledState) {
        ledStart = millis();
    } else if(ledStart > 0 && (millis() - ledStart) > ledLongest) {
        // we have a new record!
        ledLongest = millis() - ledStart;
        ledStart = 0;
    }

    delay(100); // needs a little time to settle down
}

void sendLedData() {
    unsigned m = millis();
    if(
            m - lastSend > (sendInterval * 1000)
            && ledSwitch > 0
            && Hologram.cellService()
            && Hologram.cellStrength() > 1) {

        String data = "{\"currentState\":" + String(ledState) + ",\"switched\":" + ledSwitch + ",\"longestOn\":" + ledLongest + "}";

        // Reset everything
        lastSend = m;
        ledSwitch = 0;
        ledLongest = 0;

        // send into space
        Hologram.send(data, String("LED_STATS, HACKERBOX"));
        //Hologram.sendSMS("+13125556666", "Sending SMS, your LED data is in the cloud ☁️");
    }
}

