/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int dynamic_setenvtcl(char *string, char *value);

/**
* call & load TerminateTclTk in tclsci dynamic library
* see SCI/modules/tclsci/src/c/TerminateTclTk.c
* @return TRUE or FALSE
*/
BOOL dynamic_TerminateTclTk(void);

/** 
* check if it is with TCL/TK
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
int C2F(withtk)(int *rep);

/** 
* check if it is with TCL/TK
* @return TRUE or FALSE
*/
BOOL withtk(void);

#endif /* __DYNAMIC_TCLSCI_H__ */
/*--------------------------------------------------------------------------*/
