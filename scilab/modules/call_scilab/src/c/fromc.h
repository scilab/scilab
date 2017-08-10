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
#ifndef __FROMC__
#define __FROMC__

#include <stdio.h>
#include <string.h>
#include "dynlib_call_scilab.h"
#include "BOOL.h"

/**
 * @TODO add comment
 *
 * @param void
 */
CALL_SCILAB_IMPEXP void SetFromCToON(void);

/**
 * @TODO add comment
 *
 * @param void
 * @return <ReturnValue>
 */
CALL_SCILAB_IMPEXP int IsFromC(void);

#endif /*FROMC*/
/*--------------------------------------------------------------------------*/
