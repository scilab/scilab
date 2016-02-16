/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
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

/* desc : search position of a character string in an other string        */
/*        using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "pcre.h"
#include "localization.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
}
/*------------------------------------------------------------------------*/
#define WCHAR_S L's'
#define WCHAR_R L'r'
/*------------------------------------------------------------------------*/
struct In
{
    int data;
    int position;
};
/*------------------------------------------------------------------------*/
int ComparaisonCallback( const void *in1 , const void *in2)
{
    In* data1 = (In*)in1;
    In* data2 = (In*)in2;

    if (data1->data == data2->data)
    {
        return data1->position > data2->position ? 1 : -1;
    }

    return data1->data > data2->data ? 1 : -1;
}
/*------------------------------------------------------------------------*/
types::Function::ReturnValue sci_strindex(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bRegExp = false;
    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "strindex", 2, 3);
        return types::Function::Error;
    }

    if (in.size() > 2)
    {
        if (in[2]->isString() == false && in[2]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strindex", 3);
            return types::Function::Error;
        }

        if (in[2]->getAs<types::String>()->get(0)[0] == WCHAR_R)
        {
            bRegExp = true;
        }
        else if (in[2]->getAs<types::String>()->get(0)[0] == WCHAR_S)
        {
            bRegExp = false;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 's' or 'r' expected.\n"), "strindex", 3);
            return types::Function::Error;
        }
    }

    if (in[1]->isString() == false || (in[1]->getAs<types::String>()->getRows() != 1 && in[1]->getAs<types::String>()->getCols() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string or string vector expected.\n"), "strindex", 2);
        return types::Function::Error;
    }

    types::String* pS = in[1]->getAs<types::String>();
    wchar_t** pwstSearch = pS->get();

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strindex", 1);
        return types::Function::Error;
    }

    wchar_t* pwstData = in[0]->getAs<types::String>()->get()[0];
    if (wcslen(pwstData) == 0)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }

        return types::Function::OK;
    }

    In* pstrResult = new In[wcslen(pwstData)];

    //number of occurances
    int iValues = 0;
    if (bRegExp)
    {
        //pcre
        pcre_error_code iPcreStatus = PCRE_FINISHED_OK;
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            int iStart      = 0;
            int iEnd        = 0;
            int iStep       = 0;

            do
            {
                iPcreStatus = wide_pcre_private(pwstData + iStep, pwstSearch[i], &iStart, &iEnd, NULL, NULL);
                if (iPcreStatus == PCRE_FINISHED_OK)
                {
                    pstrResult[iValues].data        = iStart + iStep + 1;
                    pstrResult[iValues].position    = i + 1;
                    iStep                           += iEnd;
                    iValues++;
                }
                else
                {
                    if (iPcreStatus != NO_MATCH)
                    {
                        pcre_error("strindex", iPcreStatus);
                        delete[] pstrResult;
                        return types::Function::Error;
                    }
                    break;
                }
            }
            while (iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd);
        }
    }
    else
    {
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            wchar_t* pCur = pwstData;
            do
            {
                pCur = wcsstr(pCur, pwstSearch[i]);
                if (pCur != NULL)
                {
                    pstrResult[iValues].data      = (int)(pCur - pwstData + 1);
                    pstrResult[iValues].position  = i + 1;
                    pCur++;
                    iValues++;
                }
            }
            while (pCur != NULL && pCur[0] != L'\0');
        }
    }

    qsort(pstrResult, iValues, sizeof(In), ComparaisonCallback);

    types::Double* pIndex = NULL;
    if (iValues == 0)
    {
        pIndex = types::Double::Empty();
    }
    else
    {
        pIndex = new types::Double(1, iValues);
        for (int i = 0 ; i < iValues ; i++)
        {
            pIndex->set(0, i, pstrResult[i].data);
        }
    }
    out.push_back(pIndex);

    if (_iRetCount == 2)
    {
        types::Double* pPos = NULL;
        if (iValues == 0)
        {
            pPos = types::Double::Empty();
        }
        else
        {
            pPos = new types::Double(1, iValues);
            for (int i = 0 ; i < iValues ; i++)
            {
                pPos->set(0, i, pstrResult[i].position);
            }
        }
        out.push_back(pPos);
    }

    delete[] pstrResult;
    return types::Function::OK;
}

