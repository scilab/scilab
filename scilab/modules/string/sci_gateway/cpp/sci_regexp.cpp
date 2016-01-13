/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Cong WU
* Copyright (C) 2008 - 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/* desc : search position of a character string in an other string
using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include "function.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "pcre.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_string.h"
#include "freeArrayOfString.h"
}
/*------------------------------------------------------------------------*/
#define CHAR_S 's'
#define CHAR_R 'r'
#define STR_ONCE 'o'
/*------------------------------------------------------------------------*/
types::Function::ReturnValue sci_regexp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char cType = CHAR_S;
    char* pstInput = NULL;
    char* pstPattern = NULL;

    int iPcreStatus = 0;
    int iStart = 0;
    int iStep = 0;
    int iwStep = 0;
    int iEnd = 0;
    int* piStart = NULL;
    int* piEnd = NULL;
    int iOccurs = 0;

    /*for captured sub strings*/
    char*** pstCapturedString = NULL;
    int* piCapturedStringCount = NULL;

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "regexp", 2, 3);
        return types::Function::Error;
    }

    // check output parameters
    if (_iRetCount < 1 || _iRetCount > 4)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "regexp", 1, 4);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "regexp", 1);
        return types::Function::Error;
    }

    pstInput = in[0]->getAs<types::String>()->get(0);

    if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "regexp", 2);
        return types::Function::Error;
    }

    pstPattern = in[1]->getAs<types::String>()->get(0);

    if (in.size() == 3)
    {
        if (in[2]->isString() == false || in[2]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "regexp", 3);
            return types::Function::Error;
        }

        if (in[2]->getAs<types::String>()->get(0)[0] != STR_ONCE)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: '%s' expected.\n"), "regexp", 3, "o");
            return types::Function::Error;
        }
        
        cType = STR_ONCE;
    }

    //input is empty
    if (strlen(pstInput) == 0)
    {
        types::Double* pStart = new types::Double(0, 0);
        out.push_back(pStart);
        if (_iRetCount > 1)
        {
            types::Double* pEnd = new types::Double(0, 0);
            out.push_back(pEnd);

            if (_iRetCount > 2)
            {
                types::String* pS = new types::String(0, 0);
                out.push_back(pS);
            }
        }
        return types::Function::OK;
    }

    piStart = new int[strlen(pstInput)];
    piEnd = new int[strlen(pstInput)];

    pstCapturedString = (char***)CALLOC(sizeof(char**), strlen(pstInput));
    piCapturedStringCount = (int*)CALLOC(sizeof(int), strlen(pstInput));

    do
    {
        iPcreStatus = pcre_private(pstInput + iStep, pstPattern, &iStart, &iEnd, &pstCapturedString[iOccurs], &piCapturedStringCount[iOccurs]);
        if (iPcreStatus == PCRE_FINISHED_OK)
        {
            if (iEnd != iStart)
            {
                //convert strat and end to codepoint value
                char* pstTempStart = NULL;
                char* pstTempEnd = NULL;
                wchar_t* pwstTempStart = NULL;
                wchar_t* pwstTempEnd = NULL;

                pstTempStart = os_strdup(pstInput + iStep);
                pstTempEnd = os_strdup(pstInput + iStep);
                pstTempEnd[iEnd] = 0;
                pstTempStart[iStart] = 0;


                pwstTempStart = to_wide_string(pstTempStart);
                pwstTempEnd = to_wide_string(pstTempEnd);

                int iwStart = (int)wcslen(pwstTempStart);
                int iwEnd = (int)wcslen(pwstTempEnd);

                piStart[iOccurs] = iwStart + iwStep;
                piEnd[iOccurs++] = iwEnd + iwStep;
                iStep += iEnd;
                iwStep += iwEnd;
            }
            else if (iEnd == 0 && pstInput[iStep] != '\0')
            {
                //avoid infinite loop
                iStep++;
            }
        }
        else if (iPcreStatus != NO_MATCH)
        {
            pcre_error("regexp", iPcreStatus);
            delete[] piStart;
            delete[] piEnd;
            for (int i = 0; i < iOccurs; i++)
            {
                freeArrayOfString(pstCapturedString[i], piCapturedStringCount[i]);
            }

            FREE(pstCapturedString);
            FREE(piCapturedStringCount);
            return types::Function::Error;
        }
    }
    while (iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd && cType != STR_ONCE);

    if (iOccurs == 0)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount > 1)
        {
            out.push_back(types::Double::Empty());
        }

        if (_iRetCount > 2)
        {
            out.push_back(new types::String(""));
        }

        if (_iRetCount > 3)
        {
            out.push_back(new types::String(""));
        }

        freeArrayOfString(pstCapturedString[0], piCapturedStringCount[0]);
        FREE(pstCapturedString);
        FREE(piCapturedStringCount);
        delete[] piStart;
        delete[] piEnd;
        return types::Function::OK;
    }

    types::Double* pStart = new types::Double(1, iOccurs);
    double* pdblStart = pStart->getReal();

    for (int i = 0 ; i < iOccurs ; i++)
    {
        pdblStart[i] = piStart[i] + 1; //one indexed
    }

    out.push_back(pStart);

    if (_iRetCount > 1)
    {
        types::Double* pEnd = new types::Double(1, iOccurs);
        double* pdblEnd = pEnd->getReal();
        for (int i = 0 ; i < iOccurs ; i++)
        {
            pdblEnd[i]   = piEnd[i];
        }
        out.push_back(pEnd);
    }

    if (_iRetCount > 2)
    {
        types::String *pS = NULL;
        if (iOccurs == 0)
        {
            pS = new types::String(1, 1);
            pS->set(0, "");
        }
        else
        {
            pS = new types::String(iOccurs, 1);
            for (int i = 0 ; i < iOccurs ; i++)
            {
                wchar_t* pwstTemp = new wchar_t[piEnd[i] - piStart[i] + 1];
                wchar_t* pwstInput = to_wide_string(pstInput);
                wcsncpy(pwstTemp, pwstInput + piStart[i], piEnd[i] - piStart[i]);
                pwstTemp[piEnd[i] - piStart[i]] = 0;
                char* pstTemp = wide_string_to_UTF8(pwstTemp);
                pS->set(i, 0, pstTemp);
                delete[] pstTemp;
                delete[] pwstInput;
                delete[] pwstTemp;
            }
        }
        out.push_back(pS);
    }

    if (_iRetCount > 3)
    {
        //find max occurrences
        int iMax = 0;
        for (int i = 0 ; i < iOccurs ; i++)
        {
            iMax = std::max(iMax, piCapturedStringCount[i]);
        }

        types::String* pS = NULL;
        if (iOccurs == 0 || iMax == 0)
        {
            pS = new types::String("");
        }
        else
        {
            int index = 0;
            pS = new types::String(iOccurs, iMax);
            for (int i = 0 ; i < iMax ; i++)
            {
                for (int j = 0 ; j < iOccurs ; j++)
                {
                    if (i < piCapturedStringCount[j])
                    {
                        pS->set(index, pstCapturedString[j][i]);
                    }
                    else
                    {
                        pS->set(index, "");
                    }

                    index++;
                }
            }
        }

        out.push_back(pS);
    }

    for (int i = 0; i < iOccurs; i++)
    {
        freeArrayOfString(pstCapturedString[i], piCapturedStringCount[i]);
    }

    FREE(pstCapturedString);
    FREE(piCapturedStringCount);
    delete[] piStart;
    delete[] piEnd;
    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
