/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

/* desc : This function convert Scilab string to a vector of ascii code  */
/*        or vector of ascii code to Scilab strings.                     */
/*        If  txt  is a matrix of string,  ascii(txt)  is equivalent to  */
/*          ascii(strcat(txt))                                           */
/*-----------------------------------------------------------------------*/

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"

extern "C"
{
#include <ctype.h>
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "configvariable_interface.h"
}

#define MAX_ASCII 255
/* Benchmark
str_test_mat =  ["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];

tic();
for i=1:10000000
ascii(["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"]);
end
duree = toc();

printf("\nDUREE 1 = %d seconds\n\n",duree);
*/

/* on windows 17 3770 @ 3.4 GHz */
/* scilab 4.1.2 : 17 s */
/* scilab 5.4.0 : 32 s */
/* scilab 6 dev : 15 s */

/*----------------------------------------------------------------------------*/
types::Double* StringToDouble(types::String* _pst);
template <typename Y, class T>
types::String* TypeToString(T* _pI);
/*----------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ascii(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ascii", 1);
        return types::Function::Error;
    }

    types::InternalType* pOut;
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble :
            pOut = TypeToString<double>(in[0]->getAs<types::Double>());
            break;
        case types::InternalType::ScilabString :
            pOut = StringToDouble(in[0]->getAs<types::String>());
            break;
        case types::InternalType::ScilabInt8 :
            pOut = TypeToString<char>(in[0]->getAs<types::Int8>());
            break;
        case types::InternalType::ScilabUInt8 :
            pOut = TypeToString<unsigned char>(in[0]->getAs<types::UInt8>());
            break;
        case types::InternalType::ScilabInt16 :
            pOut = TypeToString<short>(in[0]->getAs<types::Int16>());
            break;
        case types::InternalType::ScilabUInt16 :
            pOut = TypeToString<unsigned short>(in[0]->getAs<types::UInt16>());
            break;
        case types::InternalType::ScilabInt32 :
            pOut = TypeToString<int>(in[0]->getAs<types::Int32>());
            break;
        case types::InternalType::ScilabUInt32 :
            pOut = TypeToString<unsigned int>(in[0]->getAs<types::UInt32>());
            break;
        case types::InternalType::ScilabInt64 :
            pOut = TypeToString<long long>(in[0]->getAs<types::Int64>());
            break;
        case types::InternalType::ScilabUInt64 :
            pOut = TypeToString<unsigned long long>(in[0]->getAs<types::UInt64>());
            break;
        default :
            Scierror(999, _("%s: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"), "ascii", 1);
            return types::Function::Error;
    }

    if (pOut == NULL)
    {
        Scierror(999, _("%s : wrong UTF-8 sequence.\n"), "ascii");
        return types::Function::Error;
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
template <typename Y, class T>
types::String* TypeToString(T* _pI)
{
    types::String* pOut = NULL;
    wchar_t* pst = NULL;
    int len = _pI->getSize();
    char* pcText = new char[len + 1];
    Y* p = _pI->get();

    bool bWarning = getWarningMode() == 0;
    for (int i = 0; i < len; i++)
    {
        if (bWarning == false && p[i] > MAX_ASCII)
        {
            sciprint(_("WARNING : \n"));
            sciprint(_("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), "ascii", 1, 0, MAX_ASCII);
            bWarning = true;
        }

        pcText[i] = static_cast<char>(p[i]);

    }
    pcText[len] = '\0';

    pst = to_wide_string(pcText);
    pOut = new types::String(pst);

    delete[] pcText;
    FREE(pst);
    return pOut;
}
/*--------------------------------------------------------------------------*/
types::Double* StringToDouble(types::String* _pst)
{
    types::Double* pOut = NULL;
    /*compute total length*/
    int iTotalLen = 0;
    int iSize = _pst->getSize();

    char** pst = new char*[iSize];
    int* pstLen = new int[iSize];
    for (int i = 0 ; i < iSize ; i++)
    {
        pst[i] = wide_string_to_UTF8(_pst->get(i));
        pstLen[i] = (int)strlen(pst[i]);
        iTotalLen += pstLen[i];
    }

    if (iTotalLen == 0)
    {
        for (int i = 0; i < iSize; ++i)
        {
            if (pst[i])
            {
                FREE(pst[i]);
            }
        }

        delete[] pst;
        delete[] pstLen;
        return types::Double::Empty();
    }

    pOut = new types::Double(1, iTotalLen);
    double* pdbl = pOut->get();
    int index = 0;

    //for each element of input string matrix
    for (int i = 0 ; i < iSize ; i++)
    {
        //for each character of input string matrix
        for (int j = 0; j < pstLen[i]; j++, index++)
        {
            //transform character value as double.
            pdbl[index] = (unsigned char)pst[i][j];
        }
        FREE(pst[i]);
    }

    delete[] pstLen;
    delete[] pst;
    return pOut;
}
/*--------------------------------------------------------------------------*/
