
//#include "config.h"

#define IO_USERNAME  "mchacon21543"
#define IO_KEY       "aio_FRdo18ynuCtceCpepWBELXcq4q9P"


#define WIFI_SSID "Miguel's iPhone"
#define WIFI_PASS "maincraxd"

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_DEV_ADDR 0x55

#include "AdafruitIO_WiFi.h"
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>
#include "Wire.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
// this int will hold the current count for our sketch
int temp = 0;
int seg = 0; 
//int vel = 0;
//int min = 0;

// set up the 'counter' feed
AdafruitIO_Feed *temperatura = io.feed("temperatura");
// AdafruitIO_Feed *velocidad = io.feed("velocidad");
// AdafruitIO_Feed *minutos = io.feed("minutos");

uint32_t i = 0;

void onRequest(){
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
}

void onReceive(int len){
  Serial.printf("onReceive[%d]: ", len);
  while(Wire.available()){
    Serial.write(Wire.read());
  }
  Serial.println();
}

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);

#if CONFIG_IDF_TARGET_ESP32
  char message[64];
  snprintf(message, 64, "%u Packets.", i++);
  Wire.slaveWrite((uint8_t *)message, strlen(message));
#endif

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
  temp = Wire.read();
  vel = Wire.read();
  min = Wire.read();
  seg = Wire.read();

  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(temp);
  temperatura->save(temp);

  temp++;

  delay(3000);



