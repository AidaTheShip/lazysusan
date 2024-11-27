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
#define SPEAKER_PIN A0 // PWM-capable pin connected to PAM8302A A+

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT); // Set speaker pin as output
}

void loop() {
  playToneWithAmplitude(200, 1000, 20); // Play A4 for 500ms at 50% amplitude
  delay(1000);                          // Wait before playing again
}

// Function to generate a tone with adjustable amplitude
void playToneWithAmplitude(int frequency, int duration, int amplitude) {
  const int pwmResolution = 256; // 8-bit resolution (0-255 for amplitude)
  long period = 1000000L / frequency; // Period of the wave in microseconds
  long halfPeriod = period / 2;

  unsigned long startMillis = millis();
  while (millis() - startMillis < duration) {
    analogWrite(SPEAKER_PIN, amplitude); // Set high level to the specified amplitude
    delayMicroseconds(halfPeriod);

    analogWrite(SPEAKER_PIN, 0); // Set low level
    delayMicroseconds(halfPeriod);
  }
}
