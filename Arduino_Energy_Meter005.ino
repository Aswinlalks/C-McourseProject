#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 16);  // LCD object with I2C address 0x27, 2 rows, and 16 columns
const int analogInPin = A0;  // Analog pin for voltage measurement
const int relayPin = 12;  // Pin connected to the relay module
const int buttonPin = 11;  // Pin connected to the manual control button
bool isButtonPressed = false;  // Flag to track button press
unsigned long lastButtonPress = 0;  // Store the time of the last button press

int a, b, c;  // Variables for sensor readings
long volt, current, power, ah, wh;  // Variables for voltage, current, power, ampere-hours, and watt-hours
unsigned long msec = 0;  // Current milliseconds
float time = 0.0;  // Elapsed time
int sample = 0;  // Sample count for averaging
float totalCharge = 0.0;  // Total charge in ampere-seconds
float averageAmps = 0.0;  // Average current
float ampSeconds = 0.0;  // Ampere-seconds
long ampHours = 0;  // Ampere-hours

// Pacman Intro Theme
// Melody and note definitions for the piezo buzzer
// (Note: Music part has been removed in this version)
// ...

int tempo = 105;  // Tempo of the musical notes
int buzzer = 4;  // Pin connected to the piezo buzzer

int melody[] = {
  // Melody array (Note: Music part has been removed in this version)
  // ...
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;  // Calculate the number of notes in the melody
int wholenote = (60000 * 4) / tempo;  // Calculate the duration of a whole note
int divider = 0, noteDuration = 0;  // Variables for note duration calculations

// Setup function: Initializes pins, LCD, and performs startup melody and scrolling text
void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Ensure the relay is initially turned off
  pinMode(buttonPin, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("GOOD DAY");

  // Play startup melody (Note: Music part has been removed in this version)
  // ...

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energy Meter");

  delay(2000);

  scrollText("Circuits & Measurements Lab  COURSE PROJECT  ");

  for (int i = 0; i < 2; ++i) {
    lcd.setCursor(0, 1);
    lcd.print("Initializing");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.clear();
  }

  lcd.clear();
}

// Main loop function: Reads sensor values, calculates parameters, and updates the LCD
void loop() {
  // Read analog sensor values for voltage and current
  for (int i = 0; i < 200; i++) {
    c = analogRead(A1);
    a = analogRead(A2);
    volt = volt + a;
    current = current + c;
    delay(1);
  }

  // Calculate average current and adjust values
  current = (current / 200 - 514);
  if (current < 0.01) current = 0;
  current = current * 0.9999;
  volt = volt / 30;
  power = (volt * current) / 1000;

  sample = sample + 1;

  long msec = millis();
  float time = (float)msec / 1000.0;
  totalCharge = totalCharge + current;
  averageAmps = totalCharge / sample;
  ampSeconds = averageAmps * time;
  ampHours = ampSeconds / 3600;
  wh = ampHours * volt;

  // Print electrical parameters to the serial monitor
  Serial.print("{\"voltage\":");
  Serial.print(volt);
  Serial.print("}  ");
  Serial.print(current);
  Serial.print("mA ");
  Serial.print(power, 6);
  Serial.print("W ");
  Serial.print(ampHours, 6);
  Serial.print("Ah ");
  Serial.print(wh);
  Serial.print(" WATTHOUR   ");
  Serial.print(" units   ");
  Serial.print(wh / 1000.0, 6);
  Serial.println();

  // Control the relay based on current limits and button presses
  controlRelay();

  // Update the LCD with real-time information
  lcd.setCursor(6, 0);
  lcd.print("V");
  b = volt % 10;
  lcd.setCursor(5, 0);
  lcd.print(b);
  b = (volt / 10) % 10;
  lcd.setCursor(4, 0);
  lcd.print(b);
  lcd.setCursor(3, 0);
  lcd.print(".");
  b = (volt / 100) % 10;
  lcd.setCursor(2, 0);
  lcd.print(b);
  b = (volt / 1000) % 10;
  lcd.setCursor(1, 0);
  if (volt > 999) lcd.print(b);
  else lcd.print(" ");

  lcd.setCursor(14, 0);
  lcd.print("A");

  b = current % 10;
  lcd.setCursor(13, 0);
  lcd.print(b);
  b = (current / 10) % 10;
  lcd.setCursor(12, 0);
  lcd.print(b);
  lcd.setCursor(11, 0);
  lcd.print(".");
  b = (current / 100) % 10;
  lcd.setCursor(10, 0);
  lcd.print(b);

  lcd.setCursor(6, 1);
  lcd.print("W");
  b = power % 10;
  lcd.setCursor(5, 1);
  lcd.print(b);
  b = (power / 10) % 10;
  lcd.setCursor(3, 1);
  lcd.print(b);
  lcd.setCursor(4, 1);
  lcd.print(".");
  b = (power / 100) % 10;
  lcd.setCursor(2, 1);
  if (power > 99) lcd.print(b);
  else lcd.print(" ");
  b = (power / 1000) % 10;

  lcd.setCursor(1, 1);
  if (power > 999) lcd.print(b);
  else lcd.print(" ");

  delay(100);
  lcd.setCursor(14, 1);
  lcd.print("Wh");

  b = wh % 10;
  lcd.setCursor(13, 1);
  lcd.print(b);
  b = (wh / 10) % 10;
  lcd.setCursor(12, 1);
  lcd.print(b);
  lcd.setCursor(11, 1);
  lcd.print(".");
  b = (wh / 100) % 10;
  lcd.setCursor(10, 1);
  lcd.print(b);
  b = (wh / 1000) % 10;
  lcd.setCursor(9, 1);
  lcd.print(b);

  // Control the relay and introduce a delay
  controlRelay();
  delay(800);

  // Clear the LCD and display energy consumption information
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energy Cons:");
  lcd.setCursor(0, 1);
  lcd.print(wh / 1000.0, 6);
  lcd.setCursor(9, 1);
  lcd.print("Units");
  delay(1000);
  lcd.clear();
  controlRelay();

  // Calculate and display energy cost
  float costPerUnit = 8.00;
  float energyCost = (wh / 1000.0) * costPerUnit;
  lcd.setCursor(0, 0);
  lcd.print("Cost/Unit: ");
  lcd.setCursor(10, 0);
  lcd.print(costPerUnit);

  lcd.setCursor(0, 1);
  lcd.print("Cost: ");
  lcd.print(energyCost, 2);
  lcd.setCursor(11, 1);
  lcd.print("Rs");

  delay(1000);
  lcd.clear();

  // Control the relay at the end of each iteration
  controlRelay();
}

// Function to control the relay based on current limits and button presses
void controlRelay() {
  if (current >= 30) {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay OFF - Current exceeded limit");
  } else {
    if (digitalRead(buttonPin) == HIGH && !isButtonPressed) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relay ON - Button pressed");
      isButtonPressed = true;
      tone(buzzer, 1000, 1000);  // Sound the buzzer for button press feedback
    } else if (isButtonPressed) {
      isButtonPressed = false;
    }
  }
}

// Function to scroll text on the LCD during startup
void scrollText(const char* text) {
  int textLength = strlen(text);

  for (int i = 0; i < textLength - 16; ++i) {
    int startPos = i % textLength;
    lcd.clear();
    lcd.setCursor(0, 0);

    // Display a portion of the text in a scrolling fashion
    for (int j = 0; j < 16; ++j) {
      lcd.write(text[(startPos + j) % textLength]);
    }

    delay(400);
  }
}
