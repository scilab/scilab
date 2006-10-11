#include "SciEnv.h"
#include "machine.h"
#define PATH_MAX 1024


/********************************************************************************************************/
/* Les variables d'environnements SCI,TCL_LIBRARY,TK_LIBRARY */
/* sont définies directement dans scilex */
/* scilex peut donc etre executé seul */
/********************************************************************************************************/
void SciEnv(void) {
#ifdef _MSC_VER
	char *SCIPathName=NULL;

	SCIPathName=GetScilabDirectory(TRUE);

	// Correction Bug 1579
	if (!IsTheGoodShell()) 
	{
		if ( (!Set_Shell()) || (!IsTheGoodShell()))
		{
			MessageBox(NULL,MSG_SCIMSG121,MSG_WARNING22,MB_ICONWARNING|MB_OK);
		}
	}

	set_sci_env(SCIPathName);
	if (SCIPathName) {FREE(SCIPathName);SCIPathName=NULL;}
#else
	char *buf;
	int  *nbuf;
	long int  lbuf;
	C2F(getsci)(buf,nbuf,PATH_MAX);
#endif
}
