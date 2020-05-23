/* ====================================================================
 * Copyright (c) 2019 Martin D. Waller - G0PJO.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by Martin D. Waller - G0PJO"
 *
 * 4. The names "D70Box" must not be used to endorse or promote 
 *    products derived from this software without prior written permission.
 *
 * 5. Products derived from this software may not be called "D70Box"
 *    nor may "D70Box" appear in their names without prior written
 *    permission of Martin D. Waller.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *
 *    "This product includes software developed by Martin D. Waller - G0PJO"
 *
 * THIS SOFTWARE IS PROVIDED BY Martin D. Waller ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

//    Created:	  M.D.Waller		May 16th, 2019
//                a) Initial coding.
//
//    Modified:   M.D.Waller    May 23rd, 2019
//                a) Add support for Bluetooth.
//
//    Modified:   M.D.Waller    May 23rd, 2020
//                a) Bug fix, it was not producing 'Z' or
//                '9' in the random list!
//
// TODO
//
// -
//
// The DATONG D70 Morse Tutor first appeared in 1980, almost 40 years
// ago. It was described by DATONG as:
//
// "The revolutionary new way to practice Morse Code reception. An 
// unlimited supply of precision Morse at the turn of a switch, plus 
// built-in oscillator for sending practice."
//
// They were not cheap at the time but sold well and even today can 
// demand £30+ on eBay.
//
// DATONG
//
// Font: MicrogrammeD-BoldExte
//
// How long is standard dit?
//
// The relationship between Dits and Dahs is well documented.
//
// 1 Dah = 3 Dits
// The gap between Dits and Dahs is the duration of 1 Dit
// The gap between letters is the duration of 3 Dits
// The gap between words is the duration of 7 Dits

// Standard for CW speed is based around the word PARIS. If you sum up the
// duration of all the letters, associated gaps, and trailing word gap then
// it would be equivalent to 50 Dits.
//
// If you were to send the word PARIS 5 times then that would be equivalant
// to 250 Dits. Assuming a sending speed of 5 Word Per Minutes then:
//
// 1 Minutes = 60 Seconds
// 60 Seconds / 250 Dits = 0.24 Seconds (250ms) Per Dit
//
// The Delay and Speed potentiometers are linear but the legend on
// the D70 is anything but! To replicate the device as closely as
// possible the actual values are generated via polynomials fitted
// to the data taken from the actual value returned from the A to D
// reading and the legend value. These polynomials were generated
// using a tool called CurveExpert from https://www.curveexpert.net.
//
// Bluetooth
//
// There are many Bluetooth devices available and they are very confusing!
//
// Command Set
//
// HC-05
//
//  AT+BAUD4      - Set the baud rate to 9600
//  AT+NAMED70    - Set the device name to D70
//  AT+REST       - Reset the device

#include <Arduino.h>
#include "Entropy.h"

// Debug manifests

//define DEBUG_DISPLAY_POT_DATA
//define DEBUG_LETTERS_MIXED_NUMBERS
//define DEBUG_DIT_LENGTH_MS

// Bluetooth Manifests

#define BUILD_WITH_BLUETOOTH 1
#define BLUETOOTH_BAUDRATE 9600

// Version manifests

#define PROGRAM_NAME "D70BOX"
#define PROGRAM_VERSION_MAJOR 1
#define PROGRAM_VERSION_MINOR 0

// Hardware related manifests

#define SPEAKER_PIN 5       // Pin 5
#define SPEAKER_ON_PIN 7    // Pin 7
#define LETTERS_PIN 8       // Pin 8
#define NUMBERS_PIN 9       // Pin 9
#define SPEED_PIN 18        // Pin A0
#define DELAY_PIN 19        // Pin A1

#define GROUPS_PER_LINE 5
#define GROUPS_BETWEEN_VERSION 50

#define ANALOGUE_HIGH 1023

// D70 related manifests

#define D70_GROUP_LENGTH 5
#define D70_TONE_FREQUENCY 630 

// This enumeration is used to indicate the state
// of the Letters | Mixed | Numbers switch

enum SwitchStates {
  LETTERS,
  MIXED,
  NUMBERS        
};

// Morse related manifests

#define DITPDAH 3
#define DITBLET 3
#define DITBWRD 7

// The following manifests contains all the Morse characters that we
// will be sending. The top 3 bits of each byte contains the length
// of Morse character - the number of dits and dahs. The lower
// 5 bits contain the code, a 1 represents a dah and a 0 represents
// a dit.

#define MORSE_CODE_A 0b01001000 // A ._      5 dits
#define MORSE_CODE_B 0b10010000 // B _...    9 dits
#define MORSE_CODE_C 0b10010100 // C _._.   11 dits
#define MORSE_CODE_D 0b01110000 // D _..     7 dits
#define MORSE_CODE_E 0b00100000 // E .       1 dits
#define MORSE_CODE_F 0b10000100 // F .._.    9 dits
#define MORSE_CODE_G 0b01111000 // G __.     9 dits
#define MORSE_CODE_H 0b10000000 // H ....    7 dits
#define MORSE_CODE_I 0b01000000 // I ..      3 dits
#define MORSE_CODE_J 0b10001110 // J .___   13 dits
#define MORSE_CODE_K 0b01110100 // K _._     9 dits
#define MORSE_CODE_L 0b10001000 // L ._..    9 dits
#define MORSE_CODE_M 0b01011000 // M __      7 dits
#define MORSE_CODE_N 0b01010000 // N _.      5 dits
#define MORSE_CODE_O 0b01111100 // O ___    11 dits
#define MORSE_CODE_P 0b10001100 // P .__.   11 dits
#define MORSE_CODE_Q 0b10011010 // Q __._   13 dits
#define MORSE_CODE_R 0b01101000 // R ._.     7 dits
#define MORSE_CODE_S 0b01100000 // S ...     5 dits
#define MORSE_CODE_T 0b00110000 // T _       3 dits
#define MORSE_CODE_U 0b01100100 // U .._     7 dits
#define MORSE_CODE_V 0b10000010 // V ..._    9 dits
#define MORSE_CODE_W 0b01101100 // W .__     9 dits
#define MORSE_CODE_X 0b10010010 // X _.._   11 dits
#define MORSE_CODE_Y 0b10010110 // Y _.__   13 dits
#define MORSE_CODE_Z 0b10011000 // Z __..   11 dits
#define MORSE_CODE_0 0b10111111 // 0 _____  19 dits
#define MORSE_CODE_1 0b10101111 // 1 .____  17 dits
#define MORSE_CODE_2 0b10100111 // 2 ..___  15 dits
#define MORSE_CODE_3 0b10100011 // 3 ...__  13 dits
#define MORSE_CODE_4 0b10100001 // 4 ...._  11 dits
#define MORSE_CODE_5 0b10100000 // 5 .....   9 dits
#define MORSE_CODE_6 0b10110000 // 6 _....  11 dits
#define MORSE_CODE_7 0b10111000 // 7 __...  13 dits
#define MORSE_CODE_8 0b10111100 // 8 ___..  15 dits
#define MORSE_CODE_9 0b10111110 // 9 ____.  17 dits

// The following array contains all the characters that we may need.

char morseCodeAscii[] = {
    'A', // ._    
    'B', // _...  
    'C', // _._.  
    'D', // _..   
    'E', // .     
    'F', // .._.  
    'G', // __.   
    'H', // ....  
    'I', // ..    
    'J', // .___  
    'K', // _._   
    'L', // ._..  
    'M', // __    
    'N', // _.    
    'O', // ___   
    'P', // .__.  
    'Q', // __._  
    'R', // ._.   
    'S', // ...   
    'T', // _     
    'U', // .._   
    'V', // ..._  
    'W', // .__   
    'X', // _.._  
    'Y', // _.__  
    'Z', // __..  
    '0', // _____ 
    '1', // .____ 
    '2', // ..___ 
    '3', // ...__ 
    '4', // ...._ 
    '5', // ..... 
    '6', // _.... 
    '7', // __... 
    '8', // ___.. 
    '9'  // ____. 
};

byte morseCodeCharacters[] = {
    MORSE_CODE_A, // [ 0] A ._    
    MORSE_CODE_B, // [ 1] B _...  
    MORSE_CODE_C, // [ 2] C _._.  
    MORSE_CODE_D, // [ 3] D _..   
    MORSE_CODE_E, // [ 4] E .     
    MORSE_CODE_F, // [ 5] F .._.  
    MORSE_CODE_G, // [ 6] G __.   
    MORSE_CODE_H, // [ 7] H ....  
    MORSE_CODE_I, // [ 8] I ..    
    MORSE_CODE_J, // [ 9] J .___  
    MORSE_CODE_K, // [10] K _._   
    MORSE_CODE_L, // [11] L ._..  
    MORSE_CODE_M, // [12] M __    
    MORSE_CODE_N, // [13] N _.    
    MORSE_CODE_O, // [14] O ___   
    MORSE_CODE_P, // [15] P .__.  
    MORSE_CODE_Q, // [16] Q __._  
    MORSE_CODE_R, // [17] R ._.   
    MORSE_CODE_S, // [18] S ...   
    MORSE_CODE_T, // [19] T _     
    MORSE_CODE_U, // [20] U .._   
    MORSE_CODE_V, // [21] V ..._  
    MORSE_CODE_W, // [22] W .__   
    MORSE_CODE_X, // [23] X _.._  
    MORSE_CODE_Y, // [24] Y _.__  
    MORSE_CODE_Z, // [25] Z __..  
    MORSE_CODE_0, // [26] 0 _____ 
    MORSE_CODE_1, // [27] 1 .____ 
    MORSE_CODE_2, // [28] 2 ..___ 
    MORSE_CODE_3, // [29] 3 ...__ 
    MORSE_CODE_4, // [30] 4 ...._ 
    MORSE_CODE_5, // [31] 5 ..... 
    MORSE_CODE_6, // [32] 6 _.... 
    MORSE_CODE_7, // [33] 7 __... 
    MORSE_CODE_8, // [34] 8 ___.. 
    MORSE_CODE_9  // [35] 9 ____. 
};

// PARIS
//
//	0b10001100, // [15] P .__.   11 dits
//	0b01001000, // [ 0] A ._      5 dits
//	0b01101000, // [17] R ._.     7 dits
//	0b01000000, // [ 8] I ..      3 dits
//	0b01100000, // [18] S ...     5 dits
//
// Total bits = 11 + 3 +
//               5 + 3 +
//               7 + 3 +
//               3 + 3 +
//               5 + 7
//            = 50
//
// So PARIS repeated 5 times would be 5 * 50 = 250.
    
// The following manifests are used to extract the length
// and associated code bits.

#define LENGTH_MASK 0b11100000
#define LENGTH_MASK_SHIFT 5
#define LENGTH_OF_CODE(X) (((X) & LENGTH_MASK) >> LENGTH_MASK_SHIFT)
#define CODE_MASK 0b00011111

/*
 * This method is called to evaluate a polynomial. It is passed
 * the X value, the degree of the polynomial, and an array of
 * coefficients. The coefficient array starts with the constant
 * followed by the X, X^2, X^3, ... coefficients.
 */
float evalPolynomial(float x, int degree, float *coefficients)
{
  // Start off with the constant
  
  float retVal = *coefficients++;

  // Set V to the X value
  
  float v = x;
  for(int i = 0; i < degree; i++)
  {
    // Add in the coefficient times the value
    
    retVal += *coefficients++ * v;

    // Turn X into X^2, etc.
    v = v * x;
  }

  // Return the result
  
  return retVal;
}

/*
 * This method is called to read the delay control. It will return
 * the required number of seconds delay.
 */
float readDelay()
{
  int potValue = analogRead(DELAY_PIN);

  // Convert value to a speed in Delay in seconds
  
  float coefficients[] = {3.486453, -3.50352e-03, 4.90175e-08};

  float delaySeconds = evalPolynomial(potValue,2,&coefficients[0]);
  if (delaySeconds < 0)
    delaySeconds = 0;

#ifdef DEBUG_DISPLAY_POT_DATA
  Serial.println("readDelay()");

  Serial.print("potValue = ");
  Serial.println(potValue);
    
  Serial.print("Delay Seconds = ");
  Serial.println(delaySeconds);
#endif
        
  return delaySeconds;
}

/*
 * This method is called to read the speed control and return the
 * currently selected words per minute.
 */
float readSpeed()
{
  // Read the potentiometer value
  
  int potValue = analogRead(SPEED_PIN);

  // Convert value to a speed in WPM

  float coefficients[] = {6.58477440249751, 8.026122191756975e-03, -2.064085150344916e-5, 4.181177226081519e-8};

  float speedWPM = evalPolynomial(potValue,3,&coefficients[0]);

#ifdef DEBUG_DISPLAY_POT_DATA
  Serial.println("readSpeed()");

  Serial.print("potValue = ");
  Serial.println(potValue);
    
  Serial.print("Speed WPM = ");
  Serial.println(speedWPM);
#endif
  
  return (speedWPM);
}

/*
 * This method is called to convert a number of Words Per Minute
 * to the number of milliseconds that represents ond dit at that
 * given speed.
 */
float ditLengthMS(float wordsPerMinute)
{
  // Calculate the dit length in MS for the given speed

  float retVal = (60.0 / (wordsPerMinute * 50.0)) * 1000.0;
  
#ifdef DEBUG_DIT_LENGTH_MS
  Serial.println("ditLengthMS()");
  Serial.print("Dit length = ");
  Serial.println(retVal);
#endif

  return retVal;
}

/*
 * This method is called to send out the given Morse code
 * character using the given number of milliseconds for each
 * dit.
 */
void sendCharacter(byte character, int ditLengthMS)
{
  byte characterLength = LENGTH_OF_CODE(character);

  byte codeMask = (CODE_MASK + 1) >> 1;
  for(int i = 0; i < characterLength; i++)
  {
    int toneOnDuration = ditLengthMS;
    if ((character & codeMask) > 0)
    {
      // We're sending a Dah, update the tone on duration
        
      toneOnDuration *= DITPDAH;
    }

    // Turn the tone on, wait, turn it off.

    // Send the Dit or Dah
    
    digitalWrite(SPEAKER_ON_PIN,HIGH);
    delay(toneOnDuration);
    digitalWrite(SPEAKER_ON_PIN,LOW);

    // Delay for the inter dit/dah gap but only if
    // we need it

    if (i != characterLength - 1)
      delay(ditLengthMS);      

    // Move the code mask along one
      
    codeMask >>= 1;
  }
}

/*
 * This method is called to read the state of the Letters |
 * Mixed | Numbers switch. It will return the associated
 * enumeration value.
 */
SwitchStates readLettersMixedNumbers()
{
  SwitchStates retVal = MIXED;
  
  if (LOW == digitalRead(LETTERS_PIN))
    retVal = LETTERS;
  else if (LOW == digitalRead(NUMBERS_PIN))
    retVal = NUMBERS;

#ifdef DEBUG_LETTERS_MIXED_NUMBERS
  Serial.println("readLettersMixedNumbers()");
  Serial.print("retVal = ");
  switch(retVal) 
  {
    case LETTERS:
      Serial.println("LETTERS");
      break;
    case MIXED:
      Serial.println("MIXED");
      break;
    case NUMBERS:
      Serial.println("NUMBERS");
      break;
    default:
      Serial.println("?");
  }
#endif
  
  return retVal;
}
  
void setup() {

  // Setup the serial link for debugging
  
  Serial.begin(9600);

#ifdef BUILD_WITH_BLUETOOTH   
  Serial1.begin(BLUETOOTH_BAUDRATE);
#endif
  
  // put your setup code here, to run once:

  Entropy.initialize();

  randomSeed(Entropy.random());

  // Initialize pins

  pinMode(LETTERS_PIN,INPUT_PULLUP);
  pinMode(NUMBERS_PIN,INPUT_PULLUP);
  pinMode(SPEED_PIN,INPUT);
  pinMode(DELAY_PIN,INPUT);
  pinMode(SPEAKER_PIN,OUTPUT);
  pinMode(SPEAKER_ON_PIN,OUTPUT);

  // Turn off the SPEAK_ON_PIN

  digitalWrite(SPEAKER_ON_PIN,LOW);
  
  // Turn on the tone
      
  tone(SPEAKER_PIN,D70_TONE_FREQUENCY);   

  if (readSpeed() > 30) 
  {
    // Announce the version details

    // D70BOX VERSION V

    byte _space = -1;

    // Build the text to send
    
    int textCode[] = {
      MORSE_CODE_D,
      MORSE_CODE_7,
      MORSE_CODE_0,
      MORSE_CODE_B,
      MORSE_CODE_O,
      MORSE_CODE_X,
      _space,
      MORSE_CODE_V
    };

    // Get the dit length for 12WPM
    float ditLength12WPM = ditLengthMS(12);

    // Send the text
    
    for(int i = 0; i < 8; i++)
    {
      int characterToSend = textCode[i];

      if (_space == characterToSend)
      {
        delay(DITBWRD * ditLength12WPM);
      }
      else
      {
        sendCharacter((byte)characterToSend,ditLength12WPM);
        delay(DITBLET * ditLength12WPM);
      }
    }

    // Add in the version number
    
    sendCharacter(morseCodeCharacters[26 + PROGRAM_VERSION_MAJOR],ditLength12WPM);
    delay(DITBLET * ditLength12WPM);
    sendCharacter(morseCodeCharacters[26 + PROGRAM_VERSION_MINOR],ditLength12WPM);

    // Final delay
    
    delay(DITBWRD * ditLength12WPM * 2);
  }

#ifdef BUILD_WITH_BLUETOOTH 

    // Wait for the serial device
    
    while(!Serial1) { ; }
    
    // Set the name of the device and read any reply
      
    Serial1.write("AT+NAME");
    Serial1.write(PROGRAM_NAME);
    Serial1.write("\r\n");
    delay(500);
    while(Serial1.available())
      Serial.write(Serial1.read());  

    // Reset the device. This should allow contacts to
    // the name given above.

    Serial1.write("AT+RESET\r\n");
    delay(500);
    while(Serial1.available())
      Serial.write(Serial1.read());     
#endif 
}

void buildDateTime(char *buffer) { 
  
    char month[4];
    int day;
    int year;
    
    // Pull the date apart
    
    sscanf(__DATE__, "%s %d %d", month, &day, &year);

    // Reformat it
    
    sprintf(buffer, "%02d-%s-%04d %s",day,month,year,__TIME__);
}

/*
 * This is the main loop. Here we simply need to send out a group of 
 * random characters with the correct spacing and the correct word
 * spacing at the end. The Arduino base loop will take care of the
 * rest.
 */
 
int groupNumber = 0;
int totalGroups = 0;

void loop() {

  // We need to produce a group of random characters but we need to look
  // out for the operating mode changing.
  
  int currentDitLengthMS;
  
  // In the loop we are simply going to send out a well formed group of five characters
  // depending on the character groups selected, the delay, and the speed.

#ifdef BUILD_WITH_BLUETOOTH 

  if((totalGroups % GROUPS_BETWEEN_VERSION) == 0)
  {
    char lineBuffer[30];
    
    Serial1.println();
    sprintf(lineBuffer,"%s V%d.%d",PROGRAM_NAME,PROGRAM_VERSION_MAJOR,PROGRAM_VERSION_MINOR);
    Serial1.println(lineBuffer);
    Serial1.println("Copyright (C) 2019 M.D.Waller G0PJO");
    Serial1.println("Email: Martin@The-Wallers.Net");
        
    buildDateTime(lineBuffer);
    
    Serial1.print("Built: ");
    Serial1.println(lineBuffer);

    Serial1.println();

    Serial1.print("Output: ");
    SwitchStates lettersMixedNumbers = readLettersMixedNumbers();
    switch(lettersMixedNumbers)
    {
      case LETTERS:
        Serial1.println("LETTERS");
        break;
      case MIXED:
        Serial1.println("MIXED");
        break;
      case NUMBERS:
        Serial1.println("NUMBERS");
        break;
    }

    Serial1.print("Delay: ");
    Serial1.println(readDelay());

    Serial1.print("Speed: ");
    Serial1.print(readSpeed());
    Serial1.println("WPM");

    Serial1.println();
  }
  
#endif

  totalGroups++;
  
  for(int i = 0; i < D70_GROUP_LENGTH; i++)
  {
    // Get the Letters/Mixed/Numbers state

    SwitchStates lettersMixedNumbers = readLettersMixedNumbers();

    // Sort out the random character range

    int minCharacter = 0;
    int maxCharacter = 36;

    switch(lettersMixedNumbers)
    {
      case LETTERS:
        maxCharacter = 26;
        break;
      case NUMBERS:
        minCharacter = 26;
        break;
    }
    
    // Pull out the random character

    int randomIndex = Entropy.random(minCharacter, maxCharacter);
    char asciiCharracterToSend = morseCodeAscii[randomIndex];
    byte characterToSend = morseCodeCharacters[randomIndex];

    // Display the character

#ifdef BUILD_WITH_BLUETOOTH     
    Serial1.print(char(asciiCharracterToSend));
#endif
    
    // Read the current speed value
    
    float currentSpeed = readSpeed();

    // Workout the associated dit length in milliseconds
    
    currentDitLengthMS = (int)ditLengthMS(currentSpeed);
    
    // Send out the character with the given dit length
    
    sendCharacter(characterToSend,currentDitLengthMS);

    // Put in the inter letter gap if we are not at the end
    // of the group.

    if (i != (D70_GROUP_LENGTH - 1))
      delay(DITBLET * currentDitLengthMS);

    // Add any extra delay that might be required

    delay(readDelay() * 1000);
  }

#ifdef BUILD_WITH_BLUETOOTH 

  // Update the group number and space / line break as required

  groupNumber++;
  if (groupNumber > GROUPS_PER_LINE - 1) 
  {
    Serial1.println();
    groupNumber = 0;
  }
  else
    Serial1.print(" ");
    
#endif    

  // Put in the word break after ths group

  delay(DITBWRD * currentDitLengthMS);

  // Add any extra delay that might be required

  delay(readDelay() * 1000);  
}
