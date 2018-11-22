#include "src/MLP/CommandHandler.h"

struct Data {
  int Number;
  char Text[6+1];
};

Data ExampleData;

CommandHandler<20> SerialCommandHandler(Serial, '$', '\n');

void setup()
{
  Serial.begin(9600);

  Serial.print("Struct at startup: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  SerialCommandHandler.AddCommand(F("Cmd1"), Cmd_1);
  SerialCommandHandler.AddCommand(F("Cmd2"), Cmd_2);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);
}


void loop()
{
  SerialCommandHandler.Process();
}

void Cmd_1(CommandParameter &parameters)
{
  Serial.print("Struct at the start of Cmd_1: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);
  char* tmpText;
  tmpText = parameters.NextParameter();
  int tmpNumber = parameters.NextParameterAsInteger();

  strlcpy(ExampleData.Text, tmpText, 6+1);
  ExampleData.Number = tmpNumber;

  Serial.print("Struct at the end of Cmd_1: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);
}

void Cmd_2(CommandParameter &parameters)
{
  Serial.print("Struct at the start of Cmd_2: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);

  char* tmpText;
  tmpText = parameters.NextParameter();
  int tmpNumber = parameters.NextParameterAsInteger();

  strlcpy(ExampleData.Text, tmpText, 6+1);
  ExampleData.Number = tmpNumber;

  Serial.print("Struct at the end of Cmd_2: ");
  Serial.print(ExampleData.Number);
  Serial.println(ExampleData.Text);
}

void Cmd_Unknown()
{
  Serial.println(F("command not found"));
  Serial.println(F("Try '$Help' for more information"));
}
