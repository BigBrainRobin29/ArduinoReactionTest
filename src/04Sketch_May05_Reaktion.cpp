#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

// Method declarations
long check();
void printLcd();

// Initialize ldc object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Config
const int LED = 8;
const int BUTTON = 13;

const int MIN_DELAY = 1000;
const int MAX_DELAY = 5000;

String currentText = "";

// Print to lcd, allows new line character
void printLcd(String str) {
  // If the string is the same as the current text, do not update
  if (str == currentText) {
    return;
  }

  currentText = str;

  if (str.indexOf('\n') == -1) {
    // String contains 1 line
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str);
  } else {
    // String contains 2 lines
    String lines[2] = {str.substring(0, str.indexOf('\n')), str.substring(str.indexOf('\n') + 1, str.length())};

    lcd.clear();

    int y = 0;
  
    for (String line : lines) {
      lcd.setCursor(0, y);
      lcd.print(line);
      y++;
    }
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize LED and button
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Initialize random seed
  randomSeed(analogRead(A0));

  // Initialize Serial
  Serial.begin(9600);
  Serial.println("Initialized Reaction Test!");
}

void loop() {
  // Arrays to store reaction times
  long times1[5];
  long times2[5];

  // Run test 5 times with glasses on
  for (int i = 0; i < 5; i++) {
    // Get response time
    long time = check();

    // Button was pressed too early
    if (time == -1) {
      i--;
      continue;
    }

    // Store the time
    times1[i] = time;
  }

  printLcd("Now take your\nglasses off!");

  delay(2000);

  // Run test 5 times without glasses
  for (int i = 0; i < 5; i++) {
    // Get response time
    long time = check();

    // Button was pressed too early
    if (time == -1) {
      i--;
      continue;
    }

    // Store the time
    times2[i] = time;
  }

  printLcd("Test done!");

  delay(2000);

  printLcd("Check serial\nfor results.");

  // Print results as CSV to Serial
  Serial.println("Results:");
  Serial.println("With glasses;Without glasses");
  for (int i = 0; i < 5; i++) {
    Serial.print(times1[i]);
    Serial.print(";");
    Serial.println(times2[i]);
  }
  Serial.println("=MITTELWERT(A1:A6);=MITTELWERT(B1:B6)");

  delay(5000);
}

long check() {
  printLcd("Press button\nto start!");

  // Wait for button to be pressed
  while (digitalRead(BUTTON) == 1) {
    // pass
  } 

  // Countdown before starting
  for (int i = 3; i >= 0; i--) {
    printLcd("Starting in " + String(i) + "...");
    delay(750);
  }

  printLcd("Waiting...");

  // Generate random delay between MIN_DELAY and MAX_DELAY
  int delayms = random(MIN_DELAY, MAX_DELAY);

  // Wait for a random time while checking if the button is pressed to early
  for (int i = 0; i < delayms / 10; i++) {
    delay(10);

    if (digitalRead(BUTTON) == 0) {
      printLcd("Too early!");
      delay(1000);

      // Return -1 to indicate that the button was pressed too early
      return -1L;
    }
  }
  
  digitalWrite(LED, HIGH);
  printLcd("Press now!");

  long startTime = millis();
  long endTime;

  // Wait for button to be pressed
  while (digitalRead(BUTTON) == 0) {
    // pass
  }

  // Button was pressed, wait for release
  while (true) {
    int digital = digitalRead(BUTTON);
    if (digital == 0) {
      endTime = millis();
      break;
    }
  }

  // Calculate the reaction time
  long diff = endTime - startTime;

  digitalWrite(LED, LOW);
  printLcd("Time:\n" + String(diff) + "ms");
  delay(1000);

  return diff;
}
