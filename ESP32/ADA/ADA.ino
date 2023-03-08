
//#include "config.h"

#define IO_USERNAME  "mchacon21543"
#define IO_KEY       "aio_FRdo18ynuCtceCpepWBELXcq4q9P"


#define WIFI_SSID "Miguel's iPhone"
#define WIFI_PASS "maincraxd"

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

#include "AdafruitIO_WiFi.h"
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

void receiveEvent(int howMany);

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
// this int will hold the current count for our sketch
int temp = 0;
int verif = 0; 
//int vel = 0;
//int min = 0;

// set up the 'counter' feed
AdafruitIO_Feed *temperatura = io.feed("temperatura");
// AdafruitIO_Feed *velocidad = io.feed("velocidad");
// AdafruitIO_Feed *minutos = io.feed("minutos");

void setup() {

  // start the serial connection
    Serial.begin(115200);

    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!success) {
        Serial.println("I2C slave init failed");
        while(1) delay(100);
    }

    WireSlave.onReceive(receiveEvent);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(temp);
  temperatura->save(temp);

  temp++;

  delay(3000);

  WireSlave.update();

    // let I2C and other ESP32 peripherals interrupts work
  delay(1);


void receiveEvent(int howMany)
{
    while (1 < WireSlave.available()){ // loop through all but the last byte
        c = WireSlave.read();  // receive byte as a character
        Serial.print(c);            // print the character
    }

      int x = WireSlave.read();   // receive byte as an integer
      Serial.println(x);          // print the integer
}
}

// if (c == 10){
//   temp = WireSlave.read();
// }
// else if (c == 20){
//   velocidad = WireSlave.read();
// }
// else if (c == 30){
//   umin = WireSlave.read();
// }
// else if (c == 40){
//   dmin = WireSlave.read();
// }

