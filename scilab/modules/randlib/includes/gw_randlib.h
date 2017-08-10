
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
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

#ifndef __GW_RANDLIB_H__
#define __GW_RANDLIB_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_randlib.h"
/*--------------------------------------------------------------------------*/
RANDLIB_IMPEXP int gw_randlib(void);
/*--------------------------------------------------------------------------*/
RANDLIB_IMPEXP int sci_Rand(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_RANDLIB_H__ */
/*--------------------------------------------------------------------------*/
