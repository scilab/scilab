/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#include "dynamic_link_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "freeArrayOfString.h"
}
/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_link(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iSizeSubNames   = 0;
    char** pstSubNames  = NULL;
    char* pstLibName    = NULL;
    BOOL bFortran       = TRUE;
    int iIDSharedLib    = -1;

    if(in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"c_link", 0, 3);
        return types::Function::Error;
    }

    if(in.size() == 0)
    {
        int sizeFunctionsList = 0;
        char ** FunctionsList = NULL;

        FunctionsList = getNamesOfFunctionsInSharedLibraries(&sizeFunctionsList);
        if(sizeFunctionsList == 0 || FunctionsList == NULL)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        types::String* pSFunctionNames = new types::String(1 ,sizeFunctionsList);
        for(int i = 0 ; i < sizeFunctionsList ; i++)
        {
            wchar_t* pwstFunc = to_wide_string(FunctionsList[i]);
            pSFunctionNames->set(i, pwstFunc);
            FREE(pwstFunc);
        }

        out.push_back(pSFunctionNames);
        return types::Function::OK;
    }

    if(in.size() == 3)
    {//flag
        types::String* pSFlag = in[2]->getAs<types::String>();
        if(pSFlag == NULL || pSFlag->isScalar() == false)
        {
            ScierrorW(999 ,_W("%ls : Wrong type for input argument #%d: A string expected.\n"), L"link", 3);
            return types::Function::Error;
        }

        wchar_t* pwstFlag = pSFlag->get(0);
        if(wcscmp(pwstFlag, L"f") == 0 || wcscmp(pwstFlag, L"c") == 0)
        {
            if(wcscmp(pwstFlag, L"c") == 0)
            {
                bFortran = FALSE;
            }
        }
        else
        {
            ScierrorW(999, _W("%ls Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"link", 3, L"f", L"c");
            return types::Function::Error;
        }

    }

    if(in.size() >= 2)
    {//sub names
        types::String* pSSubNames = in[1]->getAs<types::String>();
        if(pSSubNames == NULL || ( pSSubNames->isVector() == false && pSSubNames->isScalar() == false))
        {
            ScierrorW(999, _W("%ls Wrong type for input argument #%d: A string or a string vector expected.\n"), L"link", 2);
            return types::Function::Error;
        }

        iSizeSubNames = pSSubNames->getSize();
        pstSubNames = (char**)MALLOC(iSizeSubNames * sizeof(char*));
        for(int i = 0 ; i < iSizeSubNames ; i++)
        {
            pstSubNames[i] = wide_string_to_UTF8(pSSubNames->get(i));
        }
    }

    if(in.size() >= 1)
    {
        if(in[0]->getType() == types::InternalType::RealDouble)
        {
            types::Double* pDSharedLib = in[0]->getAs<types::Double>();
            if(pDSharedLib == NULL || pDSharedLib->isScalar() == false)
            {
                ScierrorW(999, _W("%ls : Wrong value for argument #%d: %ls\n"), L"link", 1, _W("Unique id of a shared library expected."));
                return types::Function::Error;
            }

            iIDSharedLib = (int)pDSharedLib->get(0);
        }
        else if(in[0]->getType() == types::InternalType::RealString)
        {
            types::String* pS = in[0]->getAs<types::String>();
            if(pS == NULL || pS->isScalar() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"link", 1);
                return types::Function::Error;
            }
            
            if(wcscmp(pS->get(0), L"show") == 0)
            {//show option
                int *piIds      = NULL;
                int iSizeIds    = 0;

                ShowDynLinks();
                piIds = getAllIdSharedLib(&iSizeIds);
                if(iSizeIds == 0 || piIds == NULL)
                {
                    out.push_back(types::Double::Empty());
                    return types::Function::OK;
                }

                types::Double* pDIds = new types::Double(1, iSizeIds);
                for(int i = 0 ; i < iSizeIds ; i++)
                {
                    pDIds->set(i, (double)piIds[i]);
                }

                out.push_back(pDIds);
                return types::Function::OK;
            }

            //library name
            pstLibName = wide_string_to_UTF8(pS->get(0));
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar or a string expected.\n"), L"link", 1);
            return types::Function::Error;
        }
    }

    int iErr    = 0;
    int iRetID  = scilabLink(iIDSharedLib, pstLibName, pstSubNames, iSizeSubNames, bFortran, &iErr);

    if(in.size() >= 2)
    {
        freeArrayOfString(pstSubNames, iSizeSubNames);
    }

    if(iErr)
    {
        dl_genErrorMessage("link", iErr, pstLibName);
        if(pstLibName)
        {
            FREE(pstLibName);
            pstLibName = NULL;
        }
        return types::Function::Error;
    }

    if(pstLibName)
    {
        FREE(pstLibName);
        pstLibName = NULL;
    }
    
    out.push_back(new types::Double(iRetID));
    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
