//#include "notes.h"  //add note library

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330

//notes in the melody
int melody[]={NOTE_C4, NOTE_D4, NOTE_E4, 0, NOTE_C4, NOTE_D4, NOTE_E4, 0};

//digital pin 12 has a button attached to it. Give it an name
int buttonPin= 12;

//note durations. 4=quarter note / 8=eighth note
int noteDurations[]={4, 4, 4, 4, 4, 4, 4, 4};

int reading;
int prev = LOW;

void setup(){

  //make the button's pin input
  pinMode(buttonPin, INPUT);
}


void loop(){
  reading = digitalRead(buttonPin);
  if ((reading == HIGH) && (prev == LOW)) {
    //iterate over the notes of the melody
    for (int thisNote=0; thisNote <8; thisNote++){

      //to calculate the note duration, take one second. Divided by the note type
      int noteDuration = 1000 / noteDurations [thisNote];
      tone(8, melody [thisNote], noteDuration);

      //to distinguish the notes, set a minimum time between them
      //the note's duration +30% seems to work well
      int pauseBetweenNotes = noteDuration * 1;
      delay(pauseBetweenNotes);

      //stop the tone playing
      noTone(8);
    }
  }
  prev = reading;
}
