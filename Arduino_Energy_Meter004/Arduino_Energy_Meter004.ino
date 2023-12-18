#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 16);
const int analogInPin = A0;
const int relayPin = 12;
const int buttonPin = 11;
bool isButtonPressed = false;
unsigned long lastButtonPress = 0;

int a, b, c;
long volt, current, power, ah, wh;
unsigned long msec = 0;
float time = 0.0;
int sample = 0;
float totalCharge = 0.0;
float averageAmps = 0.0;
float ampSeconds = 0.0;
long ampHours = 0;

/*
  Pacman Intro Theme
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs

                                              Robson Couto, 2019
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
int tempo = 105;
int buzzer = 4;

int melody[] = {
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16,
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(buttonPin, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("GOOD DAY");

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }

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

void loop() {
  for (int i = 0; i < 200; i++) {
    c = analogRead(A1);
    a = analogRead(A2);
    volt = volt + a;
    current = current + c;
    delay(1);
  }
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

  controlRelay();
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
  controlRelay();
  delay(800);

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

  controlRelay();
}

void controlRelay() {
  if (current >= 30) {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay OFF - Current exceeded limit");
  } else {
    if (digitalRead(buttonPin) == HIGH && !isButtonPressed) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relay ON - Button pressed");
      isButtonPressed = true;
      tone(buzzer, 1000, 1000);
    } else if (isButtonPressed) {
      isButtonPressed = false;
    }
  }
}

void scrollText(const char* text) {
  int textLength = strlen(text);

  for (int i = 0; i < textLength - 16; ++i) {
    int startPos = i % textLength;
    lcd.clear();
    lcd.setCursor(0, 0);

    for (int j = 0; j < 16; ++j) {
      lcd.write(text[(startPos + j) % textLength]);
    }

    delay(400);
  }
}
