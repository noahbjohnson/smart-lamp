//
// Created by Noah Johnson on 8/17/21.
//

#ifndef SMARTLAMP_MAIN_H
#define SMARTLAMP_MAIN_H

#include <Arduino.h>
#include <CapacitiveSensor.h>
#include "FastLED.h"
#include <OneButton.h>

/**
 * LAMP MODES
 */

void pride();
void white();

/**
 * CONFIGURATION
 * Edit these values for your setup as needed
 */

// PINS
extern const uint8_t SEND_PIN;
extern const uint8_t RECEIVE_PIN;

// RGB
extern const int NUM_LED;

// The list of lamp modes to cycle through, in order. Power cycle will always reset to first item
#define lampModeLength 2
extern callbackFunction lampModes[2];

// INPUT
//const unsigned long debounceMilliseconds = 30;

// CAPACITANCE

// The minimum capacitance required to trigger an on state.
// Use the serial output to get an idea for what your value should be.
extern long unsigned onThreshold;

// How often to write the analog value to the serial port in ms
extern unsigned long serialWriteFrequency;

// The number of samples to average for a sensor reading
extern unsigned long sampleSize;

// The period of time to use as a baseline calibration period
extern unsigned long calibrationSeconds;

/**
 * END CONFIG
 */


/**
 * GLOBAL DECLARATIONS
 */

extern CapacitiveSensor cs;
extern unsigned long serialWriteTime;
extern unsigned char maxBrightnessLevel;
extern unsigned char currentBrightnessLevel;
extern bool isOn;
extern bool currentlyTouched;
extern int8_t lampModeIndex;
extern unsigned long currentCapacitance;
extern unsigned long lastReadLength;
extern OneButton touchController;

// RGB
extern CRGB ledList[];

#endif //SMARTLAMP_MAIN_H
