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

	#if ( defined(WITH_GTK) || defined(__CYGWIN32__) || defined(__MINGW32__) )
		sciprint("\r\n\r\nWarning :No Scilab GTK version is provided and supported by the operational\r\nteam of Scilab Consortium.\r\n");
		sciprint("Same way about MinGW and Cygwin.\r\n\r\n");
	#endif

	#if defined(__MSC__) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) /* Visual Studio C++ 6.0 */
		sciprint("\r\n\r\nWarning : No Scilab version built with Microsoft Visual C++ 6.0 is provided\r\nand supported by the operational team of Scilab Consortium\r\n\r\n");
	#endif

	#if defined(__APPLE__)
		sciprint("\r\n\r\nWarning : No Scilab Apple version is provided and supported by the operational\r\nteam of Scilab Consortium.\r\n\r\n");
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


