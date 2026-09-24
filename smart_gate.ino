#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Set the I2C address (usually 0x27 or 0x3F) and dimensions (16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create Servo object
Servo gateServo;

// Define Pins
const int trigPin = 9;
const int echoPin = 8;
const int servoPin = 6;

// Gate Configuration Thresholds
const int openDistance = 15; // Gate opens if object is closer than 15 cm
long duration;
int distance;
bool isGateOpen = false;

void setup() {
  // Pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach Servo Motor and set initial closed position (0 degrees)
  gateServo.attach(servoPin);
  gateServo.write(0);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Startup screen
  lcd.setCursor(0, 0);
  lcd.print("Smart Gate Sys");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Trigger Ultrasonic Sensor pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculate distance in cm
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;

  // Display distance on Line 1
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  if (distance > 400 || distance < 2) {
    lcd.print("Out Range");
  } else {
    lcd.print(distance);
    lcd.print(" cm   "); // Extra spaces clear previous double/triple digits
  }

  // Gate Logic Control based on distance
  if (distance > 0 && distance <= openDistance) {
    if (!isGateOpen) {
      gateServo.write(90); // Open the gate (90 degrees)
      isGateOpen = true;
    }
    lcd.setCursor(0, 1);
    lcd.print("Gate: OPEN      ");
  } else {
    if (isGateOpen) {
      delay(1500); // Hold gate open for 1.5 seconds before closing
      gateServo.write(0); // Close the gate (0 degrees)
      isGateOpen = false;
    }
    lcd.setCursor(0, 1);
    lcd.print("Gate: CLOSED    ");
  }

  delay(100); // Small loop delay
}
