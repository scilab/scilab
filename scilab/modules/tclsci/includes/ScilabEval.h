/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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

#ifndef __SCILABEVAL__
#define __SCILABEVAL__

#include "dynlib_tclsci.h"

/**
 * @TODO add comment
 *
 * @param clientData
 * @param theinterp
 * @param objc
 * @param argv
 * @return <ReturnValue>
 */
TCLSCI_IMPEXP  int TCL_EvalScilabCmd(ClientData clientData, Tcl_Interp * theinterp, int objc, CONST char ** argv);

#endif /* __SCILABEVAL__ */
/*--------------------------------------------------------------------------*/
