#include "../machine.h"
#include "../version.h"

extern void sciprint __PARAMS((char *fmt,...));

extern int sci_show_banner ; 

int C2F(banier)(integer *flag)
{
  if (*flag != 999 && sci_show_banner == 1) {
    sciprint("        -------------------------------------------\r\n");
    sciprint("                       %s\r\n\n",VERSION);
    sciprint("                  Copyright (c) 1989-2004          \r\n");
    sciprint("              Consortium Scilab (INRIA, ENPC)      \r\n");
    sciprint("        -------------------------------------------\r\n");
  }
  return 0;
} 
  

