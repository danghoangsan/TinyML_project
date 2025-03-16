# Objects classification
This is the project that allows classification of different electronic components via the OV767X camera using the model trained by Edge Impulse. From there, it will pass this data to Node-Red and count the number of detected objects.

# To run this project, please follow these steps:

1. Use the zip file provided in the PARTIE_2_ClassificationComposants/ArduinoCamera/ folder. Extract this file in the Arduino's library folder.
2. Charge file nano_ble33_sense_camera.ino to Arduino card. You need to turn off the serial monitor to perform the next steps.
3. Launch Node-Red at local IP address (http://localhost:1880). Load flows.json file. Then deploy it.
4. Then open the Node-Red dashboard at http://localhost:1880/ui. You can see the results from here.

# Or if you want to train the model from the start :

1. Follow the instructions in instructions.md in the PARTIE_2_ClassificationComposants/EdgeImpulse/ folder
2. Start over from step 2 to 4.

# Some notes of the project :
1. For the Arduino inference, I reused the code provided by Edge Impulse, however, I made some changes so that it only sends information on the serial port about the class with the highest prediction score.
2. I have attached some images for you to try in the TestImages folder.
3. Node-Red will receive information from the serial port about the predicted object, thereby increasing the counter of the corresponding object.
4. Each prediction will be made automatically every 2 seconds.
