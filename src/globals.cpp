//
// Created by Noah Johnson on 8/17/21.
//
#include "main.h"

CapacitiveSensor cs = CapacitiveSensor(SEND_PIN, RECEIVE_PIN);

unsigned long serialWriteTime = millis();

// LAMP GLOBALS
unsigned char maxBrightnessLevel = 255;
unsigned char currentBrightnessLevel;
bool isOn = true;
bool currentlyTouched = false;
int8_t lampModeIndex = 0; // the index of the mode (in the list of modes) that the lamp should be in

// CAPACITANCE GLOBALS
unsigned long currentCapacitance;
unsigned long lastReadLength; // how long the last capacitance sensor read took
unsigned long lastStateChange;

OneButton touchController = OneButton();
