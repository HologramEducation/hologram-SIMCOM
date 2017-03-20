# Hologram SIMCOM SIM800 Helper Library

Hologram800 is a library to make it easier to interact with Hologram's Cloud from hardware using a SIMCOM SIM800 family of cellular modules.

The SIMCOM SIM800 module is one of the most popular and affordable GPRS/GSM modules on the market. It only works with 2G networks. Hologram offers global 2G coverage with 2G in the U.S. until 2020.

- Buy [SIM800 on Amazon](https://www.amazon.com/s/ref=nb_sb_noss/157-0659706-0121026?url=search-alias%3Daps&field-keywords=SIM800L+gprs+board)
- Buy [SIM800 on AliExpress](https://www.aliexpress.com/wholesale?catId=0&SearchText=SIM800L+gprs+board)
- Buy [Hologram Global SIM Card](https://hologram.io/store/)

## Dependencies

- Requires the SoftwareSerial library

## Installation

1. Download the latest version of the library from https://github.com/hologram-io/Hologram-SIM800/releases and save the file somewhere
2. In the Arduino IDE, go to the Sketch -> Import Library -> Add Library... menu option
3. Find the zip file that you saved in the first step, and choose that
4. Check that it has been successfully added by opening the Sketch -> Import Library menu.  You should now see **Hologram SIMCOM SIM800** listed among the available libraries.

## Usage

This library requires an activated Hologram SIM.

1. Activate a SIM card through [Hologram's new device form](https://dashboard.hologram.io/activate).
2. Under that device's details, go to configuration tab and generate Router Credentials.
3. Use the Shared ID and Shared Key values in your sketch.

See the [KitchenSink](https://github.com/hologram-io/hologram-SIM800/blob/master/examples/KitchenSink/KitchenSink.ino) example sketch for more detail on how the library is used.
