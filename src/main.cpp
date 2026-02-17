#include <Arduino.h>
#include <ESP32Servo.h>

Servo tilt;
Servo pan;

void setup()
{
  Serial.begin(115200);
  delay(1500);

  Serial.println("In Setup");
  tilt.attach(3);
  pan.attach(4);
  delay(1000);

  Serial.println("Zeroing motors");
  pan.write(0);
  delay(500);
  tilt.write(0);
  delay(2000);
}

void loop()
{
  Serial.println("Alive");
  
  tilt.write(30);
  delay(2000);
  tilt.write(10);
  delay(2000);
  tilt.write(0);
  delay(2000);
  pan.write(30);
  delay(2000);
  pan.write(60);
  delay(2000);
  pan.write(0);
  delay(2000);
  
}
