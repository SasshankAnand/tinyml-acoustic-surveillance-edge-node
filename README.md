# Field Deployable TinyML Edge Node for Autonomous Acoustic Surveillance

An autonomous, resource-constrained, zero-RF emission tactical edge surveillance system running on the **Arduino Nano 33 BLE Sense (ARM Cortex-M4F)**. It performs continuous 16 kHz acoustic sampling, real-time 26-MFCC feature extraction, and 8-bit quantized CNN inference with deterministic sub-millisecond latency.

## Inventors & Affiliation
- **Dr. Vidhyapathi C.M** (Assistant Professor Sr. Grade 1, SENSE, VIT Vellore)
- **Gokulapriya S** (23BEC0444, VIT Vellore)
- **Kavya S** (23BEC0222, VIT Vellore)
- **Sasshank Anand** (23BEC0099, VIT Vellore)

## Key Technical Specifications
- **Target Hardware:** Arduino Nano 33 BLE Sense (Nordic nRF52840 / ARM Cortex-M4F with FPU & DSP extensions)
- **Sensor:** On-board STMicroelectronics MP34DT05 MEMS Digital Microphone (PDM)
- **Memory Footprint:**
  - **Flash / Program Space:** 409,240 bytes (~41% of 983,040 bytes available)
  - **Dynamic RAM (SRAM):** 105,248 bytes (~40% of 262,144 bytes available)
- **Inference Latency:** Sub-millisecond (0 to 1 ms)
- **Operational Architecture:** Zero-RF emission / cloud-independent / anti-jamming tactical design

## Threat Taxonomy & Hardware Actuation
| Class ID | Threat Class | OLED Display Output | LED Indicator | Confidence Threshold |
| :--- | :--- | :--- | :--- | :--- |
| `0` | **Background** | `BACKGROUND` | 🟢 Green LED | Default state |
| `1` | **UAV / Drone** | `DRONE` | 🟡 Yellow LED | $\ge$ 90% |
| `2` | **Gunshot** | `GUNSHOT` | 🔵 Blue LED | $\ge$ 90% |
| `3` | **Artillery / Shelling** | `SHELLING` | 🔴 Red LED | $\ge$ 90% |

## DSP & ML Pipeline
1. **Acoustic Acquisition:** 16 kHz sampling via PDM microphone, segmented into 1-second discrete temporal snapshots.
2. **Deterministic DSP:** DC offset elimination, Hamming windowing, 256-point FFT, Mel-filterbank integration, and Discrete Cosine Transform (DCT) to extract 26 MFCCs with dynamic Z-score normalization.
3. **Quantized Inference:** 8-bit integer quantized (`int8`) CNN executed via **TensorFlow Lite Micro**.
4. **Actuation:** Direct GPIO pin triggering and SSD1306 I2C OLED (128x64) real-time event updates.

## Repository Layout
```text
├── firmware/
│   ├── ACOUSTIC_SURVEILLANCE_NODE.ino   # Main embedded application firmware
│   ├── mfcc_extractor.h                 # DSP & 26-MFCC feature extraction
│   ├── model_data.h                     # Quantized TFLite Micro int8 byte array
│   └── pin_definitions.h                # GPIO & I2C hardware pin mappings
├── training/
│   └── train_tinyml_model.py            # CNN training, MFCC pipeline, and TFLite quantization
├── requirements.txt
└── README.md
```
