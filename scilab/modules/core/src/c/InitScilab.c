/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "MALLOC.h"
#include "getmodules.h"

#ifdef _MSC_VER
extern void SciEnv(void);
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

	#ifdef _MSC_VER
	SciEnv();
	#endif

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
