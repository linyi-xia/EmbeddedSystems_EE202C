// color sensor
#include <Wire.h>
#include "SFE_ISL29125.h"
// line sensor
#include <QTRSensors.h>
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

// Declare color sensor object
SFE_ISL29125 RGB_sensor;

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9, 10}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int position = 0;
char dir='C';
char clr='B';
int flag = 0;
int i=9;
int incomingByte = 0;   // for incoming serial data
void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
Serial.begin(115200);
// Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
  
 /* for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();*/
  delay(1000);
}

// control direction of servo
void loop() {
  delay(100*(9-i));
  i=0;
  for(i=0;i<9;i++){
    position = qtrrc.readLine(sensorValues);
    if (position < 6999 &position >0)
    {
  //     Serial.println(position);
       if (position >4500)
       {
          dir = 'L';
          //Serial.print("L"); 
       }else if(position <2000)
       {
          dir = 'R';
          //Serial.print("R"); 
       }
       else
       {
          dir = 'C';
          //Serial.print("C");
       }
       
    }else{
      dir = ' '; 
      //Serial.print(" ");
      }
    unsigned int red = RGB_sensor.readRed();
    unsigned int green = RGB_sensor.readGreen();
    unsigned int blue = RGB_sensor.readBlue();
    if ((red > 1.5*green) && (red>blue*1.5)) {
      clr = 'M';
      flag = 1;
      break;
      //Serial.println("M");
    } else {
      clr = 'B';
      //Serial.println("B");
    }
    //  Serial.print(i); 
    delay(100);
  }  
  Serial.print(dir);
  Serial.println(clr);
  if(clr =='M')
  {
      while(Serial.available() < 1)
      {
        incomingByte = Serial.read();
        if(incomingByte == 72)
        {
          break;
        }
          delay(1000);
          Serial.write('l');
          digitalWrite(13, HIGH); 
      }
      digitalWrite(13, LOW); 
  }
  Serial.flush();
  flag = 0;
  clr ='B';
  
  delay (100);
  //Serial.flush();
}
