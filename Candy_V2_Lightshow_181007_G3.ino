/*

Todo

int LightShow_Interupt_Trigger_INPUT_INPUT_State = LOW; 

Interupts:  https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  
*/
#include "SPI.h"
#include "Adafruit_WS2801.h"


// Debug Verbose
bool Debug_Verbose = true;
bool TEST_LEDS = false;
bool SINGLE_PROCESSOR = false;

uint8_t dataPin  = 13;    // Yellow wire on Adafruit Pixels // Physical Pin 26
uint8_t clockPin = 24;    // Green wire on Adafruit Pixels .  // Physical Pin 20

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels in a row and
// the second value to number of pixels in a column.
Adafruit_WS2801 strip = Adafruit_WS2801(24, dataPin, clockPin);


// Define Interrupt Pins for Buttons
const int LightShow_Interupt_Trigger_INPUT = A3;    // Physical Pin 9 

// number of the i/o pints
const int Reset_InterBoard_OUTPUT = 19;  // Physical Pin 47 
const int InterBoard_1 = 14;    // Physical Pin 3 (A0)
const int InterBoard_2 = 15;    // Physical Pin 7 (A1)
const int InterBoard_3 = 16;    // Physical Pin 8 (A2)

int Reset_InterBoard_OUTPUT_State = LOW; 
int InterBoard_1_State = HIGH; 
int InterBoard_2_State = HIGH; 
int InterBoard_3_State = HIGH; 

uint32_t c;  // Used for default color wiping

bool LightShow_Triggered = false;

int CandyLEDS [24] = {
18, 19, 20, 21, 22, 23,  // C1 Top to Bottom
17, 16, 15, 14, 13, 12,  // C2
6, 7, 8, 9, 10, 11,      // C3
5, 4, 3, 2, 1, 0,        // C4
};

int Cool_Colors [] = { // Not currently used
85, 255, 0,
255, 0, 170,
0, 0, 153,
255, 102, 102,
255, 51, 255,
102, 0, 0,
255,51, 255,
0, 230, 230,
0, 153, 0,
0, 0, 153
};

const int WIPE_DELAY = 50 ;
const int WIPES = 4;

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}



void LightShow_Candy(){

  if (TEST_LEDS == true){
    LightShow_Triggered = true;
  }


while (LightShow_Triggered == true){

Reset_InterBoard_OUTPUT_State = HIGH; // Send signal to pause further candy selections
digitalWrite(Reset_InterBoard_OUTPUT, Reset_InterBoard_OUTPUT_State);

if (SINGLE_PROCESSOR == true){  // Test using Candy_1 selection
InterBoard_1_State = LOW;
InterBoard_2_State = HIGH;
InterBoard_3_State = HIGH;
}

else {

InterBoard_1_State = digitalRead(InterBoard_1);
InterBoard_2_State = digitalRead(InterBoard_2);
InterBoard_3_State = digitalRead(InterBoard_3);
}

int random_number = random(10);

// uint32_t c1 = Color (Cool_Colors[3*random_number], Cool_Colors[3*random_number+1], Cool_Colors[3*random_number+2]);
uint32_t c1 = Color (0,0,155);
uint32_t c2 = Color (0,0,0);
uint32_t c3 = Color (0,0,255);
uint32_t c4 = Color (255,85,0);
  
if (InterBoard_1_State == LOW && InterBoard_2_State == HIGH && InterBoard_3_State == HIGH){   // Candy 1
// Candy_1 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 1 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(1,c1,WIPE_DELAY);
   Channel_Wipe_Down(1,c2,WIPE_DELAY);
 }
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(4,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(1,c4,25);
 }

if (InterBoard_1_State == HIGH && InterBoard_2_State == LOW && InterBoard_3_State == HIGH){   // Candy 2
// Candy_2 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 2 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(2,c1,WIPE_DELAY);
   Channel_Wipe_Down(2,c2,WIPE_DELAY);
 }
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(4,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(1,c4,25);
 
}


if (InterBoard_1_State == HIGH && InterBoard_2_State == HIGH && InterBoard_3_State == LOW){   // Candy 3
// Candy_3 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 3 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(3,c1,WIPE_DELAY);
   Channel_Wipe_Down(3,c2,WIPE_DELAY);
 }
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(1,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(4,c4,25);
 
}

if (InterBoard_1_State == LOW && InterBoard_2_State == LOW && InterBoard_3_State == HIGH){   // Candy 4
// Candy_4 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 4 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(4,c1,WIPE_DELAY);
   Channel_Wipe_Down(4,c2,WIPE_DELAY);
 }
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(1,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(4,c4,25);
 
}

// Send Reset Signal
Reset_InterBoard_OUTPUT_State = LOW; 
digitalWrite(Reset_InterBoard_OUTPUT, Reset_InterBoard_OUTPUT_State);
LightShow_Triggered = false;


 if (Debug_Verbose) {
 Serial.println("Light Show Function"); 
 Serial.println("Ouput State / Pin; LightShow Triggered State; Input State Pin");
 Serial.println(Reset_InterBoard_OUTPUT_State);   
 Serial.println(digitalRead(Reset_InterBoard_OUTPUT));   
 Serial.println(LightShow_Triggered);   
 Serial.println(digitalRead(LightShow_Interupt_Trigger_INPUT));   

 }
  
}
}


void setup() {
    
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

// Initial Color Setup Routine
  uint32_t c = Color (0,0,88);
  colorWipe(c,50);

  pinMode(LightShow_Interupt_Trigger_INPUT, INPUT);
  pinMode(InterBoard_1, INPUT);
  pinMode(InterBoard_2, INPUT);
  pinMode(InterBoard_3, INPUT);

  pinMode(Reset_InterBoard_OUTPUT, OUTPUT);

  // set Reset_InterBoard_OUTPUT to Low
  digitalWrite(Reset_InterBoard_OUTPUT, Reset_InterBoard_OUTPUT_State);

  
// Start Serial to terminal for debugging
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello Stinky");

 if (Debug_Verbose) {
 Serial.println("Setup"); 
 Serial.println("Ouput State / Pin; LightShow Triggered State; Input State Pin");
 Serial.println(Reset_InterBoard_OUTPUT_State);   
 Serial.println(digitalRead(Reset_InterBoard_OUTPUT));   
 Serial.println(LightShow_Triggered);   
 Serial.println(digitalRead(LightShow_Interupt_Trigger_INPUT));   

 }
  

if (TEST_LEDS == true){
    Serial.println("Test LED Mode");
}

if (SINGLE_PROCESSOR == true){
    Serial.println("Single Processor Mode");
}

  // Setup Interrupts
 pinMode(LightShow_Interupt_Trigger_INPUT, INPUT_PULLUP);  // Can't Use A0 and A5 at the same time.
 attachInterrupt(LightShow_Interupt_Trigger_INPUT, LightShow_Interupt, RISING);  

}



void LightShow_Interupt() {
 if (Debug_Verbose) { Serial.println("LightShow Interupt Triggered");  }

  LightShow_Triggered = true;

}


void loop() {

  LightShow_Candy(); 
  
  c = Color (255, 85, 0);
  colorWipe(c,20);

  LightShow_Candy(); 
  
  c = Color (0,102, 51);
  colorWipe(c,20);


  
  // Some example procedures showing how to display to the pixels
 // drawX(7, 7, 100);
 // bounce(7, 6, 50);
}




void Channel_Wipe_Down(uint8_t Candy_Selection, uint32_t c, uint8_t wait) {
  uint8_t x;
  for (x=0; x<6; x++) {
    
    strip.setPixelColor(CandyLEDS[x+(Candy_Selection-1)*6], c);
    strip.show();
    delay(wait);
  }
  }


void drawX(uint8_t w, uint8_t h, uint8_t wait) {
  uint16_t x, y;
  for (x=0; x<w; x++) {
    strip.setPixelColor(x, x, 255, 0, 0);
    strip.show();
    delay(wait);
  }
  for (y=0; y<h; y++) {
    strip.setPixelColor(w-1-y, y, 0, 0, 255);
    strip.show();
    delay(wait);
  }

}

void bounce(uint8_t w, uint8_t h, uint8_t wait) {
  int16_t x = 1;
  int16_t y = 2;
  int8_t xdir = +1;
  int8_t ydir = -1;
  int j;
  for (j=0; j < 256; j++) {
     x = x + xdir;
     y = y + ydir;
     if (x < 0) {
       x = -x;
       xdir = - xdir;
     }
     if (y < 0) {
       y = -y;
       ydir = - ydir;
     }
     if (x == w) {
       x = w-2;
       xdir = - xdir;
     }
     if (y == h) {
       y = h-2;
       ydir = - ydir;
     }
     strip.setPixelColor(x, y, Wheel(j));
     strip.show();
     delay(wait);
     strip.setPixelColor(x, y, 0, 0, 0);
  }
}

/* Helper functions */


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

/* Remove - handled elsewhere
void Read_InterBoard() {
  if (Debug_Verbose) { Serial.println("Reading InterBoard");  } 

InterBoard_1_State = digitalRead(InterBoard_1);
InterBoard_2_State = digitalRead(InterBoard_2);
InterBoard_3_State = digitalRead(InterBoard_3);

} */





