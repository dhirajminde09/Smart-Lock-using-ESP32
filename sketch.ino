#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// =====================
// PIN DEFINITIONS
// =====================

#define SERVO_PIN 18
#define BUZZER_PIN 19
#define RED_LED 4
#define GREEN_LED 5

// =====================
// LCD
// =====================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// =====================
// SERVO
// =====================

Servo doorServo;

// =====================
// KEYPAD
// =====================

const byte ROWS = 4;
const byte COLS = 4;

char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(
  makeKeymap(keyMap),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// =====================
// PASSWORD
// =====================

const char correctPassword[] = "2226";

char enteredPassword[9];
byte passwordLength = 0;

// =====================
// SECURITY
// =====================

byte failedAttempts = 0;

const byte MAX_ATTEMPTS = 3;

bool systemLocked = false;

unsigned long lockStartTime = 0;

const unsigned long LOCK_TIME = 30000;

// =====================
// SERVO POSITIONS
// =====================

const int LOCKED = 0;
const int UNLOCKED = 90;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Output pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  // Servo
  doorServo.setPeriodHertz(50);
  doorServo.attach(SERVO_PIN, 500, 2400);

  // Start in locked position
  doorServo.write(LOCKED);

  // LED status
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  clearPassword();

  showLockedScreen();
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // -----------------------------
  // CHECK SYSTEM LOCKOUT
  // -----------------------------

  if (systemLocked) {

    unsigned long elapsed = millis() - lockStartTime;

    if (elapsed >= LOCK_TIME) {

      systemLocked = false;
      failedAttempts = 0;

      clearPassword();
      showLockedScreen();

    } 
    else {

      unsigned long remaining =
        (LOCK_TIME - elapsed + 999) / 1000;

      lcd.setCursor(0, 0);
      lcd.print("SYSTEM LOCKED   ");

      lcd.setCursor(0, 1);
      lcd.print("Wait ");
      lcd.print(remaining);
      lcd.print(" sec   ");

      delay(200);

      return;
    }
  }

  // -----------------------------
  // READ KEYPAD
  // -----------------------------

  char key = keypad.getKey();

  if (key != NO_KEY) {

    processKey(key);
  }
}


// =====================================================
// PROCESS KEYPAD
// =====================================================

void processKey(char key) {

  // -----------------------------
  // CLEAR
  // -----------------------------

  if (key == '*') {

    clearPassword();

    lcd.setCursor(0, 1);
    lcd.print("                ");

    return;
  }

  // -----------------------------
  // ENTER / SUBMIT
  // -----------------------------

  if (key == '#') {

    checkPassword();

    return;
  }

  // -----------------------------
  // NUMBER
  // -----------------------------

  if (key >= '0' && key <= '9') {

    if (passwordLength < 8) {

      enteredPassword[passwordLength] = key;

      passwordLength++;

      enteredPassword[passwordLength] = '\0';

      lcd.setCursor(0, 1);

      lcd.print("                ");

      lcd.setCursor(0, 1);

      for (byte i = 0; i < passwordLength; i++) {

        lcd.print('*');
      }
    }
  }
}


// =====================================================
// CHECK PASSWORD
// =====================================================

void checkPassword() {

  if (passwordLength == 0) {

    return;
  }

  if (strcmp(enteredPassword, correctPassword) == 0) {

    failedAttempts = 0;

    unlockDoor();

  } 
  else {

    failedAttempts++;

    denyAccess();

    if (failedAttempts >= MAX_ATTEMPTS) {

      activateLockout();
    }
  }

  clearPassword();
}


// =====================================================
// UNLOCK DOOR
// =====================================================

void unlockDoor() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS GRANTED");

  lcd.setCursor(0, 1);
  lcd.print("Door Opening");

  // LEDs
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // Success beep
  tone(BUZZER_PIN, 1500, 200);

  // Unlock
  doorServo.write(UNLOCKED);

  delay(4000);

  // Lock again
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Door Locking...");

  doorServo.write(LOCKED);

  delay(1500);

  // Back to locked state
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  showLockedScreen();
}


// =====================================================
// ACCESS DENIED
// =====================================================

void denyAccess() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS DENIED");

  lcd.setCursor(0, 1);
  lcd.print("Attempt ");
  lcd.print(failedAttempts);
  lcd.print("/3");

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  // Error beep
  tone(BUZZER_PIN, 500, 500);

  delay(1500);

  showLockedScreen();
}


// =====================================================
// LOCKOUT
// =====================================================

void activateLockout() {

  systemLocked = true;

  lockStartTime = millis();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SYSTEM LOCKED");

  lcd.setCursor(0, 1);
  lcd.print("WAIT 30 SECONDS");

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  // Alarm
  tone(BUZZER_PIN, 700, 1000);

  delay(1000);
}


// =====================================================
// CLEAR PASSWORD
// =====================================================

void clearPassword() {

  passwordLength = 0;

  enteredPassword[0] = '\0';
}


// =====================================================
// LOCKED SCREEN
// =====================================================

void showLockedScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART DOOR LOCK");

  lcd.setCursor(0, 1);
  lcd.print("Enter Password:");

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}