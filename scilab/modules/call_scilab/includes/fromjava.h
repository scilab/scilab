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
#ifndef __FROMJAVA__
#define __FROMJAVA__

#include "BOOL.h"
#include "dynlib_call_scilab.h"

/**
 * <long-description>
 *
 * @param void
 */
CALL_SCILAB_IMPEXP void SetFromJavaToON(void);

/**
 * <long-description>
 *
 * @param void
 * @return <ReturnValue>
 */
CALL_SCILAB_IMPEXP BOOL IsFromJava(void);

#endif /*FROMJAVA*/
/*--------------------------------------------------------------------------*/
