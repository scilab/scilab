#include "../machine.h"
#include "../version.h"

extern void sciprint __PARAMS((char *fmt,...));

extern int sci_show_banner ; 

void banner(void)
{
	#ifdef WIN32
	sciprint("        ___________________________________________\r\n");
	#else
    sciprint("        -------------------------------------------\r\n");
	#endif

    sciprint("                         %s\r\n\n",VERSION);
    sciprint("                  Copyright (c) 1989-2005          \r\n");
    sciprint("              Consortium Scilab (INRIA, ENPC)      \r\n");

    #ifdef WIN32
	sciprint("        ___________________________________________\r\n");
	#else
    sciprint("        -------------------------------------------\r\n");
	#endif
}

int C2F(banier)(integer *flag)
{
  if (*flag != 999 && sci_show_banner == 1)
  {
	banner();
  }
  return 0;
} 
  

