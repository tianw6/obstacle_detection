// This project uses 2 laser sensors to and map the distance
// to the amplitude of haptic motor

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X s1;
VL53L1X s2;
// define the 3 distances
float dis1, dis2;
// define the 3 haptic motor pins
int motor1 = 9;
int motor2 = 10;
// define the max & min detected distance
float max_dis = 1.0;
float min_dis = 0.45;
// define the max vibration;
int max_vib = 170;

// define SHUT pin
int sh1 = 2;
int sh2 = 3;

void setup()
{
  // pullup resistor
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  
  
  // define reset pin (SHUT)
  pinMode(sh1, OUTPUT);
  pinMode(sh2, OUTPUT);

  digitalWrite(sh1, LOW);  
  digitalWrite(sh2, LOW);

  // define the analog output pins (chose of 3,5,6,9,10,11)
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  // initialize the haptic motor pins: all low
  digitalWrite(motor1, LOW);  
  digitalWrite(motor2, LOW);
  delay(500);
  
  Wire.begin();
  Serial.begin(115200);
  // reset the address of each I2C device
  pinMode(sh1, INPUT_PULLUP);
  delay(150);
  s1.init(true);
  Serial.println("check1");
  delay(100);
  s1.setAddress((uint8_t)22);
//  Serial.println("s1set");

  pinMode(sh2, INPUT_PULLUP);
  delay(150);
  s2.init(true);
  Serial.println("check2");
  delay(100);
  s2.setAddress((uint8_t)23);
//  Serial.println("s2set");
  
  // scan I2C device
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  for (byte i = 1; i < 120; i++)
  {

    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1); 
    }
  }
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  
  s1.setTimeout(500);
  s2.setTimeout(500);

  //detect distance continuously, parameter: update every 32ms
  s1.startContinuous(32);
  s2.startContinuous(32);

}

void loop()
{
  // print out the distance in mm
  dis1 = s1.readRangeContinuousMillimeters()/1000.0;
  dis2 = s2.readRangeContinuousMillimeters()/1000.0;
  // control the vibration
  analogWrite(motor1, motorControl(dis1));
  analogWrite(motor2, motorControl(dis2));

  Serial.print("s1: ");
  Serial.print(dis1);
  Serial.print(" vib:");
  Serial.print(motorControl(dis1));
  Serial.print(" ");
  
  Serial.print("s2: ");
  Serial.print(dis2);
  Serial.print(" vib:");
  Serial.print(motorControl(dis2));
  Serial.print(" ");

  Serial.println();
//  delay(200);
}

// method that uses the distance to control the vibration
int motorControl(float dis) 
{
  // max distance detected is 1m (more than 1m is counted as 1m)
  if (dis > max_dis) {dis = max_dis;}
  // min distance detected is 5cm (less than 5cm is counted as 0m)
  if (dis < min_dis) {dis = 0;}  
  return (1 - dis/max_dis)*max_vib;
}
