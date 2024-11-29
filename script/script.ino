#include <Arduino.h>
#include "pitches.h"

// Button definitions
const int numKeys = 3;
const int buttons[numKeys] = {5, 3, A6}; // Pins for buttons

// Speaker pin
#define SPEAKER_PIN A1

// Notes to play
int notes1[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_B3}; // Notes set 1
int notes2[] = {NOTE_C3, NOTE_D3, NOTE_F3, NOTE_A3}; // Notes set 2

// Volume control
int volume = 100;

// Sine wave table
const int numSamples = 100; // Number of samples in a sine wave cycle
uint8_t sineWave[numSamples];

// State variables
bool systemOn = true;   // Tracks whether the system is on or off
int activeNotesSet = -1; // Tracks which set of notes to play (-1 = off)

void setup() {
  Serial.begin(9600);

  // Initialize buttons as inputs
  for (int i = 0; i < numKeys; i++) {
    pinMode(buttons[i], INPUT);
  }

  // Initialize speaker pin
  pinMode(SPEAKER_PIN, OUTPUT);

  // Generate sine wave table
  for (int i = 0; i < numSamples; i++) {
    sineWave[i] = 128 + 127 * sin(2 * PI * i / numSamples);
  }
}

void loop() {
  // Check button states
  if (digitalRead(buttons[0]) == HIGH) { // Button 5 pressed
    activeNotesSet = 1;
    systemOn = true;
  } else if (digitalRead(buttons[1]) == HIGH) { // Button 3 pressed
    activeNotesSet = 2;
    systemOn = true;
  } else if (digitalRead(buttons[2]) == HIGH) { // Button A6 pressed
    activeNotesSet = -1;
    systemOn = false;
    stopSound();
  }

  // Play notes based on active set
  if (systemOn) {
    if (activeNotesSet == 1) {
      playNotes(notes1, sizeof(notes1) / sizeof(notes1[0]));
    } else if (activeNotesSet == 2) {
      playNotes(notes2, sizeof(notes2) / sizeof(notes2[0]));
    }
  }
}

// Function to play notes continuously
void playNotes(int notes[], int numNotes) {
  static int currentNoteIndex = 0; // Tracks the current note
  static unsigned long lastSwitchTime = 0; // Tracks the last time a note changed
  const unsigned long noteDuration = 500; // Duration for each note in milliseconds

  if (millis() - lastSwitchTime >= noteDuration) {
    currentNoteIndex = (currentNoteIndex + 1) % numNotes; // Cycle through notes
    playSineWave(notes[currentNoteIndex], volume); // Play the next note
    lastSwitchTime = millis();
  }
}

// Function to play a sine wave at a specific frequency
void playSineWave(int frequency, int amplitudePercentage) {
  const int maxAmplitude = 255;
  int amplitude = map(amplitudePercentage, 0, 100, 0, maxAmplitude);

  unsigned long period = 1000000L / frequency; // Period in microseconds
  unsigned long sampleDelay = period / numSamples;
  int sampleIndex = 0;

  // Play sine wave
  for (int i = 0; i < numSamples; i++) {
    analogWrite(SPEAKER_PIN, (sineWave[sampleIndex] * amplitude) / maxAmplitude);
    delayMicroseconds(sampleDelay);
    sampleIndex = (sampleIndex + 1) % numSamples;
  }
}

// Function to stop the sound
void stopSound() {
  analogWrite(SPEAKER_PIN, 0); // Stop any PWM output
}
