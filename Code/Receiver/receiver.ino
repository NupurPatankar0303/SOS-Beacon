/*
 * SOS Beacon - Receiver
 * Emergency communication system using LoRa and LCD
 *
 * Functions:
 * - Receive SOS messages through LoRa
 * - Extract GPS coordinates from received messages
 * - Display latitude and longitude on a 16x2 I2C LCD
 * - Send acknowledgement messages to the transmitter
 * - Provide buzzer feedback when an SOS is received
 */
#include <SPI.h>

#include <LoRa.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// === LoRa Pins ===

#define LORA_SS 10

#define LORA_RST 9

#define LORA_DIO0 2

// === Other Pins ===

#define BUZZER_PIN 3

// === LCD Address and Size ===

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change address if needed

// === Signal and Acknowledgement Tracking ===

String lastSignalID = "";

unsigned long lastAckTime = 0;

const int ackDuration = 2000; // Buzzer ON duration in ms

void setup() {

  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();

  lcd.backlight();

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Receiver Ready");

  // LoRa init

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {

    Serial.println("LoRa init failed!");

    lcd.setCursor(0, 1);

    lcd.print("LoRa Failed");

    while (1);

  }

  Serial.println("LoRa init success");

  lcd.setCursor(0, 1);

  lcd.print("LoRa OK");

  delay(1000);

  lcd.clear();

}

void loop() {

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    String incoming = "";

    while (LoRa.available()) {

      incoming += (char)LoRa.read();

    }

    Serial.println("Received: " + incoming);

    if (incoming.startsWith("SOS|")) {

      String id = incoming.substring(4, 10);

      String gpsData = incoming.substring(11); // After "SOS|<id>|"

      displayLocation(gpsData);

      sendACK(id);

    }

  }

  // Optional: turn off buzzer after duration

  if (digitalRead(BUZZER_PIN) == HIGH && (millis() - lastAckTime >= ackDuration)) {

    digitalWrite(BUZZER_PIN, LOW);

  }

}

void displayLocation(String gpsData) {

  int commaIndex = gpsData.indexOf(',');

  String lat = gpsData.substring(0, commaIndex);

  String lng = gpsData.substring(commaIndex + 1);

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Lat:" + lat.substring(0, 8));

  lcd.setCursor(0, 1);

  lcd.print("Lng:" + lng.substring(0, 8));

  Serial.println("Lat: " + lat);

  Serial.println("Lng: " + lng);

}

void sendACK(String id) {

  String ackMsg = "ACK|" + id;

  // Broadcast ACK multiple times (for redundancy)

  for (int i = 0; i < 3; i++) {

    LoRa.beginPacket();

    LoRa.print(ackMsg);

    LoRa.endPacket();

    delay(100);

  }

  Serial.println("ACK sent for ID: " + id);

  // Buzzer feedback

  digitalWrite(BUZZER_PIN, HIGH);

  lastAckTime = millis();

}

