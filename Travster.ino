#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "dht.h"

#define DHT11_PIN 8
#define BUTTON_PIN 12

const int latchPin = 5;  // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin  = 6;  // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 7;  // Pin connected to Pin 11 of 74HC595 (Clock)

LiquidCrystal lcd(9);
dht DHT;

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int reading;
int prev = LOW;
int counter = 0;
//SoftwareSerial BTSerial(4, 5);

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
//  String setName = String("AT+NAME=My Arduino\r\n"); //Setting name as 'MyBTBee'
//  BTSerial.begin(38400);
//  BTSerial.print("AT\r\n"); //Check Status
//  delay(500);
//  while (BTSerial.available()) {
//    Serial.write(BTSerial.read());
//  }
//  BTSerial.print(setName); //Send Command to change the name
//  delay(500);
//  while (BTSerial.available()) {
//    Serial.write(BTSerial.read());
//  }
  pinMode(BUTTON_PIN, INPUT);
  int chk = DHT.read11(DHT11_PIN);
  lcd.begin(16, 2);
}

void loop() {
//  char c;
//  if (Serial.available()) {
//    c = Serial.read();
//    Serial.print(c);
//  }
//  if (c == 't') {
//    readSensor();
//  }

  // Read Button
  reading = digitalRead(BUTTON_PIN);
  if ((reading == HIGH) && (prev == LOW)) {
    Serial.print("Button pressed!!!");
//    counter++;
//    lcd.setCursor(0, 1);
//    lcd.print("Counter = ");
//    lcd.print(millis()/1000);
//    lcd.print(counter);
//    Serial.print("Counter = ");
//    Serial.print(counter);
  }
  prev = reading;

  // Show Digit
  for (int i = 0; i < 10; i++) {
    showNumber(i);
    delay(1000);
  }
  
  showSensor();
//  lcd.setCursor(0,1);
//  lcd.print(millis()/1000);
}

void showSensor() {
  float hum = DHT.humidity;
  lcd.setCursor(0, 0);
  lcd.print("Hum = ");
  lcd.print(hum);
  lcd.print(" %");
  delay(10);
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
