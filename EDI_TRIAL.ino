//ALCOHOL SENDOR
#define Sober 200   // Define max value that we consider sober
#define Drunk 400   // Define min value that we consider drunk
#define MQ3 0
#define ledPin 6 
float sensorValue;  //variable to store sensor value
int c=0;
  
//ACCELEROMETER
#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3); //make RX arduino line is pin 2, make TX arduino line is pin 3.

#define x A1
#define y A2
#define z A3

int xsample=0;
int ysample=0;
int zsample=0;

#define samples 10
#define minVal -200
#define MaxVal 200

//int i=0,k=0;

//RF TRANSMITTER
#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup() {

  Serial.begin(9600); // sets the serial port to 9600
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //Serial.println("MQ3 Heating Up!");
  delay(15000); // allow the MQ3 to warm up
  
  // Initialize ASK Object
  rf_driver.init();
}

void loop() 
{   
    delay(1000);
    for(int i=0;i<samples;i++)
    {
      xsample+=analogRead(x);
      ysample+=analogRead(y);
      zsample+=analogRead(z);
    }
    xsample/=samples;
    ysample/=samples;
    zsample/=samples;
    Serial.println(xsample);
    Serial.println(ysample);
    Serial.println(zsample);
  
    int value1=analogRead(x);
    int value2=analogRead(y);
    int value3=analogRead(z);
    int xValue=xsample-value1;
    int yValue=ysample-value2;
    int zValue=zsample-value3;
    Serial.print("x=");
    Serial.println(xValue);
    Serial.print("y=");
    Serial.println(yValue);
    Serial.print("z=");
    Serial.println(zValue);
    delay(1000);
        
    if(c<=100)
    {
    digitalWrite(3,HIGH);
    sensorValue = analogRead(MQ3); // read analog input pin 0
    Serial.print("Alcohol Sensor Value: ");
    Serial.println(sensorValue);
    c++;
    }
    if(c>100)
    {
      digitalWrite(3,LOW);
    }
    if(sensorValue >= Drunk)
    {
      const char *msg = "N";
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      {
      // Message Transmitted
      Serial.println("Message Transmitted: ");
      delay(1000);
      
      }
    }
    else
    {
       const char *msg = "Y";
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      {
      // Message Transmitted
      Serial.println("Message Transmitted: ");
      delay(1000);
      }
    }
    if(xValue < minVal || xValue > MaxVal  || yValue < minVal || yValue > MaxVal  || zValue < minVal || zValue > MaxVal )
    {
      Serial.println("Contact Emergency");
    }
    delay(1000); 
}
