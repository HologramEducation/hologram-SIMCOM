/**
 * @file       HologramSIMCOM.h
 * @author     Ben Strahan
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2017 Ben Strahan
 * @date       Apr 2017
**/

#ifndef __HologramSIMCOM_H__
#define __HologramSIMCOM_H__

#include "Arduino.h"
#include <SoftwareSerial.h>

class HologramSIMCOM {
public:

    // Pre-setup - set globals -----------------------------------------
    HologramSIMCOM(const int txPin,const int rxPin,const int resetPin,const char* deviceKey)
            :serialHologram(txPin, rxPin){
        _DEVICEKEY = deviceKey;
        _RESETPIN = resetPin;
    };
    SoftwareSerial serialHologram;

    // Setup Methods ----------------------------------------------------
    bool begin(const int baud); // Must pass baud to setup module
    bool begin(const int baud, const int port); // Passing port will also start module server

    // Loop Methods ------------------------------------------------------
    bool cellService(); // checks if we are on the network
    int cellStrength(); // return cell reception strength [0-none,1-poor,2-good,3-great]
    void debug(); // enables manual serial and verbose monitoring

    bool send(char* data); // Send a char array to Hologram Cloud
    bool send(String data); // Send a String to Hologram Cloud
    bool send(char* data, const char* topics); // optionally send topics as char array
    bool send(String data, const String topics); // optionally send topics as String

    bool sendSMS(const char* phoneNum, const char* message); // Send Cloud SMS as char array
    bool sendSMS(const String phoneNum, String message); // Send Cloud SMS as Strings

    int availableMessage(); // checks if server message, returns message length
    String readMessage(); // returns message as String, resets server, resets message buffer

private:
    // Globals ------------------------------------------------------------
    const char* _DEVICEKEY; // Hologram's Device Key
    int _RESETPIN;
    int _SERVERPORT; // Modem's server port
    int _MODEMSTATE = 1; // State of modem [0-busy, 1-available]
    int _DEBUG; // State of debug flag [0-false, 1-true]
    String _MESSAGEBUFFER = ""; // Where we store inbound messages (maybe make it a char array?)
    String _SERIALBUFFER = ""; // Where we store serial reads on line at a time (maybe make it a char array?)

    // General modem functions --------------------------------------------
    void _writeSerial(const char* string); // send command to modem without waiting
    void _writeCommand(const char* command, const long timeout); // send command to modem and wait
    // Send command, wait response or timeout, return [0-timeout,1-error,2-success]
    int _writeCommand(const char* command, const long timeout, const String successResp, const String errorResp);
    void _readSerial(); // reads/analyze modem serial, store read/inbound in globals, set states, etc
    void _checkIfInbound(); // check modem server for inbound messages
    bool _connectNetwork(); // establish a connection to network and set cipstatus in prep to send/receive data

    // Server functions ---------------------------------------------------
    // future versions

    // Client functions
    bool _sendMessage(const String data);
    bool _sendResponse(const char* data);
};

#endif