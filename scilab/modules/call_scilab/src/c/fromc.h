/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
