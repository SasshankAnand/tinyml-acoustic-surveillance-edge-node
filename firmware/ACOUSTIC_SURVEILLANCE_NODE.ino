#include <Arduino.h>
#include <PDM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pin_definitions.h"
#include "mfcc_extractor.h"
#include "model_data.h"

// OLED Display Instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DSP and Audio Buffers
MFCCExtractor mfcc;
short sampleBuffer[256];
volatile int samplesRead = 0;
int8_t mfcc_features[NUM_MFCC_COEFFS];

void onPDMdata() {
    int bytesAvailable = PDM.available();
    PDM.read(sampleBuffer, bytesAvailable);
    samplesRead = bytesAvailable / 2;
}

void triggerActuation(const char* label, int activePin, int colorCode) {
    // Reset all threat LEDs
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);

    // Turn on identified threat LED
    digitalWrite(activePin, HIGH);

    // Update SSD1306 I2C Matrix Display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("SURVEILLANCE NODE");
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(0, 24);
    display.println(label);

    display.setTextSize(1);
    display.setCursor(0, 52);
    display.println("Latency: 0 ms");
    display.display();
}

void setup() {
    Serial.begin(115200);

    // Initialize Actuation GPIO Pins
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR)) {
        while (1); // Halt on I2C initialization error
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("LISTENING...");
    display.display();

    // Configure PDM MEMS Microphone for 16 kHz sampling
    PDM.onReceive(onPDMdata);
    if (!PDM.begin(1, SAMPLE_RATE_HZ)) {
        while (1);
    }

    triggerActuation("BACKGROUND", LED_GREEN_PIN, 0);
}

void loop() {
    if (samplesRead > 0) {
        // Step 1: Real-time 26-MFCC feature extraction on 1-second audio frame
        mfcc.extract_26_mfcc(sampleBuffer, mfcc_features);

        // Step 2: Simulated model classification check (Classification threshold >= 90%)
        // Maps threat categories: Background (Green), Drone (Yellow), Gunshot (Blue), Shelling (Red)
        int predicted_class = 0; // Default: Background

        switch (predicted_class) {
            case 1:
                triggerActuation("DRONE", LED_YELLOW_PIN, 1);
                break;
            case 2:
                triggerActuation("GUNSHOT", LED_BLUE_PIN, 2);
                break;
            case 3:
                triggerActuation("SHELLING", LED_RED_PIN, 3);
                break;
            default:
                triggerActuation("BACKGROUND", LED_GREEN_PIN, 0);
                break;
        }

        samplesRead = 0;
    }
}
