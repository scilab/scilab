/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
#include <Windows.h>
#endif
#include <stdio.h>
#include "version.h"
/*-----------------------------------------------------------------------------------*/ 
extern int IsConsoleMode(void);
/*-----------------------------------------------------------------------------------*/ 
void disp_scilab_version(void)
{
	if (!IsConsoleMode())
	{
		#if _MSC_VER
		{
			char msg[1024];
			wsprintf(msg,"Scilab version \"%d.%d.%d.%d\"\n%s\n",SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION,SCI_VERSION_STRING);
			MessageBox(NULL,msg,"Scilab Version Info.",MB_ICONINFORMATION);
		}
		
		#else
		printf("Scilab version \"%d.%d.%d.%d\"\n",SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION);
		printf("%s\n\n",SCI_VERSION_STRING);
		#endif
	}
	else
	{
		printf("Scilab version \"%d.%d.%d.%d\"\n",SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION);
		printf("%s\n\n",SCI_VERSION_STRING);
	}
}
/*-----------------------------------------------------------------------------------*/ 
