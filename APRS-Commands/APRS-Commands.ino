#include "src/LibAPRS/src/LibAPRS.h"
#include "src/MLP/CommandHandler.h"
#include <EEPROM.h>

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

#define eeAddress 0
bool loadEEPROMSettings = false;

struct APRS_Settings {
  char Call[6+1];
  int SSID;
  char DestCall[6+1];
  int DestSSID;
  char Path1Call[6+1];
  int Path1SSID;
  char Path2Call[6+1];
  int Path2SSID;
  unsigned long Preamble;
  unsigned long Tail;
  bool AltSymTable;
  char Sym;
  int Power;
  int Height;
  int Gain;
  int Directivity;
};

/*
char* tmp_Call;
int tmp_SSID;
char tmp_DestCall;
int tmp_DestSSID;
char tmp_Path1Call;
int tmp_Path1SSID;
char tmp_Path2Call;
int tmp_Path2SSID;
unsigned long tmp_Preamble;
unsigned long tmp_Tail;
bool tmp_AltSymTable;
char tmp_Sym;
int tmp_Power;
int tmp_Height;
int tmp_Gain;
int tmp_Directivity;
*/

const APRS_Settings aprsDefaultSettings = {
  "NOCALL",
  0,
  "APZMDM",
  0,
  "WIDE1",
  1,
  "WIDE2",
  2,
  350UL,
  50UL,
  false,
  'n',
  10,
  10,
  10,
  10,
};

APRS_Settings aprsSettings;

CommandHandler<20> SerialCommandHandler(Serial, '$', '\n');

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  Serial.begin(4800);

  SerialCommandHandler.AddCommand(F("SetCall"), Cmd_SetCall);
  /*SerialCommandHandler.AddCommand(F("SetDest"), Cmd_SetDest);
  SerialCommandHandler.AddCommand(F("SetPath1"), Cmd_SetPath1);
  SerialCommandHandler.AddCommand(F("SetPath2"), Cmd_SetPath2);
  SerialCommandHandler.AddCommand(F("SetPreamble"), Cmd_SetPreamble);
  SerialCommandHandler.AddCommand(F("SetTail"), Cmd_SetTail);
  SerialCommandHandler.AddCommand(F("SetSymTable"), Cmd_SetSymTable);
  SerialCommandHandler.AddCommand(F("SetSym"), Cmd_SetSym);
  SerialCommandHandler.AddCommand(F("SetPHG"), Cmd_SetPHG);
*/
  SerialCommandHandler.AddCommand(F("SaveSet"), Cmd_SaveSet);
  SerialCommandHandler.AddCommand(F("LoadSet"), Cmd_LoadSet);
  SerialCommandHandler.AddCommand(F("ApplySet"), Cmd_ApplySet);
  SerialCommandHandler.AddCommand(F("PrintSet"), Cmd_PrintSet);/*
  SerialCommandHandler.AddCommand(F("StartupSet"), Cmd_StartupSet);
  SerialCommandHandler.AddCommand(F("DefaultSet"), Cmd_DefaultSet);*/
  SerialCommandHandler.AddCommand(F("Help"), Cmd_Help);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);

  if(loadEEPROMSettings)
  {
    EEPROM.get(eeAddress, aprsSettings);
    ApplySet();
    Serial.println(F("Loaded APRS Settings from EEPROM"));
  }
  else
  {
    aprsSettings = aprsDefaultSettings;
    ApplySet();
    Serial.println(F("Loaded APRS default Settings"));
  }

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
}

void loop()
{
  SerialCommandHandler.Process();
  //updateStruct(&aprsSettings);
}
/*
void updateStruct(struct APRS_Settings *settings)
{
  settings->Call = tmp_Call;
  settings->SSID = tmp_SSID;
  settings->DestCall = tmp_DestCall;
  settings->DestSSID = tmp_DestSSID;
  settings->Path1Call = tmp_Path1Call;
  settings->Path1SSID = tmp_Path1SSID;
  settings->Path2Call = tmp_Path2Call;
  settings->Path2SSID = tmp_Path2SSID;
  settings->Preamble = tmp_Preamble;
  settings->Tail = tmp_Tail;
  settings->AltSymTable = tmp_AltSymTable;
  settings->Sym = tmp_Sym;
  settings->Power = tmp_Power;
  settings->Height = tmp_Height;
  settings->Gain = tmp_Gain;
  settings->Directivity = tmp_Directivity;
  //Serial.println(tmp_Call);
  //Serial.println(settings->Call);
  //Serial.println(aprsSettings.Call);
}*/

void ApplySet()
{
  APRS_setCallsign(aprsSettings.Call, aprsSettings.SSID);
  APRS_setDestination(aprsSettings.DestCall, aprsSettings.DestSSID);
  APRS_setPath1(aprsSettings.Path1Call, aprsSettings.Path1SSID);
  APRS_setPath2(aprsSettings.Path2Call, aprsSettings.Path2SSID);
  APRS_setPreamble(aprsSettings.Preamble);
  APRS_setTail(aprsSettings.Tail);
  APRS_useAlternateSymbolTable(aprsSettings.AltSymTable);
  APRS_setSymbol(aprsSettings.Sym);
  APRS_setPower(aprsSettings.Power);
  APRS_setHeight(aprsSettings.Height);
  APRS_setGain(aprsSettings.Gain);
  APRS_setDirectivity(aprsSettings.Directivity);
}

void Cmd_SetCall(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();
  strlcpy(aprsSettings.Call, tmp, 6+1);
  aprsSettings.SSID = parameters.NextParameterAsInteger(0);
}

/*
void Cmd_SetDest(CommandParameter &parameters)
{
  tmp_DestCall = parameters.NextParameter();
  tmp_DestSSID = parameters.NextParameterAsInteger(0);
}

void Cmd_SetPath1(CommandParameter &parameters)
{
  tmp_Path1Call = parameters.NextParameter();
  tmp_Path1SSID = parameters.NextParameterAsInteger();
}

void Cmd_SetPath2(CommandParameter &parameters)
{
  tmp_Path2Call = parameters.NextParameter();
  tmp_Path2SSID = parameters.NextParameterAsInteger();
}

void Cmd_SetPreamble(CommandParameter &parameters)
{
  tmp_Preamble = parameters.NextParameterAsUnsignedLong();
}

void Cmd_SetTail(CommandParameter &parameters)
{
  tmp_Tail = parameters.NextParameterAsUnsignedLong();
}

void Cmd_SetSymTable(CommandParameter &parameters)
{
  char *parameter = parameters.NextParameter();

  if(strcmp(parameter, "Alternate") == 0)
  {
    tmp_AltSymTable = true;
  }
  else if(strcmp(parameter, "Normal") == 0)
  {
    tmp_AltSymTable = false;
  }
  else
  {
    Serial.print(F("SetSymTable: invalid argument"));
    Serial.println(parameter);
    Serial.println(F("Valid arguments are:"));
    Serial.println(F("  - 'Normal'"));
    Serial.println(F("  - 'Alternate'"));
    Serial.println(F("Try '$Help' for more information."));
  }
}

void Cmd_SetSym(CommandParameter &parameters)
{
  tmp_Sym = parameters.NextParameter();
}

void Cmd_SetPHG(CommandParameter &parameters)
{
  tmp_Power = parameters.NextParameterAsInteger();
  tmp_Height = parameters.NextParameterAsInteger();
  tmp_Gain = parameters.NextParameterAsInteger();
  tmp_Directivity = parameters.NextParameterAsInteger();
}
*/
void Cmd_SaveSet(CommandParameter &parameters)
{
  EEPROM.put(eeAddress, aprsSettings);
  Serial.println(F("Saved APRS Settings to EEPROM"));
}

void Cmd_LoadSet(CommandParameter &parameters)
{
  EEPROM.get(eeAddress, aprsSettings);
  ApplySet();
  Serial.println(F("Loaded APRS Settings from EEPROM"));
}

void Cmd_ApplySet(CommandParameter &parameters)
{
  ApplySet();
  Serial.println(F("Applied current APRS Settings"));
}

void Cmd_PrintSet(CommandParameter &parameters)
{
  APRS_printSettings();
}
/*
void Cmd_StartupSet(CommandParameter &parameters)
{
  char *parameter = parameters.NextParameter();

  if(strcmp(parameter, "EEPROM") == 0)
  {
    loadEEPROMSettings = true;
  }
  else if(strcmp(parameter, "Default") == 0)
  {
    loadEEPROMSettings = false;
  }
  else
  {
    Serial.print(F("StartupSet: invalid argument"));
    Serial.println(parameter);
    Serial.println(F("Valid arguments are:"));
    Serial.println(F("  - 'EEPROM'"));
    Serial.println(F("  - 'Default'"));
    Serial.println(F("Try '$Help' for more information."));
  }
}

void Cmd_DefaultSet(CommandParameter &parameters)
{
  aprsSettings = aprsDefaultSettings;
  ApplySet();
  Serial.println(F("Loaded APRS default Settings"));
}
*/
void Cmd_Help(CommandParameter &parameters)
{
  Serial.println(F("Usage: $[Command]"));
  Serial.println(F("Commands:"));
  Serial.println(F("  SetCall [Callsign] [SSID]                           set your callsign and SSID"));
  Serial.println(F("  SetDest [Callsign] [SSID]                           set the destination callsign and SSID"));
  Serial.println(F("  SetPath1 [Callsign] [SSID]                          set the path 1 callsign and SSID"));
  Serial.println(F("  SetPath2 [Callsign] [SSID]                          set the path 2 callsign and SSID"));
  Serial.println(F("  SetPreamble [Preamble]                              set the TX preamble"));
  Serial.println(F("  SetTail [Tail]                                      set the TX tail"));
  Serial.println(F("  SetSymTable ['Normal', 'Alternate']                 choose between 'Normal' or 'Alternate' symbol table"));
  Serial.println(F("  SetSym [Symbol]                                     set your symbol"));
  Serial.println(F("  SetPHG [Power] [Height] [Gain] [Directivity]        set the power/height/gain/directivity information of your station"));
  Serial.println(F("  SaveSet                                             save the current APRS settings to the EEPROM"));
  Serial.println(F("  LoadSet                                             load and apply the settings saved in the EEPROM"));
  Serial.println(F("  ApplySet                                            apply the current APRS settings to LibAPRS"));
  Serial.println(F("  PrintSet                                            print the current APRS settings"));
  Serial.println(F("  StartupSet ['Default', 'EEPROM']                    choose whether 'Default' or 'EEPROM' settings are loaded and applied at startup"));
  Serial.println(F("                                                      default is 'EEPROM'"));
  Serial.println(F("  DefaultSet                                          load and apply the default APRS settings"));
  Serial.println(F("  Help                                                display this help"));
  Serial.println();
  Serial.println(F("APRS-Tracker version 0.0, URL: <https://github.com/jstjst/APRS-Tracker>"));
}

void Cmd_Unknown()
{
  Serial.println(F("command not found"));
  Serial.println(F("Try '$Help' for more information"));
}
