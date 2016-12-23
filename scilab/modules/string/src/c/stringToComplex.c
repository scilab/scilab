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
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <wctype.h>
#include "stringToComplex.h"
#include "stringToDouble.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "BOOL.h"
#include "strsubst.h"
/* ========================================================================== */
#define PlusChar '+'
#define PlusCharW L'+'
#define LessChar '-'
#define LessCharW L'-'
#define ComplexCharI 'i'
#define ComplexCharIW L'i'
#define ComplexCharJ 'j'
#define ComplexCharJW L'j'
#define ComplexScilab "%i"
#define ComplexScilabW L"%i"
#define ComplexI "i"
#define ComplexIW L"i"
/* ========================================================================== */
#ifndef _MSC_VER
#ifndef strnicmp
#define strnicmp strncasecmp
#endif
#else
#define stricmp _stricmp
#endif
#ifdef _MSC_VER
#undef strnicmp
#define strnicmp _strnicmp
#endif
/* ========================================================================== */
static int ParseNumber(const char* tx);
static int ParseNumberW(const wchar_t* tx);
static stringToComplexError ParseComplexValue(const char *tx, BOOL bConvertByNAN, double *real, double *imag);
static stringToComplexError ParseComplexValueW(const wchar_t *tx, BOOL bConvertByNAN, double *real, double *imag);
static char* midstring(const char *tx, size_t pos, int nb);
static wchar_t* midstringW(const wchar_t *tx, size_t pos, int nb);
static char *leftstring(const char *tx, size_t pos);
static wchar_t* leftstringW(const wchar_t* tx, size_t pos);
static BOOL is_unit_imaginary (const char *src, double *im);
static BOOL is_unit_imaginaryW (const wchar_t* src, double *im);
static double returnNAN(void);
/* ========================================================================== */
complexArray *stringsToComplexArray(const char **pSTRs, int nbElements,
                                    const char *decimal,
                                    BOOL bConvertByNAN,
                                    stringToComplexError *ierr)
{
    complexArray *pComplexArray = NULL;

    *ierr = STRINGTOCOMPLEX_ERROR;
    if (nbElements <= 0)
    {
        return NULL;
    }

    if (pSTRs == NULL)
    {
        *ierr = STRINGTOCOMPLEX_MEMORY_ALLOCATION;
    }
    else
    {
        pComplexArray = createComplexArrayEmpty(nbElements);
        if (pComplexArray)
        {
            int i = 0;
            for (i = 0; i < nbElements; i++)
            {
                doublecomplex dComplexValue = stringToComplex(pSTRs[i], decimal, bConvertByNAN, ierr);
                if (*ierr != STRINGTOCOMPLEX_NO_ERROR)
                {
                    freeComplexArray(pComplexArray);
                    return NULL;
                }
                else
                {
                    pComplexArray->realPart[i] = dComplexValue.r;
                    pComplexArray->imagPart[i] = dComplexValue.i;
                }
            }
            cleanImagPartComplexArray(pComplexArray);
        }
        else
        {
            *ierr = STRINGTOCOMPLEX_MEMORY_ALLOCATION;
        }
    }
    return pComplexArray;
}
/* ========================================================================== */
doublecomplex stringToComplex(const char *pSTR, const char *decimal, BOOL bConvertByNAN, stringToComplexError *ierr)
{
    doublecomplex dComplexValue;
    *ierr = STRINGTOCOMPLEX_ERROR;

    dComplexValue.r = 0.;
    dComplexValue.i = 0.;

    if (pSTR)
    {
        double real = 0.;
        double imag = 0.;
        char *pStrTemp = strsub((char*)pSTR, " ", "");

        if (pStrTemp)
        {
            char *pStrFormatted = strsub(pStrTemp, decimal, ".");
            FREE(pStrTemp);

            if (pStrFormatted)
            {
                int lenStrFormatted = (int) strlen(pStrFormatted);

                /* case .4 replaced by 0.4 */
                if (pStrFormatted[0] == '.')
                {
                    /* case .4 replaced by 0.4 */
                    char *pstStrTemp = (char*)MALLOC(sizeof(char) * (lenStrFormatted + strlen("0") + 1));
                    strcpy(pstStrTemp, "0");
                    strcat(pstStrTemp, pStrFormatted);
                    FREE(pStrFormatted);
                    pStrFormatted = pstStrTemp;
                }

                if (lenStrFormatted > 1)
                {
                    if (((pStrFormatted[0] == '+') || (pStrFormatted[0] == '-')) &&
                            (pStrFormatted[1] == '.'))
                    {
                        /* case +.4 replaced by +0.4 */
                        char *pstStrTemp = strsub(pStrFormatted, "+.", "+0.");
                        FREE(pStrFormatted);

                        /* case -.4 replaced by -0.4 */
                        pStrFormatted = strsub(pstStrTemp, "-.", "-0.");
                        FREE(pstStrTemp);
                    }
                }

                /* Case: "i", "+i", "-i", and with "j"  */
                if (is_unit_imaginary (pStrFormatted, &imag))
                {
                    *ierr = STRINGTOCOMPLEX_NO_ERROR;
                    dComplexValue.r = 0.;
                    dComplexValue.i = imag;
                }
                else
                {
                    *ierr = ParseComplexValue(pStrFormatted, bConvertByNAN, &real, &imag);
                }
                FREE(pStrFormatted);
            }
        }
        dComplexValue.r = real;
        dComplexValue.i = imag;
    }
    return dComplexValue;
}
/* ========================================================================== */
doublecomplex stringToComplexW(const wchar_t *pSTR, const wchar_t *decimal, BOOL bConvertByNAN, stringToComplexError *ierr)
{
    doublecomplex dComplexValue;
    *ierr = STRINGTOCOMPLEX_ERROR;

    dComplexValue.r = 0.;
    dComplexValue.i = 0.;

    if (pSTR)
    {
        double real = 0.;
        double imag = 0.;
        wchar_t *pStrTemp = wcssub(pSTR, L" ", L"");

        if (pStrTemp)
        {
            wchar_t *pStrFormatted = wcssub(pStrTemp, decimal, L".");
            FREE(pStrTemp);

            if (pStrFormatted)
            {
                int lenStrFormatted = (int) wcslen(pStrFormatted);

                /* case .4 replaced by 0.4 */
                if (pStrFormatted[0] == '.')
                {
                    /* case .4 replaced by 0.4 */
                    wchar_t *pstStrTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStrFormatted + wcslen(L"0") + 1));
                    wcscpy(pstStrTemp, L"0");
                    wcscat(pstStrTemp, pStrFormatted);
                    FREE(pStrFormatted);
                    pStrFormatted = pstStrTemp;
                }

                if (lenStrFormatted > 1)
                {
                    if (((pStrFormatted[0] == '+') || (pStrFormatted[0] == '-')) &&
                            (pStrFormatted[1] == '.'))
                    {
                        /* case +.4 replaced by +0.4 */
                        wchar_t *pstStrTemp = wcssub(pStrFormatted, L"+.", L"+0.");
                        FREE(pStrFormatted);

                        /* case -.4 replaced by -0.4 */
                        pStrFormatted = wcssub(pstStrTemp, L"-.", L"-0.");
                        FREE(pstStrTemp);
                    }
                }

                /* Case: "i", "+i", "-i", and with "j"  */
                if (is_unit_imaginaryW(pStrFormatted, &imag))
                {
                    *ierr = STRINGTOCOMPLEX_NO_ERROR;
                    dComplexValue.r = 0.;
                    dComplexValue.i = imag;
                }
                else
                {
                    *ierr = ParseComplexValueW(pStrFormatted, bConvertByNAN, &real, &imag);
                }
                FREE(pStrFormatted);
            }
        }
        dComplexValue.r = real;
        dComplexValue.i = imag;
    }
    return dComplexValue;
}
/* ========================================================================== */

static int ParseNumber(const char* tx)
{
    int lookahead = 0;
    int len = 0;

    if (tx[len] == NULL)
    {
        return lookahead;
    }
    if (tx[len] < 0)
    {
        return lookahead;
    }

    // Special cases: constants
    if (strlen(tx) >= 5 && (strncmp(tx, "+%eps", 5) == 0 || strncmp(tx, "-%eps", 5) == 0 || strncmp(tx, "+%nan", 5) == 0 || strncmp(tx, "-%nan", 5) == 0 || strncmp(tx, "+%inf", 5) == 0 || strncmp(tx, "-%inf", 5) == 0))
    {
        return 5;
    }
    else if (strlen(tx) >= 4 && (strncmp(tx, "%eps", 4) == 0 || strncmp(tx, "+%pi", 4) == 0 || strncmp(tx, "-%pi", 4) == 0 ||
                                 strncmp(tx, "+Inf", 4) == 0 || strncmp(tx, "-Inf", 4) == 0 || strncmp(tx, "+Nan", 4) == 0 ||
                                 strncmp(tx, "-Nan", 4) == 0 || strncmp(tx, "%nan", 4) == 0 || strncmp(tx, "%inf", 4) == 0 ))
    {
        return 4;
    }
    else if (strlen(tx) >= 3 && (strncmp(tx, "+%e", 3) == 0 || strncmp(tx, "-%e", 3) == 0 || strncmp(tx, "%pi", 3) == 0 ||
                                 strncmp(tx, "Nan", 3) == 0 || strncmp(tx, "Inf", 3) == 0 || strncmp(tx, "%pi", 3) == 0))
    {
        return 3;
    }
    else if (strlen(tx) >= 2 && strncmp(tx, "%e", 2) == 0)
    {
        return 2;
    }

    if ((tx[len] == '+') || (tx[len] == '-'))
    {
        len++;
    }

    while (isdigit(tx[len]))
    {
        len++;
    }
    lookahead = len;

    if (tx[lookahead] == '.')
    {
        lookahead++;
        len = 0;
        while (isdigit(tx[len + lookahead]))
        {
            len++;
        }
        lookahead += len;
    }

    if ((tx[lookahead] == 'E') || (tx[lookahead] == 'e') ||
            (tx[lookahead] == 'D') || (tx[lookahead] == 'd'))
    {

        lookahead++;
        if ((tx[lookahead] == '+') || (tx[lookahead] == '-'))
        {
            lookahead++;
        }

        len = 0;
        while (isdigit(tx[len + lookahead]))
        {
            len++;
        }

        lookahead += len;
    }
    return lookahead;
}
/* ========================================================================== */
static int ParseNumberW(const wchar_t* tx)
{
    int lookahead = 0;
    int len = 0;

    if (tx[len] == NULL)
    {
        return lookahead;
    }
    if (tx[len] < 0)
    {
        return lookahead;
    }

    if ((tx[len] == L'+') || (tx[len] == L'-'))
    {
        len++;
    }

    while (iswdigit(tx[len]))
    {
        len++;
    }
    lookahead = len;

    if (tx[lookahead] == L'.')
    {
        lookahead++;
        len = 0;
        while (iswdigit(tx[len + lookahead]))
        {
            len++;
        }
        lookahead += len;
    }

    if ((tx[lookahead] == L'E') || (tx[lookahead] == L'e') ||
            (tx[lookahead] == L'D') || (tx[lookahead] == L'd'))
    {

        lookahead++;
        if ((tx[lookahead] == L'+') || (tx[lookahead] == L'-'))
        {
            lookahead++;
        }

        len = 0;
        while (iswdigit(tx[len + lookahead]))
        {
            len++;
        }

        lookahead += len;
    }
    return lookahead;
}
/* ========================================================================== */
static stringToComplexError ParseComplexValue(const char *tx, BOOL bConvertByNAN, double *real, double *imag)
{
    stringToDoubleError ierrDouble = STRINGTODOUBLE_NO_ERROR;
    stringToComplexError ierr = STRINGTOCOMPLEX_NO_ERROR;
    char *rnum_string = NULL;
    char *inum_string = NULL;
    size_t lnum = 0;
    BOOL haveImagI = FALSE;
    char *modifiedTxt = NULL;
    int i = 0;

    *real = stringToDouble(tx, FALSE, &ierrDouble);
    *imag = 0;

    /* test on strlen(tx) > 1 to remove case 'e' */
    if ((int)strlen(tx) < 2)
    {
        if (ierrDouble == STRINGTODOUBLE_NO_ERROR)
        {
            ierr = (stringToComplexError) ierrDouble;
        }
        else
        {
            if (bConvertByNAN)
            {
                ierrDouble = STRINGTODOUBLE_NOT_A_NUMBER;
                *real = returnNAN();
                *imag = 0;
            }
            else
            {
                *real = 0;
                *imag = 0;
                ierr = (stringToComplexError) ierrDouble;
            }
        }
    }
    else if (ierrDouble != STRINGTODOUBLE_NO_ERROR)
    {
        modifiedTxt = strsub((char*)tx, ComplexScilab, ComplexI);
        lnum = ParseNumber(modifiedTxt);
        if (lnum <= 1)
        {
            /* manages special cases nan + nani, ... */
            if (strnicmp(modifiedTxt, NanString, strlen(NanString)) == 0)
            {
                lnum = strlen(NanString);
            }
            else if (strnicmp(modifiedTxt, InfString, strlen(InfString)) == 0)
            {
                lnum = strlen(InfString);
            }
            else if (strnicmp(modifiedTxt, NegInfString, strlen(NegInfString)) == 0)
            {
                lnum = strlen(NegInfString);
            }
            else if (strnicmp(modifiedTxt, PosInfString, strlen(PosInfString)) == 0)
            {
                lnum = strlen(PosInfString);
            }
            else if (strnicmp(modifiedTxt, NegNanString, strlen(NegNanString)) == 0)
            {
                lnum = strlen(NegNanString);
            }
            else if (strnicmp(modifiedTxt, PosNanString, strlen(PosNanString)) == 0)
            {
                lnum = strlen(PosNanString);
            }
        }
        inum_string = midstring(modifiedTxt, lnum, -1);

        if ((inum_string[strlen(inum_string) - 1] == 'i') ||
                (inum_string[strlen(inum_string) - 1] == 'j')) // The imaginary part looks like "a*%i"
        {
            inum_string[strlen(inum_string) - 1] = 0;
            if (inum_string[strlen(inum_string) - 1] == '*')
            {
                inum_string[strlen(inum_string) - 1] = 0;
            }

            if (strcmp(inum_string, "+") == 0)
            {
                FREE(inum_string);
                inum_string = os_strdup("+1");
            }

            if (strcmp(inum_string, "-") == 0)
            {
                FREE(inum_string);
                inum_string = os_strdup("-1");
            }
            haveImagI = TRUE;
        }
        else if (inum_string[1] == 'i' || inum_string[1] == 'j') // The imaginary part looks like "%i*a". For instance if string() has been used
        {
            int len_inum_string = strlen(inum_string);
            for (i = 1; i < len_inum_string; ++i)
            {
                inum_string[i] = inum_string[i + 1];    // Removing the "i"
            }
            if (inum_string[1] == '*')
            {
                for (i = 1; i < len_inum_string; ++i)
                {
                    inum_string[i] = inum_string[i + 1];    // Removing the "*"
                }
            }

            if (strcmp(inum_string, "+") == 0)
            {
                FREE(inum_string);
                inum_string = strdup("+1");
            }

            if (strcmp(inum_string, "-") == 0)
            {
                FREE(inum_string);
                inum_string = strdup("-1");
            }
            haveImagI = TRUE;
        }
        else
        {
            haveImagI = FALSE;
        }
        rnum_string = leftstring(modifiedTxt, lnum);

        if (strcmp(inum_string, "") == 0)
        {
            *imag = stringToDouble(rnum_string, bConvertByNAN, &ierrDouble);
            ierr = (stringToComplexError)(ierrDouble);
            *real = 0.;
        }
        else
        {
            double dReal = 0.;
            double dImag = 0.;

            stringToDoubleError ierrReal = STRINGTODOUBLE_NO_ERROR;
            stringToDoubleError ierrImag = STRINGTODOUBLE_NO_ERROR;
            dReal = stringToDouble(rnum_string, FALSE, &ierrReal);
            dImag = stringToDouble(inum_string, FALSE, &ierrImag);

            if ((ierrReal == STRINGTODOUBLE_NO_ERROR) && (ierrImag == STRINGTODOUBLE_NO_ERROR))
            {
                if (!haveImagI)
                {
                    if (bConvertByNAN)
                    {
                        ierr = STRINGTOCOMPLEX_NO_ERROR;
                        *real = returnNAN();
                        *imag = 0.;
                    }
                    else
                    {
                        ierr = STRINGTOCOMPLEX_ERROR;
                    }
                }
                else
                {
                    ierr = STRINGTOCOMPLEX_NO_ERROR;
                    *real = dReal;
                    *imag = dImag;
                }
            }
            else
            {
                if (bConvertByNAN)
                {
                    ierr = STRINGTOCOMPLEX_NO_ERROR;
                    *real = returnNAN();
                    *imag = 0.;
                }
                else
                {
                    ierr = STRINGTOCOMPLEX_ERROR;
                }
            }
        }

        FREE(rnum_string);
        FREE(inum_string);
        FREE(modifiedTxt);
    }
    return ierr;
}
/* ========================================================================== */
static stringToComplexError ParseComplexValueW(const wchar_t *tx, BOOL bConvertByNAN, double *real, double *imag)
{
    stringToDoubleError ierrDouble = STRINGTODOUBLE_NO_ERROR;
    stringToComplexError ierr = STRINGTOCOMPLEX_NO_ERROR;
    wchar_t *rnum_string = NULL;
    wchar_t *inum_string = NULL;
    size_t lnum = 0;
    BOOL haveImagI = FALSE;
    wchar_t *modifiedTxt = NULL;

    *real = stringToDoubleW(tx, FALSE, &ierrDouble);
    *imag = 0;

    /* test on strlen(tx) > 1 to remove case 'e' */
    if ((int)wcslen(tx) < 2)
    {
        if (ierrDouble == STRINGTODOUBLE_NO_ERROR)
        {
            ierr = (stringToComplexError) ierrDouble;
        }
        else
        {
            if (bConvertByNAN)
            {
                ierrDouble = STRINGTODOUBLE_NOT_A_NUMBER;
                *real = returnNAN();
                *imag = 0;
            }
            else
            {
                *real = 0;
                *imag = 0;
                ierr = (stringToComplexError) ierrDouble;
            }
        }
    }
    else if (ierrDouble != STRINGTODOUBLE_NO_ERROR)
    {
        modifiedTxt = wcssub(tx, ComplexScilabW, ComplexIW);
        lnum = ParseNumberW(modifiedTxt);
        if (lnum <= 1)
        {
            /* manages special cases nan + nani, ... */
            if (wcsnicmp(modifiedTxt, NanStringW, wcslen(NanStringW)) == 0)
            {
                lnum = wcslen(NanStringW);
            }
            else if (wcsnicmp(modifiedTxt, InfStringW, wcslen(InfStringW)) == 0)
            {
                lnum = wcslen(InfStringW);
            }
            else if (wcsnicmp(modifiedTxt, NegInfStringW, wcslen(NegInfStringW)) == 0)
            {
                lnum = wcslen(NegInfStringW);
            }
            else if (wcsnicmp(modifiedTxt, PosInfStringW, wcslen(PosInfStringW)) == 0)
            {
                lnum = wcslen(PosInfStringW);
            }
            else if (wcsnicmp(modifiedTxt, NegNanStringW, wcslen(NegNanStringW)) == 0)
            {
                lnum = wcslen(NegNanStringW);
            }
            else if (wcsnicmp(modifiedTxt, PosNanStringW, wcslen(PosNanStringW)) == 0)
            {
                lnum = wcslen(PosNanStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabEpsStringW, wcslen(ScilabEpsStringW)) == 0)
            {
                lnum = wcslen(ScilabEpsStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabPosEpsStringW, wcslen(ScilabPosEpsStringW)) == 0)
            {
                lnum = wcslen(ScilabPosEpsStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabNegEpsStringW, wcslen(ScilabNegEpsStringW)) == 0)
            {
                lnum = wcslen(ScilabNegEpsStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabPiStringW, wcslen(ScilabPiStringW)) == 0)
            {
                lnum = wcslen(ScilabPiStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabNegPiStringW, wcslen(ScilabNegPiStringW)) == 0)
            {
                lnum = wcslen(ScilabNegPiStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabPosPiStringW, wcslen(ScilabPosPiStringW)) == 0)
            {
                lnum = wcslen(ScilabPosPiStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabEStringW, wcslen(ScilabEStringW)) == 0)
            {
                lnum = wcslen(ScilabEStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabPosEStringW, wcslen(ScilabPosEStringW)) == 0)
            {
                lnum = wcslen(ScilabPosEStringW);
            }
            else if (wcsnicmp(modifiedTxt, ScilabNegEStringW, wcslen(ScilabNegEStringW)) == 0)
            {
                lnum = wcslen(ScilabNegEStringW);
            }
        }
        inum_string = midstringW(modifiedTxt, lnum, -1);

        if ((inum_string[wcslen(inum_string) - 1] == L'i') ||
                (inum_string[wcslen(inum_string) - 1] == L'j'))
        {
            inum_string[wcslen(inum_string) - 1] = 0;
            if (inum_string[wcslen(inum_string) - 1] == L'*')
            {
                inum_string[wcslen(inum_string) - 1] = 0;
            }

            if (wcscmp(inum_string, L"+") == 0)
            {
                FREE(inum_string);
                inum_string = os_wcsdup(L"+1");
            }

            if (wcscmp(inum_string, L"-") == 0)
            {
                FREE(inum_string);
                inum_string = os_wcsdup(L"-1");
            }
            haveImagI = TRUE;
        }
        else if ((inum_string[1] == L'i') ||
                 (inum_string[1] == L'j'))
        {
            if (inum_string[2] == L'*')
            {
                int i = 0;
                for (i = 1; (i + 2) < wcslen(inum_string); i++)
                {
                    inum_string[i] = inum_string[i + 2];
                }
                inum_string[wcslen(inum_string) - 1] = 0;
                inum_string[wcslen(inum_string) - 1] = 0;
            }

            if (wcscmp(inum_string, L"+") == 0)
            {
                FREE(inum_string);
                inum_string = os_wcsdup(L"+1");
            }

            if (wcscmp(inum_string, L"-") == 0)
            {
                FREE(inum_string);
                inum_string = os_wcsdup(L"-1");
            }
            haveImagI = TRUE;
        }
        else
        {
            haveImagI = FALSE;
        }
        rnum_string = leftstringW(modifiedTxt, lnum);

        if (wcscmp(inum_string, L"") == 0)
        {
            *imag = stringToDoubleW(rnum_string, bConvertByNAN, &ierrDouble);
            ierr = (stringToComplexError)(ierrDouble);
            *real = 0.;
        }
        else
        {
            double dReal = 0.;
            double dImag = 0.;

            stringToDoubleError ierrReal = STRINGTODOUBLE_NO_ERROR;
            stringToDoubleError ierrImag = STRINGTODOUBLE_NO_ERROR;
            dReal = stringToDoubleW(rnum_string, FALSE, &ierrReal);
            dImag = stringToDoubleW(inum_string, FALSE, &ierrImag);

            if ((ierrReal == STRINGTODOUBLE_NO_ERROR) && (ierrImag == STRINGTODOUBLE_NO_ERROR))
            {
                if (!haveImagI)
                {
                    if (bConvertByNAN)
                    {
                        ierr = STRINGTOCOMPLEX_NO_ERROR;
                        *real = returnNAN();
                        *imag = 0.;
                    }
                    else
                    {
                        ierr = STRINGTOCOMPLEX_ERROR;
                    }
                }
                else
                {
                    ierr = STRINGTOCOMPLEX_NO_ERROR;
                    *real = dReal;
                    *imag = dImag;
                }
            }
            else
            {
                if (bConvertByNAN)
                {
                    ierr = STRINGTOCOMPLEX_NO_ERROR;
                    *real = returnNAN();
                    *imag = 0.;
                }
                else
                {
                    ierr = STRINGTOCOMPLEX_ERROR;
                }
            }
        }

        if (rnum_string)
        {
            FREE(rnum_string);
            rnum_string = NULL;
        }
        FREE(inum_string);
        inum_string = NULL;
        FREE(modifiedTxt);
        modifiedTxt = NULL;
    }
    return ierr;
}
/* ========================================================================== */
static char *midstring(const char *tx, size_t pos, int nb)
{
    char *returnString = NULL;
    if (tx)
    {
        int lenTx = (int) strlen(tx);
        int posEnd = 0;
        int newLen = 0;

        if (nb < 0)
        {
            posEnd = lenTx;
        }
        else
        {
            posEnd = nb;
        }
        newLen = posEnd + 1;
        if (newLen > 0)
        {
            returnString = (char*)MALLOC(sizeof(char) * newLen);
            strncpy(returnString, &tx[pos], posEnd);
            returnString[posEnd] = 0;
        }
    }
    return returnString;
}
/* ========================================================================== */
static wchar_t* midstringW(const wchar_t* tx, size_t pos, int nb)
{
    wchar_t *returnString = NULL;
    if (tx)
    {
        int lenTx = (int) wcslen(tx);
        int posEnd = 0;
        int newLen = 0;

        if (nb < 0)
        {
            posEnd = lenTx;
        }
        else
        {
            posEnd = nb;
        }
        newLen = posEnd + 1;
        if (newLen > 0)
        {
            returnString = (wchar_t*)MALLOC(sizeof(wchar_t) * newLen);
            wcsncpy(returnString, &tx[pos], posEnd);
            returnString[posEnd] = 0;
        }
    }
    return returnString;
}
/* ========================================================================== */
static char *leftstring(const char *tx, size_t pos)
{
    char *returnString = NULL;
    if (tx)
    {
        int lenTx = (int) strlen(tx);
        returnString = os_strdup(tx);
        if (pos > lenTx)
        {
            return returnString;
        }
        else
        {
            returnString[pos] = 0;
        }
    }
    return returnString;
}
/* ========================================================================== */
static wchar_t *leftstringW(const wchar_t *tx, size_t pos)
{
    wchar_t *returnString = NULL;
    if (tx)
    {
        int lenTx = (int) wcslen(tx);
        returnString = os_wcsdup(tx);
        if (pos > lenTx)
        {
            return returnString;
        }
        else
        {
            returnString[pos] = 0;
        }
    }
    return returnString;
}
/* ========================================================================== */
static BOOL is_unit_imaginary (const char *src, double *im)
{
    char *modifiedSrc = strsub((char*)src, ComplexScilab, ComplexI);
    char *nextChar = NULL;
    BOOL isUnitImag = FALSE;

    if (modifiedSrc == NULL)
    {
        return isUnitImag;
    }

    if (modifiedSrc[0] == LessChar)
    {
        *im = -1.0;
        nextChar = modifiedSrc + 1;
    }
    else
    {
        *im = +1.0;
        if (modifiedSrc[0] == PlusChar)
        {
            nextChar = modifiedSrc + 1;
        }
        else
        {
            nextChar = modifiedSrc;
        }
    }

    if (nextChar)
    {
        if ((nextChar[0] == ComplexCharI || nextChar[0] == ComplexCharJ) && nextChar[1] == 0)
        {
            isUnitImag = TRUE;
        }
    }

    if (modifiedSrc)
    {
        FREE(modifiedSrc);
        modifiedSrc = NULL;
    }
    return isUnitImag;
}
/* ========================================================================== */
static BOOL is_unit_imaginaryW(const wchar_t *src, double *im)
{
    wchar_t *modifiedSrc = wcssub(src, ComplexScilabW, ComplexIW);
    wchar_t *nextChar = NULL;
    BOOL isUnitImag = FALSE;

    if (modifiedSrc == NULL)
    {
        return isUnitImag;
    }

    if (modifiedSrc[0] == LessChar)
    {
        *im = -1.0;
        nextChar = modifiedSrc + 1;
    }
    else
    {
        *im = +1.0;
        if (modifiedSrc[0] == PlusChar)
        {
            nextChar = modifiedSrc + 1;
        }
        else
        {
            nextChar = modifiedSrc;
        }
    }

    if (nextChar)
    {
        if ((nextChar[0] == ComplexCharI || nextChar[0] == ComplexCharJ) && nextChar[1] == 0)
        {
            isUnitImag = TRUE;
        }
    }

    if (modifiedSrc)
    {
        FREE(modifiedSrc);
        modifiedSrc = NULL;
    }
    return isUnitImag;
}
/* ========================================================================== */
static double returnNAN(void)
{
    static int first = 1;
    static double nan = 1.0;

    if ( first )
    {
        nan = (nan - (double) first) / (nan - (double) first);
        first = 0;
    }
    return (nan);
}
// =============================================================================
