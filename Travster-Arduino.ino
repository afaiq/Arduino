#include <SPI.h>
#include <LiquidCrystal.h>
#include "dht.h"

#define DHT11_PIN 5
#define LCD_PIN 10
#define BUTTON_PIN 12
#define LED_PIN 9
#define BUZZER_PIN 8
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define KEYPAD_PIN A0

const int latchPin = 2;  // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin  = 3;  // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 4;  // Pin connected to Pin 11 of 74HC595 (Clock)

int thresholds[16] = {62, 42, 21, 0, 144, 127, 110, 92, 214, 200, 184, 170, 273, 260, 247, 236};
char keypad[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'}; 
int melody[] = {NOTE_C4, NOTE_D4, NOTE_E4, 0, NOTE_C4, NOTE_D4, NOTE_E4, 0};
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};

LiquidCrystal lcd(LCD_PIN);
dht DHT;
int reading;
int prev = LOW;
int counter = 0;
char data = 0;

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  int chk = DHT.read11(DHT11_PIN);
  lcd.begin(16, 2);
}
 
void loop() {
  lcd.setCursor(0, 0);
  lcd.print(millis()/1000);
  readButton();
  receiveData();
  readHum();
  showNumber(5);
  readKeypad();
}

void receiveData() {
  if (Serial.available() > 0) { // Send data only when you receive data:
    data = Serial.read();      //Read the incoming data and store it into variable data
    Serial.print(data);        //Print Value inside data in Serial monitor
    Serial.print("\n");        //New line 
    if (data == '1') {            //Checks whether value of data is equal to 1 
      lcd.setCursor(0, 1);
      lcd.print("Turned on");
      digitalWrite(LED_PIN, HIGH);  //If value is 1 then LED turns ON
    } else if (data == '0') {       //Checks whether value of data is equal to 0
      lcd.setCursor(0, 1);
      lcd.print("Turned off");
      digitalWrite(LED_PIN, LOW);   //If value is 0 then LED turns OFF
    }
  }
}

void readKeypad() {
  int value = analogRead(KEYPAD_PIN);
  for (int i = 0; i < 16; i++) {
    if (abs(value - thresholds[i]) < 5) {
      Serial.println(keypad[i]);
      while (analogRead(KEYPAD_PIN) < 1000) {
        delay(100);
      }
    }
  }
}

void readButton() {
  reading = digitalRead(BUTTON_PIN);
  if ((reading == HIGH) && (prev == LOW)) {
    playNotes();
  }
  prev = reading;
}

void playNotes() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}

void readHum() {
  lcd.setCursor(0, 1);
  lcd.print("Hum = ");
  lcd.print(DHT.humidity);
  lcd.print(" %");
}

void showNumber(int num) {
  digitalWrite(latchPin, LOW);
  switch (num) {
    case 0 :
      shiftOut(dataPin, clockPin, MSBFIRST, 63);
      break;
    case 1 :
      shiftOut(dataPin, clockPin, MSBFIRST, 6);
      break;
    case 2 :
      shiftOut(dataPin, clockPin, MSBFIRST, 91);
      break;
    case 3 :
      shiftOut(dataPin, clockPin, MSBFIRST, 79);
      break;
    case 4 :
      shiftOut(dataPin, clockPin, MSBFIRST, 102);
      break;
    case 5 :
      shiftOut(dataPin, clockPin, MSBFIRST, 109);
      break;
    case 6 :
      shiftOut(dataPin, clockPin, MSBFIRST, 125);
      break;
    case 7 :
      shiftOut(dataPin, clockPin, MSBFIRST, 7);
      break;
    case 8 :
      shiftOut(dataPin, clockPin, MSBFIRST, 127);
      break;
    case 9 :
      shiftOut(dataPin, clockPin, MSBFIRST, 111);
      break;
    default: 
      shiftOut(dataPin, clockPin, MSBFIRST, 63);
      break;
  }
  digitalWrite(latchPin, HIGH);
}
