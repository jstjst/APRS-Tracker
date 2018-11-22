#include "src/LibAPRS/src/LibAPRS.h"

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

#define eeAddress = 0

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(4800);
  APRS_printSettings();
}

void loop() {}

/* --- Default Settings should look like this:
LibAPRS Settings:
Callsign:     NOCALL-0
Destination:  APZMDM-0
Path1:        WIDE1-1
Path2:        WIDE2-2
Message dst:  N/A
TX Preamble:  350
TX Tail:      50
Symbol table: Normal
Symbol:       n
Power:        N/A
Height:       N/A
Gain:         N/A
Directivity:  N/A
Latitude:     N/A
Longtitude:   N/A
*/
