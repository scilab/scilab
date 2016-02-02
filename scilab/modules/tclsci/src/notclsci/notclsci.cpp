/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#include "tclsci_gw.hxx"
extern "C"
{
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
    int XTKsocket = 0;
#endif
}

/*--------------------------------------------------------------------------*/
int TclsciModule::Load()
{
    return 0;
}
/*--------------------------------------------------------------------------*/

extern "C"
{
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
}
