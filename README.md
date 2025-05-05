# ESP32 Orientation Data Visualizer

This project demonstrates how to wirelessly transmit orientation data from a BNO055 IMU sensor using ESP-NOW and display it in real-time using Pygame.

## 🧭 Overview

- **Transmitter**: An ESP32 with a BNO055 IMU sensor collects orientation data and transmits it using ESP-NOW.
- **Receiver**: A second ESP32 receives the data and sends it over serial to a computer.
- **Visualizer**: A Python script reads the serial data and visualizes the orientation using a simple Pygame interface.

## 🛠️ Hardware Requirements

- 2x ESP32 boards
- 1x Adafruit BNO055 IMU sensor
- Jumper wires, breadboard
- (Optional) USB power bank for portable operation

## 🔌 Wiring (Transmitter ESP32 + BNO055)

| BNO055 Pin | ESP32 Pin |
|------------|-----------|
| VIN        | 3.3V      |
| GND        | GND       |
| SDA        | GPIO 21   |
| SCL        | GPIO 22   |

## 🚀 Getting Started

### 1. Flash the ESP32 Transmitter
- Upload `Transmitter.ino` to the ESP32 connected to the BNO055.
- Make sure the ESP-NOW peer MAC address matches the receiver's.

### 2. Flash the ESP32 Receiver
- Upload `Receiver.ino` to the second ESP32.
- Connect this ESP32 to your computer via USB.

### 3. Run the Python Visualizer
Install the required Python packages:

```bash
pip install pyserial pygame
