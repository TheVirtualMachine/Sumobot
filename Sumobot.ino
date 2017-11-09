/********************************************
* Vincent Macri, David White, Rian Waterson *
* Mr. Wong                                  *
* 2017-11-09                                *
* Sumobot program.                          *
*********************************************/
// Define ultrasonic sensor pin numbers.
#define FRONT_TRIGGER 12
#define FRONT_ECHO 13
#define BACK_TRIGGER 3
#define BACK_ECHO 4

// Define LDR pins.
#define LDR_FRONT_LEFT A0
#define LDR_FRONT_RIGHT A1
#define LDR_BACK_LEFT A2
#define LDR_BACK_RIGHT A3

// Define motor pin numbers.
#define LEFT_FORWARD 9 // The pin to move the left motor forwards.
#define LEFT_BACKWARD 10 // The pin to move the left motor forwards.
#define RIGHT_FORWARD 6 // The pin to move the left motor forwards.
#define RIGHT_BACKWARD 7 // The pin to move the left motor forwards.

// Define constants.
#define SPEED_OF_SOUND 0.03434 // Speed of sound as STP in cm per microsecond.

// Variables for LDRs.
int frontLeftLight, frontRightLight, backLeftLight, backRightLight;

// Variables for distance tracking.

// The ultrasonic sensor reading.
double frontDistance, backDistance;

// Define thresholds.
#define DISTANCE_THRESHOLD 50
#define LDR_THRESHOLD 550

// Setup the program.
void setup() {
	// Setup the ultrasonic sensors.
	pinMode(FRONT_TRIGGER, OUTPUT); // Set the trigger pin as an output.
	pinMode(FRONT_ECHO, INPUT); // Set the echo pin as an input.
	pinMode(BACK_TRIGGER, OUTPUT); // Set the trigger pin as an output.
	pinMode(BACK_ECHO, INPUT); // Set the echo pin as an input.

	// Setup the motors.
	pinMode(LEFT_FORWARD, OUTPUT);
	pinMode(LEFT_BACKWARD, OUTPUT);
	pinMode(RIGHT_FORWARD, OUTPUT);
	pinMode(RIGHT_BACKWARD, OUTPUT);

	Serial.begin(9600); // Start the serial communication.
}

// Read the LDR sensor.
void readLDR() {
	frontLeftLight = analogRead(LDR_FRONT_LEFT);
	frontRightLight = analogRead(LDR_FRONT_RIGHT);
	backLeftLight = analogRead(LDR_BACK_LEFT);
	backRightLight = analogRead(LDR_BACK_RIGHT);
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
}

// Read the ultrasonic sensors.
void readUltrasonic() {
	readFrontUltrasonic();
	readBackUltrasonic();
}

void forward() {
	Serial.println("Forward");

	// Move forward.
	digitalWrite(LEFT_FORWARD, HIGH);
	digitalWrite(LEFT_BACKWARD, LOW);
	digitalWrite(RIGHT_FORWARD, HIGH);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

void left() {
	Serial.println("Left");

	// Move left.
	digitalWrite(LEFT_FORWARD, LOW);
	digitalWrite(LEFT_BACKWARD, HIGH);
	digitalWrite(RIGHT_FORWARD, HIGH);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

void right() {
	Serial.println("Right");

	// Move right.
	digitalWrite(LEFT_FORWARD, HIGH);
	digitalWrite(LEFT_BACKWARD, LOW);
	digitalWrite(RIGHT_FORWARD, LOW);
	digitalWrite(RIGHT_BACKWARD, HIGH);
}

void backward() {
	Serial.println("Backward");

	// Move forward.
	digitalWrite(LEFT_FORWARD, LOW);
	digitalWrite(LEFT_BACKWARD, HIGH);
	digitalWrite(RIGHT_FORWARD, LOW);
	digitalWrite(RIGHT_BACKWARD, HIGH);
}

void stop() {
	Serial.println("Stop");

	digitalWrite(LEFT_FORWARD, LOW);
	digitalWrite(LEFT_BACKWARD, LOW);

	digitalWrite(RIGHT_FORWARD, LOW);
	digitalWrite(RIGHT_BACKWARD, LOW);
}

// Check if a given LDR reading should trigger a reaction.
bool isLDRTriggered(int level) {
	return (level <= LDR_THRESHOLD);
}

// Check if any of the LDRs are triggered.
bool areLDRsTriggered() {
	return (isLDRTriggered(frontLeftLight) || isLDRTriggered(frontRightLight) || isLDRTriggered(backLeftLight) || isLDRTriggered(backRightLight));
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
	} else if (isLDRTriggered(frontLeftLight) && isLDRTriggered(backLeftLight)) {
		right();
		return true;
	} else if (isLDRTriggered(frontRightLight) && isLDRTriggered(backRightLight)) {
		left();
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
	Serial.println(frontLeftLight);
	Serial.print(" ");
	return;

	Serial.print(frontLeftLight);
	Serial.print(" ");
	Serial.print(frontRightLight);
	Serial.print(" ");
	Serial.print(backLeftLight);
	Serial.print(" ");
	Serial.println(backRightLight);

	if (!moveAwayFromEdge()) {
		if (frontDistance <= DISTANCE_THRESHOLD && frontDistance <= backDistance) {
			forward();
		} else if (backDistance <= DISTANCE_THRESHOLD && backDistance < frontDistance) {
			backward();
		} else {
			right();
		}
	}
}
