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
extern int StartJVM(char *SCILAB_PATH); 
#endif

#if WITH_TK
extern void initTCLTK ();
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
	#endif

	#ifdef _MSC_VER
	SCIPATH=GetScilabDirectory(TRUE);
	InitializeHashTableScilabErrors(SCIPATH);
	StartJVM(SCIPATH); 
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/
