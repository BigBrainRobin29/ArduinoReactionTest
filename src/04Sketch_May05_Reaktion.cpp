#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

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
  lcd.init();
  lcd.backlight();

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("init");
}

void loop() {
  long times1[5];
  long times2[5];

  for (int i = 0; i < 5; i++) {
    long time = check();

    if (time == -1) {
      i--;
      continue;
    }

    times1[i] = time;
  }

  printLcd("Now take your\nglasses off!");

  delay(2000);

  for (int i = 0; i < 5; i++) {
    long time = check();

    if (time == -1) {
      i--;
      continue;
    }

    times2[i] = time;
  }

  printLcd("Test done!");

  delay(2000);

  printLcd("Check serial\nfor results.");

  delay(5000);
}

long check() {
  printLcd("Press button\nto start!");
  while (digitalRead(BUTTON) == 1) {
    // pass
  } 

  for (int i = 3; i >= 0; i--) {
    printLcd("Starting in " + String(i) + "...");
    delay(1000);
  }

  printLcd("Waiting...");

  int delayms = random(MIN_DELAY, MAX_DELAY);
  Serial.println(delayms);

  for (int i = 0; i < delayms / 10; i++) {
    Serial.println(i);
    delay(10);

    if (digitalRead(BUTTON) == 0) {
      printLcd("Too early!");
      delay(1000);
      return -1L;
    }
  }
  
  digitalWrite(LED, HIGH);
  printLcd("Press now!");

  long startTime = millis();
  long endTime;

  while (digitalRead(BUTTON) == 0) {
    // pass
  }

  while (true) {
    int digital = digitalRead(BUTTON);
    Serial.println(digital);
    if (digital == 0) {
      endTime = millis();
      break;
    }
  }

  long diff = endTime - startTime;
  Serial.println(diff);

  digitalWrite(LED, LOW);
  printLcd("Time:\n" + String(diff) + "ms");
  delay(1000);

  return diff;
}
