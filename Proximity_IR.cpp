/*
    Proximity_IR.cpp - Library for IR Proximity sensors.
    Created by Benfpv, 2023.
    Released into the public domain.
*/

#include "Arduino.h"
#include "Proximity_IR.h"
#include "funcs_arduino.h"

Proximity_IR::Proximity_IR(int pinInput)
{
    _pinInput = pinInput;
}

void Proximity_IR::clear_attributes()
{
    _currentStatus = -1;
    _sizeOfStatusSTM = 0;
    set_int_array_value(6, _statusSTM, -1);
    _statusMode = -1;
    _statusHold = -1;
}

void Proximity_IR::begin()
{
    pinMode(_pinInput, INPUT);
}

void Proximity_IR::read()
{   
    int thisStatus = digitalRead(_pinInput);
    _currentStatus = thisStatus;
}

void Proximity_IR::readMode()
{
    // Get Reading
    int thisStatus = digitalRead(_pinInput);
    _currentStatus = thisStatus;
    // Roll current statusSTM
    int newStatusSTM[6];
    // Serial.print("- oldSTM: ");
    // print_array_values(6, _statusSTM);
    newStatusSTM[0] = thisStatus;
    for (int i = 0; i < 6-1; i++) {
        newStatusSTM[i+1] = _statusSTM[i];
    }
    // Set new STM values and get counts to determine statusMode
    int thisValue;
    int sizeOfValidValues = 0;
    int sizeOfZeroes = 0;
    int sizeOfOnes = 0;
    for (int i = 0; i < 6; i++) {
        thisValue = newStatusSTM[i];
        _statusSTM[i] = thisValue;
        if (thisValue == 1) { // Default or Prox Undetected
            sizeOfValidValues += 1;
            sizeOfOnes += 1;
        }
        else if (thisValue == 0) { // Prox Detected
            sizeOfValidValues += 1;
            sizeOfZeroes += 1;
        }
    }
    _sizeOfStatusSTM = sizeOfValidValues;
    // Serial.print("- newSTM: ");
    // print_array_values(6, _statusSTM);
    // Determine statusMode - the mode of the STM becomes statusMode
    int statusMode;
    if (sizeOfOnes > sizeOfZeroes) {
        statusMode = 1; // Prox Undetected
    }
    else if (sizeOfOnes < sizeOfZeroes) {
        statusMode = 0; // Prox Detected
    }
    _statusMode = statusMode;
    // Serial.print("- _statusMode: "); Serial.println(_statusMode);
}

void Proximity_IR::readHold()
{
    // Get Reading
    int thisStatus = digitalRead(_pinInput);
    _currentStatus = thisStatus;
    // Roll current statusSTM
    int newStatusSTM[6];
    // Serial.print("- oldSTM: ");
    // print_array_values(6, _statusSTM);
    newStatusSTM[0] = thisStatus;
    for (int i = 0; i < 6-1; i++) {
        newStatusSTM[i+1] = _statusSTM[i];
    }
    // Set new public value and get counts to determine statusMode
    int thisValue;
    int sizeOfValidValues = 0;
    int sizeOfZeroes = 0;
    for (int i = 0; i < 6; i++) {
        thisValue = newStatusSTM[i];
        _statusSTM[i] = thisValue;
        if (thisValue == 1) { // Default or Prox Undetected
            sizeOfValidValues += 1;
        }
        else if (thisValue == 0) { // Prox Detected
            sizeOfValidValues += 1;
            sizeOfZeroes += 1;
        }
    }
    _sizeOfStatusSTM = sizeOfValidValues;
    // Serial.print("- newSTM: ");
    // print_array_values(6, _statusSTM);
    // Determine statusMode - As long as one Zero remains in STM, 0 becomes statusHold
    int statusHold;
    if (sizeOfZeroes > 0) {
      statusHold = 0; // Prox Undetected
    }
    else {
      statusHold = 1;
    }
    _statusHold = statusHold;
    // Serial.print("- _statusHold: "); Serial.println(_statusHold);
}