/**
 * @file       Hologram800.cpp
 * @author     Ben Strahan
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2017 Ben Strahan
 * @date       Mar 2017
**/

#include "Arduino.h"
#include "Hologram800.h"

bool Hologram800::init() {
  // Start serials
  Serial.begin(9600);

  // Check if module is available for commands
  if (!sendCommandWait("AT\r\n", "OK\r\n", 10)) {
    Serial.println(F("ERROR no modem serial available"));
    return false;
  }

  // Set Phone Functionality to full
  if (!sendCommandWait("AT+CFUN=1\r\n", "OK", 10)) {
    Serial.println(F("ERROR setting modem mode"));
    return false;
  }

  // Set SMS mode to Text
  if (!sendCommandWait("AT+CMGF=1\r\n", "OK", 5)) {
    Serial.println(F("ERROR setting SMS mode"));
    return false;
  }

  // Set APN for data transmittal
  // Check if state equals 'IP INITIAL'
  // If it does then APN needs to be set
  if (sendCommandWait("AT+CIPSTATUS\r\n", "INITIAL", 5)) {
    sendCommandWait("AT+CSTT=\"hologram\"\r\n", "OK", 5);
    if (!sendCommandWait("AT+CSTT?\r\n", "hologram", 5) && !sendCommandWait("AT+CIPSTATUS\r\n", "START", 5)) {
      Serial.println(F("ERROR APN not set correctly or module state not IP START"));
      return false;
    }
  }

  return true;
}

bool Hologram800::sendData(const char* data) {
  // open TCP connection
  if (!openTCP()) {
    return false;
  }

  // Write data to serial
  serialHologram.print(F("{\"s\": \""));
  serialHologram.print(_deviceId);
  serialHologram.print(F("\", \"c\": \""));
  serialHologram.print(_deviceKey);
  serialHologram.print(F("\", \"d\": \""));
  serialHologram.print(data);
  serialHologram.println(F("\"}\x1A"));

  // close TCP connection
  closeTCP();

  return true;
}

bool Hologram800::sendCloudSMS(char* phoneNum, char* message) {
  // open TCP connection
  if (!openTCP()) {
    return false;
  }

  // Write data to serial
  serialHologram.print(F("S"));
  serialHologram.print(_deviceId);
  serialHologram.print(_deviceKey);
  serialHologram.print(phoneNum);
  serialHologram.print(F(" "));
  serialHologram.print(message);
  serialHologram.println(F("\x1A"));

  // close TCP connection
  closeTCP();

  return true;
}

bool Hologram800::sendSMS(char* phoneNum, char* message) {
  // Initiate SMS send
  char SMS_command[32];
  snprintf(SMS_command, sizeof(SMS_command), "AT+CMGS=\"%s\"\r\n", phoneNum);
  if (!sendCommandWait(SMS_command, ">", 5)) {
    Serial.println(F("ERROR initiating SMS send"));
    return false;
  }

  serialHologram.print(message);
  serialHologram.println(F("\x1A"));

  return true;
}

bool Hologram800::sendSMStoCloud(char* message) {
  return Hologram800::sendSMS("+447937405250", message);
}

void Hologram800::serialDebug(void) {
  // Serial.println("serialDebug called");
  while (serialHologram.available()) {
    Serial.write(serialHologram.read());
  }
  while (Serial.available()) {
    serialHologram.write(Serial.read());
  }
}

bool Hologram800::openTCP() {
  // Check is state equals IP INITIAL
  // If it does then APN needs to be set
  if (sendCommandWait("AT+CIPSTATUS\r\n", "INITIAL", 2)) {
    sendCommandWait("AT+CSTT=\"hologram\"\r\n", "OK", 5);
  }

  // Bring up GPRS connection
  // if error check state AT+CIPSTATUS
  if (sendCommandWait("AT+CIPSTATUS\r\n", "START", 2)) {
    if (!sendCommandWait("AT+CIICR\r\n", "OK", 85)) {
      Serial.println(F("ERROR: brining up GPRS connection"));
      return false;
    }
  }

  // Get local IP address
  while (sendCommandWait("AT+CIPSTATUS\r\n", "STATE: IP GPRSACT", 2)) {
    serialHologram.write("AT+CIFSR\r\n");
  }

  // TCP server connection
  if (!sendCommandWait("AT+CIPSTART=\"TCP\",\"23.253.146.203\",\"9999\"\r\n", "CONNECT OK", 160)) {
    Serial.println(F("ERROR starting TCP connection"));
    return false;
  }

  // Initiate data sending command
  if (!sendCommandWait("AT+CIPSEND\r\n", ">", 5)) {
    Serial.println(F("ERROR initiating data send"));
    return false;
  }

  return true;
}

void Hologram800::closeTCP() {
  delay(2000);

  // Close server connection
  serialHologram.write("AT+CIPCLOSE\r\n");

  // Destroy GPRS connection
  serialHologram.write("AT+CIPSHUT\r\n");
}

bool Hologram800::sendCommandWait(const char* command, const char* response, unsigned timeout) {
  int len = strlen(response);
  int sum = 0;
  unsigned long timerStart, timerEnd;
  timerStart = millis();

  serialHologram.write(command);

  while (1) {
    if (serialHologram.available()) {
      char c = serialHologram.read();
      sum = (c == response[sum]) ? sum+1 : 0;
      if (sum == len)break;
    }
    timerEnd = millis();
    if (timerEnd - timerStart > 1000 * timeout) {
      return false;
    }
  }

  while (serialHologram.available()) {
    serialHologram.read();
  }

  return true;
}
