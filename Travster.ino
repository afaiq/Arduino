#include <LiquidCrystal.h>
#include <SPI.h>
#include "dht.h"

#define DHT11_PIN 10
#define BUTTON_PIN 12

const int latchPin = 5;  // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin  = 6;  // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 7;  // Pin connected to Pin 11 of 74HC595 (Clock)

LiquidCrystal lcd(9);
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
  pinMode(4, OUTPUT);        //Sets digital pin 13 as output pin
  int chk = DHT.read11(DHT11_PIN);
  lcd.begin(16, 2);
  showNumber(counter);
}

void loop() {
//  Read Button
  reading = digitalRead(BUTTON_PIN);
  if ((reading == HIGH) && (prev == LOW)) {
    counter++;
    showNumber(counter);
  }
  prev = reading;

  if (Serial.available() > 0) { // Send data only when you receive data:
    data = Serial.read();      //Read the incoming data and store it into variable data
    Serial.print(data);        //Print Value inside data in Serial monitor
    Serial.print("\n");        //New line 
    if (data == '1') {            //Checks whether value of data is equal to 1 
      lcd.setCursor(0, 1);
      lcd.print("Turned on");
      digitalWrite(4, HIGH);  //If value is 1 then LED turns ON
    } else if (data == '0') {       //Checks whether value of data is equal to 0
      lcd.setCursor(0, 1);
      lcd.print("Turned off");
      digitalWrite(4, LOW);   //If value is 0 then LED turns OFF
    }
  }

  lcd.setCursor(0, 0);
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
