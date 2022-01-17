//---------------------------------------------------------------------------

// include header files we need
#include <vcl.h>
#include <stdio.h>
#include <ctype.h>
#include <registry.hpp>
#pragma hdrstop

// global application return value - passed back to Flash Magic
// when the program finishes
extern int returnvalue;

// endian values
#define BIGENDIAN    0
#define LITTLEENDIAN 1

// default settings
#define DEFAULTSERIALNUMBER "1234"
#define DEFAULTLOCATION     "1000"
#define DEFAULTENDIAN       BIGENDIAN

// registry location for storing settings
#define KEYNAME "\\Software\\Embedded Systems Academy\\ManualSerialNumberJITModule\\"

// include our header file
#include "mainform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tmain *main;
//---------------------------------------------------------------------------
// the main dialog window constructor
__fastcall Tmain::Tmain(TComponent* Owner)
        : TForm(Owner)
{
  // init the close requested flag
  closerequested = false;

  // check to make sure we have enough command line parameters passed from
  // Flash Magic
  if (ParamCount() < 1)
  {
    Application->MessageBox("Invalid number of parameters", "Serial Number", MB_OK);
    // close the window - we are done
    closerequested = true;
    return;
  }

  // load settings (if any) from registry, otherwise use defaults
  LoadSettings();
}
//---------------------------------------------------------------------------
// function called when the OK button is clicked
void __fastcall Tmain::okbuttonClick(TObject *Sender)
{
  unsigned int serialnumber;
  unsigned int seriallocation;
  bool endian;

  // get the serial number and location
  serialnumber =   (unsigned int)strtoul(number->Text.c_str(), (char **)NULL, 10);
  seriallocation = (unsigned int)strtoul(location->Text.c_str(), (char **)NULL, 16);

  // get the endian-ness we want
  if (bigendian->Checked)
    endian = BIGENDIAN;
  else
    endian = LITTLEENDIAN;

  // generate the necessary hex records and files that Flash Magic is expected
  if (!generatehex(serialnumber, seriallocation, endian))
  {
    // error occurred - let Flash Magic know by returning a zero
    returnvalue = 0;
  }
  else
  {
    // no errors occurred - let Flash Magic know by returning a one
    returnvalue = 1;
  }

  // close the window - we are done
  Close();
}
//---------------------------------------------------------------------------
// called when the window is redrawn
void __fastcall Tmain::FormPaint(TObject *Sender)
{
  // time to close the window and end the application?
  if (closerequested) Close();
}
//---------------------------------------------------------------------------
// generate the hex record for Flash Magic to program into the device
// passed is the serial number, flash memory location and endian-ness to use
int __fastcall Tmain::generatehex(unsigned int serialnumber, unsigned int seriallocation, bool endian)
{
  FILE *commandfile;
  FILE *datafile;
  FILE *userfile;
  char datafilepath[2000];
  char userfilepath[2000];
  unsigned char buffer[100];
  char hexrecord[100];
  unsigned char serial[2];

  // swap byte order if needed
  if (endian == BIGENDIAN)
  {
    serial[0] = (serialnumber >> 8) & 0xFF;
    serial[1]  = serialnumber & 0xFF;
  }
  else
  {
    serial[0]  = serialnumber & 0xFF;
    serial[1] = (serialnumber >> 8) & 0xFF;
  }

  // open the command file
  commandfile = fopen(ParamStr(1).c_str(), "r");
  if (!commandfile) return 0;

  // read in the path to the data file to create
  if (fgets(datafilepath, 2000, commandfile) == NULL)
  {
    fclose(commandfile);
    return 0;
  }

  // read in the path to the user file to create
  if (fgets(userfilepath, 2000, commandfile) == NULL)
  {
    fclose(commandfile);
    return 0;
  }

  // done with the command file
  fclose(commandfile);

  // get rid of newline junk at the end of the path names
  strip_newline(datafilepath);
  strip_newline(userfilepath);

  // open the data file
  datafile = fopen(datafilepath, "wb");
  if (!datafile) return 0;

  // create a hex record holding the serial number
  hexfile_generate_record(seriallocation, 2, serial, hexrecord);

  // output the hex record to the data file
  fprintf(datafile, "%s\n", hexrecord);

  // done with the data file
  fclose(datafile);

  // don't output anything for the user
  userfile = fopen(userfilepath, "w");
  if (!userfile) return 0;
  fclose(userfile);

  // success
  return 1;
}

// function to strip newline junk from the end of a line
void __fastcall Tmain::strip_newline(char *text)
{
  char *ptr = text;
  while (*ptr)
  {
    if (*ptr == '\n' || *ptr == '\r') *ptr = '\0';
    ptr++;
  }
}

// func: hexfile_generate_record
// desc: converts raw data in inbuffer into a hex record in outbuffer
// passed: starting address for record, number of bytes in record, input and output buffers
// returns: nothing
void __fastcall Tmain::hexfile_generate_record(unsigned int addr, int bytes, unsigned char *inbuffer, char *outbuffer)
{
  int byte;
  unsigned char checksum;

  outbuffer[0] = ':';
  outbuffer[1] = hex_highchar(bytes);
  outbuffer[2] = hex_lowchar(bytes);
  outbuffer[3] = hex_char16(addr, 0);
  outbuffer[4] = hex_char16(addr, 1);
  outbuffer[5] = hex_char16(addr, 2);
  outbuffer[6] = hex_char16(addr, 3);
  outbuffer[7] = '0';
  outbuffer[8] = '0';

  for (byte = 0; byte < bytes; byte++)
  {
    outbuffer[9 + (byte * 2)] = hex_highchar(inbuffer[byte]);
    outbuffer[10 + (byte * 2)] = hex_lowchar(inbuffer[byte]);
  }
  checksum = hexfile_calculate_checksum(outbuffer, 9 + (bytes * 2));
  outbuffer[9 + (bytes * 2)] = hex_highchar(checksum);
  outbuffer[10 + (bytes * 2)] = hex_lowchar(checksum);
  outbuffer[11 + (bytes * 2)] = 0;
}

// func: hex_highchar
// desc: returns a character containing the high nibble of
// a value. e.g. A2H -> 'A'
// passed: value
// returns: character
unsigned char __fastcall Tmain::hex_highchar(unsigned char value)
{
  char buffer[6];
  sprintf(buffer, "%2.2X", value);
  return buffer[0];
}

// func: hex_lowchar
// desc: returns a character containing the low nibble of
// a value. e.g. A2H -> '2'
// passed: value
// returns: character
unsigned char __fastcall Tmain::hex_lowchar(unsigned char value)
{
  char buffer[6];
  sprintf(buffer, "%2.2X", value);
  return buffer[1];
}

// func: hex_char16
// desc: returns a character containing a specific nibble of a
// 16-bit value. e.g. A2B3H -> pos 0 = 'A', pos 3 = '3'
// passed: value and nibble number 0 - 3
// returns: character
unsigned char __fastcall Tmain::hex_char16(unsigned int value, int pos)
{
  char buffer[12];
  sprintf(buffer, "%4.4X", value);
  return buffer[pos];
}

// func: hex_calculate_value
// desc: takes two chars such as '4' and 'A' and converts them to the
// hex value 4AH
// passed: high and low characters
// returns: 8-bit value
unsigned char __fastcall Tmain::hex_calculate_value(unsigned char high, unsigned char low)
{
  unsigned char value, nibble;

  nibble = high - '0';
  if (nibble > 9 && nibble < 23) nibble -= 7;
  if(nibble > 22) nibble -= 39;
  value = (16 * nibble);

  nibble = low - '0';
  if (nibble > 9 && nibble < 23) nibble -= 7;
  if (nibble > 22) nibble -= 39;
  value += nibble;

  return value;
}

// func: hexfile_calculate_checksum
// desc: calculates checksum based on hex file data
// passed: pointer to buffer containing hexfile data, number of bytes of data
// returns: checksum
unsigned char __fastcall Tmain::hexfile_calculate_checksum(unsigned char *buffer, int bytes)
{
  long checksum = 0;
  int i;

  for (i = 1; i < bytes; i+=2)
  {
    checksum += hex_calculate_value(buffer[i], buffer[i+1]);
  }

  checksum = 0x100 - (unsigned char)checksum;

  return checksum;
}

// saves the settings in the registry
void __fastcall Tmain::SaveSettings(void)
{
  // create new TRegistry object
  TRegistry *registry = new TRegistry;

  // by default keys are created under HKEY_CURRENT_USER
  // open/create key - if error opening/creating then give up
  // note: OpenKey will fail on win95 if 2nd param is false for some unknown reason
  if (!registry->OpenKey(AnsiString(KEYNAME),true))
  {
    delete registry;
    return;
  }

  // store settings in registry
  registry->WriteString(AnsiString("serialnumber"), number->Text);
  registry->WriteString(AnsiString("location"), location->Text);
  registry->WriteBool(AnsiString("bigendian"), bigendian->Checked);

  delete registry;
}

// loads the settings from the registry. if not found then use the default settings
void __fastcall Tmain::LoadSettings(void)
{
  // create new TRegistry object
  TRegistry *registry = new TRegistry;

  // default settings
  number->Text = DEFAULTSERIALNUMBER;
  location->Text = DEFAULTLOCATION;
#if (DEFAULTENDIAN == BIGENDIAN)
  bigendian->Checked = true;
  littleendian->Checked = false;
#else
  bigendian->Checked = false;
  littleendian->Checked = true;
#endif

  // by default keys are opened under HKEY_CURRENT_USER
  // open key - if error then give up
  // note: OpenKey will fail on win95 if 2nd param is false for some unknown reason
  if (!registry->OpenKey(AnsiString(KEYNAME),true))
  {
    delete registry;
    return;
  }

  // if entry in registry then load
  if (registry->ValueExists("serialnumber"))
  {
    // load settings from registry
    number->Text = registry->ReadString(AnsiString("serialnumber"));
  }

  // if entry in registry then load
  if (registry->ValueExists("location"))
  {
    // load settings from registry
    location->Text = registry->ReadString(AnsiString("location"));
  }

  // if entry in registry then load
  if (registry->ValueExists("bigendian"))
  {
    // load settings from registry
    bigendian->Checked = registry->ReadBool(AnsiString("bigendian"));
    littleendian->Checked = !bigendian->Checked;
  }
}

// called when the window is closed
void __fastcall Tmain::FormClose(TObject *Sender, TCloseAction &Action)
{
  // store settings in registry
  SaveSettings();        
}
//---------------------------------------------------------------------------

