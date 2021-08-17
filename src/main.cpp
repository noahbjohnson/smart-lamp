#include <main.h>

// RGB
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define RGB_PIN 5

// When the lamp is touched once, turn on/off the light and reset the brightness
void handleSingleTap() {
    Serial.println("handling tap");
    isOn = !isOn;
    currentBrightnessLevel = maxBrightnessLevel;
}

// When the lamp is touched twice, cycle to the next mode
void handleDoubleTap() {
    Serial.println("handling double tap");
    lampModeIndex++;
    if (lampModeIndex >= lampModeLength) {
        lampModeIndex = 0;
    }
}

[[maybe_unused]] void setup() {
    Serial.begin(9600);
    cs.set_CS_AutocaL_Millis(calibrationSeconds * 1000);

    // Set the dimmer to max
    currentBrightnessLevel = maxBrightnessLevel;

    // Setup LED
    delay(1000); // for recovery?
    CFastLED::addLeds<LED_TYPE, RGB_PIN, COLOR_ORDER>(ledList, NUM_LED)
            .setCorrection(TypicalLEDStrip)
            .setDither(false);

    // Setup handlers
    touchController.attachClick(handleSingleTap);
    touchController.attachDoubleClick(handleDoubleTap);
}

// get the capacitance reading
void readCapacitance() {
    unsigned long start = millis();
    currentCapacitance = cs.capacitiveSensor(sampleSize);
    lastReadLength = millis() - start;
}

// log the measurements to uart
void logCapacitance() {
    if ((millis() - serialWriteTime) > serialWriteFrequency) {
        serialWriteTime = millis();
        Serial.print(lastReadLength);
        Serial.print("\t");
        Serial.print(currentCapacitance);
        Serial.print("\t");
        Serial.println(isOn);
    }
}

[[maybe_unused]] void loop() {
    // Get the capacitance reading from the lamp and log it
    readCapacitance();
    logCapacitance();

    // decide if the sensor is being touched based on the threshold and value
    currentlyTouched = currentCapacitance > onThreshold;

    // Send touch status to controller
    touchController.tick(currentlyTouched);

    // Show the selected mode or nothing
    if (isOn) {
        lampModes[lampModeIndex]();
    } else {
        FastLED.showColor(CRGB::Black);
    }

    // Update LED state
    FastLED.show();
}

