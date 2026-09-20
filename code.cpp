#include <LiquidCrystal.h>

#define STEPPER_PIN_1 2
#define STEPPER_PIN_2 3
#define STEPPER_PIN_3 4
#define STEPPER_PIN_4 5
const int buttonPin = 6;
const int buzzerPin = 7;
LiquidCrystal lcd(8, 9, 10, A2, A3, A4);
const int potPin = A0;

int step_number = 0;
bool scanning = false;
bool lastButton = HIGH;
int baselinePot = 0;
bool inTrench = false;

void OneStep(bool dir) {
  if (dir) {
    switch (step_number) {
      case 0: digitalWrite(STEPPER_PIN_1, HIGH); digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, LOW);  break;
      case 1: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, HIGH); digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, LOW);  break;
      case 2: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, HIGH); digitalWrite(STEPPER_PIN_4, LOW);  break;
      case 3: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, HIGH); break;
    }
  } else {
    switch (step_number) {
      case 0: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, HIGH); break;
      case 1: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, HIGH); digitalWrite(STEPPER_PIN_4, LOW);  break;
      case 2: digitalWrite(STEPPER_PIN_1, LOW);  digitalWrite(STEPPER_PIN_2, HIGH); digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, LOW);  break;
      case 3: digitalWrite(STEPPER_PIN_1, HIGH); digitalWrite(STEPPER_PIN_2, LOW);  digitalWrite(STEPPER_PIN_3, LOW);  digitalWrite(STEPPER_PIN_4, LOW);  break;
    }
  }
  step_number++;
  if (step_number > 3) step_number = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(STEPPER_PIN_1, OUTPUT); pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT); pinMode(STEPPER_PIN_4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("Profilometer");
  lcd.setCursor(0, 1);
  lcd.print("Booting...");
  digitalWrite(buzzerPin, HIGH); delay(100); digitalWrite(buzzerPin, LOW);
  delay(300);
  baselinePot = analogRead(potPin);
  lcd.clear(); lcd.print("Ready"); lcd.setCursor(0, 1); lcd.print("Press to scan");
}

void loop() {
  bool button = digitalRead(buttonPin);
  if (button == LOW && lastButton == HIGH) {
    scanning = !scanning;
    delay(200);
  }
  lastButton = button;

  if (scanning) {
    for (int i = 0; i < 20; i++) {
      OneStep(false);
      delay(2);
    }
  }

 
  int potVal = analogRead(potPin);
  int deflection = potVal - baselinePot;
  if (abs(deflection) > 15 && !inTrench) {
    inTrench = true;
    digitalWrite(buzzerPin, HIGH); delay(30); digitalWrite(buzzerPin, LOW);
  } else if (abs(deflection) <= 15 && inTrench) {
    inTrench = false;
  }

  Serial.print("H:"); Serial.println(potVal);

  lcd.setCursor(0, 0);
  lcd.print("H:"); lcd.print(potVal); lcd.print("        ");
  lcd.setCursor(0, 1);
  lcd.print(scanning ? "SCANNING       " : "idle, press btn");
}
