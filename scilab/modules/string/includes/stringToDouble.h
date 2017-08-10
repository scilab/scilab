/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __STRINGTODOUBLE_H__
#define __STRINGTODOUBLE_H__

#include "dynlib_string.h"
#include "BOOL.h"
#include <wchar.h>

#define NanString "Nan"
#define NanStringW L"Nan"
#define InfString "Inf"
#define InfStringW L"Inf"
#define NegInfString "-Inf"
#define NegInfStringW L"-Inf"
#define PosInfString "+Inf"
#define PosInfStringW L"+Inf"
#define NegNanString "-Nan" /* no sense but used by some users */
#define NegNanStringW L"-Nan" /* no sense but used by some users */
#define PosNanString "+Nan" /* no sense but used by some users */
#define PosNanStringW L"+Nan" /* no sense but used by some users */
#define ScilabNanString "%nan"
#define ScilabNanStringW L"%nan"
#define ScilabInfString "%inf"
#define ScilabInfStringW L"%inf"
#define ScilabNegInfString "-%inf"
#define ScilabNegInfStringW L"-%inf"
#define ScilabPosInfString "+%inf"
#define ScilabPosInfStringW L"+%inf"
#define ScilabNegNanString "-%nan" /* no sense but used by some users */
#define ScilabNegNanStringW L"-%nan" /* no sense but used by some users */
#define ScilabPosNanString "+%nan" /* no sense but used by some users */
#define ScilabPosNanStringW L"+%nan" /* no sense but used by some users */
#define ScilabPiString "%pi"
#define ScilabPiStringW L"%pi"
#define ScilabNegPiString "-%pi"
#define ScilabNegPiStringW L"-%pi"
#define ScilabPosPiString "+%pi"
#define ScilabPosPiStringW L"+%pi"
#define ScilabEString "%e"
#define ScilabEStringW L"%e"
#define ScilabPosEString "+%e"
#define ScilabPosEStringW L"+%e"
#define ScilabNegEString "-%e"
#define ScilabNegEStringW L"-%e"
#define ScilabEpsString "%eps"
#define ScilabEpsStringW L"%eps"
#define ScilabPosEpsString "+%eps"
#define ScilabPosEpsStringW L"+%eps"
#define ScilabNegEpsString "-%eps"
#define ScilabNegEpsStringW L"-%eps"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
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
STRING_IMPEXP double stringToDoubleW(const wchar_t *pSTR, BOOL bConvertByNAN, stringToDoubleError *ierr);

#ifdef __cplusplus
}
#endif


#endif /* __STRINGTODOUBLE_H__ */
// =============================================================================
