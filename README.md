# Integrated Arduino Mobile Driving System (IAMDS)
IAMDS is a proof-of-concept for a low-cost, open-source Advanced Driver-Assistance System (ADAS) built for a model vehicle. This project demonstrates how affordable microcontrollers and sensors can be used to implement critical safety features like Autonomous Emergency Braking (AEB) and Adaptive Cruise Control (ACC).
The system operates on two main components: the vehicle itself, which acts on commands and sensor data, and a custom-built remote control that provides the human-machine interface. Communication is handled by a real-time, bidirectional 2.4GHz wireless link, allowing the remote to send control inputs and the vehicle to send back crucial telemetry data.

# Group 4 - ECE 372A Microcontroller Organization
- Edison
- Dzaky
- Rainer
- Bintang
- Michael

## **Technology Stack**
**Hardware:**
- Microcontroller: Arduino Mega 2560 (x2)
- Wireless Transceiver: nRF24L01 (x2)
- Proximity Sensor: HC-SR04 Ultrasonic Sensor
- Display: 0.96" SSD1306 I2C OLED Screen
- Actuators: DC Motors, Relays, MOSFET (for PWM speed control)
**Software & Libraries:**
- Framework: Arduino
- IDE: PlatformIO
- Libraries:
  - RF24 for nRF24L01 communication.
  - Adafruit_GFX & Adafruit_SSD1306 for the OLED display.
  - SPI.h and Wire.h for device communication.

## **Features:**
- **Autonomous Emergency Braking (AEB):** The vehicle uses its ultrasonic sensor to constantly monitor the distance to objects ahead. If an object is detected within a critical threshold (e.g., < 20cm), the system overrides user commands to engage the brakes and prevent a collision.
- **Adaptive Cruise Control (ACC):** Leverages the ultrasonic sensor to maintain a safe following distance from a leading vehicle or object.
- **Bidirectional Wireless Communication:** Employs nRF24L01 transceivers for a robust, real-time command and telemetry link. Control signals (throttle, steering) are sent to the car, while sensor data (distance) is efficiently returned to the remote using ACK payloads, eliminating the need for a separate listening window.
- **Real-Time Telemetry:** The remote control features an SSD1306 OLED screen that displays live distance and speed data received from the vehicle, giving the operator full situational awareness.
- **Full Manual Control:** The system allows for complete manual operation, including proportional throttle, steering, braking, and a temporary boost function.

## **System Architecture**
The project is split into two distinct Arduino-based systems:
- **The Car:** This is the primary unit responsible for all physical actions. Its main loop listens for commands from the remote. It continuously runs the collision avoidance algorithm, and if a threat is detected, it takes precedence over user commands. It reads from the HC-SR04 sensor and attaches this data to acknowledgment (ACK) packets sent back to the remote.
- **The Remote:** This unit serves as the command center. It reads user inputs from buttons and an analog potentiometer. It sends this data in a structured message to the car at a consistent interval. Simultaneously, it listens for ACK payloads from the car to receive and display real-time telemetry on its OLED screen.

## **Hardware Setup & Pinout**
For a complete visual guide, please refer to the circuit diagrams in PDF.

**Key Connections (Car):**
- nRF24L01: CE to Pin 48, CSN to Pin 49, plus standard SPI (MOSI, MISO, SCK).
- HC-SR04: Trig to Pin 21, Echo to Pin 20.
- Motor Control: MOSFET Gate to Pin 44 (PWM).
- Actuators: Relays for Brake, Direction, Boost on Pins 24, 25, 26.
- Lights/Beeper: Connected to Pins 22, 23, 27, 28, 29.
  
**Key Connections (Remote):**
- nRF24L01: CE to Pin 48, CSN to Pin 49, plus standard SPI.
- OLED Display: Standard I2C (SDA, SCL).
- Controls: Buttons to Pins 22-26, Throttle to Analog Pin A0.

## **Main Project Directories**
`car/` → Contains the PlatformIO project and source code for the vehicle's microcontroller.
  - `src/main.cpp` → Main firmware that handles motor/actuator control, processes ultrasonic sensor data, and executes the collision avoidance logic.

`remote/` → Contains the PlatformIO project and source code for the remote control's microcontroller.
  - `src/main.cpp` → Main firmware that reads user inputs, sends commands wirelessly, and displays received telemetry on the OLED screen.

## **External Files**
`Alat Peraga_ MOBIL PENGENDALIAN JARAK JAUH DENGAN REM OTOMATIS JARAK DEKAT` → Summary of functional features of the car and system. [For HAKI Publication Purposes Only]
