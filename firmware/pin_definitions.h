#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// Hardware Actuation Pins (LEDs)
#define LED_GREEN_PIN    2   // Background
#define LED_YELLOW_PIN   3   // Drone / UAV
#define LED_BLUE_PIN     4   // Gunshot
#define LED_RED_PIN      5   // Artillery / Shelling

// OLED Display Configuration (I2C)
#define SCREEN_WIDTH     128
#define SCREEN_HEIGHT    64
#define OLED_RESET       -1
#define SCREEN_I2C_ADDR  0x3C

// Sampling & DSP Configuration
#define SAMPLE_RATE_HZ   16000
#define AUDIO_WINDOW_MS  1000
#define TOTAL_SAMPLES    (SAMPLE_RATE_HZ * AUDIO_WINDOW_MS / 1000) // 16000 samples
#define NUM_MFCC_COEFFS  26

#endif // PIN_DEFINITIONS_H
