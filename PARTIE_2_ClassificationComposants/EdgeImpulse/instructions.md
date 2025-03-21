# Instructions for Edge Impulse

To train the model, please follow these steps :

1. Start a new Edge Impulse project. Go to Data acquisition. Click the button to Upload existing data. Click Choose Files and select all images for one class. Proceed to upload each respective folder and set Label for each class. I have divided into 4 classes: BACKGROUND, CAPACITOR, LED, RESISTOR.
2. Head to Impulse design. Change the Image data setting to 32 x 32 (width x height).
Select :
Processing block : Image
Learning block : Transfer Learning (Images)
Then click Save Impulse.
4. Go to Image, change Color Depth to Grayscale, then Save parameters. Then Generate features.
5. Navigate to Transfer learning, customize the parameters to your liking, then click Save & Train.
6. Navigate to Model testing and click Classify all to test the newly trained model.
7. Navigate to Deployment, select Arduino library, then press Build.
8. Download the zip file and Add this library through the Arduino IDE via: Sketch > Include Library > Add .ZIP Library...
9. Charge the ino file I provided to the Arduino card. From here, the system can start predicting.
10. Charge the ino file I provided to the Arduino card. From here, the system can start predicting. Go back to the READNE in the PARTIE_2_ClassificationComposants folder.

# Some notes for the project :
1. Leaving the image size at 32x32 and changing it to Grayscale is necessary because it will help reduce the size of the model. The memory of the Arduino card is limited, if it exceeds the limit, the model cannot be launched.
2. The model works pretty well, however it often confuses random background with led, so the reliability is not high. However, it still works to some extent.
3. I reused the image library you provided to train this model.
