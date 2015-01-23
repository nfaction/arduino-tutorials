int pwm_red = 9; 
int pwm_blue = 10; 
int pwm_green = 11; 
int i; 

void setup() 
{ 
pinMode(pwm_red, OUTPUT); 
pinMode(pwm_blue, OUTPUT); 
pinMode(pwm_green, OUTPUT); 

Serial.begin(9600); 
} 

void loop() 
{ 

// RED
Serial.println("Fading Red pwm channel up to max."); 
analogWrite(pwm_blue, 255);
analogWrite(pwm_green, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_red, i); 
delay(10); 
}
delay(100);
// BLUE
Serial.println("Fading Blue pwm channel up to max."); 
analogWrite(pwm_red, 255);
analogWrite(pwm_green, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_blue, i); 
delay(10); 
}
delay(100);
// GREEN
Serial.println("Fading Green pwm channel up to max."); 
analogWrite(pwm_red, 255);
analogWrite(pwm_blue, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_green, i); 
delay(10); 
}

// Two Color
delay(100);
// Cyan
Serial.println("Fading Cyan pwm channel up to max."); 
analogWrite(pwm_red, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_blue, i);
analogWrite(pwm_green, i);
delay(10); 
}
delay(100);
// Orange
Serial.println("Fading Orange pwm channel up to max."); 
analogWrite(pwm_blue, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_red, i);
analogWrite(pwm_green, i);
delay(10); 
}
delay(100);
// Magenta
Serial.println("Fading Magenta pwm channel up to max."); 
analogWrite(pwm_green, 255);
for (i=255; i>=0; i--) 
{ 
analogWrite(pwm_blue, i);
analogWrite(pwm_green, i);
delay(10); 
}
delay(100);
}
