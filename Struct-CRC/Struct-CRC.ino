#include <util/crc16.h>
#include <EEPROM.h>

#define eeAddress 0

struct Data {
  bool eins;
  char zwei[4+1];
  int drei;
  unsigned long vier;
  word checksum;
};

Data data = {
  true,
  "test",
  2345,
  23456,
};

const Data default1 = {
  false,
  "test",
  5678,
  56789,
};

void setup()
{
  Serial.begin(9600);

  //saveConfig();
  Serial.println(loadConfig());

  Serial.println(data.eins);
  Serial.println(data.zwei);
  Serial.println(data.drei);
  Serial.println(data.vier);
  Serial.println(data.checksum);
}

void loop() {}

bool loadConfig()
{
  word EEPROMchecksum = 0xFFFF;

  EEPROM.get(eeAddress, data);

  for(int i=0; i<sizeof(data)-sizeof(data.checksum); i++)
  {
    EEPROMchecksum = _crc16_update(EEPROMchecksum, *((byte*)&data + i));
  }

  if(data.checksum == EEPROMchecksum)
  {
    return true;
  }
  else
  {
    data = default1;
    return false;
  }
}

void saveConfig()
{
  data.checksum = 0xFFFF;

  for(int i=0; i<sizeof(data)-sizeof(data.checksum); i++)
  {
    data.checksum = _crc16_update(data.checksum, *((byte*)&data + i));
  }
  EEPROM.put(eeAddress, data);
}
