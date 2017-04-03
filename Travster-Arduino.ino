#include <SPI.h>
#include <LiquidCrystal.h>
//#include <SerialCommand.h>
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
char keypad[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'}; 
int melody[] = {NOTE_E4, NOTE_E4, 0, 0, NOTE_E4, NOTE_E4, 0, 0};
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};

//SerialCommand sCmd;
LiquidCrystal lcd(LCD_PIN);
dht DHT;
int reading;
int prev = LOW;
int counter = 0;
char data = 0;

void setup() {
  Serial.begin(9600);
//  while (!Serial);
//  sCmd.addCommand("ECHO", echoHandler);
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
  if (Serial.available() > 0) { // Send data only when you receive data:
//    sCmd.readSerial();
    receiveData();
  }
  readHum();
  showNumber(8);
  readKeypad();
}

void receiveData() {
  data = Serial.read();      //Read the incoming data and store it into variable data
  Serial.print(data);        //Print Value inside data in Serial monitor
  Serial.print("\n");        //New line 
  if (data == '1') {            //Checks whether value of data is equal to 1 
    ledOn();
//      digitalWrite(LED_PIN, HIGH);  //If value is 1 then LED turns ON
  } else if (data == '0') {       //Checks whether value of data is equal to 0
    ledOff();
//      digitalWrite(LED_PIN, LOW);   //If value is 0 then LED turns OFF
  }
}

void ledOn() {
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(LED_PIN, fadeValue);
    delay(30);
  }
}

void ledOff() {
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(LED_PIN, fadeValue);
    delay(30);
  }
}

void readKeypad() {
  int value = analogRead(KEYPAD_PIN);
  for (int i = 0; i < 16; i++) {
    if (abs(value - thresholds[i]) < 5) {
      Serial.println(keypad[i]);
      Serial.write(keypad[i]);
      Serial.flush();
      delay(20);
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

//void echoHandler() {
//  char *arg;
//  arg = sCmd.next();
//  if (arg != NULL) {  
//    Serial.print("Received argument = ");
//    Serial.println(arg);
//    sCmd.addCommand("ECHO", echoHandler);
//  } else {
//    Serial.println("Nothing to echo");
//  }
//}
