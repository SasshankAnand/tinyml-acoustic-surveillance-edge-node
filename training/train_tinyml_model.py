"""
Off-device training pipeline for the Acoustic Surveillance Node.
Extracts 26 MFCC features, trains a quantized CNN, and converts to TFLite Micro.
"""
import tensorflow as tf
from tensorflow.keras import layers, models
import numpy as np

def build_quantized_acoustic_cnn(input_shape=(26, 1, 1), num_classes=4):
    model = models.Sequential([
        layers.Input(shape=input_shape),
        layers.Conv2D(16, (3, 1), padding='same', activation='relu'),
        layers.BatchNormalization(),
        layers.MaxPooling2D((2, 1)),
        layers.Conv2D(32, (3, 1), padding='same', activation='relu'),
        layers.MaxPooling2D((2, 1)),
        layers.Flatten(),
        layers.Dense(64, activation='relu'),
        layers.Dropout(0.2),
        layers.Dense(num_classes, activation='softmax')
    ])
    model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
    return model

def export_int8_tflite(model, representative_data):
    def representative_dataset():
        for sample in representative_data:
            yield [sample.astype(np.float32)]

    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8
    
    tflite_model = converter.convert()
    with open("model_quantized.tflite", "wb") as f:
        f.write(tflite_model)
    print("Exported int8 TFLite model successfully.")

if __name__ == "__main__":
    model = build_quantized_acoustic_cnn()
    model.summary()
