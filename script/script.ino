#include <Arduino.h>
#include "pitches.h"

// Define some fixed variables re: the keyboard
const int numKeys = 3; // This is a 13-key keyboard
const int buttons[numKeys] = {5, 3, A6}; // The keys will be connected to these pins on the Arduino

// Initialize the notePlaying array to be all zeros. This array describes the status of all the notes - whether it is currently playing or not ('YES playing' or 'NOT playing').
bool notePlaying[numKeys] = {0};  // bool has two states: [0 and 1] = [FALSE and TRUE] = [LOW and HIGH]. 0 = this note is not playing (the key is not pressed); 1 = this note is already playing (the key is pressed) 

// Notes to play based on sensors
int notes[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_B3};

// Ultrasonic sensor pins
#define SENSOR1_TRIG 13
#define SENSOR1_EC 14
#define SENSOR2_TRIG 1
#define SENSOR2_EC 0
#define SENSOR3_TRIG 8
#define SENSOR3_EC 9
#define SENSOR4_TRIG 6
#define SENSOR4_EC 7

// Speaker connected to PAM8302A
#define SPEAKER_PIN A1 // PWM-capable pin connected to PAM8302A

// Volume control (percentage: 0 to 100)
int volume = 100;

void setup() {
  Serial.begin(9600);

  // define pinMode for all the buttons as INPUT
  for (int i = 0; i < numKeys; i++) {
    pinMode(buttons[i], INPUT);
  }

  // set up the built-in LED as an OUTPUT (If we don't do this, the Arduino does not know "how to talk to" the built-inLED and we wouldn't be able to use it)
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_EC, INPUT);
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_EC, INPUT);
  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_EC, INPUT);
  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_EC, INPUT);

  // Initialize speaker pin
  pinMode(SPEAKER_PIN, OUTPUT);
}

void loop() {
  // Get distances from all sensors
  int distances[] = {
    getDistance(SENSOR1_TRIG, SENSOR1_EC),
    getDistance(SENSOR2_TRIG, SENSOR2_EC),
    getDistance(SENSOR3_TRIG, SENSOR3_EC),
    getDistance(SENSOR4_TRIG, SENSOR4_EC)
  };

  // Log distances for debugging
  Serial.print("Distances: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(distances[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Determine the closest sensor
  int closestIndex = getClosestSensor(distances);

  // Play the corresponding note if a valid sensor is selected
  if (closestIndex != -1) {
    playBeepingTone(notes[closestIndex], 500, volume); // Play the note with beeping effect
  } else {
    analogWrite(SPEAKER_PIN, 0); // Ensure no sound if no valid sensor
  }

  delay(200); // Short delay before the next cycle
}

int getDistance(int trigPin, int echoPin) {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout of 30 ms

  // Calculate distance in cm (speed of sound ~0.0343 cm/us)
  int distance = (duration * 0.0343) / 2;
  return distance > 0 ? distance : 9999; // Return a large number if no reading
}

int getClosestSensor(int distances[]) {
  int minDistance = 9999; // Start with a high value
  int closestIndex = -1;
  int tieIndices[4]; // To handle ties
  int tieCount = 0;

  // Find the minimum distance
  for (int i = 0; i < 4; i++) {
    if (distances[i] < minDistance) {
      minDistance = distances[i];
      closestIndex = i;
      tieCount = 0; // Reset tie count
      tieIndices[tieCount++] = i; // Add this index as the first tie
    } else if (distances[i] == minDistance) {
      tieIndices[tieCount++] = i; // Add to tie list
    }
  }

  // If there are ties, pick a random one
  if (tieCount > 1) {
    closestIndex = tieIndices[random(0, tieCount)];
  }

  return closestIndex; // Return the closest sensor index
}

// Function to generate a beeping tone with adjustable volume
void playBeepingTone(int frequency, int duration, int amplitudePercentage) {
  const int maxAmplitude = 255; // Max PWM value (8-bit resolution)
  int amplitude = map(amplitudePercentage, 0, 100, 0, maxAmplitude); // Map percentage to 0-255
  long period = 1000000L / frequency; // Period of the wave in microseconds
  long halfPeriod = period / 2;

  unsigned long startMillis = millis();
  while (millis() - startMillis < duration) {
    analogWrite(SPEAKER_PIN, amplitude); // High with adjusted amplitude
    delayMicroseconds(halfPeriod);

    analogWrite(SPEAKER_PIN, 0);        // Low
    delayMicroseconds(halfPeriod);
  }

  // Small pause to create the beeping effect
  delay(50); // Pause between beeps
 
}
