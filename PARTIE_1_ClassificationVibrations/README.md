# To implement the project, please follow these steps:
1. Upload the ino file in the DataGeneration_Arduino folder to the Arduino card so that the Arduino can measure with the accelerometer.
2. Launch the Python program data_acquisition_interface to collect data. Enter the Number of sample points or Acquisition time. Click Calculate, the program will automatically convert between these two variables. Enter the name of the csv output file. Then click Start Acquisition to start the process. Please wait until a notification window appears indicating that the acquisition process is complete.
3. The csv output file will appear in the same folder, copy it to the Training/dataset path.
4. Open the Jupyter notebook file and train the machine learning model following the code sequence. In fact, I reused the code you provided during class, just modified it a bit to suit your needs.
5. After completing step 4, model.h will appear in the same folder of Jupyter Notebook file. Copy it to Inference_Arduino/inference_vibrations path and upload the ino file to Arduino card.
6. Enable serial monitor in Arduino IDE and observe the prediction results.

# Some notes
My model consists of a classification of two classes:
- Handshake: hold the Arduino card in your hand and shake it with a large amplitude.
- Wind: Vibration mode in the Android app that I submitted in the discord group.

Prediction will not occur when the Arduino card is in a static state.

My model is not very good, the accuracy in practice is not high. I have trained more than 10 models with different vibration modes and parameters, but none of them work well. 
The system is generally not reliable. However, this model still allows classification to some extent.
