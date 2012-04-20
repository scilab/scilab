/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include "stack-def.h"
#include "typename.h"
#include "funcprot.h"

/* Force some definitions "globals" (linear_algebra)*/

SCIGSCH_struct C2F(scigsch);
SCISCH_struct C2F(scisch);
SCIZGSCH_union C2F(scizgsch);

IERODE_struct C2F(ierode);

/* manages CTRL-C event */
BASBRK_struct C2F(basbrk);

/** 
* simpmd : rational fraction simplification mode
* used in polynomials and inisci.f (core)
*/
CSIMP_struct C2F(csimp);

/* Previously on Windows commons were defined at first use
*  it was a hazardous way ...
*  We force definitions in core
*/

ADRE_struct C2F(adre);
CHA1_struct C2F(cha1);
COM_struct C2F(com);
ERRGST_struct C2F(errgst);
INTERSCI_struct C2F(intersci);
IOP_struct C2F(iop);
MTLBC_struct C2F(mtlbc);
RECU_struct C2F(recu);
STACK_struct C2F(stack);
VSTK_struct C2F(vstk);
DBG_struct C2F(dbg);
TYPE_struct C2F(typnams);
MPROT_struct C2F(mprot);

struct {
    int byptr[40], nbyptr;
} C2F(ippty);

struct {
    int bkflag;
    char bckbuf[512];
} C2F(keepme);

struct {
    int lwk;
} C2F(ptkeep);

#else
/***********************************************************
* Unused function just here to force linker to load some 
* functions 
***********************************************************/
/*
extern int   Blas_contents(int);
extern int   Lapack_contents(int);
extern int   Calelm_contents(int);
extern int   Sun_contents(int);
extern int   System2_contents(int);
extern int   System_contents(int);
extern int   Intersci_contents(int);
extern int   Sparse_contents(int);
*/
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
	Sparse_contents(0);
	*/
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/ 
