/******************************************************************************
  LOGSTATION-REC v0,4
  TinyGPS++ Library Example for the SparkFun GPS Logger Shield
  By Jorrit Okkerman 
  February 9, 2019
  https://github.com/jorritxl/voyage-logging

  After uploading the code, and setting up the other libaries open your serial monitor, set it to 9600 baud, and
  watch for latitude, longitude, altitude, course, speed, date, time, and the
  number of visible satellites. Tip;watch henk.

  Resources:
  //TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
  SoftwareSerial Library
  RF24Network

  Development/hardware environment specifics:
  Arduino IDE 1.8.5
  GPS Logger Shield v2.0 - Make sure the UART switch is set to SW-UART
  Arduino Uno, RedBoard, Pro, etc.
******************************************************************************/


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

int henk = 0;

int rpmm;
float gpslatt;
float gpslngg;
int gpssogg;
int gpssatss;
int gpshdopp; 

struct sensor_bericht
{
  int rpm_val;
  float gpslat;
  float gpslng;
  int gpssog;
  int gpssats;
  int gpshdop; 
};

RF24 radio(7, 8);               
RF24Network network(radio);      
const uint16_t this_node = 00;   
const uint16_t node01 = 01;      
const uint16_t node02 = 02;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(90, this_node);  
}
void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() and henk == 0) {    // Is there any incoming data and whats up with henk?
    RF24NetworkHeader header;
    sensor_bericht bericht;
    network.read(header, &bericht, sizeof(bericht)); 

    if (header.from_node == 02) {  
      rpmm = bericht.rpm_val;
      henk = 1;
    }
  }
    
  while ( network.available() and henk == 1) {     // Is there any incoming data and whats up with henk?
    RF24NetworkHeader header;
    sensor_bericht bericht;
    network.read(header, &bericht, sizeof(bericht)); // Read the incoming data

    if (header.from_node == 01) {    
      gpslatt = bericht.gpslat, 7;
      gpslngg = bericht.gpslng, 7;
      gpssogg = bericht.gpssog;
      gpssatss = bericht.gpssats;
      gpshdopp = bericht.gpshdop;
      henk = 2;
    }

  }
  while (henk == 2){
    Serial.print(rpmm);
    Serial.print(","); Serial.print(gpslatt, 7);
    Serial.print(","); Serial.print(gpslngg, 7);
    Serial.print(","); Serial.print(gpssogg);
    Serial.print(","); Serial.print(gpssatss);
    Serial.print(","); Serial.println(gpshdopp);
    delay(200);
    henk = 0;
  }
}









