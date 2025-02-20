/*
  Arduino code for a trash can that automatically opens its lid.
  
  This Arduino code controls an SG90 servo motor based on readings from an ultrasonic distance sensor. 

  Board: Arduino Uno R3
  Components: Servo motor(SG90) and Ultrasonic distance Sensor(HC-SR04)
*/

#include <Servo.h>

int counter = 0;
int const trigPin = 6;
int const echoPin = 5;
int const ledPin = 13;
int const servoPin = 9;

Servo servoMotor;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  servoMotor.attach(servoPin);
  Serial.begin(9600);
  while (!Serial); // Wait until Serial is ready
  Serial.println("Debug ready!");
}

void loop() {
    int distance;

    distance = computeDistance();
    
    Serial.print(counter);
    Serial.print(" - Distance is ");
    Serial.println(distance);

    // if distance less than d cm (<=0 is over range) 
    if (distance <= 50 && distance > 0) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ON");
        servoOpenLid();
    } else {
        digitalWrite(ledPin, LOW);
        Serial.println("LED OFF");
        servoCloseLid();
    }
    
    // wait 1000ms
    delay(1000);
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

void servoOpenLid() {  
  // Desplazamos a la posición 0º
  servoMotor.write(0);
}

void servoCloseLid() {  
  // Desplazamos a la posición 90º
  servoMotor.write(90);
}

