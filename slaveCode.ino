// Set up some useful global variables
#define ledPin 5
#define buttonPin 6
int pwmLevel = 0;
int buttonState = 0;

void setup()
{ 
  pinMode(buttonPin, INPUT);
  Serial.begin(38400);
}
 
void loop()
{
  // Read pot value from master and adjust pwm level for LED accordingly
  if (Serial.available() > 0) {
    pwmLevel = Serial.read();  
  }
  analogWrite(ledPin, pwmLevel); 
  Serial.print("PWM Level: ");
  Serial.print(pwmLevel);
  Serial.print("\n");
  delay(10);

  // Read button and send information to master
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.write('1');  
  }
  else {
    Serial.write('0');
  }
  Serial.print("Button State: ");
  Serial.print(buttonState);
  Serial.print("\n");
}
