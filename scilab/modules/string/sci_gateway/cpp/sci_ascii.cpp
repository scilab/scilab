/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_warning.h"
}

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

using namespace types;

/*----------------------------------------------------------------------------*/
Double* StringToDouble(String* _pst);
template <typename Y, class T>
String* TypeToString(T* _pI);

/*----------------------------------------------------------------------------*/
Function::ReturnValue sci_ascii(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ascii", 1);
        return Function::Error;
    }

    InternalType* pOut;
    switch (in[0]->getType())
    {
        case InternalType::RealDouble :
            pOut = TypeToString<double>(in[0]->getAs<Double>());
            break;
        case InternalType::RealString :
            pOut = StringToDouble(in[0]->getAs<types::String>());
            break;
        case InternalType::RealInt8 :
            pOut = TypeToString<char>(in[0]->getAs<Int8>());
            break;
        case InternalType::RealUInt8 :
            pOut = TypeToString<unsigned char>(in[0]->getAs<UInt8>());
            break;
        case InternalType::RealInt16 :
            pOut = TypeToString<short>(in[0]->getAs<Int16>());
            break;
        case InternalType::RealUInt16 :
            pOut = TypeToString<unsigned short>(in[0]->getAs<UInt16>());
            break;
        case InternalType::RealInt32 :
            pOut = TypeToString<int>(in[0]->getAs<Int32>());
            break;
        case InternalType::RealUInt32 :
            pOut = TypeToString<unsigned int>(in[0]->getAs<UInt32>());
            break;
        case InternalType::RealInt64 :
            pOut = TypeToString<long long>(in[0]->getAs<Int64>());
            break;
        case InternalType::RealUInt64 :
            pOut = TypeToString<unsigned long long>(in[0]->getAs<UInt64>());
            break;
        default :
            Scierror(999, _("%s: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"), "ascii", 1);
            return Function::Error;
    }

    out.push_back(pOut);
    return Function::OK;

}
/*--------------------------------------------------------------------------*/
template <typename Y, class T>
String* TypeToString(T* _pI)
{
    String* pOut = NULL;
    char* pst = NULL;
    Y* p = _pI->get();
    pst = (char*)MALLOC(sizeof(char) * _pI->getSize() + 1);
    memset(pst, 0x00, sizeof(char) * _pI->getSize() + 1);

    bool bWarning = getWarningMode() == 0;
    for (int i = 0 ; i < _pI->getSize() ; i++)
    {
        if (bWarning == false && p[i] > 255)
        {
            sciprint(_("WARNING : \n"));
            sciprint(_("%ls: Wrong value for input argument #%d: Must be between %d and %d.\n"), L"ascii", 1, 0, 255);
            bWarning = true;
        }

        if (p[i] == 0)
        {
            pst[i] = ' ';
        }
        else
        {
            pst[i] = (char)p[i];
        }
    }

    wchar_t* pwst = to_wide_string(pst);
    pOut = new String(1, 1);
    pOut->set(0, 0, pwst);
    return pOut;
}
/*--------------------------------------------------------------------------*/
Double* StringToDouble(String* _pst)
{
    Double* pOut = NULL;
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
        return Double::Empty();
    }

    pOut = new Double(1, iTotalLen);
    double* pdbl = pOut->get();
    int index = 0;

    //for each element of input string matrix
    for (int i = 0 ; i < iSize ; i++)
    {
        //for each character of input string matrix
        for (int j = 0 ; j < pstLen[i] ; j++)
        {
            //transform character value as double.
            pdbl[index++] = pst[i][j];
        }
    }

    delete[] pstLen;
    for (int i = 0 ; i < iSize ; i++)
    {
        delete pst[i];
    }

    delete[] pst;
    return pOut;
}
/*--------------------------------------------------------------------------*/
