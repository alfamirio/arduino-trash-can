/*
  Arduino code for a trash can that automatically opens its lid.
  
  This Arduino code controls an SG90 servo motor based on readings from an ultrasonic distance sensor. 

  Board: Arduino Uno R3
  Components: Servo motor(SG90) and Ultrasonic distance Sensor(HC-SR04)
*/

int counter = 0;
int const trigPin = 6;
int const echoPin = 5;
int const ledPin = 13;


void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT);

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
    if (distance <= 10 && distance >= 0) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ON");
    } else {
        digitalWrite(ledPin, LOW);
        Serial.println("LED OFF");
    }
    // wait 100ms
    delay(200);
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


