/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

#include <math.h>
#include <sstream>
#include <string>
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"
#include "tostring_common.hxx"
#include "printvisitor.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "symbol.hxx"
#include "tlist.hxx"
#include "overload.hxx"
#include "sparse.hxx"
#include "int.hxx"
#include "implicitlist.hxx"

extern "C"
{
#include "os_string.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
}

static void getMacroString(types::Macro* _pM, types::InternalType** _pOut, types::InternalType** _pIn, types::InternalType** _pBody)
{
    //get body
    ast::Exp* exp = _pM->getBody();

    std::wostringstream ostr;
    ast::PrintVisitor pv(ostr, false);

    exp->accept(pv);

    std::wstring wstBody = ostr.str();
    const wchar_t* pwstBody = wstBody.c_str();

    //first loop to find number of lines
    int iLines = 2; //for first and last one-space lines
    for (int i = 0 ; i < (int)wcslen(pwstBody) ; i++)
    {
        if (pwstBody[i] == L'\n')
        {
            iLines++;
        }
    }

    types::String* pBody = new types::String(iLines, 1);
    pBody->set(0, L" ");
    //second loop to assign lines to output data
    int iOffset = 0;
    int iIndex = 1;
    for (int i = 0 ; i < (int)wcslen(pwstBody) ; i++)
    {
        if (pwstBody[i] == L'\n')
        {
            int iLen = i - iOffset;
            wchar_t* pwst = new wchar_t[iLen + 1];
            wcsncpy(pwst, pwstBody + iOffset, iLen);
            pwst[iLen] = L'\0';
            pBody->set(iIndex++, pwst);
            delete[] pwst;
            iOffset = i + 1;
        }
    }

    pBody->set(iIndex, L" ");
    *_pBody = pBody;

    //get inputs
    std::list<symbol::Variable*>* pIn = _pM->getInputs();

    if (pIn->size() == 0)
    {
        *_pIn = types::Double::Empty();
    }
    else
    {
        types::String *pSIn = new types::String(1, (int)pIn->size());

        std::list<symbol::Variable*>::iterator itIn = pIn->begin();
        for (int i = 0 ; i < pIn->size() ; i++, itIn++)
        {
            pSIn->set(i, (*itIn)->getSymbol().getName().c_str());
        }

        *_pIn = pSIn;
    }

    //get outputs
    std::list<symbol::Variable*>* pOut = _pM->getOutputs();
    if (pOut->size() == 0)
    {
        *_pOut = types::Double::Empty();
    }
    else
    {
        types::String* pSOut = new types::String(1, (int)pOut->size());

        std::list<symbol::Variable*>::iterator itOut = pOut->begin();
        for (int i = 0 ; i < pOut->size() ; i++, itOut++)
        {
            pSOut->set(i, (*itOut)->getSymbol().getName().c_str());
        }

        *_pOut = pSOut;
    }
}

static void DoubleComplexMatrix2String(std::wostringstream *_postr,  double _dblR, double _dblI)
{
    /*
    if R && !C -> R
    if R && C -> R + Ci
    if !R && !C -> 0
    if(!R aa C	-> Ci
    */
    DoubleFormat dfR, dfI;
    dfR.bPrintBlank = false;
    dfI.bPrintBlank = false;

    getDoubleFormat(_dblR, &dfR);
    getDoubleFormat(_dblI, &dfI);

    dfR.bPrintPoint = dfR.bExp;
    dfR.bPaddSign = false;

    dfI.bPrintPoint = dfI.bExp;
    dfI.bPaddSign = false;

    // decrease precision when the real number will be rounded
    // format(10) with number 1.12345678 should return 1.1234568
    if (dfR.iWidth == ConfigVariable::getFormatSize())
    {
        if (dfR.iPrec != 0)
        {
            dfR.iPrec--;
        }

        dfR.iWidth--;
    }

    if (dfI.iWidth == ConfigVariable::getFormatSize())
    {
        if (dfI.iPrec != 0)
        {
            dfI.iPrec--;
        }

        dfI.iWidth--;
    }

    if (_dblR == 0)
    {
        //no real part
        if (_dblI == 0)
        {
            //no imaginary part

            //0
            addDoubleValue(_postr, 0, &dfR);
        }
        else
        {
            //imaginary part

            //I
            *_postr << (_dblI < 0 ? L"-" : L"");
            *_postr << L"%i";
            if (fabs(_dblI) != 1 || dfI.bExp)
            {
                //specail case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
    else
    {
        //real part
        if (_dblI == 0)
        {
            //no imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
        }
        else
        {
            //imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
            //I
            *_postr << (_dblI < 0 ? L"-%i" : L"+%i");
            if (fabs(_dblI) != 1 || dfI.bExp)
            {
                //special case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
}

template <class T>
types::Function::ReturnValue intString(T* pInt, types::typed_list &out)
{
    int iDims = pInt->getDims();
    int* piDimsArray = pInt->getDimsArray();
    types::String *pstOutput = new types::String(iDims, piDimsArray);
    int iSize = pInt->getSize();
    for (int i = 0 ; i < iSize ; i++)
    {
        std::wostringstream ostr;
        DoubleComplexMatrix2String(&ostr, (double)pInt->get(i), 0);
        pstOutput->set(i, ostr.str().c_str());
    }

    out.push_back(pstOutput);
    return types::Function::OK;
}

types::Function::ReturnValue booleanString(types::Bool* pB, types::typed_list &out)
{
    int iDims = pB->getDims();
    int* piDimsArray = pB->getDimsArray();
    int* pb = pB->get();
    types::String *pstOutput = new types::String(iDims, piDimsArray);
    int iSize = pB->getSize();
    for (int i = 0 ; i < iSize ; i++)
    {
        pstOutput->set(i, pb[i] == 0 ? "F" : "T");
    }

    out.push_back(pstOutput);
    return types::Function::OK;
}

types::Function::ReturnValue doubleString(types::Double* pDbl, types::typed_list &out)
{
    int iDims = pDbl->getDims();
    int* piDimsArray = pDbl->getDimsArray();
    double* pdblReal = pDbl->get();

    // Special case string([]) == []
    if (pDbl->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else if (piDimsArray[0] == -1 && piDimsArray[1] == -1)
    {
        out.push_back(new types::String(L""));
        return types::Function::OK;
    }

    types::String *pstOutput = new types::String(iDims, piDimsArray);
    if (pDbl->isComplex())
    {
        double* pdblImg = pDbl->getImg();
        for (int i = 0; i < pDbl->getSize(); ++i)
        {
            std::wostringstream ostr;
            DoubleComplexMatrix2String(&ostr, pdblReal[i], pdblImg[i]);
            pstOutput->set(i, ostr.str().c_str());
        }
    }
    else
    {
        double dblImg  = 0.0;
        for (int i = 0; i < pDbl->getSize(); ++i)
        {
            std::wostringstream ostr;
            DoubleComplexMatrix2String(&ostr, pdblReal[i], dblImg);
            pstOutput->set(i, ostr.str().c_str());
        }
    }
    out.push_back(pstOutput);
    return types::Function::OK;
}

types::Function::ReturnValue implicitListString(types::ImplicitList* pIL, types::typed_list &out)
{
    std::wostringstream ostr;
    pIL->toString(ostr);
    std::wstring str = ostr.str();
    //erase fisrt character " "
    str.erase(str.begin());
    //erase last character "\n"
    str.erase(str.end() - 1);

    out.push_back(new types::String(str.c_str()));
    return types::Function::OK;
}

types::Function::ReturnValue sci_string(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "string", 1);
        return types::Function::Error;
    }

    switch (in[0]->getType())
    {
        case types::GenericType::ScilabSparse:
        {
            //C=sparse([0 0 4 0 9;0 0 5 0 0;1 3 0 7 0;0 0 6 0 10;2 0 0 8 0]);string(C)
            types::Sparse* pS = in[0]->getAs<types::Sparse>();
            int iRows = pS->getRows();
            int iCols = pS->getCols();
            bool isComplex = pS->isComplex();
            std::wostringstream ostr;
            std::vector<std::wstring> vect;


            ostr << "(" << iRows << "," << iCols << ") sparse matrix";

            vect.push_back(ostr.str());
            ostr.str(L"");
            ostr.clear();

            for (int i = 0 ; i < iRows ; i++)
            {
                for (int j = 0 ; j < iCols ; j++)
                {
                    std::wostringstream temp;
                    double real = pS->getReal(i, j);
                    double cplx = 0;
                    if (isComplex)
                    {
                        cplx = pS->getImg(i, j).imag();
                    }

                    if (real || cplx )
                    {
                        temp << L"(" << i + 1 << L"," << j + 1 << L")    ";

                        if (real)
                        {
                            temp << pS->getReal(i, j);
                        }

                        if (cplx)
                        {
                            if (real && cplx > 0)
                            {
                                temp << L"+";
                            }
                            else if (cplx < 0)
                            {
                                temp << L"-";
                            }

                            temp << L"%i*" << std::abs(cplx);
                        }

                        ostr << temp.str();
                        vect.push_back(ostr.str());
                        ostr.str(L"");
                        ostr.clear();
                    }
                }
            }

            types::String* pSt = new types::String((int)vect.size(), 1);
            for (int i = 0 ; i < vect.size(); i++)
            {
                pSt->set(i, vect[i].c_str());
            }

            out.push_back(pSt);
            break;
        }

        case types::InternalType::ScilabInt8 :
        {
            return intString(in[0]->getAs<types::Int8>(), out);
        }
        case types::InternalType::ScilabUInt8 :
        {
            return intString(in[0]->getAs<types::UInt8>(), out);
        }
        case types::InternalType::ScilabInt16 :
        {
            return intString(in[0]->getAs<types::Int16>(), out);
        }
        case types::InternalType::ScilabUInt16 :
        {
            return intString(in[0]->getAs<types::UInt16>(), out);
        }
        case types::InternalType::ScilabInt32 :
        {
            return intString(in[0]->getAs<types::Int32>(), out);
        }
        case types::InternalType::ScilabUInt32 :
        {
            return intString(in[0]->getAs<types::UInt32>(), out);
        }
        case types::InternalType::ScilabInt64 :
        {
            return intString(in[0]->getAs<types::Int64>(), out);
        }
        case types::InternalType::ScilabUInt64 :
        {
            return intString(in[0]->getAs<types::UInt64>(), out);
        }
        case types::InternalType::ScilabDouble :
        {
            return doubleString(in[0]->getAs<types::Double>(), out);
        }
        case types::InternalType::ScilabString :
        {
            out.push_back(in[0]);
            break;
        }
        case types::InternalType::ScilabFunction:
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "string", 1);
            return types::Function::Error;
        }
        case types::InternalType::ScilabMacroFile :
        {
            if (_iRetCount != 3)
            {
                Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "string", 3);
                return types::Function::Error;
            }

            types::MacroFile* pMF = in[0]->getAs<types::MacroFile>();
            types::InternalType* pOut = NULL;
            types::InternalType* pIn = NULL;
            types::InternalType* pBody = NULL;

            getMacroString(pMF->getMacro(), &pOut, &pIn, &pBody);

            out.push_back(pOut);
            out.push_back(pIn);
            out.push_back(pBody);
            break;
        }
        case types::InternalType::ScilabMacro :
        {
            if (_iRetCount != 3)
            {
                Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "string", 3);
                return types::Function::Error;
            }

            types::Macro* pM = in[0]->getAs<types::Macro>();
            types::InternalType* pOut = NULL;
            types::InternalType* pIn = NULL;
            types::InternalType* pBody = NULL;

            getMacroString(pM, &pOut, &pIn, &pBody);

            out.push_back(pOut);
            out.push_back(pIn);
            out.push_back(pBody);
            break;
        }
        case types::InternalType::ScilabTList :
        case types::InternalType::ScilabMList :
        case types::InternalType::ScilabPolynom :
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_string";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
        case types::InternalType::ScilabBool:
        {
            return booleanString(in[0]->getAs<types::Bool>(), out);
        }
        case types::InternalType::ScilabLibrary:
        {
            types::Library* pL = in[0]->getAs<types::Library>();
            std::wstring path = pL->getPath();
            std::list<std::wstring> macros;
            int size = pL->getMacrosName(macros);
            types::String* pS = new types::String(size + 1, 1);
            pS->set(0, path.c_str());
            int i = 1;
            for (auto it : macros)
            {
                pS->set(i++, it.c_str());
            }

            out.push_back(pS);
            break;
        }
        case types::InternalType::ScilabImplicitList:
        {
            return implicitListString(in[0]->getAs<types::ImplicitList>(), out);
        }
        case types::InternalType::ScilabColon:
        {
            out.push_back(new types::String(L""));
            break;
        }
        default:
        {
            std::wostringstream ostr;
            in[0]->toString(ostr);
            out.push_back(new types::String(ostr.str().c_str()));
            break;
        }
    }
    return types::Function::OK;
}
