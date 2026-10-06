/*
 * SOS Beacon - Transmitter
 * Emergency communication system using LoRa and GPS
 *
 * Functions:
 * - Motion monitoring using an accelerometer
 * - GPS coordinate acquisition
 * - SOS transmission through LoRa
 * - SOS acknowledgement handling
 * - SOS forwarding from other transmitter nodes
 */
#include <SPI.h>

#include <LoRa.h>

#include <SoftwareSerial.h>

#include <TinyGPS++.h>

// === Pin Definitions ===

#define LORA_SS 10

#define LORA_RST 9

#define LORA_DIO0 2

#define BUZZER_PIN 3

#define LED_ACK 4

#define BUTTON_PIN 5

#define ACCEL_PIN A0

#define GPS_RX 6

#define GPS_TX 7

// === GPS Setup ===

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

TinyGPSPlus gps;

// === System State Variables ===

unsigned long lastMotionTime = 0;

unsigned long sosSentTime = 0;

bool buzzerOn = false;

bool accelEnabled = true;

bool sosSent = false;

String lastSignalID = "";

void setup() {

  Serial.begin(9600);

  gpsSerial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED_ACK, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(BUZZER_PIN, LOW);

  digitalWrite(LED_ACK, LOW);

  // LoRa setup

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {

    Serial.println("LoRa init failed!");

    while (1);

  }

  Serial.println("Transmitter ready");

  lastMotionTime = millis();

}

void loop() {

  checkAccelerometer();

  checkButton();

  checkGPS();

  checkLoRaReceive();

  if (accelEnabled && !buzzerOn && !sosSent) {

    unsigned long noMotionDuration = millis() - lastMotionTime;

    if (noMotionDuration >= 20UL * 60UL * 1000UL) { // 20 mins inactivity

      sendSOS();

    }

  }

  if (buzzerOn && (millis() - sosSentTime >= 15UL * 60UL * 1000UL)) {

    digitalWrite(BUZZER_PIN, LOW);

    buzzerOn = false;

    Serial.println("Buzzer turned OFF");

  }

}

// === Check accelerometer movement ===

void checkAccelerometer() {

  if (!accelEnabled) return;

  int accelValue = analogRead(ACCEL_PIN);

  if (accelValue > 10) {  // Adjust threshold

    lastMotionTime = millis();

  }

}

// === Toggle accelerometer with button ===

void checkButton() {

  static bool lastState = HIGH;

  bool currentState = digitalRead(BUTTON_PIN);

  if (lastState == HIGH && currentState == LOW) {

    accelEnabled = !accelEnabled;

    Serial.println(accelEnabled ? "Accelerometer ON" : "Accelerometer OFF");

    delay(300);

  }

  lastState = currentState;

}

// === Read GPS data ===

void checkGPS() {

  while (gpsSerial.available()) {

    gps.encode(gpsSerial.read());

  }

}

// === Send SOS via LoRa ===

void sendSOS() {

  if (!gps.location.isValid()) {

    Serial.println("GPS not ready.");

    return;

  }

  String signalID = String(random(100000, 999999));  // Unique ID

  lastSignalID = signalID;

  sosSent = true;

  String message = "SOS|" + signalID + "|";

  message += String(gps.location.lat(), 6) + ",";

  message += String(gps.location.lng(), 6);

  Serial.println("Sending: " + message);

  LoRa.beginPacket();

  LoRa.print(message);

  LoRa.endPacket();

  digitalWrite(BUZZER_PIN, HIGH);

  buzzerOn = true;

  sosSentTime = millis();

  Serial.println("Buzzer ON for 15 mins");

}

// === Receive and process LoRa messages ===

void checkLoRaReceive() {

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    String incoming = "";

    while (LoRa.available()) {

      incoming += (char)LoRa.read();

    }

    Serial.println("Received: " + incoming);

    // === ACK received for this transmitter ===

    if (incoming.startsWith("ACK|")) {

      String ackID = incoming.substring(4, 10);

      if (ackID == lastSignalID) {

        digitalWrite(LED_ACK, HIGH);

        Serial.println("ACK matched — LED ON");

      }

    }

    // === Relay SOS from other transmitters ===

    else if (incoming.startsWith("SOS|")) {

      String incomingID = incoming.substring(4, 10);

      if (incomingID != lastSignalID) {

        forwardToReceiver(incoming);

      }

    }

  }

}

// === Forward another transmitter's SOS ===

void forwardToReceiver(String msg) {

  Serial.println("Forwarding SOS: " + msg);

  LoRa.beginPacket();

  LoRa.print(msg);

  LoRa.endPacket();

}

