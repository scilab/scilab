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

namespace types
{
//check argument types and compute, dimensions, count of combinations, max indexes
int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim)
{
    int iDims       = static_cast<int>(_pArgsIn->size());
    int iSeqCount   = 1;
    bool bUndefine  = false;
    for (int i = 0 ; i < iDims ; i++)
    {
        InternalType* pIT = (*_pArgsIn)[i];
        Double *pCurrentArg = NULL;

        if (pIT->isDouble())
        {
            pCurrentArg = pIT->getAs<Double>();
            if (pCurrentArg->isEmpty())
            {
                return 0;
            }

        }
        else if (pIT->isColon() || pIT->isImplicitList())
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
                    delete pIL;
                    continue;
                }
                //evalute polynom with "MaxDim"
                int iMaxDim = _pRef->getAs<GenericType>()->getVarMaxDim(i, iDims);
                Double dbl(iMaxDim);
                if (pIL->getStart()->isPoly())
                {
                    Polynom *poPoly	= pIL->getStart()->getAs<types::Polynom>();
                    pIL->setStart(poPoly->evaluate(&dbl));
                }
                if (pIL->getStep()->isPoly())
                {
                    Polynom *poPoly	= pIL->getStep()->getAs<types::Polynom>();
                    pIL->setStep(poPoly->evaluate(&dbl));
                }
                if (pIL->getEnd()->isPoly())
                {
                    Polynom *poPoly	= pIL->getEnd()->getAs<types::Polynom>();
                    pIL->setEnd(poPoly->evaluate(&dbl));
                }
            }

            pCurrentArg = pIL->extractFullMatrix()->getAs<Double>();
            delete pIL;
        }
        else if (pIT->isString())
        {
            //see later for extract from struct or Tlist
            _pArgsOut->push_back(NULL);
            bUndefine = true;
            break;
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
            Double dbl(iMaxDim); // $
            pCurrentArg = pMP->evaluate(&dbl);
        }
        else if (pIT->isBool())
        {
            //[T F F T F]
            Bool *pB    = pIT->getAs<types::Bool>();
            int *piB    = pB->get();

            //find true item count
            int iItemCount = 0;
            for (int j = 0 ; j < pB->getSize() ; j++)
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
            for (int l = 0 ; l < pB->getSize() ; l++)
            {
                if (piB[l])
                {
                    pdbl[j++] = l + 1;
                }
            }
            pCurrentArg = pDbl;
        }

        if (pCurrentArg)
        {
            _piCountDim[i] = pCurrentArg->getSize();
            _piMaxDim[i] = 0;
            for (int j = 0 ; j < _piCountDim[i] ; j++)
            {
                _piMaxDim[i] = Max(_piMaxDim[i], static_cast<int>(pCurrentArg->get(j)));
            }

            iSeqCount *= _piCountDim[i];
        }

        _pArgsOut->push_back(pCurrentArg);

    }

    //returns a negative value if at least one parameter is undefined
    //case with : or $ for creation by insertion
    return (!bUndefine ? iSeqCount : - iSeqCount);
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
}
