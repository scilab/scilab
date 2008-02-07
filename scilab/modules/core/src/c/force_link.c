/*--------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/ 
#include "machine.h"

#ifdef _MSC_VER
#include "stack-def.h"

/* Force some definitions "globals" (linear_algebra)*/

SCIGSCH_struct C2F(scigsch);
SCISCH_struct C2F(scisch);
SCIZGSCH_union C2F(scizgsch);

IERODE_struct C2F(ierode);

#else
/***********************************************************
* Unused function just here to force linker to load some 
* functions 
***********************************************************/
extern int   Blas_contents __PARAMS((int));
extern int   Lapack_contents __PARAMS((int));
extern int   Calelm_contents __PARAMS((int));
extern int   Sun_contents __PARAMS((int));
extern int   System2_contents __PARAMS((int));
extern int   System_contents __PARAMS((int));
extern int   Intersci_contents __PARAMS((int));
extern int   Sparse_contents __PARAMS((int));

int ForceLink(void);
int ForceLink(void)
{
	/*  
	commented for test purpose by Sylvestre (no sure it is mandatory
	Blas_contents(0);
	Lapack_contents(0);
	Calelm_contents(0);
	Sun_contents(0);
	System2_contents(0);
	System_contents(0);
	Intersci_contents(0);
	Sparse_contents(0);*/
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/ 
