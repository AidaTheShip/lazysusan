#include <Arduino.h>
#include "pitches.h"

// Define some fixed variables re: the keyboard
const int numKeys = 3;
const int buttons[numKeys] = {5, 3, A6};
bool notePlaying[numKeys] = {0};
int notes[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_B3};

#define SENSOR1_TRIG 13
#define SENSOR1_EC 14
#define SENSOR2_TRIG 1
#define SENSOR2_EC 0
#define SENSOR3_TRIG 8
#define SENSOR3_EC 9
#define SENSOR4_TRIG 6
#define SENSOR4_EC 7

#define SPEAKER_PIN A1

// Volume control (percentage: 0 to 100)
int volume = 100;

// Precompute a sine wave table
const int numSamples = 100;  // Number of samples in one cycle
uint8_t sineWave[numSamples];

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < numKeys; i++) {
    pinMode(buttons[i], INPUT);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_EC, INPUT);
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_EC, INPUT);
  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_EC, INPUT);
  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_EC, INPUT);

  pinMode(SPEAKER_PIN, OUTPUT);

  // Generate sine wave values scaled to 8-bit PWM (0–255)
  for (int i = 0; i < numSamples; i++) {
    sineWave[i] = 128 + 127 * sin(2 * PI * i / numSamples);
  }
}

void loop() {
  int distances[] = {
    getDistance(SENSOR1_TRIG, SENSOR1_EC),
    getDistance(SENSOR2_TRIG, SENSOR2_EC),
    getDistance(SENSOR3_TRIG, SENSOR3_EC),
    getDistance(SENSOR4_TRIG, SENSOR4_EC)
  };

  Serial.print("Distances: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(distances[i]);
    Serial.print(" ");
  }
  Serial.println();

  int closestIndex = getClosestSensor(distances);

  if (closestIndex != -1) {
    playSineWave(notes[closestIndex], 500, volume);
  } else {
    analogWrite(SPEAKER_PIN, 0);
  }

  delay(200);
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  int distance = (duration * 0.0343) / 2;
  return distance > 0 ? distance : 9999;
}

int getClosestSensor(int distances[]) {
  int minDistance = 9999;
  int closestIndex = -1;
  int tieIndices[4];
  int tieCount = 0;

  for (int i = 0; i < 4; i++) {
    if (distances[i] < minDistance) {
      minDistance = distances[i];
      closestIndex = i;
      tieCount = 0;
      tieIndices[tieCount++] = i;
    } else if (distances[i] == minDistance) {
      tieIndices[tieCount++] = i;
    }
  }

  if (tieCount > 1) {
    closestIndex = tieIndices[random(0, tieCount)];
  }

  return closestIndex;
}

void playSineWave(int frequency, int duration, int amplitudePercentage) {
  const int maxAmplitude = 255;
  int amplitude = map(amplitudePercentage, 0, 100, 0, maxAmplitude);

  unsigned long period = 1000000L / frequency;
  unsigned long startTime = millis();

  int sampleIndex = 0;

  while (millis() - startTime < duration) {
    // Adjust amplitude based on volume
    int value = (sineWave[sampleIndex] * amplitude) / maxAmplitude;
    analogWrite(SPEAKER_PIN, value);

    delayMicroseconds(period / numSamples);

    sampleIndex++;
    if (sampleIndex >= numSamples) sampleIndex = 0;
  }

  analogWrite(SPEAKER_PIN, 0);  // Ensure no sound after playing
}
