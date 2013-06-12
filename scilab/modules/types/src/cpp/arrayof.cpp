/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "double.hxx"

namespace types
{
/*    template <typename T>
    ArrayOf<T>* createEmptyDouble()
    {
        return Double::Empty();
    }
*/
//n-uplet in french
int computeTuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex)
{
    //if bRet == 1, previous dims has reach max value.
    int iRet = 0;

    if (_iCurrentDim == 0)
    {
        //last dims
        if (_piIndex[_iCurrentDim] >= _piCountDim[_iCurrentDim])
        {
            _piIndex[_iCurrentDim] = 0;
            return 1;
        }
    }
    else
    {
        iRet = computeTuples(_piCountDim, _iDims, _iCurrentDim - 1, _piIndex);
        if (iRet)
        {
            _piIndex[_iCurrentDim]++;
            if (_piIndex[_iCurrentDim] >= _piCountDim[_iCurrentDim])
            {
                _piIndex[_iCurrentDim] = 0;
                return 1;
            }
        }
    }
    return 0;
}

InternalType* createEmptyDouble()
{
    return Double::Empty();
}

int getIntValueFromDouble(InternalType* _pIT, int _iPos)
{
    return static_cast<int>(_pIT->getAs<Double>()->get(_iPos));
}

double* getDoubleArrayFromDouble(InternalType* _pIT)
{
    return _pIT->getAs<Double>()->get();
}

InternalType* createDoubleVector(int _iSize)
{
    int piDims[] = {1, _iSize};
    Double* pOut = new Double(2, piDims);
    for (int i = 0 ; i < _iSize ; i++)
    {
        pOut->set(i, i + 1);
    }
    return pOut;
}

bool checkArgValidity(typed_list& _Arg)
{
    for (int i = 0 ; i < _Arg.size() ; i++)
    {
        if ( _Arg[i]->isDouble() == false)
        {
            return false;
        }

        Double* pDbl = _Arg[i]->getAs<Double>();
        double* pdbl = pDbl->get();
        for (int j = 0 ; j < pDbl->getSize() ; j++)
        {
            if (pdbl[j] <= 0)
            {
                return false;
            }
        }
    }

    return true;
}
}
