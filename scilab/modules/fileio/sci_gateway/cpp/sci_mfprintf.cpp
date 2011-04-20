/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "scilab_sprintf.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sci_mode.h"
#include "mputl.h"
}
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
static BOOL forceSTDERRredirect = TRUE;
#endif
/*--------------------------------------------------------------------------*/ 

Function::ReturnValue sci_mfprintf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    BOOL isSTD                      = FALSE;
    BOOL isSTDErr                   = FALSE;
    int iFile                       = -1; //default file : last opened file
    int iRet                        = 1;  // push not iRet
    unsigned int iNumberPercent     = 0;
    unsigned int iNumberCols        = 0;
    int nbrOfLines                  = 0;
    double dfileMode                = 0;
    wchar_t* wcsInput               = NULL;
    wchar_t** wcsStringToWrite      = NULL;
    ArgumentPosition* pArgs         = NULL;

    if(in.size() < 2)
    {
       ScierrorW(77, _W("%ls: Wrong number of input argument(s): At least %d expected.\n"), L"mfprintf", 2);
        return types::Function::Error;
    }

    if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mfprintf", 1);
        return types::Function::Error;
    }

    if(in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mfprintf", 2);
        return types::Function::Error;
    }

    for(unsigned int i = 2 ; i < in.size() ; i++)
    {
        if(in[i]->isDouble() == false && in[i]->isString() == false)
        {
            std::wstring wstFuncName = L"%"  + in[i]->getShortTypeStr() + L"_mfprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }

// checking ID of file
    iFile = static_cast<int>(in[0]->getAs<types::Double>()->get(0));

	if(FileManager::getFile(iFile) == NULL)
	{           
        ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mfprintf", iFile);
        return types::Function::Error;
	}

    switch (iFile)
    {
    case 0:
        #ifdef _MSC_VER
        if((getScilabMode()  == SCILAB_STD) && (forceSTDERRredirect == TRUE))
        {
        //  Console redirect stderr --> CONOUT$
        freopen("CONOUT$", "wb", stderr); 
        forceSTDERRredirect = FALSE;
        }
        #endif
        isSTDErr = TRUE;
    case 6:
        isSTD = TRUE;
        break;
    case 5:
        ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mfprintf", iFile);
        return types::Function::Error;
    default:
        isSTD = FALSE;
        dfileMode = FileManager::getFile(iFile)->getFileModeAsDouble();
        break;
    }
    
    /* checks file mode */
    /* bug 3898 */
    /* read only attrib 1xx*/
    if((dfileMode >= 100) && (dfileMode < 200) && !isSTD)
    {
        ScierrorW(999, _W("%ls: Wrong file mode: READ only.\n"), L"mfprintf");
        return types::Function::Error;
    }

// Checking input string to write in file
    wcsInput = in[1]->getAs<types::String>()->get(0);

    for(int i = 0; i < (int)wcslen(wcsInput); i++)
    {
        if (wcsInput[i] == '%') 
        {
            iNumberPercent++;
            if (wcsInput[i+1] == '%') 
            {
                iNumberPercent--;
                i++;
            }
        }
    }

    //Input values must be less or equal than excepted
    if((in.size() - 2) > iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at most %d expected.\n"), L"mprintf", iNumberPercent);
        return types::Function::Error;
    }

    //determine if imput values are ... multiple values

    if( in.size() > 2 )
    {
        int iRefRows = in[2]->getAsGenericType()->getRows();
        for(unsigned int i = 2 ; i < in.size() ; i++)
        {
            //all arguments must have the same numbers of rows !
            if(iRefRows != in[i]->getAsGenericType()->getRows())
            {
                ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"mprintf");
                return types::Function::Error;
            }

            iNumberCols += in[i]->getAsGenericType()->getCols();
        }
    }

    if(iNumberCols != iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"mprintf");
        return types::Function::Error;
    }  

    //fill ArgumentPosition structure
    pArgs = new ArgumentPosition[iNumberPercent];
    int idx = 0;
    for(unsigned int i = 2 ; i < in.size() ; i++)
    {
        for(int j = 0 ; j < in[i]->getAsGenericType()->getCols() ; j++)
        {
            pArgs[idx].iArg = i;
            pArgs[idx].iPos = j;
            pArgs[idx].type = in[i]->getType();
            idx++;
        }
    }

    wcsStringToWrite = scilab_sprintf(L"mfprintf", wcsInput, in, pArgs, false, iNumberPercent, &nbrOfLines);

    if(isSTD)
    {
        for(int i=0; i<nbrOfLines; i++)
        {
            if(isSTDErr)
            {
                std::wcerr << wcsStringToWrite[i];
            }
            else
            {
                YaspWriteW(wcsStringToWrite[i]);
            }
        }
        std::cout << "\n";
        iRet = 0;
    }
    else
    {
        iRet = mputl(iFile, wcsStringToWrite, nbrOfLines, FALSE); // FALSE = don't add the "\n" at the end.
    }

    for(int i=0; i<nbrOfLines; i++)
    {
        FREE(wcsStringToWrite[i]);
    }

    FREE(wcsStringToWrite);
    FREE(pArgs);

    out.push_back(new Bool(!iRet));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
