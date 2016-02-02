/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#ifndef __TERMINATETCLTK_H__
#define __TERMINATETCLTK_H__

#include "dynlib_tclsci.h"
#include "BOOL.h"

/**
 * Stops the TCL engine in Scilab
 *
 * @return The result of the stop
 */
TCLSCI_IMPEXP BOOL TerminateTclTk(void);

#endif /* _TERMINATETCLTK_H__ */
/*--------------------------------------------------------------------------*/
