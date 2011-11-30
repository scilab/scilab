/*
 *  Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __STRINGTODOUBLE_H__
#define __STRINGTODOUBLE_H__

#include "BOOL.h"
#include "dynlib_string.h"

#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"
#define PosInfString "+Inf"
#define NegNanString "-Nan" /* no sense but used by some users */
#define PosNanString "+Nan" /* no sense but used by some users */

typedef enum {
    STRINGTODOUBLE_NO_ERROR = 0,
    STRINGTODOUBLE_MEMORY_ALLOCATION = 1,
    STRINGTODOUBLE_NOT_A_NUMBER = 2,
    STRINGTODOUBLE_ERROR = 3
} stringToDoubleError;

STRING_IMPEXP double stringToDouble(const char *pSTR,
                       BOOL bConvertByNAN,
                       stringToDoubleError *ierr);

#endif /* __STRINGTODOUBLE_H__ */
/* ========================================================================== */
