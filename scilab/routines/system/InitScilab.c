/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#ifdef _MSC_VER
#include "../os_specific/win_mem_alloc.h"  /* malloc */
#else
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#endif

#ifdef _MSC_VER
extern void SciEnv(void);
extern int InitializeHashTableScilabErrors(char* SCIPATH);
extern char *GetScilabDirectory(BOOL UnixStyle);
/* extern int StartJVM(void); */
#endif

#if WITH_TK
extern void initTCLTK ();
extern void flushTKEvents ();
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	char *SCIPATH=NULL;

	#ifdef _MSC_VER
	SciEnv();
	#endif

	#if WITH_TK
	initTCLTK();
	flushTKEvents();
	#endif

	#ifdef _MSC_VER
	SCIPATH=GetScilabDirectory(TRUE);
	InitializeHashTableScilabErrors(SCIPATH);
	/* StartJVM(); */
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/
