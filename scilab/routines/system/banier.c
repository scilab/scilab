#include "../machine.h"
#include "../version.h"

extern void sciprint __PARAMS((char *fmt,...));

int C2F(banier)(integer *flag)
{
  if (*flag != 999) {
    sciprint("                    ==========\r\n");
    sciprint("                      Scilab\r\n");
    sciprint("                    ==========\r\n\r\n");
    sciprint("                    %s",VERSION);
    sciprint("                    Copyright (C) 1989-2002 INRIA/ENPC\r\n");
  }
  return 0;
} 
  

