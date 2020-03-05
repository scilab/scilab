/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET , Cong WU
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

/* desc : concatenate character strings                                   */
/* Examples: strcat(string(1:10),',')                                     */
/*                                                                        */
/*------------------------------------------------------------------------*/

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
}
/*-------------------------------------------------------------------------------------*/
#define STAR '*'
#define COL 'c'
#define ROW 'r'
#define ONE_CHAR 1
#define EMPTY_CHAR ""
/*-------------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_strcat(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iMode               = 0;
    wchar_t* pwstToInsert   = NULL;

    //check input paramters
    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "strcat", 1, 3);
        return types::Function::Error;
    }

    for (int i = 1 ; i < in.size() ; i++)
    {
        if (in[i]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "strcat", i + 1);
            return types::Function::Error;
        }
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strcat", 1);
        return types::Function::Error;
    }

    if (in.size() == 3)
    {
        wchar_t wcMode = in[2]->getAs<types::String>()->get(0)[0];
        switch (wcMode)
        {
            case L'r' :
                iMode = 1;
                break;
            case L'c' :
                iMode = 2;
                break;
            default :
                Scierror(999, _("%s: Wrong type for input argument #%d: '%s' or '%s' expected.\n"), "strcat", 3, "c", "r");
                return types::Function::Error;
        }
    }

    if (in.size() > 1)
    {
        if (in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strcat", 2);
            return types::Function::Error;
        }

        pwstToInsert = in[1]->getAs<types::String>()->get(0);
    }

    types::String* pS = in[0]->getAs<types::String>();

    types::String* pOut = NULL;
    switch (iMode)
    {
        case 0 : //"*"
        {
            pOut = new types::String(1, 1);

            int size = pS->getSize();
            wchar_t** s = pS->get();
            std::wstring string_out(s[0]);

            if (pwstToInsert)
            {
                for (int i = 1; i < size; ++i)
                {
                    string_out += pwstToInsert;
                    string_out += s[i];
                }
            }
            else
            {
                for (int i = 1; i < size; ++i)
                {
                    string_out += s[i];
                }
            }

            pOut->set(0, string_out.data());
        }
        break;
        case 1 : //"r"
        {
            wchar_t** s = pS->get();
            int cols = pS->getCols();
            int rows = pS->getRows();

            pOut = new types::String(1, cols);

            /*compute final size*/
            std::wstring string_out;
            for (int i = 0 ; i < cols ; ++i)
            {
                string_out.assign(s[i * rows]);

                if (pwstToInsert)
                {
                    for (int j = 1; j < rows; ++j)
                    {
                        string_out += pwstToInsert;
                        string_out += s[i * rows + j];
                    }
                }
                else
                {
                    for (int j = 1; j < rows; ++j)
                    {
                        string_out += s[i * rows + j];
                    }
                }

                pOut->set(0, i, string_out.data());
            }
            break;
        }
        case 2 : //"c"
        {
            wchar_t** s = pS->get();
            int cols = pS->getCols();
            int rows = pS->getRows();

            pOut = new types::String(rows, 1);

            /*compute final size*/
            std::wstring string_out;
            for (int i = 0 ; i < rows ; ++i)
            {
                string_out.assign(s[i]);
                if (pwstToInsert)
                {
                    for (int j = 1; j < cols; ++j)
                    {
                        string_out += pwstToInsert;
                        string_out += s[j * rows + i];
                    }
                }
                else
                {
                    for (int j = 1; j < cols; ++j)
                    {
                        string_out += s[j * rows + i];
                    }
                }

                pOut->set(i, 0, string_out.data());
            }
            break;
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*-------------------------------------------------------------------------------------*/
