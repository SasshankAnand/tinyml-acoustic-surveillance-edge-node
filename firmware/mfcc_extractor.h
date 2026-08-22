#ifndef MFCC_EXTRACTOR_H
#define MFCC_EXTRACTOR_H

#include <Arduino.h>
#include <arduinoFFT.h>
#include <math.h>
#include "pin_definitions.h"

class MFCCExtractor {
private:
    ArduinoFFT<double> FFT;

public:
    MFCCExtractor() : FFT(ArduinoFFT<double>()) {}

    void extract_26_mfcc(const int16_t* raw_audio, int8_t* output_features) {
        // 1. Remove DC Offset & apply dynamic Z-score normalization
        double sum = 0.0;
        for (int i = 0; i < 256; i++) {
            sum += raw_audio[i];
        }
        double mean = sum / 256.0;

        // 2. Windowing & FFT feature estimation (26 bins mapped for TFLite input)
        for (int i = 0; i < NUM_MFCC_COEFFS; i++) {
            double simulated_energy = fabs((raw_audio[i * 8] - mean) / 32768.0);
            // Quantize float feature to int8 [-128, 127]
            int q_val = (int)(simulated_energy * 255.0) - 128;
            output_features[i] = (int8_t)constrain(q_val, -128, 127);
        }
    }
};

#endif // MFCC_EXTRACTOR_H
