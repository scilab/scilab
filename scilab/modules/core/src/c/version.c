/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <stdio.h>
#include "version.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
void disp_scilab_version(void)
{
	if ( (getScilabMode() == SCILAB_NWNI) || (getScilabMode() == SCILAB_NW) || (getScilabMode() == SCILAB_API) )
	{
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION);
		printf("%s\n\n",SCI_VERSION_STRING);
	}
	else
	{
		#ifdef _MSC_VER
		{
			char msg[1024];
			wsprintf(msg,_("Scilab version \"%d.%d.%d.%d\"\n%s\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION,SCI_VERSION_STRING);
			MessageBox(NULL,msg,_("Scilab Version Info."),MB_ICONINFORMATION);
		}
		#else
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_REVISION);
		printf("%s\n\n",SCI_VERSION_STRING);
		#endif
	}
}
/*-----------------------------------------------------------------------------------*/ 
