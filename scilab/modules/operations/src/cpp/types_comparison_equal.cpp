/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_equal.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"

using namespace types;

InternalType *GenericComparisonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();
    InternalType *pResult = NULL;

    /*
    ** [] == ??
    */
    if(TypeL == GenericType::RealDouble && _pLeftOperand->getAs<Double>()->getSize() == 0)
    {
        if(TypeR != InternalType::RealDouble)
        {
            return (new Bool(false));
        }
    }

    /*
    ** ?? == []
    */
    if(TypeR == GenericType::RealDouble && _pRightOperand->getAs<Double>()->getSize() == 0)
    {
        if(TypeL != InternalType::RealDouble)
        {
            return(new Bool(false));
        }
    }

    /*
    ** DOUBLE == DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        if(pR->getSize() == 0 && pL->getSize() == 0)
        {
            pResult = new Bool(true);
        }
        else if(pL->getSize() == 0  || pR->getSize() == 0)
        {
            pResult = new Bool(false);
        }
        else if(pR->getSize() == 1)
        {
            pResult				= new Bool(pL->getRows(), pL->getCols());
            double dblRef	= pR->getReal(0,0);
            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) == dblRef);
                }
            }
        }
        else if(pL->getSize() == 1)
        {
            pResult				= new Bool(pR->getRows(), pR->getCols());
            double dblRef	= pL->getReal(0,0);
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    pResult->getAs<types::Bool>()->set(i, j, dblRef == pR->getReal(i, j));
                }
            }
        }
        else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
        {
            pResult				= new Bool(pR->getRows(), pR->getCols());
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) == pR->getReal(i, j));
                }
            }
        }
        else
        {
            pResult = new Bool(false);
        }

        return pResult;
    }

    /*
    ** STRING == STRING
    */
    if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL			= _pLeftOperand->getAs<types::String>();
        String *pR			= _pRightOperand->getAs<types::String>();

        if(pL->getSize() == 1)
        {
            pResult = new Bool(pR->getRows(), pR->getCols());

            wchar_t* pstL = pL->get()[0];
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    wchar_t* pstR = pR->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pR->getSize() == 1)
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            wchar_t* pstR = pR->get()[0];
            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    wchar_t* pstL = pL->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    wchar_t* pstR = pR->get(i,j);
                    wchar_t* pstL = pL->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else
        {
            pResult = new Bool(false);
        }
        return pResult;
    }

    /*
    ** BOOL == BOOL
    */
    if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
    {
        Bool *pL			= _pLeftOperand->getAs<types::Bool>();
        Bool *pR			= _pRightOperand->getAs<types::Bool>();

        if(pL->getSize() == 1)
        {
            pResult = new Bool(pR->getRows(), pR->getCols());

            int iL = pL->get()[0];
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    if(iL == pR->get(i,j))
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pR->getSize() == 1)
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            int iR = pR->get()[0];
            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    if(iR == pL->get(i,j))
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    if(pL->get(i,j) == pR->get(i,j))
                    {
                        pResult->getAs<types::Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<types::Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else
        {
            pResult = new Bool(false);
        }
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}
