/*
    Arduino code for a trash can that automatically opens its lid.
    It closes the lid automatically after 5s of no object detected.

    This Arduino code controls an SG90 servo motor based on readings from an ultrasonic distance sensor.

    Board: Arduino Uno R3
    Components: Servo motor(SG90) and Ultrasonic distance Sensor(HC-SR04)
*/

#include <Servo.h>

int const debug = true;
int const delayTime = 1000;
int const delayTimeClose = 3000;
int const trigPin = 6;
int const echoPin = 5;
int const ledPin = 13;
int const servoPin = 9;

int counter = 0;
int counterOpenMoment = 0;
Servo servoMotor;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    servoMotor.attach(servoPin);

    if (debug) {
        Serial.begin(9600);
        while (!Serial); // Wait until Serial is ready
        Serial.println("Debug ready!");
    }
}

void loop() {

    if (debug) {
        aprintf("==== %d - Loop ====\n", counter);
    }

    if (isObjectDetected()) {
        counterOpenMoment = counter;
        turnOnLed();
        servoOpenLid();
    } else {
        // cerramos la tapa si ha pasado un tiempo desde el ultumo objecto detectado
        if (!shouldBeOpen()) {
            turnOffLed();
            servoCloseLid();
        }
    }

    // wait 1000ms
    delay(delayTime);
    counter = counter + 1;
}

// Calculate the distance
float computeDistance() {
    digitalWrite(trigPin, LOW);
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);

    // Measure the pulse width of the echo pin and calculate the distance value
    return pulseIn(echoPin, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
}

// Calcula numero de segundos entre dos momentos
float computeTimeElapse(int start, int end, int step) {
    return (end - start) * step;
}

// Determina si se ha detectado algo delante del sensor
// if distance less than d cm (<=0 is over range)
bool isObjectDetected() {
    int distance = computeDistance();
    bool objectDetected = distance <= 30 && distance > 0;
    if (debug) {
        aprintf("Distance: %d, Detected %d\n", distance, objectDetected);
    }

    return objectDetected;
}

// Determina si ha estado mas de 10s encendido sin detectar movimiento
bool shouldBeOpen() {
    float timeElapsed = computeTimeElapse(counterOpenMoment, counter, delayTime);
    bool shouldLidBeOpen = (timeElapsed < delayTimeClose)  && (timeElapsed > 0);
    if (debug) {
        aprintf("TimeElapsed: %f, ShouldLidBeOpened %d\n", timeElapsed, shouldLidBeOpen);
    }
    return shouldLidBeOpen;
}

void turnOnLed() {
    digitalWrite(ledPin, HIGH);
}

void turnOffLed() {
    digitalWrite(ledPin, LOW);
}

void servoOpenLid() {
    servoMotor.write(70);
}

void servoCloseLid() {
    servoMotor.write(0);
}

// DEBUG FUNCTION  (gist https://gist.github.com/EleotleCram/eb586037e2976a8d9884)
int aprintf(char *str, ...) {
	int i, j, count = 0;

	va_list argv;
	va_start(argv, str);
	for(i = 0, j = 0; str[i] != '\0'; i++) {
		if (str[i] == '%') {
			count++;

			Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);

			switch (str[++i]) {
				case 'd': Serial.print(va_arg(argv, int));
					break;
				case 'l': Serial.print(va_arg(argv, long));
					break;
				case 'f': Serial.print(va_arg(argv, double));
					break;
				case 'c': Serial.print((char) va_arg(argv, int));
					break;
				case 's': Serial.print(va_arg(argv, char *));
					break;
				case '%': Serial.print("%");
					break;
				default:;
			};

			j = i+1;
		}
	};
	va_end(argv);

	if(i > j) {
		Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);
	}

	return count;
}
