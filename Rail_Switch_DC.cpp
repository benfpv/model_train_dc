/*
    Rail_Switch_DC.cpp - Library for Rail Switch control using L298N (or similar) motor driver.
    Created by Benfpv, 2023.
    Released into the public domain.
*/

#include "Arduino.h"
#include "Rail_Switch_DC.h"
#include "funcs_arduino.h"

Rail_Switch_DC::Rail_Switch_DC(int pin_EN, int pin_IN1, int pin_IN2)
{
  _pin_EN = pin_EN;
  _pin_IN1 = pin_IN1;
  _pin_IN2 = pin_IN2;
}

void Rail_Switch_DC::begin()
{
  pinMode(_pin_EN, OUTPUT);
  pinMode(_pin_IN1, OUTPUT);
  pinMode(_pin_IN2, OUTPUT);
  delay(_switchDelay_ms);
  // Set Idle
  digitalWrite(_pin_IN1, LOW);
  digitalWrite(_pin_IN2, LOW);
}

void Rail_Switch_DC::set_switch(int targetSwitchInt)
{
  /* 
  Inputs: 
  - switchDirectionTargetInt: Int to determine the target direction of switch:
  (-1) Left (if travelling in subordinate direction).
  (0) Center/Middle.
  (1) Right (if travelling in subordinate direction).  
  */
  if (targetSwitchInt == 1) {
    // Serial.print("Set Switch: "); Serial.println(targetSwitchInt);
    digitalWrite(_pin_IN1, LOW);
    digitalWrite(_pin_IN2, HIGH);
    analogWrite(_pin_EN, 255);
    delay(_switchDelay_ms);
    _currentSwitch = targetSwitchInt;
    analogWrite(_pin_EN, 0);
    digitalWrite(_pin_IN1, LOW);
    digitalWrite(_pin_IN2, LOW);
  }
  else if (targetSwitchInt == -1) {
    // Serial.print("Set Switch: "); Serial.println(targetSwitchInt);
    digitalWrite(_pin_IN1, HIGH);
    digitalWrite(_pin_IN2, LOW);
    analogWrite(_pin_EN, 255);
    delay(_switchDelay_ms);
    _currentSwitch = targetSwitchInt;
    analogWrite(_pin_EN, 0);
    digitalWrite(_pin_IN1, LOW);
    digitalWrite(_pin_IN2, LOW);
  }
}