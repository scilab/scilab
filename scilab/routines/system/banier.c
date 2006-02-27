/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "../machine.h"
#include "../version.h"
#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
extern int sci_show_banner ; 
/*-----------------------------------------------------------------------------------*/
void banner(void)
{
	int i;
	char *line = "        ___________________________________________        ";
	int startVersion = (int)(floor((double)(strlen(line)/2)) - floor((double)(strlen(VERSION)/2)));
	
	sciprint("%s\r\n",line);
	
	/* To center the version name */
	for( i=0 ; i<startVersion ; i++ )
	{
		sciprint(" ");
	}
	
	sciprint("%s\r\n\n",VERSION);
	sciprint("                 Copyright (c) 1989-2006                   \r\n");
	sciprint("              Consortium Scilab (INRIA, ENPC)              \r\n");
	sciprint("%s\r\n",line);

	#if defined(__APPLE__)
		sciprint("\r\n\r\n");
		sciprint("Warning: the operational team of the Scilab Consortium\r\n");
		sciprint("   doesn't provide and doesn't support the port of Scilab to MacOS.\r\n\r\n");
	#endif /* __APPLE__ */

	#if ( defined(WITH_GTK) || defined(__CYGWIN32__) || defined(__MINGW32__) ) || ( defined(__MSC__) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint("\r\n\r\n");
		sciprint("Warning: the operational team of the Scilab Consortium\r\n");
		sciprint("   doesn't provide and doesn't support this version of Scilab built with\r\n");
		sciprint("   ");
  #endif

	#if defined(__MSC__) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) 
		/* Visual Studio C++ 6.0 */
		sciprint("Microsoft Visual C++ 6.0");
	#else
		#if ( defined(WITH_GTK) )
			sciprint("the GTK widget");
		#endif

		#if ( defined(__CYGWIN32__) )
			#if ( defined(WITH_GTK) )
				sciprint(", ");
			#endif
			sciprint("Cygwin");
		#endif

		#if ( defined(__MINGW32__) )
			#if ( defined(WITH_GTK) || defined(__CYGWIN32__) )
				sciprint(", ");
			#endif
			sciprint("MinGW");
		#endif
	#endif

	#if ( defined(WITH_GTK) || defined(__CYGWIN32__) || defined(__MINGW32__) ) || ( defined(__MSC__) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint(".\r\n\r\n");
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


