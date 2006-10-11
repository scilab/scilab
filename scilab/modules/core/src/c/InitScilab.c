/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "MALLOC.h"
#include "getmodules.h"

extern void SciEnv(void);

#ifdef _MSC_VER
extern int InitializeHashTableScilabErrors(char* SCIPATH);
extern char *GetScilabDirectory(BOOL UnixStyle);
extern int StartJVMs(char *SCILAB_PATH); 
#endif

#ifdef WITH_TK
extern void initTCLTK ();
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	char *SCIPATH=NULL;

	SciEnv();

	getmodules();

	#ifdef WITH_TK
		initTCLTK();
	#endif

	#ifdef _MSC_VER
	SCIPATH=GetScilabDirectory(TRUE);
	InitializeHashTableScilabErrors(SCIPATH);
	StartJVMs(SCIPATH); 
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/
