# Hologram SIMCOM Arduino Helper Library

`HologramSIMCOM` is a library that makes it easier to interact with Hologram's Cloud using cellular modules from SIMCOM. SIMCOM 2G/3G modules are some of the most popular and affordable GPRS/GSM modules on the market.

NOTE: Hologram offers global 2G coverage, U.S. 2G coverage to mid-2020.

This library is confirmed working with the following modules:

- All modules from SIM800 family
- All modules from SIM900 family
- [Seeed Studio GPRS Shield](https://www.seeedstudio.com/GPRS-Shield-V3.0-p-2333.html)

A [Hologram account and SIM Card](https://hologram.io/store/) is required to use this library.

## Dependencies

- Requires [SoftwareSerial](https://www.arduino.cc/en/Reference/softwareSerial) library

## Installation

1. Download the latest version of the library from https://github.com/hologram-io/Hologram-SIMCOM/releases and save the file somewhere
2. In the Arduino IDE, go to the Sketch -> Import Library -> Add Library... menu option
3. Find and select the zip file you saved in the first step.
4. If successfully added you should now see **Hologram SIMCOM** listed among available libraries under Sketch -> Import.

## Getting Started

1. Activate a SIM card through [Hologram's new device form](https://dashboard.hologram.io/activate).
2. Under that device's details, go to Configuration tab and generate Router Credentials, copy the Device Key.
3. Open The [KitchenSink](https://github.com/hologram-io/hologram-SIMCOM/blob/master/examples/KitchenSink/KitchenSink.ino) example, IDE -> File -> Examples -> Hologram SIMCOM -> KitchenSink.
4. In your sketch define the Device Key we got from the Router Credentials modal.
5. Connect the TX, RX, & Reset pins.
6. Upload sketch to your Arduino, open the Serial Monitor and watch the magic happen.

## Reference

### HologramSIMCOM Hologram(rx,tx,reset,key)

Required. This goes before your start function. It instantiates HologramSIMCOM and gives the library the info it needs to connect to the device and Hologram's cloud.

```c
#include <HologramSIMCOM.h>

#define RX_PIN 8
#define TX_PIN 7
#define RESET_PIN 10
#define HOLO_KEY "********"

HologramSIMCOM Hologram(TX_PIN, RX_PIN, RESET_PIN, HOLO_KEY);
```

### .begin(baud, port)

Start-up the modem and connect to the cellular network. Typically this would go inside `start()` but could also be useful to re-establish connection inside the `loop()`.

#### Parameters
- **baud** - int : What baud do you want your module to run at? `19200` is typical for SIMCOM modules.
- **port** - int : Optionally, If you want to receive inbound messages you'll need to specify a server port.  

#### Return
Returns a boolean depending if it successfully connected or not.

#### Example

```c
void setup() {
    // connect to the network and start listening for incoming data
    Hologram.begin(19200, 8888);
}
```

### .send(data, topic) 

Send data to the Hologram cloud where you can manipulate and route it to multiple cloud providers.

#### Parameters
- **data** - char*/String : String or char* to be sent to the cloud
- **topic** - char*/String : Optionally, Hologram allows you to publish data to a topic(s).

#### Return
Returns a boolean whether send was successful or not.

#### Example

```c
void loop() {
    // Send a char array to the cloud
    Hologram.send("Hello World");
    
    // Send a String to the cloud
    Hologram.send(String("Hello World"));
    
    // Send a JSON string that can be parsed by the Hologram Router
    Hologram.send("{\"desc\":\"sensor-1\",\"temp\":78,\"hum\":20}");
    
    // Publish to one topic
    Hologram.send("Hello World", "cool-topic");
    
    // Publish to multiple topics
    Hologram.send("Hello World", "cool-topic,cooler-topic");
}
```

### .sendSMS(phoneNumber, message) 

Send an outgoing SMS. This is a soft-SMS, which means it send a data string to the Hologram cloud then Hologram sends the SMS to it's destination. Optionally you can purchase a phone number for your SIM. If you do then the receiver will get the SMS from that number. Otherwise one of Hologram's generic system numbers will be the sender.

We choose to implement a soft-SMS over standard SMS because of price, performance, and reliability. 

#### Parameters
- **phoneNumber** - char*/String : Destination phone number. Must be in international format (ex. U.S.: +1, France: +33, etc)
- **message** - char*/String : Text message to be sent.

#### Return
Returns a boolean whether send was successful or not.

#### Example
```c
void loop() {
    Hologram.sendSMS("+13125554444", "Hello World");
}
```

### .availableMessage()

Check if there's an unread inbound message. In the background, this triggers a read of the modem's buffer. This is used for both incoming TCP data and incoming SMS messages.

#### Return
Returns the string length of the unread message. If 0 then there's nothing to read, sorry.

#### Example
```c
void loop() {
    if(Hologram.availableMessage() > 0) {
        // do something
    }
}
```

### .readMessage()

Get a stored inbound data or inbound SMS string. This returns the stored String then purges it from memory after reading. That means and subsequent calls will return nothing until a new message is received by the modem.

#### Return
Returns the message as a String.

#### Example
```c
void loop() {
    String inbound;
    
    if(Hologram.availableMessage() > 0) {
        inbound = Hologram.readMessage();
    }
}
```

### .cellService()

Checks to see if you're connected or not.

#### Return
Returns true if connected, false if disconnected.

#### Example
```c
void loop() {
    if(Hologram.cellService()) {
        // do something
    }
}
```

### .cellStrength()

Lets you know how good your connection is.

#### Return
Returns an integer as follows:
- **0** No signal
- **1** Very poor signal strength
- **2** Poor signal strength
- **3** Good signal strength
- **4** Very good signal strength
- **5** Excellent signal strength

#### Example
```c
void loop() {
    switch(Hologram.cellStrength()) {
        case 0:
            // do something
        case 3:
            // do something else
    }
}
```

### .debug()

When debug is active anything written to the `Serial` is passed to the modem and anything read from the modem will be displayed in the Serial monitor.

#### Example
```c
void setup() {
    Serial.begin(19200);
    while(!Serial);

    Hologram.debug(); // put debug before begin() to monitor connection initialization
    Hologram.begin(19200, 8888);
}

void loop() {
    Hologram.debug();
}
```


## Future Goals

- Expose ability to send modem commands along with a timeout and response to wait for.
- Reduce or eliminate the dependency on `String`.
- Make send and receive functionality non-blocking.
- Make connecting more efficient (add sleep functionality and ability to control GPRS state)
- Add support for modules with integrated GPS
- Add support for modules with integrated BLE
- Support other manufactures [ublox, telit, quectel, etc]
