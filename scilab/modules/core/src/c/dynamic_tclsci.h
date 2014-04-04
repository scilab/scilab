/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __DYNAMIC_TCLSCI_H__
#define __DYNAMIC_TCLSCI_H__

#include "BOOL.h"
#include "machine.h" /* C2F */

/**
* see SCI/modules/tclsci/sci_gateway/c/gw_tclsci.c
*/
int gw_dynamic_tclsci(void);

/*
* see SCI/modules/tclsci/src/c/setenvtcl.c
*/
int dynamic_setenvtcl(const char *string, const char *value);

/**
* call & load TerminateTclTk in tclsci dynamic library
* see SCI/modules/tclsci/src/c/TerminateTclTk.c
* @return TRUE or FALSE
*/
BOOL dynamic_TerminateTclTk(void);

#endif /* __DYNAMIC_TCLSCI_H__ */
/*--------------------------------------------------------------------------*/
