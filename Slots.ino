// Define the pins for IR sensors and LEDs
const int ir1 = 2;
const int ir2 = 3;
const int ir3 = 4;

const int green1 = 5;
const int green2 = 6;
const int green3 = 7;

// Threshold value for object detection
const int x = 50;

void setup() {
  // Initialize IR sensor pins as inputs
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);

  // Initialize green LED pins as outputs
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(green3, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Read IR sensor values
  int sensorValue1 = digitalRead(ir1);
  int sensorValue2 = digitalRead(ir2);
  int sensorValue3 = digitalRead(ir3);
  ///Serial.println(sensorValue1);
  delay(500);
  // Serial.println(sensorValue2);
  // Serial.println(sensorValue3);

  // Check if object is detected for sensor 1
  if (sensorValue1 == 1) {
    digitalWrite(green1, HIGH);
  } else {
    digitalWrite(green1, LOW);
  }

  // Check if object is detected for sensor 2
  if (sensorValue2 == 1) {
    digitalWrite(green2, HIGH);
  } else {
    digitalWrite(green2, LOW);
  }

  // Check if object is detected for sensor 3
  if (sensorValue3 == 1) {
    digitalWrite(green3, HIGH);
  } else {
    digitalWrite(green3, LOW);
  }

  // Delay for stability
  delay(100);
}
