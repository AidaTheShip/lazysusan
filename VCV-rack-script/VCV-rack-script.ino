
#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Define some variables that have fixed value (const)
const int mystery1 = 0; // Acceptable values: -4 to +4
const int mystery2 = 127; // Acceptable values: 0 to 127

// Define ultrasonic sensor pins
#define SENSOR1_TRIG 11
#define SENSOR1_EC 12
#define SENSOR2_TRIG 14
#define SENSOR2_EC 13
#define SENSOR3_TRIG 9
#define SENSOR3_EC 10
#define SENSOR4_TRIG 7
#define SENSOR4_EC 8

// Define MIDI note for each sensor
const int midiNotes[4] = {60, 62, 64, 65}; // MIDI notes for sensors 1, 2, 3, 4
bool notePlaying[4] = {0};  // Array to track the playing status of each note

void setup() {
  Serial.begin(9600);

  // Initialize ultrasonic sensor pins
  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_EC, INPUT);
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_EC, INPUT);
  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_EC, INPUT);
  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_EC, INPUT);

  // Initialize built-in LED
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin();
}

void loop() {
  // Get distances from all 4 sensors
  int distance1 = getDistance(SENSOR1_TRIG, SENSOR1_EC);
  int distance2 = getDistance(SENSOR2_TRIG, SENSOR2_EC);
  int distance3 = getDistance(SENSOR3_TRIG, SENSOR3_EC);
  int distance4 = getDistance(SENSOR4_TRIG, SENSOR4_EC);

  // Find the sensor with the lowest distance (closest object)
  int minDistance = min(distance1, min(distance2, min(distance3, distance4)));
  int sensorIndex = -1;

  if (minDistance == distance1) sensorIndex = 0;
  else if (minDistance == distance2) sensorIndex = 1;
  else if (minDistance == distance3) sensorIndex = 2;
  else if (minDistance == distance4) sensorIndex = 3;

  // Play the corresponding MIDI note if it's not already playing
  if (sensorIndex != -1 && !notePlaying[sensorIndex]) {
    for (int i = 0; i < 4; i++) {
      if (notePlaying[i]) {
        MIDI.sendNoteOff(midiNotes[i] + 12 * mystery1, mystery2, 1);
        notePlaying[i] = false;
      }
    }
    notePlaying[sensorIndex] = true;
    MIDI.sendNoteOn(midiNotes[sensorIndex] + 12 * mystery1, mystery2, 1);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// Function to get distance from an ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout of 30 ms

  // Calculate distance in cm (speed of sound is ~0.0343 cm/us)
  int distance = (duration * 0.0343) / 2;

  // Return the calculated distance
  return distance;
}
