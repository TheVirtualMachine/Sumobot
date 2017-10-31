// Define sensor pin numbers.
#define TRIGGER 3 // The trigger pin for the ultrasonic sensor.
#define ECHO 4 // The echo pin for the ultrasonic sensor.
#define LDR_PIN A0 // The analog pin for LDR input,

// Define motor pin numbers.
#define LEFT_ENABLE 5 // The pin to turn the left motor on and off.
#define LEFT_FORWARD 6 // The pin to move the left motor forwards.
#define LEFT_BACKWARD 7 // The pin to move the left motor forwards.
#define RIGHT_ENABLE 8 // The pin to turn the left motor on and off.
#define RIGHT_FORWARD 9 // The pin to move the left motor forwards.
#define RIGHT_BACKWARD 10 // The pin to move the left motor forwards.

// Define constants.
#define SPEED_OF_SOUND 0.03434 // Speed of sound as STP in cm per microsecond.

// Variables for distance tracking.
long duration;
double distance;

void setup() {
  // Setup the ultrasonic sensor.
  pinMode(TRIGGER, OUTPUT); // Set the trigger pin as an output.
  pinMode(ECHO, INPUT); // Set the echo pin as an input.

  // Setup the motors.
  pinMode(LEFT_ENABLE, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_ENABLE, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  
  Serial.begin(9600); // Start the serial communication.
}

// Read the LDR sensor.
void readLDR() {
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("LDR: ");
  Serial.println(lightLevel);
}

// Read the ultrasonic sensor.
void readUltrasonic() {
  // Clear the trigger pin.
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  // Set the trigger pin on HIGH state for 10 micro seconds.
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Read the echo pin.
  duration = pulseIn(ECHO, HIGH); // Sound wave travel time in microseconds.

  distance = duration * SPEED_OF_SOUND / 2.0;

  // Print the distance to the serial monitor.
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(75);
}

void dance() {
	// Enable motors.
	digitalWrite(LEFT_ENABLE, HIGH);
	digitalWrite(RIGHT_ENABLE, HIGH);

	// Move forward.
	digitalWrite(LEFT_FORWARD, HIGH);
	digitalWrite(LEFT_BACKWARD, LOW);
	digitalWrite(RIGHT_FORWARD, HIGH);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

void loop() {
  readLDR();
  readUltrasonic();
  dance();
}
