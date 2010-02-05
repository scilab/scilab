/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TCLGLOBAL__
#define __TCLGLOBAL__

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>

#include "dynlib_tclsci.h"
#include "stack-c.h"

#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
extern Tk_Window TKmainWindow;
extern int XTKsocket;
/*--------------------------------------------------------------------------*/
#endif /* __TCLGLOBAL__ */
/*--------------------------------------------------------------------------*/
