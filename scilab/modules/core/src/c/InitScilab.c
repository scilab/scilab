/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "MALLOC.h"
#include "getmodules.h"
#include "SciEnv.h"
#include "InitScilab.h"
#include "hashtable_core.h"
#include "prompt.h"
#include "sciprint.h"

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
	#ifdef _MSC_VER
	char *SCIPATH=NULL;
	#endif
	int pause=0;
	SciEnv();

	getmodules();

	if ( create_hashtable_scilab_functions(MAXTAB) == 0 ) 
	{
		sciprint("Fatal Error : Can't create table for scilab functions \n");
		exit(1);
	}

	LoadFunctionsTab();

	#ifdef WITH_TK
		initTCLTK();
	#endif

	#ifdef _MSC_VER
	SCIPATH=GetScilabDirectory(TRUE);
	InitializeHashTableScilabErrors(SCIPATH);
	StartJVMs(SCIPATH);
	if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}
	#endif

	C2F(setprlev)(&pause);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
