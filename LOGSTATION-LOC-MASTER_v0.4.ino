/******************************************************************************
  LOGSTATION-LOC-MASTER v0,4
  GPS float position to char, send the chars's through the serial port to an other microcontroller.
  By Jorrit Okkerman 
  February 9, 2019
  https://github.com/jorritxl/voyage-logging

  After uploading the code, and setting up the other libaries open your serial monitor, set it to 9600 baud, and
  watch for latitude, longitude, altitude, course, speed, date, time, and the
  number of visible satellites. Tip;watch henk.

  Resources:
  TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
  SoftwareSerial Library
  Wire

  Development/hardware environment specifics:
  Arduino IDE 1.8.5
  ATGM336H GPS  
  Arduino Uno
******************************************************************************/
#include<Wire.h>

float x_lat;              //51.956136;
float y_lng;
float sog;
float sats;
float hdopp;

char ix[1];
char wy[1];
char so[1];
char sa[1];
char hd[1];

#include <TinyGPS++.h> 
TinyGPSPlus tinyGPS; 
#define GPS_BAUD 9600 

#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 10 
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ssGPS  
#define SerialMonitor Serial
void setup() {
  Wire.begin(2);
  Wire.onRequest(Request);
  SerialMonitor.begin(9600);
  gpsPort.begin(GPS_BAUD);
  Serial.begin(9600);
}

void loop() {
  smartDelay(1000);
  printGPSInfo();
  delay(1000);
}


void Request()
{
  if (x_lat > y_lng or x_lat < y_lng)
  {
//Converting float value to char
//The format (float, bytes, numbers of numbers after the decimal, char variable)
    dtostrf(x_lat, 8, 6, ix); 
    Wire.write(ix); // appx 8 bytes
    Wire.write(",");
    dtostrf(y_lng, 4, 6, wy);
    delay(100);
    Wire.write(wy); // appx 4 bytes
    Wire.write(",");
    dtostrf(sog, 4, 1, so);
    delay(100);
    Wire.write(so); // appx 4 bytes
    Wire.write(",");
    dtostrf(sats, 2, 0, sa);
    delay(100);
    Wire.write(sa); // appx 2 bytes
    Wire.write(",");
    dtostrf(hdopp, 2, 0, hd);
    delay(100);
    Wire.write(hd); // appx 2 bytes
    Wire.write("\n");
  }
  else
  {}

}

void printGPSInfo()
{
  Serial.print("gpslat:"); Serial.println(tinyGPS.location.lat(), 7);
  Serial.print("gpslat:"); Serial.println(tinyGPS.location.lng(), 7);
  delay(10);
  x_lat = tinyGPS.location.lat();
  y_lng = tinyGPS.location.lng();
  sog = tinyGPS.speed.kmph();
  sats = tinyGPS.satellites.value();
  hdopp = tinyGPS.hdop.value();
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    // If data has come in from the GPS module
    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read()); 
  } while (millis() - start < ms);
}
