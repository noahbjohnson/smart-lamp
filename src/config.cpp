//
// Created by Noah Johnson on 8/17/21.
//
#include "main.h"

/**
 * CONFIGURATION
 * Edit these values for your setup as needed
 */

// PINS
const uint8_t SEND_PIN = 2; // Connect a 100K-50M ohm resistor between this and the RECEIVE_PIN
const uint8_t RECEIVE_PIN = 4; // Connect the capacitance sensing input to this digital pin

// RGB
const int NUM_LED = 85;

// The list of lamp modes to cycle through, in order. Power cycle will always reset to first item
callbackFunction lampModes[2] = {
        white,
        pride
};

// INPUT
//const unsigned long debounceMilliseconds = 30;

// CAPACITANCE

// The minimum capacitance required to trigger an on state.
// Use the serial output to get an idea for what your value should be.
long unsigned onThreshold = 350;

// How often to write the analog value to the serial port in ms
unsigned long serialWriteFrequency = 100;

// The number of samples to average for a sensor reading
unsigned long sampleSize = 30;

// The period of time to use as a baseline calibration period
unsigned long calibrationSeconds = 40;

/**
 * END CONFIG
 */

CRGB ledList[NUM_LED];
