/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __STRINGSTODOUBLES_H__
#define __STRINGSTODOUBLES_H__

#include "BOOL.h"
#include "stringToDouble.h"

#ifdef __cplusplus
extern "C" {
#endif

    double *stringsToDoubles(const char **pSTRs, int nbElements,
                             BOOL bConvertByNAN,
                             stringToDoubleError *ierr);

#ifdef __cplusplus
}
#endif


#endif /* __STRINGSTODOUBLES_H__ */
// =============================================================================
