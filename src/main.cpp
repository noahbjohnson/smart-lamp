#include <Arduino.h>
#include <CapacitiveSensor.h>
#include "FastLED.h"

/**
 * CONFIGURATION
 * Edit these values for your setup as needed
 */

// PINS
const uint8_t SEND_PIN = 2; // Connect a 100K-50M ohm resistor between this and the RECEIVE_PIN
const uint8_t RECEIVE_PIN = 4; // Connect the capacitance sensing input to this digital pin
const int RGB_PIN = 5; // The output for the RGB strip data

// RGB
const int NUM_LED = 85;

// INPUT
const unsigned long debounceMilliseconds = 30;

// CAPACITANCE

// The minimum capacitance required to trigger an on state.
// Use the serial output to get an idea for what your value should be.
long unsigned onThreshold = 3500;

// How often to write the analog value to the serial port in ms
unsigned long serialWriteFrequency = 100;

// The number of samples to average for a sensor reading
unsigned long sampleSize = 75;

// The period of time to use as a baseline calibration period
unsigned long calibrationSeconds = 40;

/**
 * GLOBAL DECLARATIONS
 */

CapacitiveSensor cs = CapacitiveSensor(SEND_PIN, RECEIVE_PIN);

// SERIAL GLOBALS
unsigned long serialWriteTime = millis();

// LAMP GLOBALS
unsigned char maxLevel = 255;
unsigned char currentLevel;
bool isOn = true;
bool currentlyTouched = false;

// CAPACITANCE GLOBALS
unsigned long currentCapacitance;
unsigned long lastReadLength; // how long the last capacitance sensor read took
unsigned long lastStateChange;

// RGB
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB ledList[NUM_LED];
#define BRIGHTNESS 255

[[maybe_unused]] void setup() {
    Serial.begin(9600);
    cs.set_CS_AutocaL_Millis(calibrationSeconds * 1000);

    // Set the dimmer to max
    currentLevel = maxLevel;

    delay(1000); // for recovery?
    CFastLED::addLeds<LED_TYPE, RGB_PIN, COLOR_ORDER>(ledList, NUM_LED)
            .setCorrection(TypicalLEDStrip)
            .setDither(BRIGHTNESS < 255);

    FastLED.setBrightness(BRIGHTNESS);
}

// get the capacitance reading
void readCapacitance() {
    unsigned long start = millis();
    currentCapacitance = cs.capacitiveSensor(sampleSize);
    lastReadLength = millis() - start;
}

// decide if the sensor is being touched based on the threshold and value
void setTouched() {
    currentlyTouched = (currentCapacitance > onThreshold);
}

void logCapacitance() {
    if ((millis() - serialWriteTime) > serialWriteFrequency) {
        serialWriteTime = millis();
        Serial.print(lastReadLength);
        Serial.print("\t");
        Serial.println(currentCapacitance);
    }
}

void toggleOnOff() {
    if ((millis() - lastStateChange) < debounceMilliseconds) {
        return;
    }
    if (isOn) {
        currentLevel = maxLevel;
    }
    isOn = !isOn;

    if (isOn) {
        Serial.print("state set to on");
    } else {
        Serial.print("state set to off");
    }

    lastStateChange = millis();
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;

    uint8_t sat8 = beatsin88(87, 220, 250);
    uint8_t brightdepth = beatsin88(341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16;//gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for (uint16_t i = 0; i < NUM_LED; i++) {
        hue16 += hueinc16;
        uint8_t hue8 = hue16 / 256;

        brightnesstheta16 += brightnessthetainc16;
        uint16_t b16 = sin16(brightnesstheta16) + 32768;

        uint16_t bri16 = (uint32_t) ((uint32_t) b16 * (uint32_t) b16) / 65536;
        uint8_t bri8 = (uint32_t) (((uint32_t) bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);

        CRGB newcolor = CHSV(hue8, sat8, bri8);

        uint16_t pixelnumber = i;
        pixelnumber = (NUM_LED - 1) - pixelnumber;

        nblend(ledList[pixelnumber], newcolor, 64);
    }
}

[[maybe_unused]] void loop() {
    // Copy previous loop values
    bool previouslyTouched = currentlyTouched;

    // Check if the lamp is touched
    readCapacitance();
    logCapacitance();
    setTouched();

    // did they stop touching the lamp? if so toggle
    if (!currentlyTouched) {
        if (previouslyTouched != currentlyTouched) {
            toggleOnOff();
        }
    }

    if (isOn) {
//        pride();
//        FastLED.showColor(CRGB::White);
        FastLED.showColor(UncorrectedTemperature);
        FastLED.setBrightness(255);
        FastLED.show();
    } else {
        FastLED.clearData();
        FastLED.show();
    }
}

