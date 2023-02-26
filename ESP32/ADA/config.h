/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "mchacon21543"
#define IO_KEY       "aio_WIXt120VcAhNXnuFWA8W5xOYHo3W"

/******************************* WIFI **************************************/


#define WIFI_SSID "Miguel's iPhone"
#define WIFI_PASS "maincraxd"



// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
