#include "src/LibAPRS/src/LibAPRS.h"

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(9600);
  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  APRS_setCallsign("DC1MAK", 1);
  APRS_printSettings();

  for(int i=0; i<5; i++)
  {
    sendLocation;
  }
}

void loop()
{
}

void sendLocation()
{
  APRS_setLat("4822.909N");
  APRS_setLon("00943.195E");

  char *comment = "LibAPRS Test";

  APRS_sendLoc(comment, strlen(comment));
}

