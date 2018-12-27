/*
 Debounce

 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).

 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground

 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.


 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */
#include <IRremote.h>

IRsend irsend;

// constants won't change. They're used here to
// set pin numbers:
const int btn7Pin = 7;    // the number of the pushbutton pin
const int btn8Pin = 8;

const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin

int btn7State;             // the current reading from the input pin
int btn8State;

int lastBtn7State = LOW;   // the previous reading from the input pin
int lastBtn8State = LOW;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime7 = 0;  // the last time the output pin was toggled
long lastDebounceTime8 = 0;
long debounceDelay7 = 50;    // the debounce time; increase if the output flickers
long debounceDelay8 = 50;

void setup() {
  pinMode(btn7Pin, INPUT);
  pinMode(btn8Pin, INPUT);
  pinMode(ledPin, OUTPUT);
Serial.begin(9600);
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading7 = digitalRead(btn7Pin);
  int reading8 = digitalRead(btn8Pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading7 != lastBtn7State) {
    // reset the debouncing timer
    lastDebounceTime7 = millis();
  }

  if ((millis() - lastDebounceTime7) > debounceDelay7) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading7 != btn7State) {
      btn7State = reading7;

      // only toggle the LED if the new button state is HIGH
      if (btn7State == HIGH) {
        Serial.println("Pressed btn 7");
        irsend.sendNEC(0x20DF10EF, 32); // TV power code
      delay(500);
        irsend.sendNEC(0x20DFD02F, 32); // av/tv
        delay(500);
        irsend.sendNEC(0x41000FF, 32); //volia power on/off
        ledState = !ledState;
      }
    }
  }
  
  if (reading8 != lastBtn8State) {
    // reset the debouncing timer
    lastDebounceTime8 = millis();
  }

  if ((millis() - lastDebounceTime8) > debounceDelay8) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading8 != btn8State) {
      btn8State = reading8;

      // only toggle the LED if the new button state is HIGH
      if (btn8State == HIGH) {
        Serial.println("Pressed btn 8");
      irsend.sendNEC(0x41020DF, 32); // volia mute
      delay(500);
     // irsend.sendNEC(0x41000FF, 32);
     // delay(40);
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastBtn7State = reading7;
   lastBtn8State = reading8;
}

