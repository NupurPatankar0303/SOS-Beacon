# 🚨 SOS Beacon – Emergency Communication System

An Arduino and LoRa-based emergency communication system designed to
detect emergency situations and transmit GPS location information over
long distances without relying on cellular networks.

---

## 📖 Overview

The SOS Beacon is a wireless emergency alert system developed using
Arduino, LoRa, GPS and an accelerometer.

The system consists of two units:

- **Transmitter Unit** – carried by the user
- **Receiver Unit** – used by the rescuer or monitoring team

The transmitter monitors motion using an accelerometer. When an emergency
condition is detected, the system acquires the user's GPS coordinates
and transmits an SOS message through LoRa.

The receiver receives the SOS signal, extracts the GPS coordinates,
displays them on a 16×2 I2C LCD, activates a buzzer and sends an
acknowledgement back to the transmitter.

The system is designed for situations where cellular connectivity may
be unavailable, such as remote areas and disaster-prone environments.

---
---

## 👩‍💻 My Role

**Team Leader | Coder | Hardware Integration**

---

## 🎯 Objectives

- Develop a long-range emergency communication system.
- Detect emergency situations using motion monitoring.
- Obtain real-time GPS location information.
- Transmit emergency information using LoRa.
- Display the user's location at the receiver.
- Provide acknowledgement of received SOS signals.
- Demonstrate embedded system integration for emergency communication.

---

## ✨ Features

- 🚨 Automatic SOS detection
- 📍 GPS-based location tracking
- 📡 Long-range LoRa communication
- 📈 Accelerometer-based motion monitoring
- 🔊 Buzzer-based emergency alerts
- 💡 LED acknowledgement indication
- 📺 16×2 I2C LCD location display
- 🔄 SOS acknowledgement mechanism
- 🆔 Unique SOS signal identification
- 📡 SOS forwarding between transmitter nodes
---

## 🏗️ System Architecture

### 📐 Block Diagrams

- [Transmitter Block Diagram](sos%20becan%20transmitter.pdf)
- [Receiver Block Diagram](sos%20beacon.pdf)

### Transmitter Unit

The transmitter is built around an **Arduino Uno**.

It interfaces with:

- Accelerometer
- GPS module
- LoRa module
- Push button
- Buzzer
- LED

The transmitter monitors motion and generates an SOS event when the
configured emergency condition is reached. It then obtains GPS
coordinates and transmits the emergency information through LoRa.

### Receiver Unit

The receiver is built around an **Arduino Nano**.

It interfaces with:

- LoRa module
- 16×2 I2C LCD
- Buzzer

The receiver processes incoming SOS packets, displays the transmitted
GPS coordinates and sends an acknowledgement back through LoRa.

---

---

## 🛠️ Hardware Components

- Arduino Uno – Transmitter
- Arduino Nano – Receiver
- NEO-6M GPS Module
- Accelerometer
- SX1278 LoRa Module
- 16×2 I2C LCD
- Push Button
- Buzzer
- LED
- Power Supply
---

## 💻 Software & Libraries

- Arduino IDE
- Embedded C/C++
- SPI
- SoftwareSerial
- TinyGPS++
- LoRa
- LiquidCrystal_I2C
- Git & GitHub

---

## 🔄 Working Principle

### Transmitter

1. The accelerometer monitors motion.
2. The Arduino tracks the duration of inactivity.
3. When the configured emergency condition is reached, an SOS event is
   generated.
4. The GPS module provides the current latitude and longitude.
5. A unique signal ID is generated for the SOS message.
6. The SOS message containing the signal ID and GPS coordinates is
   transmitted through LoRa.
7. The transmitter activates the buzzer.
8. The transmitter waits for an acknowledgement from the receiver.

### Receiver

1. The receiver continuously listens for LoRa packets.
2. When an SOS packet is received, the signal ID and GPS coordinates
   are extracted.
3. The latitude and longitude are displayed on the 16×2 I2C LCD.
4. The receiver activates the buzzer.
5. An acknowledgement containing the SOS signal ID is transmitted back
   through LoRa.

### Acknowledgement

The transmitter compares the received acknowledgement ID with the ID
of the SOS message it transmitted.

When the IDs match, the acknowledgement LED is activated.

---
## 📡 Communication Format

### SOS Message

```text
SOS|<signal_ID>|<latitude>,<longitude>
```

### Acknowledgement

```text
ACK|<signal_ID>
```

## 🔁 SOS Forwarding

The transmitter code also supports forwarding SOS packets received from
another transmitter.

If an incoming SOS packet contains a different signal ID, the packet can
be forwarded through LoRa.

---

## 📊 Results & Outcome

The SOS Beacon system was implemented and tested as an emergency
communication prototype.

Testing demonstrated the operation of the complete communication
chain:

- Motion monitoring at the transmitter
- GPS coordinate acquisition
- SOS transmission through LoRa
- Reception and processing of emergency packets
- GPS location display on the receiver LCD
- Buzzer-based emergency alert
- Acknowledgement transmission back to the transmitter

The prototype was tested under different environmental conditions,
including urban and open-field environments, to evaluate its
communication and emergency alert functionality.

The project demonstrates the feasibility of using Arduino, GPS and
LoRa-based communication for emergency location reporting in situations
where conventional cellular communication may be unavailable.

---

## 📁 Repository Structure

```text
SOS-Beacon/
│
├── Code/
│   ├── Transmitter/
│   │   └── transmitter.ino
│   │
│   └── Receiver/
│       └── receiver.ino
│
├── sos beacon.pdf
├── sos becan transmitter.pdf
└── README.md
```
