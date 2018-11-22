#include "src/LibAPRS/src/LibAPRS.h"
#include "src/MLP/CommandHandler.h"

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

#define eeAddress = 0

struct APRS_Settings {
  char Callsign[6];
  int ssid;
};

APRS_Settings aprsSettings = {
  "NOCALL",
  1,
};

CommandHandler<> SerialCommandHandler;

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(4800);

  SerialCommandHandler.AddCommand(F("SetCall"), SetCall)

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  APRS_setCallsign("DC1MAK", 1);
  APRS_printSettings();
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

void SetCall(CommandParameter &paramerters)
{
  aprsSettings.Callsign = parameters.NextParameter();
  aprsSettings.ssid = parameters.NextParameterAsInteger(1);
  APRS_setCallsign(aprsSettings.Callsign, aprsSettings.ssid);
}
