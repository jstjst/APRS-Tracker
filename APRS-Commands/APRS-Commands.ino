#include "src/LibAPRS/src/LibAPRS.h"
#include "src/MLP/CommandHandler.h"
#include <util/crc16.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false

#define CONFIG_VERSION "1"
#define CONFIG_START 32
#define eeAddress 0
bool loadEEPROMSettings = true;

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
  word checksum;
};

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

SoftwareSerial CommandSerial(8, 9); // RX, TX

CommandHandler<20> SerialCommandHandler(CommandSerial, '$', '\n');

void aprs_msg_callback(struct AX25Msg *msg)
{}

void setup()
{
  CommandSerial.begin(9600);
  CommandSerial.listen();

  SerialCommandHandler.AddCommand(F("SetCall"), Cmd_SetCall);
  SerialCommandHandler.AddCommand(F("SetDest"), Cmd_SetDest);
  SerialCommandHandler.AddCommand(F("SetPath1"), Cmd_SetPath1);
  SerialCommandHandler.AddCommand(F("SetPath2"), Cmd_SetPath2);
  SerialCommandHandler.AddCommand(F("SetPreamble"), Cmd_SetPreamble);
  SerialCommandHandler.AddCommand(F("SetTail"), Cmd_SetTail);
  SerialCommandHandler.AddCommand(F("SetSymTable"), Cmd_SetSymTable);
  SerialCommandHandler.AddCommand(F("SetSym"), Cmd_SetSym);
  SerialCommandHandler.AddCommand(F("SetPHG"), Cmd_SetPHG);

  SerialCommandHandler.AddCommand(F("SaveSet"), Cmd_SaveSet);
  SerialCommandHandler.AddCommand(F("LoadSet"), Cmd_LoadSet);
  SerialCommandHandler.AddCommand(F("ApplySet"), Cmd_ApplySet);
  SerialCommandHandler.AddCommand(F("PrintSet"), Cmd_PrintSet);
  SerialCommandHandler.AddCommand(F("StartupSet"), Cmd_StartupSet);
  SerialCommandHandler.AddCommand(F("DefaultSet"), Cmd_DefaultSet);
  SerialCommandHandler.AddCommand(F("Help"), Cmd_Help);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);

  if(loadEEPROMSettings)
  {
    bool Successful = LoadSet();
    if(Successful)
    {
      CommandSerial.println(F("Loaded APRS Settings from EEPROM"));
    }
    else
    {
      aprsSettings = aprsDefaultSettings;
      CommandSerial.println(F("No vaild APRS Settings found in EEPROM,"));
      CommandSerial.println(F("Loaded APRS default Settings"));
    }

    ApplySet();
  }
  else
  {
    aprsSettings = aprsDefaultSettings;
    ApplySet();
    CommandSerial.println(F("Loaded APRS default Settings"));
  }

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
}

void loop()
{
  SerialCommandHandler.Process();
}

bool LoadSet()
{
  word EEPROMchecksum = 0xFFFF;

  EEPROM.get(eeAddress, aprsSettings);

  for(int i=0; i<sizeof(aprsSettings)-sizeof(aprsSettings.checksum); i++)
  {
    EEPROMchecksum = _crc16_update(EEPROMchecksum, *((byte*)&aprsSettings + i));
  }

  if(aprsSettings.checksum == EEPROMchecksum)
  {
    return true;
  }
  else
  {
    return false;
  }
}

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

void Cmd_SetDest(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();
  strlcpy(aprsSettings.DestCall, tmp, 6+1);
  aprsSettings.DestSSID = parameters.NextParameterAsInteger(0);
}

void Cmd_SetPath1(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();
  strlcpy(aprsSettings.Path1Call, tmp, 6+1);
  aprsSettings.Path1SSID = parameters.NextParameterAsInteger();
}

void Cmd_SetPath2(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();
  strlcpy(aprsSettings.Path2Call, tmp, 6+1);
  aprsSettings.Path2SSID = parameters.NextParameterAsInteger();
}

void Cmd_SetPreamble(CommandParameter &parameters)
{
  aprsSettings.Preamble = parameters.NextParameterAsUnsignedLong();
}

void Cmd_SetTail(CommandParameter &parameters)
{
  aprsSettings.Tail = parameters.NextParameterAsUnsignedLong();
}

void Cmd_SetSymTable(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();

  if(strcmp(tmp, "Alternate") == 0)
  {
    aprsSettings.AltSymTable = true;
  }
  else if(strcmp(tmp, "Normal") == 0)
  {
    aprsSettings.AltSymTable = false;
  }
  else
  {
    CommandSerial.print(F("SetSymTable: invalid argument"));
    CommandSerial.println(tmp);
    CommandSerial.println(F("Valid arguments are:"));
    CommandSerial.println(F("  - 'Normal'"));
    CommandSerial.println(F("  - 'Alternate'"));
    CommandSerial.println(F("Try '$Help' for more information."));
  }
}

void Cmd_SetSym(CommandParameter &parameters)
{
  aprsSettings.Sym = parameters.NextParameter();   //or use also strlcpy???
}

void Cmd_SetPHG(CommandParameter &parameters)
{
  aprsSettings.Power = parameters.NextParameterAsInteger();
  aprsSettings.Height = parameters.NextParameterAsInteger();
  aprsSettings.Gain = parameters.NextParameterAsInteger();
  aprsSettings.Directivity = parameters.NextParameterAsInteger();
}

void Cmd_SaveSet(CommandParameter &parameters)
{
  aprsSettings.checksum = 0xFFFF;

  for(int i=0; i<sizeof(aprsSettings)-sizeof(aprsSettings.checksum); i++)
  {
    aprsSettings.checksum = _crc16_update(aprsSettings.checksum, *((byte*)&aprsSettings + i));
  }
  EEPROM.put(eeAddress, aprsSettings);

  CommandSerial.println(F("Saved APRS Settings to EEPROM"));
}

void Cmd_LoadSet(CommandParameter &parameters)
{
  bool Successful = LoadSet();
  if(Successful)
  {
    CommandSerial.println(F("Loaded APRS Settings from EEPROM"));
  }
  else
  {
    aprsSettings = aprsDefaultSettings;
    CommandSerial.println(F("No vaild APRS Settings found in EEPROM,"));
    CommandSerial.println(F("Loaded APRS default Settings"));
  }

  ApplySet();
}

void Cmd_ApplySet(CommandParameter &parameters)
{
  ApplySet();
  CommandSerial.println(F("Applied current APRS Settings"));
}

void Cmd_PrintSet(CommandParameter &parameters)
{
  APRS_printSettings(CommandSerial);
}

void Cmd_StartupSet(CommandParameter &parameters)
{
  char *tmp = parameters.NextParameter();

  if(strcmp(tmp, "EEPROM") == 0)
  {
    loadEEPROMSettings = true;
  }
  else if(strcmp(tmp, "Default") == 0)
  {
    loadEEPROMSettings = false;
  }
  else
  {
    CommandSerial.print(F("StartupSet: invalid argument"));
    CommandSerial.println(tmp);
    CommandSerial.println(F("Valid arguments are:"));
    CommandSerial.println(F("  - 'EEPROM'"));
    CommandSerial.println(F("  - 'Default'"));
    CommandSerial.println(F("Try '$Help' for more information."));
  }
}

void Cmd_DefaultSet(CommandParameter &parameters)
{
  aprsSettings = aprsDefaultSettings;
  ApplySet();
  CommandSerial.println(F("Loaded APRS default Settings"));
}

void Cmd_Help(CommandParameter &parameters)
{
  CommandSerial.println(F("Usage: $[Command]"));
  CommandSerial.println(F("Commands:"));
  CommandSerial.println(F("  SetCall [Callsign] [SSID]                           set your callsign and SSID"));
  CommandSerial.println(F("  SetDest [Callsign] [SSID]                           set the destination callsign and SSID"));
  CommandSerial.println(F("  SetPath1 [Callsign] [SSID]                          set the path 1 callsign and SSID"));
  CommandSerial.println(F("  SetPath2 [Callsign] [SSID]                          set the path 2 callsign and SSID"));
  CommandSerial.println(F("  SetPreamble [Preamble]                              set the TX preamble"));
  CommandSerial.println(F("  SetTail [Tail]                                      set the TX tail"));
  CommandSerial.println(F("  SetSymTable ['Normal', 'Alternate']                 choose between 'Normal' or 'Alternate' symbol table"));
  CommandSerial.println(F("  SetSym [Symbol]                                     set your symbol"));
  CommandSerial.println(F("  SetPHG [Power] [Height] [Gain] [Directivity]        set the power/height/gain/directivity information of your station"));
  CommandSerial.println(F("  SaveSet                                             save the current APRS settings to the EEPROM"));
  CommandSerial.println(F("  LoadSet                                             load and apply the settings saved in the EEPROM"));
  CommandSerial.println(F("  ApplySet                                            apply the current APRS settings to LibAPRS"));
  CommandSerial.println(F("  PrintSet                                            print the current APRS settings"));
  CommandSerial.println(F("  StartupSet ['Default', 'EEPROM']                    choose whether 'Default' or 'EEPROM' settings are loaded and applied at startup"));
  CommandSerial.println(F("                                                      default is 'EEPROM'"));
  CommandSerial.println(F("  DefaultSet                                          load and apply the default APRS settings"));
  CommandSerial.println(F("  Help                                                display this help"));
  CommandSerial.println();
  CommandSerial.println(F("APRS-Tracker version 0.0, URL: <https://github.com/jstjst/APRS-Tracker>"));
}

void Cmd_Unknown()
{
  CommandSerial.println(F("command not found"));
  CommandSerial.println(F("Try '$Help' for more information"));
}
