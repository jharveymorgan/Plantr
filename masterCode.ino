// Define global variables
#define ledPin 6
int potValue = 0;
int state = 0;

void setup() 
{
  // Sets LED as output and defaults to low
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // Begins serial communication for bluetooth and debugging
  Serial.begin(38400);
}
 
void loop()
{
  // Receives button presses from slave
  if (Serial.available() > 0) {
    state = Serial.read();  
  }
  // Writes button press value to LED
  if (state == '1') {
    digitalWrite(ledPin, HIGH);
  } else if (state == '0') {
    digitalWrite(ledPin, LOW);
  }
  state = 0;
 
  // Reads pot value and sends to slave
  potValue = analogRead(A0);
  int potValueMapped = map(potValue, 0, 1023, 0, 255);
  Serial.write(potValueMapped);
  delay(10);


}
