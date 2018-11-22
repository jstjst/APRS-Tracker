#include <EEPROM.h>

struct Data {
  int Number;
  char Text[6+1];
};

const Data DefaultData = {
  84,
  "defaul",
};

Data ExampleData;

void setup()
{
  Serial.begin(9600);

  Serial.print("Default Struct at startup: ");
  Serial.print(DefaultData.Number);
  Serial.println(DefaultData.Text);
  Serial.print("Struct at startup: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  EEPROM.put(10, DefaultData);

  Serial.print("Struct after eep put: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  EEPROM.get(10, ExampleData);

  Serial.print("Struct after eep get: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);


  printData();
}

void loop()
{}

void writeData()
{
  Serial.print("Struct at the start of writeData: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  ExampleData.Number = 7;
  strlcpy(ExampleData.Text, "TEST22", 6+1);

  Serial.print("Struct at the end of writeData: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);
}

void printData()
{
  Serial.print("Struct at printData: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);
}
