#include <TimeLib.h>         // Library to keep track of time
#include <Wire.h>            // I2C library
#include <DS3231.h>          // Library for the DS3231 RTC

// Pin definitions
const int moistureSensorPin = A0;     // Analog pin for soil moisture sensor
const int relayPin = 3;               // Digital pin for relay controlling the valve

// Thresholds and timing
const int moistureThreshold = 500;    // Adjust according to your sensor's dry/moist values
const int wateringStartHour1 = 6;     // Start watering at 6:00 AM
const int wateringEndHour1 = 6;       // End watering before 6:30 AM
const int wateringStartHour2 = 18;    // Start watering at 6:00 PM
const int wateringEndHour2 = 18;      // End watering before 6:30 PM

// Timing variables
DS3231 rtc;
bool isWatering = false;

void setup() {
  pinMode(moistureSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Keep valve closed at startup
  Serial.begin(9600);
  rtc.begin();
}

void loop() {
  // Get current time
  Time t = rtc.getTime();
  int hour = t.hour;
  int minute = t.min;

  // Read moisture level
  int soilMoisture = analogRead(moistureSensorPin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);

  // Check if current time is within watering times
  bool isWateringTime = (hour == wateringStartHour1 && minute < 30) || 
                        (hour == wateringStartHour2 && minute < 30);

  // Control watering based on moisture level and watering time
  if (isWateringTime && soilMoisture < moistureThreshold) {
    startWatering();
  } else {
    stopWatering();
  }

  delay(10000);  // Check every 10 seconds
}

void startWatering() {
  if (!isWatering) {
    digitalWrite(relayPin, LOW); // Open the valve
    Serial.println("Watering started.");
    isWatering = true;
  }
}

void stopWatering() {
  if (isWatering) {
    digitalWrite(relayPin, HIGH); // Close the valve
    Serial.println("Watering stopped.");
    isWatering = false;
  }
}
