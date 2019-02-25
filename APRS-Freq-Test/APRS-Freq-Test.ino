#include "src/LibAPRS/src/LibAPRS.h"

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(9600);
  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);

  Serial.println(afsk_testTone(1200U, 1000UL));
  delay(1500);
  Serial.println(afsk_testTone(2200U, 1000UL));
}

void loop()
{}
