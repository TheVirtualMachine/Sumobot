#include "Music.h"

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

// The ultrasonic sensor reading.
double frontDistance, backDistance;

// Define thresholds.
#define DISTANCE_THRESHOLD 100

// Information for playing notes.
unsigned int currentNote = 0; // The index of current note being played in the arrays.
unsigned long noteEndTime = 0; // The time in milliseconds to stop the current note.

#define SPEAKER_PIN 13 // Pin for the speaker.

// Setup the program.
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

// Read the ultrasonic sensors.
void readUltrasonic() {
	// Clear the trigger pin.
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	// Set the trigger pin on HIGH state for 10 micro seconds.
	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER, LOW);

	// Read the echo pin.
	long duration = pulseIn(ECHO, HIGH); // Sound wave travel time in microseconds.

	frontDistance = duration * SPEED_OF_SOUND / 2.0;

	// Print the distance to the serial monitor.
	Serial.print("Front distance: ");
	Serial.println(frontDistance);
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
	Serial.println("Note: " + noteDuration);
	if (melody[currentNote] == NOTE_REST) { // If this note is a rest, don't play anything.
		noteEndTime = millis() + noteDuration * 1.30; // Set the time for when to play the next note to noteDuration + 30%.
	} else {
		tone(SPEAKER_PIN, melody[currentNote], noteDuration);
		noteEndTime = millis() + noteDuration * 1.30; // Set the time for when to play the next note to noteDuration + 30%.
	}
	currentNote++;
}

void loop() {
	// readLDR();
	readUltrasonic();

	/*
	if (frontDistance <= DISTANCE_THRESHOLD || backDistance <= DISTANCE_THRESHOLD) {
		if (frontDistance <= backDistance) {
			forward();
		} else if (backDistance < frontDistance) {
			backward();
		}
	} else {
		left();
	}
	*/

	//dance();
	if (millis() >= noteEndTime) {
		updateMusic();
	}
}
