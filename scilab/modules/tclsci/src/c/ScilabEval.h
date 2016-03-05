/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
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

#ifndef __SCILABEVAL_H__
#define __SCILABEVAL_H__

#include "TCL_Global.h"

/**
 * @TODO add comment
 *
 * @param clientData
 * @param theinterp
 * @param objc
 * @param argv
 * @return <ReturnValue>
 */
int TCL_EvalScilabCmd(ClientData clientData, Tcl_Interp * theinterp, int objc, CONST char ** argv);

#endif /* __SCILABEVAL_H__ */
