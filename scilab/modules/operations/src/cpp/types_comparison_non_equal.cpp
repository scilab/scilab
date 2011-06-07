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

#include "types_comparison_non_equal.hxx"
#include "types_comparison_equal.hxx"
#include "bool.hxx"

using namespace types;

InternalType *GenericComparisonNonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType* pResult = GenericComparisonEqual(_pLeftOperand, _pRightOperand);
    if(pResult == NULL)
    {//to call overloading
        return NULL;
    }

    Bool *pB = pResult->getAs<Bool>();
    if(pB == NULL)
    {// Oo
        return NULL;
    }

    for(int i = 0 ; i < pB->getSize() ; i++)
    {
        pB->set(i, pB->get(i) == 0);
    }
    return pB;
}