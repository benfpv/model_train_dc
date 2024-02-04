/*
    Proximity_IR.cpp - Library for IR Proximity sensors.
    Created by Benfpv.
    Released into the public domain.
*/

#ifndef Proximity_IR_h
#define Proximity_IR_h

#include "Arduino.h"

class Proximity_IR
{
    public:
        // Attributes - Status
        int _currentStatus;
        int _sizeOfStatusSTM;
        int _statusSTM[6];
        int _statusMode;
        int _statusHold;
        // Attributes - Temp Status
        int _statusTemp;
        // Functions
        Proximity_IR(int pinInput);
        void clear_attributes();
        void begin();
        void read();
        void readMode();
        void readHold();
    private:
        int _pinInput;
};

#endif