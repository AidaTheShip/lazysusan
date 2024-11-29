
// Updated Arduino Code for MKRZero with 4 Ultrasonic Sensors
// Sensor 1 --> Right
// Sensor 2 --> Top
// Sensor 3 --> Left
// Sensor 4 --> Bottom

#define SENSOR1_TRIG 13
#define SENSOR1_EC 14
#define SENSOR2_TRIG 1
#define SENSOR2_EC 0
#define SENSOR3_TRIG 8
#define SENSOR3_EC 9
#define SENSOR4_TRIG 6
#define SENSOR4_EC 7

#define motor A1

int lower_boundary = 0;
int upper_boundary = 100;

void setup() {
  Serial.begin(9600);

  // Set pin modes for ultrasonic sensors
  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_EC, INPUT);
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_EC, INPUT);
  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_EC, INPUT);
  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_EC, INPUT);

  // No need to set pin mode for the motor (analog read pin)
}

void loop() {
  // Read motor value
  int motorVal = analogRead(motor);
  // Serial.print("Motor Value: ");
  // Serial.println(motorVal);  

  // // Function to trigger the sensor and get the distance
  int distance1 = getDistance(SENSOR1_TRIG, SENSOR1_EC);
  int distance2 = getDistance(SENSOR2_TRIG, SENSOR2_EC);
  int distance3 = getDistance(SENSOR3_TRIG, SENSOR3_EC);
  int distance4 = getDistance(SENSOR4_TRIG, SENSOR4_EC);

  // // Print distances
  Serial.print("Distance 1 (Right): ");
  Serial.println(distance1);

  Serial.print("Distance 2 (Top): ");
  Serial.println(distance2);

  Serial.print("Distance 3 (Left): ");
  Serial.println(distance3);

  Serial.print("Distance 4 (Bottom): ");
  Serial.println(distance4);

  delay(200);
}

int getDistance(int trigPin, int echoPin) {
  // Triggering the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout of 30 ms

  // Calculate distance in cm (speed of sound is ~0.0343 cm/us)
  int distance = (duration * 0.0343) / 2;
  return distance;
}
