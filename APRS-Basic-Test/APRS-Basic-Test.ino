#include "src/LibAPRS/src/LibAPRS.h"

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(4800);

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  APRS_printSettings(Serial);
}

void loop()
{
  sendLocation();
  delay(1000);
}

void sendLocation()
{
  APRS_setLat("4822.90N");
  APRS_setLon("00943.19E");

  char *comment = "LibAPRS Test";

  APRS_sendLoc(comment, strlen(comment));
}
