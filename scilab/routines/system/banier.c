/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "../machine.h"
#include "../version.h"
/*-----------------------------------------------------------------------------------*/
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
extern int sci_show_banner ; 
/*-----------------------------------------------------------------------------------*/
void banner(void)
{
	#ifdef WIN32
	  sciprint("        ___________________________________________\r\n");
	#else
    sciprint("        -------------------------------------------\r\n");
	#endif

    sciprint("                         %s\r\n\n",VERSION);
    sciprint("                  Copyright (c) 1989-2006          \r\n");
    sciprint("              Consortium Scilab (INRIA, ENPC)      \r\n");

  #ifdef WIN32
	  sciprint("        ___________________________________________\r\n");
	#else
    sciprint("        -------------------------------------------\r\n");
	#endif

	#if ( defined(WITH_GTK) )
              sciprint("\r\n\r\n");
              sciprint("Warning: the operational team of the Scilab Consortium\r\n");
              sciprint("   does not directly provide nor officially support this\r\n");
              sciprint("   version of Scilab built with the GTK widget\r\n\r\n");
	#endif

	#if ( defined(__CYGWIN32__) )
              sciprint("\r\n\r\n");
              sciprint("Warning: the operational team of the Scilab Consortium\r\n");
              sciprint("   does not directly provide nor officially support this\r\n");
              sciprint("   version of Scilab built with Cygwin\r\n\r\n");
	#endif

	#if ( defined(__MINGW32__) )
              sciprint("\r\n\r\n");
              sciprint("Warning: the operational team of the Scilab Consortium\r\n");
              sciprint("   does not directly provide nor officially support this\r\n");
              sciprint("   version of Scilab built with MinGW\r\n\r\n");
	#endif

	#if defined(__MSC__) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) 
        /* Visual Studio C++ 6.0 */
              sciprint("\r\n\r\n");
              sciprint("Warning: the operational team of the Scilab Consortium\r\n");
              sciprint("   does not directly provide nor officially support the\r\n");
              sciprint("   version of Scilab compiled with Microsoft Visual C++ 6.0\r\n\r\n");
	#endif

	#if defined(__APPLE__)
              sciprint("\r\n\r\n");
              sciprint("Warning: the operational team of the Scilab Consortium\r\n");
              sciprint("   does not directly provide nor officially support the\r\n");
              sciprint("   port of Scilab to MacOS\r\n\r\n");
	#endif

}
/*-----------------------------------------------------------------------------------*/
int C2F(banier)(integer *flag)
{
  if (*flag != 999 && sci_show_banner == 1)
  {
		banner();
  }
  return 0;
} 
/*-----------------------------------------------------------------------------------*/


