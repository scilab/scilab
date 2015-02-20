/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "alltypes.hxx"
#include "types_tools.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
extern "C"
{
#include "os_string.h"
}

namespace types
{
//check argument types and compute, dimensions, count of combinations, max indexes
int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim)
{
    int iDims           = static_cast<int>(_pArgsIn->size());
    int iSeqCount       = 1;
    bool bUndefine      = false;

    for (int i = 0 ; i < iDims ; i++)
    {
        bool bDeleteNeeded  = false;
        InternalType* pIT = (*_pArgsIn)[i];
        Double *pCurrentArg = NULL;

        if (pIT->isDouble())
        {
            pCurrentArg = pIT->getAs<Double>();
            if (pCurrentArg->isEmpty())
            {
                return 0;
            }

            if (pCurrentArg->isIdentity())
            {
                //extract with eye() <=> :
                pIT = new Colon();
                bDeleteNeeded = true;
            }
            else if (pIT->isDeletable())
            {
                // Clone pIT when this ref is equal to zero
                // will prevent double delete.
                pCurrentArg = pIT->clone()->getAs<Double>();
            }
        }

        if (pIT->isColon() || pIT->isImplicitList())
        {
            //: or a:b:c
            ImplicitList* pIL = pIT->getAs<ImplicitList>()->clone()->getAs<ImplicitList>();
            if (pIL->isComputable() == false)
            {
                //: or $
                if (_pRef == NULL)
                {
                    //not enough information to compute indexes.
                    _pArgsOut->push_back(NULL);
                    bUndefine = true;
                    pIL->killMe();;
                    continue;
                }
                //evalute polynom with "MaxDim"
                int iMaxDim = _pRef->getAs<GenericType>()->getVarMaxDim(i, iDims);
#if defined(_SCILAB_DEBUGREF_)
                Double* pdbl = new Double(iMaxDim);
#else
                Double dbl(iMaxDim);
#endif
                if (pIL->getStart()->isPoly())
                {
                    Polynom *poPoly	= pIL->getStart()->getAs<types::Polynom>();
#if defined(_SCILAB_DEBUGREF_)
                    pIL->setStart(poPoly->evaluate(pdbl));
#else
                    pIL->setStart(poPoly->evaluate(&dbl));
#endif
                }
                if (pIL->getStep()->isPoly())
                {
                    Polynom *poPoly	= pIL->getStep()->getAs<types::Polynom>();
#if defined(_SCILAB_DEBUGREF_)
                    pIL->setStep(poPoly->evaluate(pdbl));
#else
                    pIL->setStep(poPoly->evaluate(&dbl));
#endif
                }
                if (pIL->getEnd()->isPoly())
                {
                    Polynom *poPoly	= pIL->getEnd()->getAs<types::Polynom>();
#if defined(_SCILAB_DEBUGREF_)
                    pIL->setEnd(poPoly->evaluate(pdbl));
#else
                    pIL->setEnd(poPoly->evaluate(&dbl));
#endif
                }

#if defined(_SCILAB_DEBUGREF_)
                pdbl->killMe();
#endif
            }


            pCurrentArg = pIL->extractFullMatrix()->getAs<Double>();
            pIL->killMe();
        }
        else if (pIT->isString())
        {
            String* pStr = pIT->getAs<String>();
            if (_pRef->isStruct())
            {
                Struct* pStruct = _pRef->getAs<Struct>();

                if (_pArgsIn->size() != 1 || pStr->isScalar() == false)
                {
                    bUndefine = true;
                    continue;
                }

                wchar_t* pFieldName = pStr->get(0);

                // pCurrent arg is indexed to 1 unlike the return of "getFieldIndex"
                int iIndex = pStruct->get(0)->getFieldIndex(pFieldName) + 1;
                if (iIndex == -1)
                {
                    bUndefine = true;
                    continue;
                }

                pCurrentArg = new Double((double)iIndex);
            }
            else if (_pRef->isTList())
            {
                // List cant be extract by field and MList must call overload
                TList* pTL = _pRef->getAs<TList>();
                pCurrentArg = new Double(pStr->getDims(), pStr->getDimsArray());
                double* pdbl = pCurrentArg->get();
                for (int i = 0; i < pStr->getSize(); i++)
                {
                    wchar_t* pFieldName = pStr->get(i);
                    int iIndex = pTL->getIndexFromString(pFieldName);
                    if (iIndex == -1)
                    {
                        bUndefine = true;
                        continue;
                    }
                    pdbl[i] = (double)(iIndex + 1);
                }
            }
            else if (_pRef->isCell())
            {
            }
            else
            {
                bUndefine = true;
                break;
            }
        }
        else if (pIT->isPoly())
        {
            //$
            Polynom* pMP = pIT->getAs<types::Polynom>();
            int iMaxDim     = 0;
            //if pRef == NULL, use 0 insteadof, to allow a($+1) on new variable
            if (_pRef)
            {
                iMaxDim     = _pRef->getAs<GenericType>()->getVarMaxDim(i, iDims);
            }

#ifdef _SCILAB_DEBUGREF_
            Double* pdbl = new Double(iMaxDim); // $
            pCurrentArg = pMP->evaluate(pdbl);
            pdbl->killMe();
#else
            Double dbl(iMaxDim); // $
            pCurrentArg = pMP->evaluate(&dbl);
#endif
        }
        else if (pIT->isBool())
        {
            //[T F F T F]
            Bool *pB    = pIT->getAs<types::Bool>();
            int *piB    = pB->get();
            const int size = pB->getSize();

            //find true item count
            int iItemCount = 0;
            for (int j = 0 ; j < size; j++)
            {
                if (piB[j])
                {
                    iItemCount++;
                }
            }

            //allow new Double variable
            Double* pDbl    = new Double(1, iItemCount);
            double* pdbl    = pDbl->getReal();

            int j = 0;
            for (int l = 0 ; l < size; l++)
            {
                if (piB[l])
                {
                    pdbl[j++] = l + 1;
                }
            }
            pCurrentArg = pDbl;
        }

        if (bDeleteNeeded)
        {
            pIT->killMe();
        }

        if (pCurrentArg)
        {
            const int iCountDim = pCurrentArg->getSize();
            _piMaxDim[i] = 0;
            for (int j = 0 ; j < iCountDim ; j++)
            {
                //checks if size < size(int)
                if (pCurrentArg->get(j) >= INT_MAX)
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("variable size exceeded : less than %d expected.\n").c_str(), INT_MAX);
                    throw ast::ScilabError(szError);
                }

                const int d = static_cast<int>(pCurrentArg->get(j));
                if (d > _piMaxDim[i])
                {
                    _piMaxDim[i] = d;
                }
            }

            iSeqCount *= iCountDim;
            if (_piCountDim)
            {
                _piCountDim[i] = iCountDim;
            }
        }

        _pArgsOut->push_back(pCurrentArg);

    }

    //returns a negative value if at least one parameter is undefined
    //case with : or $ for creation by insertion
    return (!bUndefine ? iSeqCount : - iSeqCount);
}

void cleanIndexesArguments(typed_list* _pArgsOrig, typed_list* _pArgsNew)
{
    //free pArg content
    for (int iArg = 0; iArg < _pArgsNew->size(); iArg++)
    {
        if ((*_pArgsNew)[iArg] != (*_pArgsOrig)[iArg])
        {
            (*_pArgsNew)[iArg]->killMe();
        }
    }

    _pArgsNew->clear();
}

void getIndexesWithDims(int _iIndex, int* _piIndexes, int* _piDims, int _iDims)
{
    int iMul = 1;
    for (int i = 0 ; i < _iDims ; i++)
    {
        _piIndexes[i] = (int)(_iIndex / iMul) % _piDims[i];
        iMul *= _piDims[i];
    }

    //matrix [2,4,3]
    //index = 12 ( 0,2,1) = 1 * 4 * 2 + 2 * 2 + 0 = 12
    //loop 1
    // (12 / 1) % 2 -> 0
    //loop 2
    // (12 / 2) % 4 -> 2
    //loop 3
    // (12 / 8) % 3 -> 1

    //matrix [3,4,3]
    //index = 22
    //loop 1
    // (22 / 1) % 3 -> 1
    //loop 2
    // (22 / 3) % 4 -> 3
    //loop 3
    // (22 / 12) % 3 -> 1

    //matrix [3,4,3]
    //index = 35
    //loop 1
    // (35 / 1) % 3 -> 2
    //loop 2
    // (35 / 3) % 4 -> 3
    //loop 3
    // (35 / 12) % 3 -> 2
}


int getIndexWithDims(int* _piIndexes, int* _piDims, int _iDims)
{
    int idx = 0;
    int iMult = 1;
    for (int i = 0 ; i < _iDims ; i++)
    {
        idx += _piIndexes[i] * iMult;
        iMult *= _piDims[i];
    }
    return idx;
}

types::Function::ReturnValue VariableToString(types::InternalType* pIT, const wchar_t* wcsVarName)
{
    if (pIT->hasToString() == false)
    {
        types::Function::ReturnValue ret = types::Function::Error;
        //call overload %type_p
        types::typed_list in;
        types::typed_list out;
        ast::ExecVisitor exec;

        pIT->IncreaseRef();
        in.push_back(pIT);

        try
        {
            ret = Overload::generateNameAndCall(L"p", in, 1, out, &exec);
            pIT->DecreaseRef();
            return ret;
        }
        catch (ast::ScilabError &e)
        {
            pIT->DecreaseRef();
            throw e;
        }
    }
    else
    {
        std::wostringstream ostr;
        if (pIT->isFunction())
        {
            pIT->getAs<types::Function>()->toString(ostr);
        }
        else if (pIT->isList() || pIT->isCallable())
        {
            ostr << wcsVarName;
        }

        //to manage lines information
        int iLines = ConfigVariable::getConsoleLines();

        bool bFinish = false;
        do
        {
            //block by block
            bFinish = pIT->toString(ostr);
            if (ConfigVariable::isError())
            {
                ConfigVariable::resetError();
                ostr.str(L"");
                return types::Function::Error;
            }

            if (bFinish == false && iLines != 0)
            {
                //show message on prompt
                bFinish = linesmore() == 1;
            }

            scilabForcedWriteW(ostr.str().c_str());
            ostr.str(L"");
        }
        while (bFinish == false);

        pIT->clearPrintState();
        return types::Function::OK;
    }
}
}
