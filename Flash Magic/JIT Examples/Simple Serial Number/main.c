/**************************************************************
Flash Magic Just In Time (JIT) Example
Serial Number/Datestamp Generation
Version 1.00
**************************************************************/

// include needed header files
#include <stdio.h>
#include <time.h>
#include "hexfile.h"

// location in flash memory to store serial number at
#define SERIALNUMBER_ADDR 0x1000

// array to hold four-byte serial number/datestamp
unsigned char serialnumber[4];

// function to generate the actual serial number and put it into
// an intel hex record ready for programming
// outputfile = handle of file to write hex record
void generate(FILE *outputfile)
{
  char hexrecord[100];
  unsigned long now;

  // get the current time
  time((time_t *)&now);

  // store current time in serial number array
  // ensuring big-endian format for 8051s
  serialnumber[0] = (now >> 24) & 0xFF;
  serialnumber[1] = (now >> 16) & 0xFF;
  serialnumber[2] = (now >> 8 ) & 0xFF;
  serialnumber[3] = now & 0xFF;

  // create a hex record holding the serial number  
  hexfile_generate_record(SERIALNUMBER_ADDR, 4, serialnumber, hexrecord);

  // output the hex record
  fprintf(outputfile, "%s\n", hexrecord);
}
 
// function to generate some feedback for the user
// in this case we tell the user what the serial number generated
// is so it can be noted and maybe put on a sticker somewhere
// outputfile = handle of file to write output
void useroutput(FILE *outputfile)
{
  // convert the serial number to a single valie
  unsigned long sn = ((unsigned long)serialnumber[0] << 24) |
                     ((unsigned long)serialnumber[1] << 16) |
                     ((unsigned long)serialnumber[2] << 8 ) |
                     ((unsigned long)serialnumber[3]);

  // output serial number
  fprintf(outputfile, "Serial Number: %8.8lX", sn);
}

// function to remove carriage returns and newlines from the
// end of a line
void strip_newline(char *text)
{
  char *ptr = text;
  while (*ptr)
  {
    if (*ptr == '\n' || *ptr == '\r') *ptr = '\0';
    ptr++;
  }
}

// main function
int main(int argc, char **argv)
{
  FILE *commandfile;
  FILE *datafile;
  FILE *userfile;
  char datafilepath[2000];
  char userfilepath[2000];
  unsigned char buffer[100];

  // if we didn't receive two arguments then we can't do
  // anything
  if (argc < 2) return 0;

  // try to open the command file passed in the first argument
  commandfile = fopen(argv[1], "r");
  if (!commandfile) return 0;

  // try to read in the path to the file to create with hex
  // records
  if (fgets(datafilepath, 2000, commandfile) == NULL)
  {
    fclose(commandfile);
    return 0;
  }

  // try to read in the path to the user output file to
  // create
  if (fgets(userfilepath, 2000, commandfile) == NULL)
  {
    fclose(commandfile);
    return 0;
  }

  // we are done with the command file
  fclose(commandfile);

  // get rid of junk we don't need
  strip_newline(datafilepath);
  strip_newline(userfilepath);

  // create file for hex records and generate records
  datafile = fopen(datafilepath, "wb");
  if (!datafile) return 0;

  generate(datafile);

  fclose(datafile);

  // create file for user output and put something in there
  userfile = fopen(userfilepath, "w");
  if (!userfile) return 0;

  useroutput(userfile);

  fclose(userfile);

  // done
  return 1;
}
