#include "../machine.h"
#include "../version.h"

extern void sciprint __PARAMS((char *fmt,...));

extern int sci_show_banner ; 

int C2F(banier)(integer *flag)
{
  if (*flag != 999 && sci_show_banner == 1) {
    sciprint("        __________________________________________\r\n");
    sciprint("                       %s\r\n\n",VERSION);
    sciprint("              Copyright (C) 1989-2004 INRIA  \r\n");
    sciprint("        __________________________________________\r\n");
  }
  return 0;
} 
  

