/*
    Rail_Switch_DC.cpp - Library for Rail Switch control using L298N (or similar) motor driver.
    Created by Benfpv.
    Released into the public domain.
*/

#ifndef Rail_Switch_DC_h
#define Rail_Switch_DC_h

#include "Arduino.h"

class Rail_Switch_DC
{
    public:
        // Attributes - Status
        int _currentSwitch = 0; // -1 left, 0 center/neutral, 1 right
        // Attributes - Switch Requirements
        int _switchDelay_ms = 10;
        // Functions
        Rail_Switch_DC(int pin_EN, int pin_IN1, int pin_IN2);
        void begin();
        // Functions - Switch
        void set_switch(int targetSwitchInt);
    private:
        int _pin_EN;
        int _pin_IN1;
        int _pin_IN2;
};

#endif