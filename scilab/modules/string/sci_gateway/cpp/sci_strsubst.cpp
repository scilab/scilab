/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
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
/*------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "freeArrayOfString.h"
}
/*-------------------------------------------------------------------------------------*/
#define WCHAR_R L'r'
#define WCHAR_S L's'
/*-------------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_strsubst(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bRegExp = false;
    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "strsubst", 3, 4);
        return types::Function::Error;
    }

    if (in.size() > 3)
    {
        if (in[3]->isString() == false && in[3]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strsubst", 4);
        }

        if (in[3]->getAs<types::String>()->get(0)[0] == WCHAR_R)
        {
            bRegExp = true;
        }
        else if (in[3]->getAs<types::String>()->get(0)[0] == WCHAR_S)
        {
            bRegExp = false;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 's' or 'r' expected.\n"), "strsubst", 4);
            return types::Function::Error;
        }
    }

    if (in[2]->isString() == false || in[2]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strsubst", 3);
        return types::Function::Error;
    }

    wchar_t* pwstReplace = in[2]->getAs<types::String>()->get()[0];

    if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strsubst", 2);
        return types::Function::Error;
    }

    wchar_t* pwstSearch = in[1]->getAs<types::String>()->get()[0];

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"), "strsubst", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::String* pOut = new types::String(pS->getRows(), pS->getCols());

    if (bRegExp)
    {
        int iErr = 0;
        wchar_t** pwstOutput = wcssubst_reg(const_cast<const wchar_t**>(pS->get()), pS->getSize(), pwstSearch, pwstReplace, &iErr);
        if (iErr != NO_MATCH && iErr != PCRE_FINISHED_OK && iErr != PCRE_EXIT)
        {
            freeArrayOfWideString(pwstOutput, pOut->getSize());
            pcre_error("strsubst", iErr);
            delete pOut;
            return types::Function::Error;
        }

        pOut->set(pwstOutput);
        freeArrayOfWideString(pwstOutput, pOut->getSize());
    }
    else
    {
        std::wstring search = pwstSearch;
        std::wstring replace = pwstReplace;
        size_t s_len = search.size();
        size_t r_len = replace.size();

        // on empty search string, convert only empty string to the replace
        if (s_len == 0)
        {
            for (int i = 0; i < pS->getSize(); ++i)
            {
                wchar_t* wcs = pS->get()[i];
                if (wcslen(wcs) > 0)
                {
                    pOut->set(i, wcs);
                }
                else
                {
                    pOut->set(i, pwstReplace);
                }
            }
            out.push_back(pOut);
            return types::Function::OK;
        }

        // regular string search
        std::wstring string_out;
        for (int i = 0; i < pS->getSize(); ++i)
        {
            string_out.assign(pS->get()[i]);

            size_t pos = string_out.find(search);
            while (pos != std::string::npos)
            {
                string_out.replace(pos, s_len, replace);
                pos = string_out.find(search, pos + r_len);
            }

            pOut->set(i, string_out.data());
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*-------------------------------------------------------------------------------------*/
