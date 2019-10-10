//  Vewlix B7 Handler
//  ARTHRIMUS, 2019
//  Configured for use with ATTINY87/167

#include <EEPROM.h>

int P1B4I = 2; // Pin numbers for button inputs.
int P1B7I = 3;
int P2B4I = 13;
int P2B7I = 12;

int P1B4O = 16; // Port numbers for button outputs.
int P1B7O = 32;
int P2B4O = 8;
int P2B7O = 4;

int P1B4V = 0; // Variables for storing button states.
int P2B4V = 0;
int P1B7V = 0;
int P2B7V = 0;

int p1buttonmap = 0;
int p2buttonmap = 0;
int p1combocount = 0;
int p2combocount = 0;
int combodelay = 30000;

int p1combo = 0;
int p2combo = 0;

int outputa = 0;
int outputb = 0;

int eepromval1 = 0;
int eepromval2 = 0;

int val = 0; // variable for reading the pin status (should be zero)

void setup() {
  pinMode(P1B4I, INPUT_PULLUP);     // Declares inputs with internal pullup.
  pinMode(P1B7I, INPUT_PULLUP);
  pinMode(P2B4I, INPUT_PULLUP);
  pinMode(P2B7I, INPUT_PULLUP);

  eepromval1 = EEPROM.read(0);
  eepromval2 = EEPROM.read(1);
  p1combo = eepromval1;
  p2combo = eepromval2;

}
void loop() {
  {
    scaninputs();                    // Scans button inputs and records them to variables.
    buttoncombos();                  // Checks for button combinations and stets values.
    setoutputs();                    // Sets output values for port manipulation based on button input variables.
  }
  DDRA = outputa;                  // Sets PORTA outputs based on inputs
  DDRB = outputb;
}

void scaninputs() {              // Scans input pins for buttonpresses.

  outputa = 0;
  outputb = 0;

  P1B4V = 0;
  P2B4V = 0;
  P1B7V = 0;
  P2B7V = 0;


  val = digitalRead(P1B4I);        // read input value from button
  if (val == LOW) {                // check if the button input is LOW (button pressed)
    P1B4V = 1;                       // store value of button state
  }
  val = digitalRead(P2B4I);
  if (val == LOW) {
    P2B4V = 1;
  }
  val = digitalRead(P1B7I);
  if (val == LOW) {
    P1B7V = 1;
  }
  val = digitalRead(P2B7I);
  if (val == LOW) {
    P2B7V = 1;
  }
}

void buttoncombos() {
  if (P1B4V == 1 && (P1B7V == 1)) {                           // Checks if p1 B4 and B7 are pressed
    p1combocount = (p1combocount + 1);                        // iterates P1 combocounter by +1
  } else {
    p1combocount = 0;
  }

  if (P2B4V == 1 && (P2B7V == 1)) {
    p2combocount = (p2combocount + 1);
  } else {
    p2combocount = 0;
  }

  if (p1combocount >= combodelay && (p1combo < 2)) {         //If p1 combocount is greater than or equal to the combodelay value and P1 combo is less than 2
    p1combo = (p1combo + 1);                                 //p1 combo is iterated by +1
    p1combocount = 0;                                        //Resets combocount to 0
    EEPROM.write(0, p1combo);                                //Writes the new combo value to the EEPROM
    }else if (p1combocount >= combodelay && (p1combo >= 2)){ //If p1 combo count is greater than or equal to combodelay value and p1combo is greater than or equal to 2
    p1combo = 0;                                             //p1combo resets to 0
    p1combocount = 0;
    EEPROM.write(0, p1combo);
  }
  if (p2combocount >= combodelay && (p2combo < 2)) {
    p2combo = (p2combo + 1);
    p2combocount = 0;
    EEPROM.write(1, p2combo);
    }else if (p2combocount >= combodelay && (p2combo >= 2)){
    p2combo = 0;
    p2combocount = 0;
    EEPROM.write(1, p2combo);
  }
  }

void setoutputs() {                // Translates button values into port outputs

  if (P1B4V == 1 && (p1combo == 0))
    outputa |= P1B4O;
  if (P1B7V == 1 && (p1combo == 0))
    outputa |= P1B7O;
  if (P1B7V == 1 && (p1combo == 1))
    outputa |= P1B4O;
  if (P1B4V == 1 && (p1combo == 2))
    outputa |= P1B4O;
  if (P1B7V == 1 && (p1combo == 2))
    outputa |= P1B4O;

  if (P2B4V == 1 && (p2combo == 0))
    outputb |= P2B4O;
  if (P2B7V == 1 && (p2combo == 0))
    outputb |= P2B7O;
  if (P2B7V == 1 && (p2combo == 1))
    outputb |= P2B4O;
  if (P2B4V == 1 && (p2combo == 2))
    outputb |= P2B4O;
  if (P2B7V == 1 && (p2combo == 2))
    outputb |= P2B4O;
}
