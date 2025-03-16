This is the project that allows classification of different electronic components via the OV767X camera using the model trained by Edge Impulse. From there, it will pass this data to Node-Red and count the number of detected objects.

To run this project, please follow these steps:

1. Use the zip file provided in the PARTIE_2_ClassificationComposants/ArduinoCamera/ folder. Extract this file in the Arduino's library folder.
2. Charge file nano_ble33_sense_camera.ino to Arduino card. You need to turn off the serial monitor to perform the next steps.
3. Launch Node-Red at local IP address (http://localhost:1880/). Load flows.json file. Then deploy it.
4. Then open the Node-Red dashboard at http://localhost:1880/ui. You can see the results from here.
