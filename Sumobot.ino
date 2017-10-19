// Define pin numbers.
#define TRIGGER 9 // The trigger pin for the ultrasonic sensor.
#define ECHO 10 // The echo pin for the ultrasonic sensor.
#define LDR_PIN A0 // The analog pin for LDR input,

// Define constants.
#define SPEED_OF_SOUND 0.03434 // Speed of sound as STP in cm per microsecond.

// Variables for distance tracking.
long duration;
double distance;

void setup() {
	pinMode(TRIGGER, OUTPUT); // Set the trigger pin as an output.
	pinMode(ECHO, INPUT); // Set the echo pin as an input.
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

void loop() {
	readLDR();
	readUltrasonic();
}
