#include "../machine.h"
#include "../version.h"

extern void sciprint __PARAMS((char *fmt,...));

extern int sci_show_banner ; 

int C2F(banier)(integer *flag)
{
  if (*flag != 999 && sci_show_banner == 1) {
    sciprint("                    ==========\r\n");
    sciprint("                    %s\r\n",VERSION);
    sciprint("        Copyright (C) 1989-2003 INRIA/ENPC\r\n");
    sciprint("                    ==========\r\n\r\n");
  }
  return 0;
} 
  

