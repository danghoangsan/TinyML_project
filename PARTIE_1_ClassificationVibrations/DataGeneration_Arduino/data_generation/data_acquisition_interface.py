import tkinter as tk
from tkinter import messagebox
import serial
import csv
import threading
import time


SERIAL_PORT = 'COM3'  
BAUD_RATE = 9600
FREQUENCY = 119  

class AccelerometerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Accelerometer Data Acquisition")
        
	# Variables to store user inputs
        self.points_var = tk.StringVar()
        self.time_var = tk.StringVar()
        self.filename_var = tk.StringVar()
        
	# Each samples is a result from accelerometter, sampling frequency is 119 Hz
        tk.Label(root, text="Number of Sample Points:").grid(row=0, column=0)
        self.points_entry = tk.Entry(root, textvariable=self.points_var)
        self.points_entry.grid(row=0, column=1)
        
	# Acquisition time is the total time of acquisition
        tk.Label(root, text="Acquisition Time (s):").grid(row=1, column=0)
        self.time_entry = tk.Entry(root, textvariable=self.time_var)
        self.time_entry.grid(row=1, column=1)
        
	# Put the output filename here
        tk.Label(root, text="Output CSV Filename:").grid(row=2, column=0)
        self.filename_entry = tk.Entry(root, textvariable=self.filename_var)
        self.filename_entry.grid(row=2, column=1)
        
	# Calculate the number of sample points from acquisition time or vers versa
        self.calc_button = tk.Button(root, text="Calculate", command=self.calculate)
        self.calc_button.grid(row=3, column=0, columnspan=2)
        
	# Start the acquisition
        self.start_button = tk.Button(root, text="Start Acquisition", command=self.toggle_acquisition)
        self.start_button.grid(row=4, column=0, columnspan=2)
        
        self.running = False
    
	# Function to calculate acquisition time from number of sample points or vers versa
    def calculate(self):
        try:
            if self.points_var.get():
                points = int(self.points_var.get())
                acquisition_time = points / FREQUENCY
                self.time_var.set(f"{acquisition_time:.2f}")
            elif self.time_var.get():
                acquisition_time = float(self.time_var.get())
                points = int(acquisition_time * FREQUENCY)
                self.points_var.set(str(points))
            else:
                messagebox.showerror("Input Error", "Please enter either number of points or acquisition time.")
        except ValueError:
            messagebox.showerror("Input Error", "Invalid input. Please enter a valid number.")
    
	# Function to start or stop the acquisition
    def toggle_acquisition(self):
        if self.running:
            self.running = False
            self.start_button.config(text="Start Acquisition")
        else:
            try:
                points = int(self.points_var.get())
                filename = self.filename_var.get().strip()
                if not filename:
                    messagebox.showerror("Input Error", "Please enter a valid filename.")
                    return
                
                self.running = True
                self.start_button.config(text="Stop Acquisition")
                threading.Thread(target=self.acquire_data, args=(points, filename), daemon=True).start()
            except ValueError:
                messagebox.showerror("Input Error", "Please enter a valid number of points.")
    
	# Function to acquire data
    def acquire_data(self, points, filename):
        try:
            ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
            time.sleep(2)  # Allow time for connection to establish
            
            data = []
            
            while len(data) < points and self.running:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    try:
                        x, y, z = map(float, line.split())
                        data.append([x,y,z])
                    except ValueError:
                        continue
            
            ser.close()
            
            with open(f"{filename}.csv", "w", newline='') as file:
                writer = csv.writer(file)
                writer.writerow(["X", "Y", "Z"])
                writer.writerows(data)
            
            messagebox.showinfo("Success", f"Data saved to {filename}.csv")
        except serial.SerialException:
            messagebox.showerror("Connection Error", "Failed to connect to the serial port.")
        
        self.running = False
        self.start_button.config(text="Start Acquisition")

# Main
if __name__ == "__main__":
    root = tk.Tk()
    app = AccelerometerApp(root)
    root.mainloop()