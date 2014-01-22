/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_eq.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "list.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "sparse.hxx"
#include "int.hxx"
#include "graphichandle.hxx"

using namespace types;

static void clearAlloc(bool _bAllocL, InternalType* _pIL, bool _bAllocR, InternalType* _pIR);
template <class T> static int EqualToArrayAndArray(T* _pL, T* _pR, GenericType** _pOut);

InternalType *GenericComparisonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    InternalType* pIL = _pLeftOperand;
    InternalType* pIR = _pRightOperand;
    bool bAllocL = false;
    bool bAllocR = false;

    InternalType *pResult = NULL;

    /*
    ** [] == ??
    */
    if (TypeL == GenericType::RealDouble && pIL->getAs<Double>()->getSize() == 0)
    {
        if (TypeR != InternalType::RealDouble)
        {
            return new Bool(false);
        }
    }

    /*
    ** ?? == []
    */
    if (TypeR == GenericType::RealDouble && pIR->getAs<Double>()->getSize() == 0)
    {
        if (TypeL != InternalType::RealDouble)
        {
            return new Bool(false);
        }
    }

    if (TypeL == GenericType::RealColon && TypeR == GenericType::RealColon)
    {
        //: == :
        return new Bool(true);
    }

    if (TypeL == GenericType::RealColon)
    {
        // : == x
        if (pIR->isGenericType())
        {
            //transform left operand in a identity matrix with same dimension of right operand
            GenericType* pGT = pIR->getAs<GenericType>();
            pIL = Double::Identity(pGT->getDims(), pGT->getDimsArray());;
            TypeL = GenericType::RealDouble;
            bAllocL = true;
        }
        else
        {
            return new Bool(false);
        }
    }

    if (TypeR == GenericType::RealColon)
    {
        // x == :
        if (pIL->isGenericType())
        {
            //transform right operand in a identity matrix with same dimensions of left operand
            GenericType* pGT = pIL->getAs<GenericType>();
            pIR = Double::Identity(pGT->getDims(), pGT->getDimsArray());;
            TypeR = GenericType::RealDouble;
            bAllocR = true;
        }
        else
        {
            return new Bool(false);
        }
    }

    /*
    ** DOUBLE == DOUBLE
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL   = pIL->getAs<Double>();
        Double *pR   = pIR->getAs<Double>();

        int iResult = EqualToDoubleAndDouble(pL, pR, (GenericType**)&pResult);
        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        if (iResult)
        {

            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** STRING == STRING
    */
    if (TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL   = pIL->getAs<String>();
        String *pR   = pIR->getAs<String>();

        if (pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            wchar_t* pstL = pL->get(0);
            for (int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, wcscmp(pstL, pR->get(i)) == 0);
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        if (pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            wchar_t* pstR = pR->get(0);
            for (int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, wcscmp(pL->get(i), pstR) == 0);
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        int iDims1 = pL->getDims();
        int iDims2 = pR->getDims();

        if (iDims1 != iDims2)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for (int i = 0 ; i < iDims1 ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for (int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, wcscmp(pL->get(i), pR->get(i)) == 0);
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** BOOL == BOOL
    */
    if (TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
    {
        Bool *pL   = pIL->getAs<Bool>();
        Bool *pR   = pIR->getAs<Bool>();

        if (pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            int iL = pL->get(0);
            for (int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, iL == pR->get(i));
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        if (pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            int iR = pR->get(0);
            for (int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, iR == pL->get(i));
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }


        int iDims1 = pL->getDims();
        int iDims2 = pR->getDims();

        if (iDims1 != iDims2)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for (int i = 0 ; i < iDims1 ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for (int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, pL->get(i) == pR->get(i));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** POLY == POLY
    */
    if (TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
    {
        Polynom *pL   = pIL->getAs<Polynom>();
        Polynom *pR   = pIR->getAs<Polynom>();

        if (pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            SinglePoly* pSL = pL->get(0);
            for (int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, *pSL == *pR->get(i));
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        if (pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            SinglePoly* pSR = pR->get(0);
            for (int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, *pSR == *pL->get(i));
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        if (pL->getDims() != pR->getDims())
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for (int i = 0 ; i < pL->getDims() ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for (int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, *pL->get(i) == *pR->get(i));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** DOUBLE == POLY
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL  = pIL->getAs<Double>();
        Polynom *pR = pIR->getAs<Polynom>();

        if (pR->isScalar())
        {
            pResult         = new Bool(pL->getDims(), pL->getDimsArray());
            int* piResult   = pResult->getAs<Bool>()->get();
            int iSize       = pL->getSize();
            int iRank       = 0;

            pR->getRealRank(&iRank);
            if(iRank != 0) // check rank
            {
                memset(piResult, 0x00, iSize * sizeof(int));
            }
            else
            {
                // check values
                double dR       = pR->get(0)->getCoef()->get(0);
                double* pdblL   = pL->get();
                if(pL->isComplex() && pR->isComplex())
                {
                    double dRImg    = pR->get(0)->getCoef()->getImg(0);
                    double* pdblLImg= pL->getImg();
                    for (int i = 0 ; i < iSize; i++)
                    {
                        piResult[i] = (int)((dR == pdblL[i]) && (dRImg == pdblLImg[i]));
                    }
                }
                else if(pL->isComplex())
                {
                    double* pdblLImg= pL->getImg();
                    for (int i = 0 ; i < iSize; i++)
                    {
                        piResult[i] = (int)((dR == pdblL[i]) && (pdblLImg[i] == 0));
                    }
                }
                else if(pR->isComplex())
                {
                    double dRImg    = pR->get(0)->getCoef()->getImg(0);
                    for (int i = 0 ; i < iSize; i++)
                    {
                        piResult[i] = (int)((dR == pdblL[i]) && (dRImg == 0));
                    }
                }
                else
                {
                    for (int i = 0 ; i < iSize; i++)
                    {
                        piResult[i] = (int)(dR == pdblL[i]);
                    }
                }
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return pResult;
        }

        if (pL->isScalar())
        {
            pResult         = new Bool(pR->getDims(), pR->getDimsArray());
            int* piResult   = pResult->getAs<Bool>()->get();
            int iSize       = pR->getSize();
            int* piRank     = new int[iSize];
            double dL       = pL->get(0);

            pR->getRealRank(piRank);

            if(pL->isComplex() && pR->isComplex())
            {
                double dLImg = pL->getImg(0);
                for (int i = 0 ; i < iSize; i++)
                {
                    if(piRank[i] == 0)
                    {
                        piResult[i] = (int)(dL == pR->get(i)->getCoef()->get(0) &&
                                            dLImg == pR->get(i)->getCoef()->getImg(0));
                    }
                    else
                    {
                        piResult[i] = 0;
                    }
                }
            }
            else if(pL->isComplex())
            {
                double dLImg = pL->getImg(0);
                for (int i = 0 ; i < iSize; i++)
                {
                    if(piRank[i] == 0)
                    {
                        piResult[i] = (int)(dL == pR->get(i)->getCoef()->get(0) &&
                                            dLImg == 0);
                    }
                    else
                    {
                        piResult[i] = 0;
                    }
                }
            }
            else if(pR->isComplex())
            {
                for (int i = 0 ; i < iSize; i++)
                {
                    if(piRank[i] == 0)
                    {
                        piResult[i] = (int)(dL == pR->get(i)->getCoef()->get(0) &&
                                            pR->get(i)->getCoef()->getImg(0) == 0);
                    }
                    else
                    {
                        piResult[i] = 0;
                    }
                }
            }
            else
            {
                for (int i = 0 ; i < iSize; i++)
                {
                    piResult[i] = piRank[i] ? 0 : (int)(dL == pR->get(i)->getCoef()->get(0));
                }
            }

            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            delete[] piRank;
            return pResult;
        }

        if (pL->getDims() != pR->getDims())
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for (int i = 0 ; i < pL->getDims() ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        pResult         = new Bool(pL->getDims(), pL->getDimsArray());
        int* piResult   = pResult->getAs<Bool>()->get();
        int iSize       = pL->getSize();
        int* piRank     = new int[iSize];
        double* pdblL   = pL->get();

        pR->getRealRank(piRank);

        if(pL->isComplex() && pR->isComplex())
        {
            double* pdblLImg = pL->getImg();
            for (int i = 0 ; i < iSize; i++)
            {
                if(piRank[i] == 0)
                {
                    piResult[i] = (int)(pdblL[i] == pR->get(i)->getCoef()->get(0) &&
                                        pdblLImg[i] == pR->get(i)->getCoef()->getImg(0));
                }
                else
                {
                    piResult[i] = 0;
                }
            }
        }
        else if(pL->isComplex())
        {
            double* pdblLImg = pL->getImg();
            for (int i = 0 ; i < iSize; i++)
            {
                if(piRank[i] == 0)
                {
                    piResult[i] = (int)(pdblL[i] == pR->get(i)->getCoef()->get(0) &&
                                        pdblLImg[i] == 0);
                }
                else
                {
                    piResult[i] = 0;
                }
            }
        }
        else if(pR->isComplex())
        {
            for (int i = 0 ; i < iSize; i++)
            {
                if(piRank[i] == 0)
                {
                    piResult[i] = (int)(pdblL[i] == pR->get(i)->getCoef()->get(0) &&
                                        pR->get(i)->getCoef()->getImg(0) == 0);
                }
                else
                {
                    piResult[i] = 0;
                }
            }
        }
        else
        {
            for (int i = 0 ; i < iSize; i++)
            {
                piResult[i] = piRank[i] ? 0 : (int)(pdblL[i] == pR->get(i)->getCoef()->get(0));
            }
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        delete[] piRank;
        return pResult;
    }

    /*
    ** POLY == DOUBLE
    */
    if (TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        return GenericComparisonEqual(_pRightOperand, _pLeftOperand);
    }

    /*
    ** LIST == LIST
    */
    if (TypeL == GenericType::RealList && TypeR == GenericType::RealList)
    {
        List* pLL = pIL->getAs<List>();
        List* pLR = pIR->getAs<List>();

        if (pLL->getSize() != pLR->getSize())
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        if (pLL->getSize() == 0 && pLR->getSize() == 0)
        {
            //list() == list() -> return true
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(true);
        }

        Bool* pB = new Bool(1, pLL->getSize());
        for (int i = 0 ; i < pLL->getSize() ; i++)
        {
            pB->set(i, *pLL->get(i) == *pLR->get(i));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pB;
    }

    /*
    ** CELL == CELL
    */
    if (TypeL == GenericType::RealCell && TypeR == GenericType::RealCell)
    {
        Cell* pCL = pIL->getAs<Cell>();
        Cell* pCR = pIR->getAs<Cell>();

        /* check dimension*/
        if (pCL->getDims() != pCR->getDims())
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDimsL = pCL->getDimsArray();
        int* piDimsR = pCR->getDimsArray();

        for (int i = 0 ; i < pCL->getDims() ; i++)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        if (pCL->getSize() == 0)
        {
            //{} == {} -> return true
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(true);
        }

        Bool *pB = new Bool(pCL->getDims(), piDimsL);
        for (int i = 0 ; i < pCL->getSize() ; i++)
        {
            pB->set(i, *pCL->get(i) == *pCR->get(i));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pB;
    }

    /*
    ** STRUCT == STRUCT
    */
    if (TypeL == GenericType::RealStruct && TypeR == GenericType::RealStruct)
    {
        Struct* pStL = pIL->getAs<Struct>();
        Struct* pStR = pIR->getAs<Struct>();

        /* check dimension*/
        if (pStL->getDims() != pStR->getDims())
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(false);
        }

        int* piDimsL = pStL->getDimsArray();
        int* piDimsR = pStR->getDimsArray();

        for (int i = 0 ; i < pStL->getDims() ; i++)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                clearAlloc(bAllocL, pIL, bAllocR, pIR);
                return new Bool(false);
            }
        }

        if (pStL->getSize() == 0)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return new Bool(true);
        }

        Bool *pB = new Bool(pStL->getDims(), piDimsL);
        for (int i = 0 ; i < pStL->getSize() ; i++)
        {
            pB->set(i, *pStL->get(i) == *pStR->get(i));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pB;
    }

    /*
    ** DOUBLE == STRING
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealString)
    {
        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return new Bool(0);
    }

    /*
    ** STRING == DOUBLE
    */
    if (TypeL == GenericType::RealString && TypeR == GenericType::RealDouble)
    {
        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return new Bool(0);
    }

    /*
    ** SPARSE == SPARSE
    */
    if (TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
    {
        Sparse* pL = pIL->getAs<Sparse>();
        Sparse* pR = pIR->getAs<Sparse>();

        int iResult = EqualToSparseAndSparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** SPARSE == DOUBLE
    */
    if (TypeL == GenericType::RealSparse && TypeR == GenericType::RealDouble)
    {
        Sparse* pL = pIL->getAs<Sparse>();
        Double* pR = pIR->getAs<Double>();

        int iResult = EqualToSparseAndDouble(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** DOUBLE == SPARSE
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealSparse)
    {
        Double* pL = pIL->getAs<Double>();
        Sparse* pR = pIR->getAs<Sparse>();

        int iResult = EqualToDoubleAndSparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** SPARSE BOOL == SPARSE BOOL
    */
    if (TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
    {
        SparseBool* pL = pIL->getAs<SparseBool>();
        SparseBool* pR = pIR->getAs<SparseBool>();

        int iResult = EqualToSparseBoolAndSparseBool(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** SPARSE BOOL == BOOL
    */
    if (TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealBool)
    {
        SparseBool* pL = pIL->getAs<SparseBool>();
        Bool* pR = pIR->getAs<Bool>();

        int iResult = EqualToSparseBoolAndBool(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** BOOL == SPARSE BOOL
    */
    if (TypeL == GenericType::RealBool && TypeR == GenericType::RealSparseBool)
    {
        Bool* pL = pIL->getAs<Bool>();
        SparseBool* pR = pIR->getAs<SparseBool>();

        int iResult = EqualToBoolAndSparseBool(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** INT == INT
    */

    if (pIL->isInt() && pIR->isInt())
    {
        if (pIL->getType() != pIR->getType())
        {
            //call overload function to convert left or right or both to have comparable type
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            return NULL;
        }

        int iResult = EqualToIntAndInt(pIL, pIR, (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** HANDLE == HANDLE
    */

    if (pIL->isHandle() && pIR->isHandle())
    {
        int iResult = EqualToArrayAndArray(pIL->getAs<GraphicHandle>(), pIR->getAs<GraphicHandle>(), (GenericType**)&pResult);
        if (iResult)
        {
            clearAlloc(bAllocL, pIL, bAllocR, pIR);
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        clearAlloc(bAllocL, pIL, bAllocR, pIR);
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    clearAlloc(bAllocL, pIL, bAllocR, pIR);
    return NULL;
}

int EqualToDoubleAndDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::GenericType** _pOut)
{
    int iFree = 0;

    if (_pDouble1->isEmpty() && _pDouble2->isEmpty())
    {
        *_pOut = new Bool(true);
        return 0;
    }

    if (_pDouble1->isEmpty() || _pDouble2->isEmpty())
    {
        *_pOut = new Bool(false);
        return 0;
    }

    if (_pDouble2->isComplex() && _pDouble1->isComplex() == false)
    {
        _pDouble1 = _pDouble1->clone()->getAs<Double>();
        _pDouble1->setComplex(true);
        iFree = 1;
    }

    if (_pDouble2->isComplex() == false && _pDouble1->isComplex())
    {
        _pDouble2 = _pDouble2->clone()->getAs<Double>();
        _pDouble2->setComplex(true);
        iFree = 2;
    }

    if (_pDouble2->isScalar())
    {
        Bool* pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());
        double dblRefR = _pDouble2->get(0);

        if (_pDouble1->isComplex())
        {
            double dblRefI = _pDouble2->getImg(0);
            for (int i = 0 ; i < _pDouble1->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, (_pDouble1->get(i) == dblRefR) && (_pDouble1->getImg(i) == dblRefI));
            }
        }
        else
        {
            for (int i = 0 ; i < _pDouble1->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, _pDouble1->get(i) == dblRefR);
            }
        }

        *_pOut = pB;
        return 0;
    }

    if (_pDouble1->isScalar())
    {
        Bool* pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
        double dblRefR = _pDouble1->get(0);

        if (_pDouble2->isComplex())
        {
            double dblRefI = _pDouble1->getImg(0);
            for (int i = 0 ; i < _pDouble2->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, (dblRefR == _pDouble2->get(i)) && (dblRefI == _pDouble2->getImg(i)));
            }
        }
        else
        {
            for (int i = 0 ; i < _pDouble2->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, dblRefR == _pDouble2->get(i));
            }
        }

        *_pOut = pB;
        return 0;
    }


    if (_pDouble1->getDims() != _pDouble2->getDims())
    {
        *_pOut = new Bool(false);
        return 0;
    }

    int* piDimsL = _pDouble1->getDimsArray();
    int* piDimsR = _pDouble2->getDimsArray();

    for (int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            *_pOut = new Bool(false);
            return 0;
        }
    }

    Bool* pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
    if (_pDouble1->isComplex())
    {
        for (int i = 0 ; i < _pDouble1->getSize() ; i++)
        {
            pB->set(i, (_pDouble1->get(i) == _pDouble2->get(i)) && (_pDouble1->getImg(i) == _pDouble2->getImg(i)));
        }
    }
    else
    {
        for (int i = 0 ; i < _pDouble1->getSize() ; i++)
        {
            pB->set(i, _pDouble1->get(i) == _pDouble2->get(i));
        }
    }

    if (iFree == 1)
    {
        delete _pDouble1;
    }
    else if (iFree == 2)
    {
        delete _pDouble2;
    }

    *_pOut = pB;
    return 0;
}

int EqualToSparseBoolAndSparseBool(SparseBool* _pSB1, SparseBool* _pSB2, GenericType** _pOut)
{
    SparseBool* pOut = NULL;
    if (_pSB1->isScalar())
    {
        pOut = new SparseBool(_pSB2->getRows(), _pSB2->getCols());
        bool bVal = _pSB1->get(0, 0);

        for (int i = 0 ; i < pOut->getRows() ; i++)
        {
            for (int j = 0 ; j < pOut->getCols() ; j++)
            {
                pOut->set(i, j, bVal == _pSB2->get(i, j));
            }
        }

        pOut->finalize();
        *_pOut = pOut;
        return 0;
    }

    if (_pSB2->isScalar())
    {
        pOut = new SparseBool(_pSB1->getRows(), _pSB1->getCols());
        bool bVal = _pSB2->get(0, 0);

        for (int i = 0 ; i < pOut->getRows() ; i++)
        {
            for (int j = 0 ; j < pOut->getCols() ; j++)
            {
                pOut->set(i, j, _pSB1->get(i, j) == bVal);
            }
        }

        *_pOut = pOut;
        pOut->finalize();
        return 0;
    }

    if (_pSB1->getRows() != _pSB2->getRows() || _pSB1->getCols() != _pSB2->getCols())
    {
        *_pOut = new Bool(false);
        return 0;
    }

    pOut = new SparseBool(_pSB1->getRows(), _pSB1->getCols());

    for (int i = 0 ; i < pOut->getRows() ; i++)
    {
        for (int j = 0 ; j < pOut->getCols() ; j++)
        {
            pOut->set(i, j, _pSB1->get(i, j) == _pSB2->get(i, j));
        }
    }

    pOut->finalize();
    *_pOut = pOut;
    return 0;
}

int EqualToSparseAndSparse(Sparse* _pSparse1, Sparse* _pSparse2, GenericType** _pOut)
{
    if ((_pSparse1->getRows() == _pSparse2->getRows() && _pSparse1->getCols() == _pSparse2->getCols()) //matrix case
            || _pSparse1->isScalar() || _pSparse2->isScalar()) //scalar cases
    {
        *_pOut = _pSparse1->newEqualTo(*_pSparse2);
    }
    else
    {
        *_pOut = new Bool(false);
    }
    return 0;
}

int EqualToDoubleAndSparse(Double* _pDouble, Sparse* _pSparse, GenericType** _pOut)
{
    Sparse* pSparse = NULL;
    if (_pDouble->isScalar())
    {
        pSparse = new Sparse(_pSparse->getRows(), _pSparse->getCols(), _pDouble->isComplex());
        if (pSparse->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            for (int i = 0 ; i < pSparse->getRows() ; i++)
            {
                for (int j = 0 ; j < pSparse->getCols() ; j++)
                {
                    pSparse->set(i, j, dbl);
                }
            }
        }
        else
        {
            double dbl = _pDouble->get(0);
            for (int i = 0 ; i < pSparse->getRows() ; i++)
            {
                for (int j = 0 ; j < pSparse->getCols() ; j++)
                {
                    pSparse->set(i, j, dbl);
                }
            }
        }
    }
    else
    {
        pSparse = new Sparse(*_pDouble);
    }

    int iRet = EqualToSparseAndSparse(pSparse, _pSparse, _pOut);
    delete pSparse;
    return iRet;
}

int EqualToSparseAndDouble(Sparse* _pSparse, Double* _pDouble, GenericType** _pOut)
{
    return EqualToDoubleAndSparse(_pDouble, _pSparse, _pOut);
}

int EqualToSparseBoolAndBool(SparseBool* _pSB1, Bool* _pB2, GenericType** _pOut)
{
    SparseBool* pSB = new SparseBool(*_pB2);
    int iRet = EqualToSparseBoolAndSparseBool(_pSB1, pSB, _pOut);
    delete pSB;
    return iRet;
}

int EqualToBoolAndSparseBool(Bool* _pB1, SparseBool* _pSB2, GenericType** _pOut)
{
    SparseBool* pSB = new SparseBool(*_pB1);
    int iRet = EqualToSparseBoolAndSparseBool(pSB, _pSB2, _pOut);
    delete pSB;
    return iRet;
}

template <class T>
static int EqualToArrayAndArray(T* _pL, T* _pR, GenericType** _pOut)
{
    if (_pL->isScalar())
    {
        Bool *pB = new Bool(_pR->getDims(), _pR->getDimsArray());

        int* pb = pB->get();

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = _pL->get(0) == _pR->get(i);
        }

        *_pOut = pB;
        return 0;
    }

    if (_pR->isScalar())
    {
        Bool *pB = new Bool(_pL->getDims(), _pL->getDimsArray());

        int* pb = pB->get();

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = _pR->get(0) == _pL->get(i);
        }

        *_pOut = pB;
        return 0;
    }

    if (_pL->getDims() != _pR->getDims())
    {
        *_pOut = new Bool(false);
        return 0;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < _pL->getDims() ; i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            *_pOut = new Bool(false);
            return 0;
        }
    }

    Bool* pB = new Bool(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pB->set(i, _pL->get(i) == _pR->get(i));
    }

    *_pOut = pB;
    return 0;
}

int EqualToIntAndInt(InternalType* _pL, InternalType*  _pR, GenericType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::RealInt8 :
        {
            return EqualToArrayAndArray(_pL->getAs<Int8>(), _pR->getAs<Int8>(), _pOut);
        }
        case InternalType::RealUInt8 :
        {
            return EqualToArrayAndArray(_pL->getAs<UInt8>(), _pR->getAs<UInt8>(), _pOut);
        }
        case InternalType::RealInt16 :
        {
            return EqualToArrayAndArray(_pL->getAs<Int16>(), _pR->getAs<Int16>(), _pOut);
        }
        case InternalType::RealUInt16 :
        {
            return EqualToArrayAndArray(_pL->getAs<UInt16>(), _pR->getAs<UInt16>(), _pOut);
        }
        case InternalType::RealInt32 :
        {
            return EqualToArrayAndArray(_pL->getAs<Int32>(), _pR->getAs<Int32>(), _pOut);
        }
        case InternalType::RealUInt32 :
        {
            return EqualToArrayAndArray(_pL->getAs<UInt32>(), _pR->getAs<UInt32>(), _pOut);
        }
        case InternalType::RealInt64 :
        {
            return EqualToArrayAndArray(_pL->getAs<Int64>(), _pR->getAs<Int64>(), _pOut);
        }
        case InternalType::RealUInt64 :
        {
            return EqualToArrayAndArray(_pL->getAs<UInt64>(), _pR->getAs<UInt64>(), _pOut);
        }
    }

    return 3;
}

static void clearAlloc(bool _bAllocL, InternalType* _pIL, bool _bAllocR, InternalType* _pIR)
{
    if (_bAllocL)
    {
        delete _pIL;
    }

    if (_bAllocR)
    {
        delete _pIR;
    }
}
