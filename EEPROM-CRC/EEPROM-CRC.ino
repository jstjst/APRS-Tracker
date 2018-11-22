#include <util/crc16.h>
#include <EEPROM.h>

#define eeAddress 0

void setup()
{
  word checksum = 0xFFFF;
  byte data[5] = {12, 58, 155, 189, 236};

  Serial.begin(9600);

  //Write data to EEPROM
  EEPROM.put(eeAddress, data);

  //Calculate data checksum
  for(int i=0; i<5; i++)
  {
    checksum = _crc16_update(checksum, data[i]);
  }
  Serial.print("Data before EEPROM checksum:");    Serial.println(checksum, HEX);

  //Read data from EEPROM and Calculate checksum
  checksum = 0xFFFF;
  for(int i=0; i<5; i++)
  {
    byte tmp = EEPROM.read(eeAddress + i);
    data[i] = tmp;
    checksum = _crc16_update(checksum, tmp);
  }
  Serial.print("EEPROM checksum:");    Serial.println(checksum, HEX);

  //Calculate checksum of new(readed) data
  checksum = 0xFFFF;
  for(int i=0; i<5; i++)
  {
    checksum = _crc16_update(checksum, data[i]);
  }
  Serial.print("Data after EEPROM checksum:");    Serial.println(checksum, HEX);
}

void loop() {}
