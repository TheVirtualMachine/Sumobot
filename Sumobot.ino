#include "Music.h"

// Define ultrasonic sensor pin numbers.
#define FRONT_TRIGGER 11
#define FRONT_ECHO 12
#define BACK_TRIGGER 3
#define BACK_ECHO 4

// Define LDR pins.
#define LDR_FRONT_LEFT A0
#define LDR_FRONT_RIGHT A1
#define LDR_BACK_LEFT A2
#define LDR_BACK_RIGHT A3

// Define motor pin numbers.
#define LEFT_ENABLE 5 // The pin to turn the left motor on and off.
#define LEFT_FORWARD 6 // The pin to move the left motor forwards.
#define LEFT_BACKWARD 7 // The pin to move the left motor forwards.
#define RIGHT_ENABLE 8 // The pin to turn the left motor on and off.
#define RIGHT_FORWARD 9 // The pin to move the left motor forwards.
#define RIGHT_BACKWARD 10 // The pin to move the left motor forwards.

// Define constants.
#define SPEED_OF_SOUND 0.03434 // Speed of sound as STP in cm per microsecond.

// Variables for LDRs.
int frontLeftLight, frontRightLight, backLeftLight, backRightLight;

// Variables for distance tracking.

// The ultrasonic sensor reading.
double frontDistance, backDistance;

// Define thresholds.
#define DISTANCE_THRESHOLD 100
#define LDR_THRESHOLD 300

// Information for playing notes.
unsigned int currentNote = 0; // The index of current note being played in the arrays.
unsigned long noteEndTime = 0; // The time in milliseconds to stop the current note.

#define SPEAKER_PIN 13 // Pin for the speaker.

// Setup the program.
void setup() {
	// Setup the ultrasonic sensors.
	pinMode(FRONT_TRIGGER, OUTPUT); // Set the trigger pin as an output.
	pinMode(FRONT_ECHO, INPUT); // Set the echo pin as an input.
	pinMode(BACK_TRIGGER, OUTPUT); // Set the trigger pin as an output.
	pinMode(BACK_ECHO, INPUT); // Set the echo pin as an input.

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
	//frontLeftLight = analogRead(LDR_FRONT_LEFT);
	//frontRightLight = analogRead(LDR_FRONT_RIGHT);
	//backLeftLight = analogRead(LDR_BACK_LEFT);
	//backRightLight = analogRead(LDR_BACK_RIGHT);
	frontLeftLight = 1000;
	frontRightLight = 1000;
	backLeftLight = 1000;
	backRightLight = 1000;
}

// Read the front ultrasonic sensor.
void readFrontUltrasonic() {
	// Clear the trigger pin.
	digitalWrite(FRONT_TRIGGER, LOW);
	delayMicroseconds(2);

	// Set the trigger pin on HIGH state for 10 micro seconds.
	digitalWrite(FRONT_TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(FRONT_TRIGGER, LOW);

	// Read the echo pin.
	long duration = pulseIn(FRONT_ECHO, HIGH); // Sound wave travel time in microseconds.

	frontDistance = duration * SPEED_OF_SOUND / 2.0;

	// Print the distance to the serial monitor.
	Serial.print("Front distance: ");
	Serial.println(frontDistance);
}

// Read the back ultrasonic sensor.
void readBackUltrasonic() {
	// Clear the trigger pin.
	digitalWrite(BACK_TRIGGER, LOW);
	delayMicroseconds(2);

	// Set the trigger pin on HIGH state for 10 micro seconds.
	digitalWrite(BACK_TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(BACK_TRIGGER, LOW);

	// Read the echo pin.
	long duration = pulseIn(BACK_ECHO, HIGH); // Sound wave travel time in microseconds.

	backDistance = duration * SPEED_OF_SOUND / 2.0;

	// Print the distance to the serial monitor.
	Serial.print("Back distance: ");
	Serial.println(backDistance);
}

// Read the ultrasonic sensors.
void readUltrasonic() {
	readFrontUltrasonic();
	readBackUltrasonic();
}

void forward() {
	Serial.println("Forward");

	// Enable motors.
	digitalWrite(LEFT_ENABLE, HIGH);
	digitalWrite(RIGHT_ENABLE, HIGH);

	// Move forward.
	digitalWrite(LEFT_FORWARD, HIGH);
	digitalWrite(LEFT_BACKWARD, LOW);
	digitalWrite(RIGHT_FORWARD, HIGH);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

void left() {
	Serial.println("Left");

	// Enable motors.
	digitalWrite(LEFT_ENABLE, HIGH);
	digitalWrite(RIGHT_ENABLE, HIGH);

	// Move left.
	digitalWrite(LEFT_FORWARD, LOW);
	digitalWrite(LEFT_BACKWARD, HIGH);
	digitalWrite(RIGHT_FORWARD, HIGH);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

void right() {
	Serial.println("Right");

	// Enable motors.
	digitalWrite(LEFT_ENABLE, HIGH);
	digitalWrite(RIGHT_ENABLE, HIGH);

	// Move right.
	digitalWrite(LEFT_FORWARD, HIGH);
	digitalWrite(LEFT_BACKWARD, LOW);
	digitalWrite(RIGHT_FORWARD, LOW);
	digitalWrite(RIGHT_BACKWARD, HIGH);
}

void backward() {
	Serial.println("Backward");

	// Enable motors.
	digitalWrite(LEFT_ENABLE, HIGH);
	digitalWrite(RIGHT_ENABLE, HIGH);

	// Move forward.
	digitalWrite(LEFT_FORWARD, LOW);
	digitalWrite(LEFT_BACKWARD, HIGH);
	digitalWrite(RIGHT_FORWARD, LOW);
	digitalWrite(RIGHT_BACKWARD, HIGH);
}

void stop() {
	Serial.println("Stop");

	digitalWrite(LEFT_ENABLE, LOW);
	digitalWrite(RIGHT_ENABLE, LOW);
}

void dance() {
	backward();
	left();
	right();
	backward();
}

// Play the next note in the song.
void updateMusic() {
	noTone(SPEAKER_PIN); // Ensure no tone is playing.

	if (currentNote >= (sizeof(melody) / sizeof(int))) { // If we have reached the end of the song.
		currentNote = 0;
	}

	int noteDuration = noteDurations[currentNote] * min(frontDistance * 2, 100);
	if (melody[currentNote] == NOTE_REST) { // If this note is a rest, don't play anything.
		noteEndTime = millis() + noteDuration * 1.30; // Set the time for when to play the next note to noteDuration + 30%.
	} else {
		tone(SPEAKER_PIN, melody[currentNote], noteDuration);
		noteEndTime = millis() + noteDuration * 1.30; // Set the time for when to play the next note to noteDuration + 30%.
	}
	currentNote++;
}

// Check if a given LDR reading should trigger a reaction.
bool isLDRTriggered(int level) {
	return (level <= LDR_THRESHOLD);
}

// Move away from the edge.
// Return if we were touching the edge to begin with.
bool moveAwayFromEdge() {
	if (isLDRTriggered(frontLeftLight) && isLDRTriggered(frontRightLight)) {
		backward();
		return true;
	} else if (isLDRTriggered(backLeftLight) && isLDRTriggered(backRightLight)) {
		forward();
		return true;
	} else if (isLDRTriggered(frontLeftLight)) {
		right();
		return true;
	} else if (isLDRTriggered(frontRightLight)) {
		left();
		return true;
	} else if (isLDRTriggered(backLeftLight)) {
		right();
		return true;
	} else if (isLDRTriggered(backRightLight)) {
		left();
		return true;
	}
	return false;
}

void loop() {
	readLDR();
	readUltrasonic();

	if (!moveAwayFromEdge()) {
		if (frontDistance <= DISTANCE_THRESHOLD || backDistance <= DISTANCE_THRESHOLD) {
			if (frontDistance <= backDistance) {
				forward();
			} else if (backDistance < frontDistance) {
				backward();
			}
		} else {
			right();
		}
	}

	if (millis() >= noteEndTime) {
		updateMusic();
	}
	delay(500);
}
