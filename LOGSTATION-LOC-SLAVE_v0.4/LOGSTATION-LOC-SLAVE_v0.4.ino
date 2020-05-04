#include <Wire.h>
////////////////////
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

struct sensor_bericht
{
  int rpm_val;
  float gpslat;
  float gpslng;
  int gpssog;
  int gpssats;
  int gpshdop; 
};

RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
const unsigned long interval = 500;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

void setup() {
  ///////////////////////////
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(90, this_node);  //(channel, node address)
  //////////////////////////
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  Wire.requestFrom(2, 30);    // request 20 bytes from slave device #2
  String string, string1, string2, string3, string4, string5;
  do   // slave may send less than requested
  {
    char c = Wire.read(); // receive a byte as character

    string = string + c; //Keep saving whatever is comming

    string1 = string.substring(0, 9); //slpit String from 0 to 8
    string2 = string.substring(10, 18); //slpit String from 0 to 8
    string3 = string.substring(19, 23); //slpit String from 0 to 8
    string4 = string.substring(24, 26); //slpit String from 0 to 8
    string5 = string.substring(27); // Split from 9 to the End of String


  } while (Wire.available());
  Serial.print("Recieving float as String...");
  Serial.println();
  Serial.println();
  Serial.print("Full String:");
  Serial.print(string);

  Serial.println();
  Serial.print("string 1:" );
  Serial.print(string1);
  //Serial.println();

  Serial.println();
  Serial.print("string 2:" );
  Serial.print(string2);

  Serial.println();
  Serial.print("string 3:" );
  Serial.print(string3);

  Serial.println();
  Serial.print("string 4:" );
  Serial.print(string4);
  Serial.println();

  Serial.print("string 5:" );
  Serial.print(string5);
  Serial.println("");

  Serial.print(string4.toInt());
  Serial.println("");
  Serial.print(string1.toFloat(),6);
  
  delay(1000);
  Serial.println("");
  ///////////////////////////////////
  network.update();
  //unsigned long now = millis();
  RF24NetworkHeader header(master00);
  int rpm_val = 0;
  float gpslat  = string1.toFloat();
  float gpslng  = string2.toFloat();
  int gpssog  = string3.toInt();
  int gpssats = string4.toInt();
  int gpshdop = string5.toInt();
  sensor_bericht bericht = {rpm_val, gpslat, gpslng, gpssog, gpssats, gpshdop};
  bool ok = network.write(header, &bericht, sizeof(bericht));
  //last_sent = now;

  /////////////////////////////////////
}

