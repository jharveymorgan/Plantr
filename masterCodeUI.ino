// Define global variables
#define seedButton 2
#define waterButton 3
#define seedLed 5
#define waterLed 6
#define RESETLINE 9 // Reset line for uLCD

// Import graphics and softwareSerial for screen
#include <SoftwareSerial.h>
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"

// Create display object. Using softwareSerial because Serial is reserved for bluetooth
SoftwareSerial DisplaySerial(7,8) ; // pin7 = TX of display, pin8 = RX
Goldelox_Serial_4DLib Display(&DisplaySerial);

// Define different button states for debouncing
// To avoid ambiguity, the seed high state is encoded as the char 'a' and the low state as 'b'
// For water, 'c' is high and 'd' is low
char seedState;
char waterState;
char lastSeedState = 'b';
char lastWaterState ='d';
char seedOut = 'b';
char waterOut = 'd';
int seedStateRaw; // proxy variables to read the pushbutton values
int waterStateRaw;

// Unsigned longs for the time since the last button press because ints would overflow
unsigned long seedLastDebounceTime = 0;
unsigned long waterLastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Starter code for UI
void init_screen(void) {
  Display.gfx_Cls();
  // Make a nice big "Plantr"
  Display.txt_Height(4);
  Display.txt_Width(3);
  Display.txt_MoveCursor(0,0);
  Display.txt_BGcolour(WHITE);
  Display.txt_FGcolour(LIME);
  Display.txt_Bold(ON);
  Display.putstr("Plantr");
  // Writes "Now Dispensing:"
  Display.txt_Height(2);
  Display.txt_Width(1);
  Display.txt_MoveCursor(3,1);
  Display.txt_BGcolour(BLACK);
  Display.txt_FGcolour(WHITE);
  Display.txt_Italic(ON);
  Display.txt_Bold(ON);
  Display.putstr("Now Dispensing:");
  // Writes "Seeds >"
  Display.txt_Height(1);
  Display.txt_Width(1);
  Display.txt_MoveCursor(11,4);
  Display.txt_FGcolour(MAGENTA);
  Display.txt_Bold(OFF);
  Display.txt_Italic(OFF);
  Display.putstr("Seeds >");
  // Writes "Water ~"
  Display.txt_MoveCursor(13,4);
  Display.txt_FGcolour(AQUA);
  Display.putstr("Water ~");
  //Draws square for seeds
  Display.gfx_RectangleFilled(83,95, 93,86, RED);
  //Draws square for water
  Display.gfx_RectangleFilled(83,110, 93,101, RED);
  //Draws rectangle to hold UI
  Display.gfx_Rectangle(23, 82, 101, 115, YELLOW);
  /*// Draws triangle for seeds
  Display.gfx_Triangle(83,95,88,86,93,95,LIME);
  // Draws triangle for water
  Display.gfx_Triangle(83,110,88,101,93,110,LIME);*/
  /*//Rectangle to Erase seed state
  Display.gfx_RectangleFilled(83,95, 93,86, BLACK);
  //Rectangle to Erase water state
  Display.gfx_RectangleFilled(83,110, 93,101, BLACK);*/
}

// Main setup
void setup() {
  //Setup for uLCD screen
  pinMode(RESETLINE, OUTPUT);  // Set D9 on Arduino to Output 
  digitalWrite(RESETLINE, 0);  // Reset the Display via D9
  delay(100);
  digitalWrite(RESETLINE, 1);  // unReset the Display via D9
  delay(3000);
  Display.TimeLimit4D   = 5000 ; // 2 second timeout on all commands
  Display.Callback4D = NULL ; // NULL ;
  DisplaySerial.begin(9600) ;
  Display.gfx_Cls() ;
  init_screen();
  // Setup for Plantr I/O
  pinMode(seedButton, INPUT);
  pinMode(waterButton, INPUT);
  pinMode(seedLed, OUTPUT);
  pinMode(waterLed, OUTPUT);
  // Begins serial communication for bluetooth and debugging purposes
  Serial.begin(38400);  
}

// Overall loop
void loop() {
  // Reads raw button values
  seedStateRaw = digitalRead(seedButton);
  waterStateRaw = digitalRead(waterButton);
  // Encodes button values for serial communication
  if (seedStateRaw == HIGH) {
    seedState = 'a';  
  } else if (seedStateRaw == LOW) {
    seedState = 'b';
  }
  if (waterStateRaw == HIGH) {
    waterState = 'c';  
  } else if (waterStateRaw == LOW) {
    waterState = 'd';
  }

  // Resets timer if the button changed value before the timer reaches the specified debounce delay.
  // If enough time has actually passed and the seed button has been pressed, let the value change
  if (((millis() - seedLastDebounceTime) > debounceDelay) && (seedState != lastSeedState)) {
    if ((seedState == 'a') && (lastSeedState == 'b')) {
      // Erase current seed state on UI
      Display.gfx_RectangleFilled(83,95, 93,86, BLACK);
      // Toggle seed out
      if (seedOut == 'a') {
        seedOut = 'b';
        // Draws new UI symbol for seed
        Display.gfx_RectangleFilled(83,95, 93,86, RED);
      }  else if (seedOut == 'b') {
        seedOut = 'a';
        // Draws new UI symbol for seed
        Display.gfx_Triangle(83,95,88,86,93,95,LIME);
      }
    }
    // Reset debounce timer and update state
    seedLastDebounceTime = millis();
    lastSeedState = seedState;
  } else if (((millis() - seedLastDebounceTime) < debounceDelay) && (seedState != lastSeedState)) {
    seedLastDebounceTime = millis();
  }

  // Resets timer if the button changed value before the timer reaches the specified debounce delay.
  // If enough time has actually passed and the water button has been pressed, let the value change
  if (((millis() - waterLastDebounceTime) > debounceDelay) && (waterState != lastWaterState)) {
    if ((waterState == 'c') && (lastWaterState == 'd')) {
      // Erase current water state on UI
      Display.gfx_RectangleFilled(83,110, 93,101, BLACK);
      // Toggle water out
      if (waterOut == 'c') {
        waterOut = 'd';
        // Draws new UI symbol for water
        Display.gfx_RectangleFilled(83,110, 93,101, RED);
      }  else if (waterOut == 'd') {
        waterOut = 'c';
        // Draws new UI symbol for water
        Display.gfx_Triangle(83,110,88,101,93,110,LIME);
      }
    }
    // Reset debounce timer and update state
    waterLastDebounceTime = millis();
    lastWaterState = waterState;
  } else if (((millis() - waterLastDebounceTime) < debounceDelay) && (waterState != lastWaterState)) {
      waterLastDebounceTime = millis();
  }

  // Write output values to the test LEDs
  if (seedOut == 'a') {
    digitalWrite(seedLed, HIGH);
  } else if (seedOut == 'b') {
    digitalWrite(seedLed, LOW);  
  }
  if (waterOut == 'c') {
    digitalWrite(waterLed, HIGH);  
  } else if (waterOut == 'd') {
    digitalWrite(waterLed, LOW);  
  }
  // Writes output values to slave
  Serial.write(seedOut);
  delay(10); // Prevents weird signal interference
  Serial.write(waterOut);
  
  //Some print statements for debugging via serial terminal
  /*Serial.println("seedState: ");
  Serial.println(seedState);
  Serial.println("lastSeedState: ");
  Serial.println(lastSeedState);
  Serial.println("seedOut: ");
  Serial.println(seedOut);
  Serial.println("delay: ");
  Serial.println(millis() - lastDebounceTime);*/
}
