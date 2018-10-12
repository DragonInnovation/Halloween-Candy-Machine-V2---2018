/*

Todo

int LightShow_Interupt_Trigger_INPUT_INPUT_State = LOW; 

Interupts:  https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  
*/
#include "SPI.h"
#include "Adafruit_WS2801.h"


// Debug Verbose
bool Debug_Verbose = true;    // Uses serial monitor for debugging.  IF TRUE, WILL MESS UP RESET!  SERIAL BUFFER ISSUE.
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

byte CANDY1_COLOR[3] = {0, 255, 0};           // Green
const int CANDY2_COLOR[3] = {255, 0, 0};      // Red
const int CANDY3_COLOR[3] = {0,0, 255};       // Blue
const int CANDY4_COLOR[3] = {255, 255, 255};  // White
const int CANDY_REFRESH_COLOR[3] = {0, 0, 0}; // Black


int led_Matrix[6][4] = { 
 {5, 6, 17, 18},
 {4, 7, 16, 19},
 {3, 8, 15, 20},
 {2, 9, 14, 21},
 {1, 10, 13, 22},
 {0, 11, 12, 23}
};



int Cool_Colors [] = { 
255, 255, 0,
255, 0, 255,
0, 255, 255
};

const int WIPE_DELAY = 30 ;
const int WIPES = 5;

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


void Channel_Wipe_Down(uint8_t Candy_Selection, uint32_t c, uint8_t wait) {
  uint8_t x;
  for (x=0; x<6; x++) {
    
    strip.setPixelColor(led_Matrix[x][Candy_Selection], c); 
    strip.show();
    delay(wait);
  }
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

uint32_t Snake_Color = Color (200,10,50);

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


uint32_t c1 = Color (CANDY1_COLOR[0], CANDY1_COLOR[1], CANDY1_COLOR[2]);
uint32_t c2 = Color (CANDY2_COLOR[0], CANDY2_COLOR[1], CANDY2_COLOR[2]);
uint32_t c3 = Color (CANDY3_COLOR[0], CANDY3_COLOR[1], CANDY3_COLOR[2]);
uint32_t c4 = Color (CANDY4_COLOR[0], CANDY4_COLOR[1], CANDY4_COLOR[2]);
uint32_t cr = Color (CANDY_REFRESH_COLOR[0], CANDY_REFRESH_COLOR[1], CANDY_REFRESH_COLOR[2]);

//uint32_t c1 = Color (0,0,155);
// uint32_t c2 = Color (0,0,0);
uint32_t cc1 = Color (Cool_Colors[0], Cool_Colors[1], Cool_Colors[2]);
uint32_t cc2 = Color (Cool_Colors[3], Cool_Colors[4], Cool_Colors[5]);
  
if (InterBoard_1_State == LOW && InterBoard_2_State == HIGH && InterBoard_3_State == HIGH){   // Candy 1
// Candy_1 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 1 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(3,c1,WIPE_DELAY);    // Column 3 is Port in 0 - 3 numbering
   Channel_Wipe_Down(3,cr,WIPE_DELAY);
 }

 Pattern(0, c1, 50);
 
 /*
  Channel_Wipe_Down(1,cc1,25);
  Channel_Wipe_Down(2,cc1,25);
  Channel_Wipe_Down(3,cc1,25);
  Channel_Wipe_Down(4,cc1,25);
  Channel_Wipe_Down(4,cc2,25);
  Channel_Wipe_Down(3,cc2,25);
  Channel_Wipe_Down(2,cc2,25);
  Channel_Wipe_Down(1,cc2,25); */
 }

if (InterBoard_1_State == HIGH && InterBoard_2_State == LOW && InterBoard_3_State == HIGH){   // Candy 2
// Candy_2 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 2 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(2,c2,WIPE_DELAY);  
   Channel_Wipe_Down(2,cr,WIPE_DELAY);
 }

Pattern(17, c1, 75);

 
 /*
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(4,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(1,c4,25); */
 
}


if (InterBoard_1_State == HIGH && InterBoard_2_State == HIGH && InterBoard_3_State == LOW){   // Candy 3
// Candy_3 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 3 Triggered");  }

 for (int y = 1; y< WIPES; y++){
   Channel_Wipe_Down(1,c3,WIPE_DELAY);
   Channel_Wipe_Down(1,cr,WIPE_DELAY);
 }

  Pattern(5, c1, 50);
 /*
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(1,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(4,c4,25); */
 
}

if (InterBoard_1_State == LOW && InterBoard_2_State == LOW && InterBoard_3_State == HIGH){   // Candy 4
// Candy_4 Light Show

 if (Debug_Verbose) { Serial.println("LightShow 4 Triggered");  }

 for (int y = 1; y< WIPES; y++){
  Channel_Wipe_Down(0,c4,WIPE_DELAY);
  Channel_Wipe_Down(0,cr,WIPE_DELAY);
 }

  Pattern(8, c1, 50);
 /*
  Channel_Wipe_Down(4,c3,25);
  Channel_Wipe_Down(3,c3,25);
  Channel_Wipe_Down(2,c3,25);
  Channel_Wipe_Down(1,c3,25);
  Channel_Wipe_Down(1,c4,25);
  Channel_Wipe_Down(2,c4,25);
  Channel_Wipe_Down(3,c4,25);
  Channel_Wipe_Down(4,c4,25);*/
 
}

// Send Reset Signal Pulse indicating show's over

// Reset_InterBoard_OUTPUT_State = HIGH; // Send signal to pause further candy selections
// digitalWrite(Reset_InterBoard_OUTPUT, Reset_InterBoard_OUTPUT_State);
// delay (50);

Reset_InterBoard_OUTPUT_State = LOW; 
digitalWrite(Reset_InterBoard_OUTPUT, Reset_InterBoard_OUTPUT_State);
LightShow_Triggered = false;  // reset flag


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
if (Debug_Verbose){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello Stinky");
}

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
uint32_t  c_wipe_1 = Color (0, 255, 255); 
uint32_t  c_wipe_2 = Color (255, 0, 255); 
uint32_t  c_wipe_3 = Color (255, 255, 0); 

  LightShow_Candy(); 
   
  colorWipe(c_wipe_1,20);

  LightShow_Candy(); 
   
  colorWipe(c_wipe_2,20);

  LightShow_Candy(); 
   
  colorWipe(c_wipe_3,20);
  
}





  void Pattern(int Direction, uint32_t c, uint8_t wait) {
  uint8_t row;
  uint8_t col;

// Direction
// 0 - Snake Up from bottom left
// 1 - Snake Up from bottom right
// 2 - Snake Down from upper left
// 3 - Snake Down from upper right
// 4 - Snake Right from upper left
// 5 - Snake Left from upper right
// 6 - Snake Right from lower left
// 7 - Snake Left from lower right
// 8 - Rain down from upper left
// 9 - Rain down from upper right
// 10 - Rain down together
// 11 - Rain up together
// 12 - Swipe left from upper right
// 13 - Swipe right from upper left
// 14 - Swipe left together
// 15 - Swipe right together
// 16 - Rain up from bottom left
// 17 - Bounch
// 18 - All at once
// 19 - DrawX


if (Direction == 0){  // 0 - Snake Up from bottom left
           if (Debug_Verbose) { Serial.println("D=0");  }
  for (int col = 0; col < 4; col++) {
    
  if (col ==  0 || col == 2){ // Go Up
           if (Debug_Verbose) { Serial.println("Up");  }
   for (int row = 5; row > -1; row--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Down
         if (Debug_Verbose) { Serial.println("Down");  }
    for (int row = 0; row < 6; row++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }

  if (Direction == 1){ // 1 - Snake Up from bottom right
           if (Debug_Verbose) { Serial.println("D=1");  }
  for (int col = 3; col > -1; col--) {
    
  if (col ==  1 || col == 3){ // Go Up
           if (Debug_Verbose) { Serial.println("Up");  }
   for (int row = 5; row > -1; row--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Down
         if (Debug_Verbose) { Serial.println("Down");  }
    for (int row = 0; row < 6; row++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


  if (Direction == 2){ // 2 - Snake Down from upper left
           if (Debug_Verbose) { Serial.println("D=2");  }
  for (int col = 0; col < 4; col++) {
    
  if (col ==  1 || col == 3){ // Go Up
           if (Debug_Verbose) { Serial.println("Up");  }
   for (int row = 5; row > -1; row--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Down
         if (Debug_Verbose) { Serial.println("Down");  }
    for (int row = 0; row < 6; row++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


 if (Direction == 3){ // 3 - Down from upper right
           if (Debug_Verbose) { Serial.println("D=3");  }
  for (int col = 3; col > -1; col--) {
    
  if (col ==  0 || col == 2){ // Go Up
           if (Debug_Verbose) { Serial.println("Up");  }
   for (int row = 5; row > -1; row--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Down
         if (Debug_Verbose) { Serial.println("Down");  }
    for (int row = 0; row < 6; row++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }

  if (Direction == 4){ // 4 - Snake Right from upper left
           if (Debug_Verbose) { Serial.println("D=4");  }
  for (int row = 0; row < 6; row++) {
    
  if (row ==  0 || row == 2 || row == 4){ // Go Right
           if (Debug_Verbose) { Serial.println("Right");  }
   for (int col = 0; col < 4; col++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Left
         if (Debug_Verbose) { Serial.println("Left");  }
    for (int col = 3; col > -1; col--){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


if (Direction == 5){ // 5 - Snake Left from upper right
           if (Debug_Verbose) { Serial.println("D=5");  }
  for (int row = 0; row < 6; row++) {
    
  if (row ==  0 || row == 2 || row == 4){ // Go Left
           if (Debug_Verbose) { Serial.println("Left");  }
   for (int col = 3; col > -1; col--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Right
         if (Debug_Verbose) { Serial.println("Right");  }
    for (int col = 0; col < 4; col++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


  if (Direction == 6){ // 6 - Snake Right from lower left
           if (Debug_Verbose) { Serial.println("D=6");  }
  for (int row = 5; row > -1; row--) {
    
  if (row ==  0 || row == 2 || row == 4){ // Go Left
           if (Debug_Verbose) { Serial.println("Left");  }
   for (int col = 3; col > -1; col--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Right
         if (Debug_Verbose) { Serial.println("Right");  }
    for (int col = 0; col < 4; col++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


   if (Direction == 7){ // 7 - Snake Left from lower right
           if (Debug_Verbose) { Serial.println("D=7");  }
  for (int row = 5; row > -1; row--) {
    
  if (row ==  1 || row == 3 || row == 5){ // Go Left
           if (Debug_Verbose) { Serial.println("Left");  }
   for (int col = 3; col > -1; col--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

   else { // Go Right
         if (Debug_Verbose) { Serial.println("Right");  }
    for (int col = 0; col < 4; col++){

     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
    }  
   }
   
  }
  }


  if (Direction == 8){ // 8 - Rain down from upper left
           if (Debug_Verbose) { Serial.println("D=8");  }
  for (int col = 0; col < 4; col++) {

   for (int row = 0; row < 6; row++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }
  }


 if (Direction == 9){ // 9 - Rain down from upper right
           if (Debug_Verbose) { Serial.println("D=9");  }
  for (int col = 3; col > -1; col--) {
  
   for (int row = 0; row < 6; row++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

  }

 
 if (Direction == 10){   // 10 - Rain down together
           if (Debug_Verbose) { Serial.println("D=10");  }
  for (int row = 0; row < 6; row ++) {
  
   for (int col = 0; col < 4; col ++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 

    delay(wait);
      }
               strip.show();
  }

  }


   if (Direction == 11){   // 11 - Rain up together
           if (Debug_Verbose) { Serial.println("D=11");  }
  for (int row = 5; row > -1; row--) {
  
   for (int col = 0; col < 4; col ++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 

    delay(wait);
      }
               strip.show();
  }

  }



if (Direction == 12){   // 12 - Swipe left from upper right
           if (Debug_Verbose) { Serial.println("D=12");  }
  for (int row = 0; row < 6; row++) {
  
   for (int col = 3; col > -1; col--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

  }


if (Direction == 13){   // 13 - Swipe right from upper left
           if (Debug_Verbose) { Serial.println("D=13");  }
  for (int row = 0; row < 6; row++) {
  
   for (int col = 0; col < 4; col++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 
         strip.show();
    delay(wait);
      }
  }

  }

  

 if (Direction == 14){   // 14 - Swipe left together
           if (Debug_Verbose) { Serial.println("D=14");  }
  for (int col = 3; col > -1; col--) {
  
   for (int row = 0; row < 6; row++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 

    delay(wait);
      }
               strip.show();
  }

  }


   if (Direction == 15){   // 15 - Swipe right together
           if (Debug_Verbose) { Serial.println("D=15");  }
  for (int col = 0; col < 4; col++) {
  
   for (int row = 0; row < 6; row++){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
     strip.setPixelColor(led_Matrix[row][col], c); 

    delay(wait);
      }
               strip.show();
  }

  }
  


if (Direction == 16){ // 16 - Rain up from bottom left
             if (Debug_Verbose) { Serial.println("D=16");  }
  for (int col = 0; col < 4; col++) { 
    for (int row = 5; row > -1; row--){
     if (Debug_Verbose) { Serial.println(led_Matrix[row][col]);  }
    
    strip.setPixelColor(led_Matrix[row][col], c); 
    strip.show();
    delay(wait);
    }
  }
  }


if (Direction == 17){ // 17 - Bounce
  if (Debug_Verbose) { Serial.println("D-17 Bounce");  }
bounce(4, 6, wait);
}


if (Direction == 18){ // 18 - All at once
if (Debug_Verbose) { Serial.println("all");  }
  for (int row = 0; row < 6; row ++) {
  
   for (int col = 0; col < 4; col ++){
     strip.setPixelColor(led_Matrix[row][col], c); 
      }
      strip.show();
  } 


}

if (Direction == 19){ // 19 - Draw X
    if (Debug_Verbose) { Serial.println("D-18 DrawX");  }
drawX(4, 6, wait); 
}

  }

void drawX(uint8_t w, uint8_t h, uint8_t wait) {
  uint16_t x, y;
  for (x=0; x<w; x++) {
//    strip.setPixelColor(x, x, 255, 0, 0);
    strip.setPixelColor(led_Matrix[x][x], c);


    strip.show();
    delay(wait);
  }
  for (y=0; y<h; y++) {
//    strip.setPixelColor(w-1-y, y, 0, 0, 255);
   strip.setPixelColor(led_Matrix[w-1-y][y], c);
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
    strip.setPixelColor(led_Matrix[y][x], Wheel(j));

     strip.show();
     delay(wait);
     strip.setPixelColor(led_Matrix[y][x], 0, 0, 0);
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





