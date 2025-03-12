// I wrote the code base on the principle of the code during the class for gesture detection
#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "model.h"

const float vibrationThreshold = 0.02; // Threshold for vibration detection
const int numSamples = 258;
const int fluctuationSamples = 2; // Number of consecutive fluctuations to confirm vibration

int samplesRead = numSamples;
int fluctuationCount = 0; // To track fluctuations for reliable detection

// TensorFlow Lite (Micro) global variables
tflite::MicroErrorReporter tflErrorReporter;
tflite::AllOpsResolver tflOpsResolver;
const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// Array to map vibration types
const char* VIBRATIONS[] = {"handshake", "wind"};

#define NUM_VIBRATIONS (sizeof(VIBRATIONS) / sizeof(VIBRATIONS[0]))

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) 
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) 
  {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);
  tflInterpreter->AllocateTensors();
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() 
{
  float aX, aY, aZ;

  while (samplesRead == numSamples) 
  {
    if (IMU.accelerationAvailable()) 
    {
      IMU.readAcceleration(aX, aY, aZ);
      // This is my algorithm to eliminate noise and prevent the arduino from running the model
      // if it does not have consistent fluctuations
      // If the arduino in a static state, there will not be any prediction
      float magnitude = sqrt(aX * aX + aY * aY + aZ * aZ);
      static float lastMagnitude = magnitude;
      float change = fabs(magnitude - lastMagnitude);
      lastMagnitude = magnitude;

      if (change >= vibrationThreshold) 
      {
        fluctuationCount++;
      } else 
      {
        fluctuationCount = 0; // Reset if no consistent fluctuations
      }

      if (fluctuationCount >= fluctuationSamples) 
      {
        samplesRead = 0;
        fluctuationCount = 0;
        break;
      }
    }
  }

  while (samplesRead < numSamples) 
  {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) 
    {
      IMU.readAcceleration(aX, aY, aZ);
      // Normalize the values
      tflInputTensor->data.f[samplesRead * 3 + 0] = (aX + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 3 + 1] = (aY + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 3 + 2] = (aZ + 4.0) / 8.0;
      samplesRead++;

      if (samplesRead == numSamples) 
      {
        if (tflInterpreter->Invoke() != kTfLiteOk) 
        {
          Serial.println("Invoke failed!");
          while (1);
        }

        for (int i = 0; i < NUM_VIBRATIONS; i++) 
        {
          Serial.print(VIBRATIONS[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 3);
        }
        Serial.println();
      }
    }
  }
}