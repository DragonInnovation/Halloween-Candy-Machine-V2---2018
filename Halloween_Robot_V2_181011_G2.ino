/*
  [Add Scott stuff here with links to onshape and DSB]
  Adafruit
  Bantam Machines
  OnShape

Setup Instructions
Need to have Candy sticks at 90 degrees to start.

Todo
* Add:  Look for launch button (interrupt) and exit main loop
* Add in solonoid and worm stepper on new motor shield


Examples
AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
}

void loop() {
  Serial.println("Single coil steps");
  myMotor->step(100, FORWARD, SINGLE); 
  myMotor->step(100, BACKWARD, SINGLE); 


  http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

Solonoid
https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
https://forums.adafruit.com/viewtopic.php?f=31&t=115668&sid=0ae7acbb773e159e8b5191f15f78cd60
  
  
*/

// Debug Verbose
bool Debug_Verbose = false;  // Uses serial monitor for debugging.  IF TRUE, WILL MESS UP RESET!  SERIAL BUFFER ISSUE.
bool OPEN_LOOP = false;  // Use for a single processor too

// Machine Geometry
const int STEPS = 100;  // 180 degree rotation for candy pusher (Total steps 200 per revolution)
const int CANDY_SPEED = 500;  // Speed of candy dispenser in ?? RPM
const int INTERUPT_PULSE = 50 ; // Pulse width for interupt
const int LIGHT_SHOW_DURATION = 250;  // Provide time for the light show to run.

// Uncomment for Launch
// const int LAUNCH_SPEED = 500;  // Speed of catapult reset stepper
// const int BUTTON_DELAY = 500;  // Delay to give lighting show time to finish.  Need to figure out experimentally.

/* Uncomment for Launch
// Internal Counters
int Candy_1_Dispensed = 0;
int Candy_2_Dispensed = 0;
int Candy_3_Dispensed = 0;
int Candy_4_Dispensed = 0;
// const int MAX_CANDY = 4;    // Max number of candies that can be loaded in the catapult
*/

// Animation Settings for Candy Button LEDS
const int NUM_FLASHES = 8;
const int MS_FLASH = 100;
const int MS_OFF = 50;
const int OPEN_LOOP_DELAY = 3000;  // 3 secs - enough time for light show to complete // Remove with associated stuff routines on timing (not OL)

// Uncomment for Launch
// const int NUM_LAUNCH_FLASHES = 20;
// const int MS_LAUNCH_FLASH = 50;
// const int MS_LAUNCH_OFF = 25;

// I/O Definitions

// Define Interrupt Pins for Buttons
const int CandyButton_1 = A0;    // Physical Pin 3 
const int CandyButton_2 = A1;    // Physical Pin 7 
const int CandyButton_3 = A2;    // Physical Pin 8 
const int CandyButton_4 = A3;    // Physical Pin 9  
const int Reset_InterBoard_INPUT = A4; // Physical Pin 10 

// Uncomment for Launch
// const int LaunchButton  = ??;    // Physical Pin ?? - watch interference with Reset_InterBoard_INPUT_INPUT pin!!!
// const int WormSwitch    = ??;    // Physical Pin ??

// pin for trigger output flag
const int LightShow_Interupt_Trigger_OUTPUT = 12;    // Physical Pin 28

// the number of the Candy LED pins
const int CandyLED_1 = 11;     // Physical Pin 25
const int CandyLED_2 = 10;     // Physical Pin 27  
const int CandyLED_3 = 6;      // Physical Pin 29
const int CandyLED_4 = 5;      // Physical Pin 24 


// Uncomment for Launch
// const int LaunchLED = 0;       // Physical Pin 16      


// number of the i/o pins
const int InterBoard_1 = 19;     // Physical Pin 47
const int InterBoard_2 = 0;      // Physical Pin 16
const int InterBoard_3 = 1;      // Physical Pin 15

// Current state of the LEDs.  Turn on the Candy LEDS and off the Launch LED at Start
int CandyLED_1_State = HIGH;         
int CandyLED_2_State = HIGH;
int CandyLED_3_State = HIGH;
int CandyLED_4_State = HIGH;

// Uncomment for Launch
// int LaunchLED_State =  LOW;

int InterBoard_1_State = HIGH; 
int InterBoard_2_State = HIGH; 
int InterBoard_3_State = HIGH; 

bool LightShow_Active = false;

int LightShow_Interupt_Trigger_OUTPUT_State = LOW;

// Buttons Initial State
bool Candy_Button_1_Untriggered = true;
bool Candy_Button_2_Untriggered = true;
bool Candy_Button_3_Untriggered = true;
bool Candy_Button_4_Untriggered = true;
// Uncomment for Launch
// bool Launch_Button_Triggered = false;

// Stepper Motor Setup
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

// Uncomment for Launch
// Adafruit_MotorShield AFMS3(0x62);  // Top
Adafruit_MotorShield AFMS2(0x61);  // Middle
Adafruit_MotorShield AFMS1(0x60);  // Bottom:  Default address, no jumpers

// Connect two candy steppers with 200 steps per revolution (1.8 degree) to shield 1
Adafruit_StepperMotor *myStepper1 = AFMS1.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMS1.getStepper(200, 2);  // This is the motor I have wired

// Connect two candy steppers with 200 steps per revolution (1.8 degree) to shield 2
Adafruit_StepperMotor *myStepper3 = AFMS2.getStepper(200, 1);
Adafruit_StepperMotor *myStepper4 = AFMS2.getStepper(200, 2);

// Uncomment for Launch
// Connect one launch steppers with 200 steps per revolution (1.8 degree) to shield 3
// Adafruit_StepperMotor *myStepper5 = AFMS3.getStepper(200, 1);

// Can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// Wrappers for the motors!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, DOUBLE);  //
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, DOUBLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  myStepper2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, DOUBLE);
}
// wrappers for the third motor!
void forwardstep3() {  
  myStepper3->onestep(FORWARD, DOUBLE);
}
void backwardstep3() {  
  myStepper3->onestep(BACKWARD, DOUBLE);
}
// wrappers for the fourth motor!
void forwardstep4() {  
  myStepper4->onestep(FORWARD, DOUBLE);
}
void backwardstep4() {  
  myStepper4->onestep(BACKWARD, DOUBLE);
}

/* . Uncomment for Launch
// wrappers for the fifth motor!
void forwardstep5() {  
  myStepper5->onestep(FORWARD, DOUBLE);
}
void backwardstep5() {  
  myStepper5->onestep(BACKWARD, DOUBLE);
}
*/

// Now we'll wrap the steppers in an AccelStepper object
AccelStepper Candy_Stepper_1(forwardstep1, backwardstep1);
AccelStepper Candy_Stepper_2(forwardstep2, backwardstep2);
AccelStepper Candy_Stepper_3(forwardstep3, backwardstep3);
AccelStepper Candy_Stepper_4(forwardstep4, backwardstep4);
// Uncomment for Launch
//AccelStepper Launch_Stepper(forwardstep5, backwardstep5);


void Dispense_Candy_1(){
// Uncomment for Launch
// while (!Candy_Button_1_Untriggered && Candy_1_Dispensed == 0){

if (OPEN_LOOP){
    LightShow_Active = false;
}

while (!Candy_Button_1_Untriggered && !LightShow_Active){ // LSA trigger in interrupt from light processor

// Trigger the light show first (it may take a bit for it to start due to where it is in its loop)
LightShow_Interupt_Trigger_OUTPUT_State = HIGH; 
InterBoard_1_State = LOW; 
InterBoard_2_State = HIGH; 
InterBoard_3_State = HIGH; 

digitalWrite(InterBoard_1, InterBoard_1_State); 
digitalWrite(InterBoard_2, InterBoard_2_State);  
digitalWrite(InterBoard_3, InterBoard_3_State);  
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State); // Send a pulse on the interupt to trigger the light show

  
// Flash the button lights
       for(int i = 0; i < NUM_FLASHES; i++){
      //  Button Pressed Flash Pattern
      digitalWrite(CandyLED_1, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(MS_FLASH);              // wait for a second
      digitalWrite(CandyLED_1, HIGH);    // turn the LED off by making the voltage LOW
      delay(MS_OFF);              // wait for a second
      }  // for Button Flash Loop
 
      // Uncomment for Launch
     //  digitalWrite(CandyLED_1, LOW);   // turn off the LED to indicate this candy can't be picked again

 // delay (INTERUPT_PULSE);  //Remove? Was to deal with inter-processor signal passing.

 delay (LIGHT_SHOW_DURATION);  // Give lightshow time to run before dispensing the candy.

// Move the stepper to dispense the candy
for (int j = 0; j < STEPS ; j++) {
Candy_Stepper_1.runSpeed();
}
// Uncomment for Launch
// Candy_1_Dispensed = 1; // UNCOMMENT

// Reset the light show
LightShow_Interupt_Trigger_OUTPUT_State = LOW;   // Reset (triggers on rising, so won't affect timing)
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);

// Reset the Button
Candy_Button_1_Untriggered = true;                                                     
// LightShow_Active = true;  // Use for closed loop
// [testing - handled in reset interupt] LightShow_Active = false;  // Use for open loop - need to have delay for light show duration

 if (Debug_Verbose) {
 Serial.println("Dispensing Candy 1"); 
 Serial.println("IB1,2,3, Trigger Output State, Trigger Pin, LS Active Flag, LS Input Pin");
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 Serial.println(digitalRead(LightShow_Interupt_Trigger_OUTPUT));     
 Serial.println(LightShow_Active);
 Serial.println(digitalRead(Reset_InterBoard_INPUT));
 }


} 
}

void Dispense_Candy_2(){
// Uncomment for Launch
// while (!Candy_Button_2_Untriggered && Candy_2_Dispensed == 0){

if (OPEN_LOOP){
    LightShow_Active = false;
}

while (!Candy_Button_2_Untriggered && !LightShow_Active){

// Trigger the light show first (it may take a bit for it to acknowledge due to where it is in its cycle)
LightShow_Interupt_Trigger_OUTPUT_State = HIGH; 
InterBoard_1_State = HIGH; 
InterBoard_2_State = LOW; 
InterBoard_3_State = HIGH; 

digitalWrite(InterBoard_1, InterBoard_1_State); 
digitalWrite(InterBoard_2, InterBoard_2_State);  
digitalWrite(InterBoard_3, InterBoard_3_State); 
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State); 
  

// Flash the lights
       for(int i = 0; i < NUM_FLASHES; i++){
      //  Button Pressed Flash Pattern
      digitalWrite(CandyLED_2, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(MS_FLASH);              // wait for a second
      digitalWrite(CandyLED_2, HIGH);    // turn the LED off by making the voltage LOW
      delay(MS_OFF);              // wait for a second
      }  // for Button Flash Loop
      
            // Uncomment for Launch
    //  digitalWrite(CandyLED_2, LOW);   // turn off the LED to indicate this candy can't be picked again

 delay (LIGHT_SHOW_DURATION);  // Open Loop - Give lightshow time to run.  In future, could trigger on signal.
    

// Move the stepper
for (int j = 0; j < STEPS ; j++) {
Candy_Stepper_2.runSpeed();
}

// Uncomment for Launch
// Candy_2_Dispensed = 1;  // UNCOMMENT

  if (Debug_Verbose) { Serial.println("Final State State Candy 2");  
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
  }

// Reset the light show
LightShow_Interupt_Trigger_OUTPUT_State = LOW;   // Reset (triggers on rising, so won't affect timing)
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);
  
Candy_Button_2_Untriggered = true; 
// LightShow_Active = true;  // Use for closed loop
// LightShow_Active = false;  // Use for open loop - need to have delay for light show duration


} 
}

void Dispense_Candy_3(){
// Uncomment for Launch
// while (!Candy_Button_3_Untriggered && Candy_3_Dispensed == 0){

if (OPEN_LOOP){
    LightShow_Active = false;
}

while (!Candy_Button_3_Untriggered && !LightShow_Active){

// Trigger the light show first (it may take a bit for it to acknowledge due to where it is in its cycle)
LightShow_Interupt_Trigger_OUTPUT_State = HIGH; 
InterBoard_1_State = HIGH; 
InterBoard_2_State = HIGH; 
InterBoard_3_State = LOW; 


digitalWrite(InterBoard_1, InterBoard_1_State); 
digitalWrite(InterBoard_2, InterBoard_2_State);  
digitalWrite(InterBoard_3, InterBoard_3_State); 
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State); 

  
// Flash the lights
       for(int i = 0; i < NUM_FLASHES; i++){
      //  Button Pressed Flash Pattern
      digitalWrite(CandyLED_3, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(MS_FLASH);              // wait for a second
      digitalWrite(CandyLED_3, HIGH);    // turn the LED off by making the voltage LOW
      delay(MS_OFF);              // wait for a second
      }  // for Button Flash Loop

            // Uncomment for Launch
    //  digitalWrite(CandyLED_3, LOW);   // turn off the LED to indicate this candy can't be picked again


     delay (LIGHT_SHOW_DURATION);  // Open Loop - Give lightshow time to run.  In future, could trigger on signal.

// Move the stepper
for (int j = 0; j < STEPS ; j++) {
Candy_Stepper_3.runSpeed();
}

// Uncomment for Launch
// Candy_3_Dispensed = 1;  // REMOVE

LightShow_Interupt_Trigger_OUTPUT_State = HIGH; 
InterBoard_1_State = HIGH; 
InterBoard_2_State = HIGH; 
InterBoard_3_State = LOW; 

digitalWrite(InterBoard_1, InterBoard_1_State); 
digitalWrite(InterBoard_2, InterBoard_2_State);  
digitalWrite(InterBoard_3, InterBoard_3_State);  
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State); 


 if (Debug_Verbose) { Serial.println("Final State State Candy 3");  
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 }

// Reset the light show
LightShow_Interupt_Trigger_OUTPUT_State = LOW;   // Reset (triggers on rising, so won't affect timing)
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);

Candy_Button_3_Untriggered = true;

// LightShow_Active = true;  // Use for closed loop
// LightShow_Active = false;  // Use for open loop - need to have delay for light show duration


} 
}

void Dispense_Candy_4(){

// Uncomment for Launch
// while (!Candy_Button_4_Untriggered && Candy_4_Dispensed == 0){

if (OPEN_LOOP){
    LightShow_Active = false;
}

while (!Candy_Button_4_Untriggered && !LightShow_Active){


LightShow_Interupt_Trigger_OUTPUT_State = HIGH; 
InterBoard_1_State = LOW; 
InterBoard_2_State = LOW; 
InterBoard_3_State = HIGH; 

digitalWrite(InterBoard_1, InterBoard_1_State); 
digitalWrite(InterBoard_2, InterBoard_2_State);  
digitalWrite(InterBoard_3, InterBoard_3_State); 
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State); 
  
// Flash the lights
       for(int i = 0; i < NUM_FLASHES; i++){
      //  Button Pressed Flash Pattern
      digitalWrite(CandyLED_4, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(MS_FLASH);              // wait for a second
      digitalWrite(CandyLED_4, HIGH);    // turn the LED off by making the voltage LOW
      delay(MS_OFF);              // wait for a second
      }  // for Button Flash Loop

            // Uncomment for Launch
   //   digitalWrite(CandyLED_4, LOW);   // turn off the LED to indicate this candy can't be picked again


    delay (LIGHT_SHOW_DURATION);  // Open Loop - Give lightshow time to run.  In future, could trigger on signal.

// Move the stepper
for (int j = 0; j < STEPS ; j++) {
Candy_Stepper_4.runSpeed();
}

// Uncomment for Launch
// Candy_4_Dispensed = 1;  // REMOVE

 


 if (Debug_Verbose) { Serial.println("Final State State Candy 4");  
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 }

 // Reset the light show
LightShow_Interupt_Trigger_OUTPUT_State = LOW;   // Reset (triggers on rising, so won't affect timing)
digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);

Candy_Button_4_Untriggered = true; // REMOVE

// LightShow_Active = true;  // Use for closed loop
//LightShow_Active = false;  // Use for open loop - need to have delay for light show duration


} 
}

/* // Uncomment for Launch
void Launch_Candy(){

// Flash the lights
       for(int i = 0; i < NUM_LAUNCH_FLASHES; i++){
      //  Button Pressed Flash Pattern
      digitalWrite(LaunchLED, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(MS_LAUNCH_FLASH);              // wait for a second
      digitalWrite(LaunchLED, HIGH);    // turn the LED off by making the voltage LOW
      delay(MS_LAUNCH_OFF);              // wait for a second
      }  // for Button Flash Loop
      digitalWrite(LaunchLED, LOW);   // turn off the LED to indicate this candy can't be picked again
    if (Debug_Verbose) { Serial.println("Completed Launch Function");  }
// Release the Worm Solonoid

}
*/



void Reset_Robot(){

/* // Uncomment for Launch
// Internal Counters
Candy_1_Dispensed = 0;
Candy_2_Dispensed = 0;
Candy_3_Dispensed = 0;
Candy_4_Dispensed = 0;

*/

// Current state of the Candy LED.  Turn on the LEDS at Start
CandyLED_1_State = HIGH;         
CandyLED_2_State = HIGH;
CandyLED_3_State = HIGH;
CandyLED_4_State = HIGH;

// Uncomment for Launch
// LaunchLED_State = LOW;

// reset initial LED state - Turn on button LEDs to solid
digitalWrite(CandyLED_1, CandyLED_1_State);
digitalWrite(CandyLED_2, CandyLED_2_State);
digitalWrite(CandyLED_3, CandyLED_3_State);
digitalWrite(CandyLED_4, CandyLED_4_State);

// Uncomment for Launch
// digitalWrite(LaunchLED, LaunchLED_State);


// Candy Buttons Initial State
Candy_Button_1_Untriggered = true;
Candy_Button_2_Untriggered = true;
Candy_Button_3_Untriggered = true;
Candy_Button_4_Untriggered = true;

// Uncomment for Launch
// Launch_Button_Triggered = false;

/* Uncomment for Launch
// Add reseting worm by moving motor until switch triggers
// Move the stepper
for (int j = 0; j < 3*STEPS ; j++) { // Place holder - have interrupt driven
Launch_Stepper.runSpeed();
}
  */  // Uncomment for Launch
}


void setup() {

 // Setup Candy Buttons LEDS
  pinMode(CandyLED_1, OUTPUT);
  pinMode(CandyLED_2, OUTPUT);
  pinMode(CandyLED_3, OUTPUT);
  pinMode(CandyLED_4, OUTPUT);


  pinMode(InterBoard_1, OUTPUT);
  pinMode(InterBoard_2, OUTPUT);
  pinMode(InterBoard_3, OUTPUT);
  
  pinMode(LightShow_Interupt_Trigger_OUTPUT, OUTPUT);

  // set initial LED state - Turn on button LEDs to solid
  digitalWrite(CandyLED_1, CandyLED_1_State);
  digitalWrite(CandyLED_2, CandyLED_2_State);
  digitalWrite(CandyLED_3, CandyLED_3_State);
  digitalWrite(CandyLED_4, CandyLED_4_State);


  digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);
  digitalWrite(InterBoard_1, InterBoard_1_State);
  digitalWrite(InterBoard_2, InterBoard_2_State);
  digitalWrite(InterBoard_3, InterBoard_3_State);


  // Start Steppers
  AFMS1.begin();
  AFMS2.begin();

  // Uncomment for Launch
 // AFMS3.begin();

Candy_Stepper_1.setSpeed(CANDY_SPEED);
Candy_Stepper_2.setSpeed(CANDY_SPEED);
Candy_Stepper_3.setSpeed(CANDY_SPEED);
Candy_Stepper_4.setSpeed(CANDY_SPEED);

// Uncomment for Launch
// Launch_Stepper.setSpeed(LAUNCH_SPEED);

  
// Start Serial to terminal for debugging
if (Debug_Verbose){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello Poopy");
   if (OPEN_LOOP){Serial.println("Open Loop Mode");}
}


  // Setup Interrupts
  pinMode(CandyButton_1, INPUT_PULLUP);  // Can't Use A0 and A5 at the same time.
  pinMode(CandyButton_2, INPUT_PULLUP);
  pinMode(CandyButton_3, INPUT_PULLUP);
  pinMode(CandyButton_4, INPUT_PULLUP);
  pinMode(Reset_InterBoard_INPUT, INPUT_PULLUP);

  // Uncomment for Launch
  // pinMode(LaunchButton, INPUT_PULLUP);


 attachInterrupt(CandyButton_1, Dispense_Candy_Interupt_1, RISING);   //RISING - On Release
 attachInterrupt(CandyButton_2, Dispense_Candy_Interupt_2, RISING);
 attachInterrupt(CandyButton_3, Dispense_Candy_Interupt_3, RISING);
 attachInterrupt(CandyButton_4, Dispense_Candy_Interupt_4, RISING);
 attachInterrupt(Reset_InterBoard_INPUT, Reset_InterBoard_INPUT_Interupt, RISING);  // FALLING = On Release - Remove

 // Uncomment for Launch
 // attachInterrupt(LaunchButton, Launch, RISING);

  // Release and Reload Worm Function
  // Start Initial Light Show Function


}


void loop() { 

// Uncomment for Launch
// while ((Candy_1_Dispensed + Candy_2_Dispensed + Candy_3_Dispensed + Candy_4_Dispensed <= MAX_CANDY) && !Launch_Button_Triggered) {

  Dispense_Candy_1(); // Need to make this fast to avoid delays on acting when button is pressed.
  Dispense_Candy_2();
  Dispense_Candy_3();
  Dispense_Candy_4();

/* . // Uncomment for Launch
  if((Candy_1_Dispensed || Candy_2_Dispensed || Candy_3_Dispensed || Candy_4_Dispensed)){
    LaunchLED_State= HIGH;
      digitalWrite(LaunchLED, LaunchLED_State);
  } . */

// Uncomment for Launch
// }
//  if (Debug_Verbose) { Serial.println("Launch Triggers in Main Loop");  }
// Uncomment for Launch
// Launch_Candy();
// Reset_Robot();
}


void Dispense_Candy_Interupt_1() {
  if (Debug_Verbose) { Serial.println("Triggered Interupt 1");  } 


  
// Uncomment for Launch
//  if (Candy_1_Dispensed == 0)
// {

if (Candy_Button_1_Untriggered && Candy_Button_2_Untriggered && Candy_Button_3_Untriggered && Candy_Button_4_Untriggered){ // Prevent multiple presses
 if (Debug_Verbose) { Serial.println("Activating Candy 4");  }

Candy_Button_1_Untriggered = false;
}
// LightShow_Active = true;  // remove - doesn't make sense


 if (Debug_Verbose) { Serial.println("Triggered Interupt 1");  
 Serial.println("IB1,2,3, Trigger Output State, Trigger Pin, LS Active Flag, LS Input Pin");
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 Serial.println(digitalRead(LightShow_Interupt_Trigger_OUTPUT));     
 Serial.println(LightShow_Active);
 Serial.println(digitalRead(Reset_InterBoard_INPUT));
 }


//  }
}


void Dispense_Candy_Interupt_2() {
  if (Debug_Verbose) { Serial.println("Triggered Interupt 2");  }
 // Uncomment for Launch
 // if (Candy_2_Dispensed == 0)
 // {

 if (Candy_Button_1_Untriggered && Candy_Button_2_Untriggered && Candy_Button_3_Untriggered && Candy_Button_4_Untriggered){ // Prevent multiple presses
 if (Debug_Verbose) { Serial.println("Activating Candy 4");  }
 
   Candy_Button_2_Untriggered = false;
 }
//  }
}


void Dispense_Candy_Interupt_3() {
  if (Debug_Verbose) { Serial.println("Triggered Interupt 3");  }
// Uncomment for Launch
// if (Candy_3_Dispensed == 0)
// {

if (Candy_Button_1_Untriggered && Candy_Button_2_Untriggered && Candy_Button_3_Untriggered && Candy_Button_4_Untriggered){ // Prevent multiple presses
 if (Debug_Verbose) { Serial.println("Activating Candy 4");  }
   Candy_Button_3_Untriggered = false;
}
// }
}


void Dispense_Candy_Interupt_4() {
 if (Debug_Verbose) { Serial.println("Triggered Interupt 4");  }

    
 // Uncomment for Launch
 // if (Candy_4_Dispensed == 0)
// {

if (Candy_Button_1_Untriggered && Candy_Button_2_Untriggered && Candy_Button_3_Untriggered && Candy_Button_4_Untriggered){ // Prevent multiple presses
 if (Debug_Verbose) { Serial.println("Activating Candy 4");  }
  Candy_Button_4_Untriggered = false;
}
// }
}



void Reset_InterBoard_INPUT_Interupt() { // When the light show is done, reset the comm and turn off the light show active flag.
 if (Debug_Verbose) { Serial.println("Resetting InterBoard - Initial State");  
 Serial.println("IB1,2,3, Trigger Output State, Trigger Pin, LS Active Flag, LS Input Pin");
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 Serial.println(digitalRead(LightShow_Interupt_Trigger_OUTPUT));     
 Serial.println(LightShow_Active);
 Serial.println(digitalRead(Reset_InterBoard_INPUT));  
 }

   LightShow_Active = false;  // Overridden in Open Loop mode

/* Reinstate?
   LightShow_Interupt_Trigger_OUTPUT_State = LOW; 
   InterBoard_1_State = HIGH; 
   InterBoard_2_State = HIGH; 
   InterBoard_3_State = HIGH;

  digitalWrite(LightShow_Interupt_Trigger_OUTPUT, LightShow_Interupt_Trigger_OUTPUT_State);
  digitalWrite(InterBoard_1, InterBoard_1_State);
  digitalWrite(InterBoard_2, InterBoard_2_State);
  digitalWrite(InterBoard_3, InterBoard_3_State);
*/
   
 if (Debug_Verbose) { Serial.println("Final State State");  
 Serial.println("IB1,2,3, Trigger Output State, Trigger Pin, LS Active Flag, LS Input Pin");
 Serial.println(InterBoard_1_State);   
 Serial.println(InterBoard_2_State);   
 Serial.println(InterBoard_3_State); 
 Serial.println(LightShow_Interupt_Trigger_OUTPUT_State);
 Serial.println(digitalRead(LightShow_Interupt_Trigger_OUTPUT));     
 Serial.println(LightShow_Active);
 Serial.println(digitalRead(Reset_InterBoard_INPUT));
 Serial.println("  ");
 
 }

}



/* // Uncomment for Launch
void Launch() {
  Serial.println("Launch Interrupt Triggered");
  Launch_Button_Triggered = true;
} . */

