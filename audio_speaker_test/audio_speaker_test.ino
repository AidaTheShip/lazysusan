// #include "pitches.h"

// int notes[] = {
//   NOTE_C3, NOTE_E3, NOTE_G3, NOTE_B3
// };

// #define speakerPin A6 // Pin for speaker

// void setup() {
//   pinMode(speakerPin, OUTPUT); // Set the speaker pin as output
// }

// void loop() {
//   // Loop through the notes array
//   for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
//     tone(speakerPin, notes[i], 500); // Play the note for 500 ms
//     delay(600); // Wait 600 ms (to include a short pause between notes)
//   }

//   delay(2000); // Wait 2 seconds before replaying the sequence
// }
// #define SPEAKER_PIN A1 // PWM-capable pin connected to PAM8302A A+

// void setup() {
// pinMode(SPEAKER_PIN, OUTPUT); // Set speaker pin as output
// }

// void loop() {
// playToneWithAmplitude(200, 1000, 20); // Play A4 for 500ms at 50% amplitude
// delay(1000);                          // Wait before playing again
// }

// // Function to generate a tone with adjustable amplitude
// void playToneWithAmplitude(int frequency, int duration, int amplitude) {
// const int pwmResolution = 100; // 8-bit resolution (0-255 for amplitude)
// long period = 1000000L / frequency; // Period of the wave in microseconds
// long halfPeriod = period / 2;

// unsigned long startMillis = millis();
// while (millis() - startMillis < duration) {
//   analogWrite(SPEAKER_PIN, amplitude); // Set high level to the specified amplitude
//   delayMicroseconds(halfPeriod);

//   analogWrite(SPEAKER_PIN, 0); // Set low level
//   delayMicroseconds(halfPeriod);
// }
// }

// #include <Arduino.h>
// #include "pitches.h"

// // Ultrasonic sensor pins
// #define TRIG_PIN 13
// #define ECHO_PIN 12

// // Speaker pin (PWM-capable pin)
// #define SPEAKER_PIN A1

// // Notes to play based on distance
// int notes[] = {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4};
// const int numNotes = sizeof(notes) / sizeof(notes[0]);

// // State variables
// int currentNoteIndex = -1; // Tracks the current note being played

// void setup() {
//   Serial.begin(9600);

//   // Initialize ultrasonic sensor pins
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);

//   // Initialize speaker pin
//   pinMode(SPEAKER_PIN, OUTPUT);
// }

// void loop() {
//   int distance = getDistance();

//   // Map distance to note index
//   int noteIndex = mapDistanceToNoteIndex(distance);

//   // If the note has changed, update the analogWrite frequency
//   if (noteIndex != currentNoteIndex) {
//     currentNoteIndex = noteIndex;
//     if (currentNoteIndex >= 0 && currentNoteIndex < numNotes) {
//       playTonePWM(notes[currentNoteIndex]);
//     } else {
//       stopSound();
//     }
//   }

//   delay(50); // Small delay for stability
// }

// // Function to get distance from ultrasonic sensor
// int getDistance() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(5);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
//   int distance = (duration * 0.0343) / 2; // Convert to centimeters
//   return (distance > 0) ? distance : 9999; // Return a large value if no valid reading
// }

// // Function to map distance to a note index
// int mapDistanceToNoteIndex(int distance) {
//   if (distance > 50) {
//     return -1; // No note if distance is too far
//   }
//   return map(distance, 0, 50, 0, numNotes - 1); // Map distance to note index
// }

// // Function to play a tone using PWM
// void playTonePWM(int frequency) {
//   int period = 1000000 / frequency; // Period in microseconds
//   int halfPeriod = period / 2;     // Half period for HIGH/LOW cycle

//   // Generate tone on the speaker pin
//   for (unsigned long start = millis(); millis() - start < 500;) { // Play for 500ms
//     analogWrite(SPEAKER_PIN, 128); // Set duty cycle (volume)
//     delayMicroseconds(halfPeriod);
//     analogWrite(SPEAKER_PIN, 0); // Silence between cycles
//     delayMicroseconds(halfPeriod);
//   }
// }

// // Function to stop the sound
// void stopSound() {
//   analogWrite(SPEAKER_PIN, 0); // Stop any PWM output
// }

const int speakerPin = A1; // Use A1 for PWM output
const int frequency = 440; // Frequency of the tone (Hz)
const int amplitude = 128; // Duty cycle for amplitude (0-255)

void setup() {
  pinMode(speakerPin, OUTPUT); // Set A1 as an output pin
}

void loop() {
  playTone(frequency, 1000); // Play a tone at 440 Hz for 1 second
  delay(1000);               // Wait for 1 second
}

void playTone(int freq, int duration) {
  int period = 1000000 / freq;          // Period of the wave in microseconds
  int pulseWidth = (amplitude * period) / 255; // Pulse width based on amplitude

  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // Generate a PWM signal
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(period - pulseWidth);
  }
}

