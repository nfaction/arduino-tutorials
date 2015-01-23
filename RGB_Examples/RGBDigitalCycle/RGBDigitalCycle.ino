/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

void setup() {
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(10, OUTPUT); // green
  pinMode(9, OUTPUT);  // Blue
  pinMode(8, OUTPUT);  // red
}

void loop() {
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(10, LOW);  // green
  delay(1200);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);   // blue
  delay(1200);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);   // red
  delay(1200);
  digitalWrite(10, LOW);  // yellow
  delay(1200);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);   // Cyan (Light Blue)
  delay(1200);
  digitalWrite(10, HIGH);
  digitalWrite(8, LOW);   // Magenta (purple)
  delay(1200);
  digitalWrite(10, LOW);  // White
  delay(1200);
} 
