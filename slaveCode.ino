// Set up some useful global variables
// Specified here as leds, these outputs will be used to control pFETs in the final design
#define seedLed 2
#define waterLed 3
char serialState = 'z'; // States are represented with characters to avoid ambiguity

void setup() { 
  pinMode(seedLed, OUTPUT);
  pinMode(waterLed, OUTPUT);
  // Begins serial communication for bluetooth and debugging
  Serial.begin(38400);
}
 
void loop() {
  // Read command value from master
  if (Serial.available() > 0) {
    serialState = (char) Serial.read();  
  }
  
  // Translates command values to slave IO
  if (serialState == 'a') {
    digitalWrite(seedLed, HIGH);
  } else if (serialState == 'b') {
    digitalWrite(seedLed, LOW);  
  }
  if (serialState == 'c') {
    analogWrite(waterLed, 25); // Puts motor at 10% power
  } else if (serialState == 'd') {
    analogWrite(waterLed, 0); // Turns off motor
  } 

  /*// Print statements for debugging via serial terminal
  Serial.print("serialState: ");
  Serial.println(serialState);*/
}
