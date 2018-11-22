struct Data {
  int Number;
  char Text[6+1];
};

Data ExampleData;

void setup()
{
  Serial.begin(9600);

  Serial.print("Struct at startup: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  ExampleData.Number = 42;
  strlcpy(ExampleData.Text, "ASCII2", 6+1);

  Serial.print("Struct after first Data: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  writeData();

  Serial.print("Struct after writeData: ");
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
