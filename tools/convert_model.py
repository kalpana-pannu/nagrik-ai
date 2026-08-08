import tensorflow as tf

model = tf.keras.models.load_model("nagrik_gas_model.keras")

# Export to SavedModel format first (more reliable for conversion)
model.export("nagrik_saved_model")

# Convert from the SavedModel directory instead of directly from .keras
converter = tf.lite.TFLiteConverter.from_saved_model("nagrik_saved_model")
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

with open("nagrik_gas_model.tflite", "wb") as f:
    f.write(tflite_model)

print(f"Saved nagrik_gas_model.tflite ({len(tflite_model)} bytes)")

