#include <Arduino.h>
#include "pitches.h"

// Ultrasonic sensor pins
#define TRIG_PIN 13
#define ECHO_PIN 12

// Speaker pin
#define SPEAKER_PIN A1

// Notes to play based on distance
int notes[] = {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4};
const int numNotes = sizeof(notes) / sizeof(notes[0]);

// State variables
int currentNoteIndex = -1; // Tracks the current note being played

void setup() {
  Serial.begin(9600);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize speaker pin
  pinMode(SPEAKER_PIN, OUTPUT);
}

void loop() {
  int distance = getDistance();

  // Map distance to note index
  int noteIndex = mapDistanceToNoteIndex(distance);

  // If the note has changed, update the PWM frequency
  if (noteIndex != currentNoteIndex) {
    currentNoteIndex = noteIndex;
    if (currentNoteIndex >= 0 && currentNoteIndex < numNotes) {
      playPWM(notes[currentNoteIndex]);
    } else {
      stopSound();
    }
  }

  delay(50); // Small delay for stability
}

// Function to get distance from ultrasonic sensor
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  int distance = (duration * 0.0343) / 2; // Convert to centimeters
  return (distance > 0) ? distance : 9999; // Return a large value if no valid reading
}

// Function to map distance to a note index
int mapDistanceToNoteIndex(int distance) {
  if (distance > 50) {
    return -1; // No note if distance is too far
  }
  return map(distance, 0, 50, 0, numNotes - 1); // Map distance to note index
}

// Function to play a note using PWM
void playPWM(int frequency) {
  int period = 1000000 / frequency; // Period in microseconds
  int halfPeriod = period / 2;     // Half period for HIGH/LOW cycle

  // Start PWM
  unsigned long startMillis = millis();
  while (millis() - startMillis < 1000) { // Play the note for 1 second
    digitalWrite(SPEAKER_PIN, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(SPEAKER_PIN, LOW);
    delayMicroseconds(halfPeriod);
  }
}

// Function to stop the sound
void stopSound() {
  digitalWrite(SPEAKER_PIN, LOW); // Stop any output
}
