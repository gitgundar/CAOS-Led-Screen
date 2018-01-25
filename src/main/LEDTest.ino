/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */

 /*
//Pin connected to ST_CP of 74HC595
int latchPin = 11;
//Pin connected to SH_CP of 74HC595
int clockPin = 9;
////Pin connected to DS of 74HC595
int dataPin = 10;

int row1 = 5;
int row2 = 6;
int lowPin = 4; 

int rowPin = 7;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[10];
byte testArray[2];

// %%%%%%%%%%%%%%%%%% renamed so it works in the scope of the main programm
// %%%%%%%%%%%%%%%%%% call setupLEDTest() inside setup() from main.ino to execute it as usual
void setupLEDTest() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  digitalWrite(lowPin, LOW);
  Serial.begin(9600);

  //Binary notation as comment
  dataArray[0] = 0xFF; //0b11111111
  dataArray[1] = 0xFE; //0b11111110
  dataArray[2] = 0xFC; //0b11111100
  dataArray[3] = 0xF8; //0b11111000
  dataArray[4] = 0xF0; //0b11110000
  dataArray[5] = 0xE0; //0b11100000
  dataArray[6] = 0xC0; //0b11000000
  dataArray[7] = 0x80; //0b10000000
  dataArray[8] = 0x00; //0b00000000

  testArray[0] = 0xAA; //0b10101010
  testArray[1] = 0x55; //0b01010101
}

// %%%%%%%%%%%%%%%%%% renamed so it works in the scope of the main programm
// %%%%%%%%%%%%%%%%%% call loopLEDTest() inside loop() from main.ino to execute it as usual
void loopLEDTest() {

//  for (int j = 0; j < 9; j++) {
//    //load the light sequence you want from array
//    data = dataArray[j];
//    //ground latchPin and hold low for as long as you are transmitting
//    digitalWrite(latchPin, 0);
//    //move 'em out
//    shiftOut(dataPin, clockPin, data);
//    //return the latch pin high to signal chip that it 
//    //no longer needs to listen for information
//    digitalWrite(latchPin, 1);
//    delay(100);
//  }

    for (int i = 0; i < 2; i++) {
      data = testArray[i];

      digitalWrite(latchPin, 0);

      if (i == 0) {
        digitalWrite(row1, HIGH);
        digitalWrite(row2, LOW);
      } else {
        digitalWrite(row1, LOW);
        digitalWrite(row2, HIGH);
      }

      shiftOut(dataPin, clockPin, data);

      digitalWrite(latchPin, 1);

      delay(1);
    }
}



// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  pinMode(rowPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  digitalWrite(rowPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}*/
