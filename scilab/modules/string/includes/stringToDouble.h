/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __STRINGTODOUBLE_H__
#define __STRINGTODOUBLE_H__

#include "dynlib_string.h"
#include "BOOL.h"

#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"
#define PosInfString "+Inf"
#define NegNanString "-Nan" /* no sense but used by some users */
#define PosNanString "+Nan" /* no sense but used by some users */
#define ScilabNanString "%nan"
#define ScilabInfString "%inf"
#define ScilabNegInfString "-%inf"
#define ScilabPosInfString "+%inf"
#define ScilabNegNanString "-%nan" /* no sense but used by some users */
#define ScilabPosNanString "+%nan" /* no sense but used by some users */
#define ScilabPiString "%pi"
#define ScilabNegPiString "-%pi"
#define ScilabPosPiString "+%pi"
#define ScilabEString "%e"
#define ScilabPosEString "+%e"
#define ScilabNegEString "-%e"
#define ScilabEpsString "%eps"
#define ScilabPosEpsString "+%eps"
#define ScilabNegEpsString "-%eps"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        STRINGTODOUBLE_NO_ERROR = 0,
        STRINGTODOUBLE_MEMORY_ALLOCATION = 1,
        STRINGTODOUBLE_NOT_A_NUMBER = 2,
        STRINGTODOUBLE_ERROR = 3
    }
                           stringToDoubleError;

    /**
     * @TODO add comment
     *
     * @param pSTR
     * @param bConvertByNAN
     * @param ierr
     * @return <ReturnValue>
     */
    STRING_IMPEXP double stringToDouble(const char *pSTR, BOOL bConvertByNAN, stringToDoubleError *ierr);

#ifdef __cplusplus
}
#endif


#endif /* __STRINGTODOUBLE_H__ */
// =============================================================================
