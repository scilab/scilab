/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "machine.h" /* C2F */
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
int XTKsocket = 0;
#endif
/*--------------------------------------------------------------------------*/
int CloseTCLsci(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL Set_TK_LIBRARY_PATH(char *DefaultPath)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
int gw_tclsci(void)
{
    Scierror(999, _("Scilab TCL/TK module not installed.\n"));
    return 0;
}
/*--------------------------------------------------------------------------*/
void initTCLTK(void)
{
}
/*--------------------------------------------------------------------------*/
int sciDestroyGed( int figureId )
{
    return 1;
}
/*--------------------------------------------------------------------------*/
int isGedOpenedOn( int figureId )
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int setenvtcl(char *string, char *value)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL InitializeTclTk(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateTclTk(void)
{
    return TRUE;
}
/*--------------------------------------------------------------------------*/

