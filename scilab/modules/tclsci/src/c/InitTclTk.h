/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __INITTCLTK__
#define __INITTCLTK__

/*#include "TCL_Global.h"*/
#include "BOOL.h"

/**
 * Initialize TCL/TK
 * @return if started (TRUE) or not (FALSE)
 */
BOOL initTCLTK(void);

/**
 * @TODO add comment
 *
 * @return <ReturnValue>
 */
int OpenTCLsci(void);

/**
 * TODO : comment
 * @return
 */
BOOL CloseTCLsci(void);

/**
 * Set if tcl/tk is started or not
 * @param isTkSet if enable or not
 */
void setTkStarted(BOOL isTkSet);

/**
 * Set if tcl/tk is started or not
 * @return says if TCL/TK is started (TRUE) or not (FALSE)
 */
BOOL isTkStarted(void);

#endif /* __INITTCLTK__ */
/*--------------------------------------------------------------------------*/
