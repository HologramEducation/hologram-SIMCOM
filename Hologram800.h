/**
 * @file       Hologram800.h
 * @author     Ben Strahan
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2017 Ben Strahan
 * @date       Mar 2017
**/

#ifndef __HOLOGRAM800_H__
#define __HOLOGRAM800_H__

#include "Arduino.h"
#include <SoftwareSerial.h>

class Hologram800 {
 public:
  Hologram800(
    const int txPin,
    const int rxPin,
    const char* deviceId,
    const char* deviceKey)
  :serialHologram(txPin, rxPin) {
    _rxPin = rxPin;
    _txPin = txPin;
    _deviceId = deviceId;
    _deviceKey = deviceKey;

    Serial.begin(9600);
    serialHologram.begin(9600);
  }
  bool init();
  bool sendData(const char* data);
  bool sendCloudSMS(char* phoneNum, char* message);
  bool sendSMS(char* phoneNum, char* message);
  bool sendSMStoCloud(char* message);
  void serialDebug(void);
  SoftwareSerial serialHologram;
 private:
  int _rxPin;
  int _txPin;
  char* _deviceId;
  char* _deviceKey;
  bool openTCP();
  void closeTCP();
  bool sendCommandWait(const char* command, const char* response, unsigned timeout);
};

#endif
