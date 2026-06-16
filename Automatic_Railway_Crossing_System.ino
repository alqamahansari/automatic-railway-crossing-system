#include <Servo.h>

Servo gate1;
Servo gate2;

// IR Sensors
const int IR1 = 2;   // Entry Sensor (D2)
const int IR2 = 3;   // Exit Sensor (D3)

// Road LEDs
const int RED1 = 4;
const int YELLOW1 = 5;
const int GREEN1 = 6;

const int RED2 = 7;
const int YELLOW2 = 8;
const int GREEN2 = 11;

// Buzzer
const int BUZZER = 12;

// Train Signal LEDs
const int TRAIN_RED = A0;
const int TRAIN_GREEN = A1;

// Servo Pins
const int SERVO1_PIN = 9;
const int SERVO2_PIN = 10;

// Servo Angles
const int OPEN_ANGLE = 0;
const int CLOSE_ANGLE = 105;

// Train Status
bool trainDetected = false;

// Railway Crossing Warning
unsigned long crossingTimer = 0;
bool crossingState = false;

void setup()
{
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(RED1, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(GREEN1, OUTPUT);

  pinMode(RED2, OUTPUT);
  pinMode(YELLOW2, OUTPUT);
  pinMode(GREEN2, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(TRAIN_RED, OUTPUT);
  pinMode(TRAIN_GREEN, OUTPUT);

  gate1.attach(SERVO1_PIN);
  gate2.attach(SERVO2_PIN);

  openGate();
}

void loop()
{
  // Train enters crossing section
  if (!trainDetected && digitalRead(IR1) == LOW)
  {
    trainDetected = true;
    closeGate();
    delay(300); // debounce
  }

  // Train leaves crossing section
  if (trainDetected && digitalRead(IR2) == LOW)
  {
    trainDetected = false;
    openGate();
    delay(300); // debounce
  }

  railwayCrossingWarning();
}

void railwayCrossingWarning()
{
  if (trainDetected)
  {
    if (millis() - crossingTimer >= 300)
    {
      crossingTimer = millis();
      crossingState = !crossingState;

      if (crossingState)
      {
        // Red1 ON, Red2 OFF
        digitalWrite(RED1, LOW);
        digitalWrite(RED2, HIGH);

        // Buzzer ON
        digitalWrite(BUZZER, HIGH);
      }
      else
      {
        // Red1 OFF, Red2 ON
        digitalWrite(RED1, HIGH);
        digitalWrite(RED2, LOW);

        // Buzzer OFF
        digitalWrite(BUZZER, LOW);
      }
    }
  }
}

void closeGate()
{
  // Close Gates
  gate1.write(CLOSE_ANGLE);
  gate2.write(CLOSE_ANGLE);

  // Road Green OFF
  digitalWrite(GREEN1, HIGH);
  digitalWrite(GREEN2, HIGH);

  // Road Yellow OFF
  digitalWrite(YELLOW1, HIGH);
  digitalWrite(YELLOW2, HIGH);

  // Train Signal RED
  digitalWrite(TRAIN_RED, HIGH);
  digitalWrite(TRAIN_GREEN, LOW);

  // Start Warning Sequence
  crossingState = false;
  crossingTimer = millis();
}

void openGate()
{
  // Open Gates
  gate1.write(OPEN_ANGLE);
  gate2.write(OPEN_ANGLE);

  // Road Red OFF
  digitalWrite(RED1, HIGH);
  digitalWrite(RED2, HIGH);

  // Road Yellow OFF
  digitalWrite(YELLOW1, HIGH);
  digitalWrite(YELLOW2, HIGH);

  // Road Green ON
  digitalWrite(GREEN1, LOW);
  digitalWrite(GREEN2, LOW);

  // Buzzer OFF
  digitalWrite(BUZZER, LOW);

  // Train Signal GREEN
  digitalWrite(TRAIN_RED, LOW);
  digitalWrite(TRAIN_GREEN, HIGH);

  crossingState = false;
}