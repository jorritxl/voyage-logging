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
const uint16_t this_node = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
const unsigned long interval = 500;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

// digital pin 2 is the hall pin
int hall_pin = 2;
// set number of hall trips for RPM reading (higher improves accuracy)
float hall_thresh = 5.0; //eik 100

void setup() {

  // make the hall pin an input:
  pinMode(hall_pin, INPUT);

  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(90, this_node);  //(channel, node address)

  //radio.setDataRate(RF24_2MBPS);

}
void loop() {
  // preallocate values for tach
  float hall_count = 1.0;
  float start = micros();
  bool on_state = false;
  // counting number of times the hall sensor is tripped
  // but without double counting during the same trip
  while (true) {
    if (digitalRead(hall_pin) == 0) {
      if (on_state == false) {
        on_state = true;
        hall_count += 1.0;
      }
    } else {
      on_state = false;
    }

    if (hall_count >= hall_thresh) {
      break;
    }
  }

  // print information about Time and RPM
  float end_time = micros();
  float time_passed = ((end_time - start) / 1000000.0);
  Serial.print("Time Passed: ");
  Serial.print(time_passed);
  Serial.println("s");
  float rpm_val = (hall_count / time_passed) * 60.0;
  Serial.print(rpm_val);
  Serial.println(" RPM");
  delay(1);        // delay in between reads for stability

  network.update();
  //===== Sending =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
    //unsigned long potValue = analogRead(A0);
    RF24NetworkHeader header(master00);   // (Address where the data is going)
    sensor_bericht bericht = {rpm_val};
    bool ok = network.write(header, &bericht, sizeof(bericht)); // Send the data
    Serial.print(rpm_val);
  }
}
















////-------------NRF24--------------//
//#include <RF24Network.h>
//#include <RF24.h>
//#include <SPI.h>
//
//struct sensor_bericht
//{
//  float rpm_val;
//};
//
//RF24 radio(7, 8);      // nRF24L01(+) transceiver aangesloten met pins 9 en 10
//RF24Network network(radio);   // netwerk definiëren
//
//const uint16_t deze_node = 1;   // adres van onze (sensor) node
//const uint16_t andere_node = 0; // adres van de andere (ontvanger) node
//
//const unsigned long interval = 2000; //om de hoeveel tijd we de sensorwaarde naar de ontvanger node sturen (2 seconden)
//
//unsigned long tijd_laatste_bericht;  // wanneer we voor het laatste een bericht hebben gestuurd
////-------------rpm--------------//
//// digital pin 2 is the hall pin
//int hall_pin = 2;
//// set number of hall trips for RPM reading (higher improves accuracy)
//float hall_thresh = 100.0;
//
//void setup(){
//// initialize serial communication at 9600 bits per second:
//  Serial.begin(9600);
//  // make the hall pin an input:
//  pinMode(hall_pin, INPUT);
//}
//
//void loop(){
////-------------rpm--------------//
//// preallocate values for tach
//  float hall_count = 1.0;
//  float start = micros();
//  bool on_state = false;
//  // counting number of times the hall sensor is tripped
//  // but without double counting during the same trip
//  while(true){
//    if (digitalRead(hall_pin)==0){
//      if (on_state==false){
//        on_state = true;
//        hall_count+=1.0;
//      }
//    } else{
//      on_state = false;
//    }
//
//    if (hall_count>=hall_thresh){
//      break;
//    }
//  }
//
//  // print information about Time and RPM
//  float end_time = micros();
//  float time_passed = ((end_time-start)/1000000.0);
//  Serial.print("Time Passed: ");
//  Serial.print(time_passed);
//  Serial.println("s");
//  float rpm_val = (hall_count/time_passed)*60.0;
//  Serial.print(rpm_val);
//  Serial.println(" RPM");
//  delay(1);        // delay in between reads for stability
////-------------nrf24--------------//
//  SPI.begin();
//  radio.begin();
//  radio.setPALevel(RF24_PA_HIGH);
//  radio.setDataRate( RF24_250KBPS ) ;
//  network.begin(/*kanaal*/ 90, /*node adres*/ deze_node);
//  network.update();
//  unsigned long tijd_nu = millis();
//  RF24NetworkHeader header(/*naar node*/ andere_node);
//
//  //float rpm;
//  sensor_bericht bericht = {rpm_val};
//  bool ok = network.write(header, &bericht, sizeof(bericht));
//  tijd_laatste_bericht = millis();
////-------------rpm--------------//
//}

