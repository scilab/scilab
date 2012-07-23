/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include "machine.h"
#include "stack-def.h"
#include "typename.h"
#include "funcprot.h"
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../../../bin/libxml2.lib")
#pragma comment(lib, "../../../../bin/libintl.lib")
#pragma comment(lib, "../../../../bin/blasplus.lib")
#pragma comment(lib, "../../../../bin/lapack.lib")
/*-----------------------------------------------------------------------------------*/
/* force linking fortran libraries */
#pragma comment(lib, "../../../../bin/core_f.lib")
#pragma comment(lib, "../../../../bin/elementary_functions_f.lib")
#pragma comment(lib, "../../../../bin/linpack_f.lib")
#pragma comment(lib, "../../../../bin/output_stream_f.lib")
/*-----------------------------------------------------------------------------------*/
extern int C2F(withf2c)(int *bOK);
/*-----------------------------------------------------------------------------------*/
/* Force some definitions "globals" (commons fortran) */

/* used by linear_algebra */

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

struct
{
    int byptr[40], nbyptr;
} C2F(ippty);

struct
{
    int bkflag;
    char bckbuf[4096];
} C2F(keepme);

struct
{
    int lwk;
} C2F(ptkeep);
/*-----------------------------------------------------------------------------------*/
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
/*-----------------------------------------------------------------------------------*/
BOOL BuildWithVS8ExpressF2C(void)
{
    BOOL bOK = FALSE;
    C2F(withf2c)(&bOK);
    return bOK;
}
/*-----------------------------------------------------------------------------------*/
