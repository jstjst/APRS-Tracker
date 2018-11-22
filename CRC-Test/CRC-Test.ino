#include <util/crc16.h>

void setup()
{
  word checksum = 0xFFFF;
  byte data[5] = {12, 58, 155, 189, 236};
  Serial.begin(9600);
  for(int i=0; i<5; i++)
  {
    checksum = _crc16_update(checksum, data[i]);
    Serial.print("i:");            Serial.print(i);
    Serial.print(" data:");        Serial.print(data[i], HEX);
    Serial.print(" checksum:");    Serial.println(checksum, HEX);
  }
}

void loop() {}
