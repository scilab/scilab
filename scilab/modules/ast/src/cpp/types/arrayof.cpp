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
#include "bool.hxx"
#include "singlepoly.hxx"
#include "singlestruct.hxx"

extern "C"
{
#include "dynlib_ast.h"
}

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
    for (int i = 0 ; i < (int)_Arg.size() ; i++)
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

template <typename T>
InternalType* ArrayOf<T>::insert(typed_list* _pArgs, InternalType* _pSource)
{
    if (getRef() > 1)
    {
        // An ArrayOf content in more than one Scilab variable
        // must be cloned before to be modified.
        ArrayOf* pClone = clone()->getAs<ArrayOf>();
        InternalType* pIT = pClone->insert(_pArgs, _pSource);
        if (pIT == NULL)
        {
            pClone->killMe();
        }

        return pIT;
    }

    bool bNeedToResize  = false;
    int iDims           = (int)_pArgs->size();
    int iDimsOrigine = m_iDims;
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //on case of resize
    int* piNewDims      = NULL;
    int iNewDims        = 0;
    ArrayOf* pSource = _pSource->getAs<ArrayOf>();

    bool bIsColon = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    if (iDims >= m_iDims)
    {
        //all case are good, we can do almost everything
        //check if resize is needed
        if (iDims > m_iDims)
        {
            bNeedToResize = true;
        }
        else //_iDims == m_iDims
        {
            for (int i = 0 ; i < m_iDims ; i++)
            {
                if (m_piDims[i] < piMaxDim[i])
                {
                    bNeedToResize = true;
                    break;
                }
            }
        }

        //determine new dimension of the matrix
        if (bNeedToResize == true)
        {
            iNewDims = iDims;
            piNewDims = new int[iNewDims];
            for (int i = 0 ; i < m_iDims ; i++)
            {
                piNewDims[i] = std::max(piMaxDim[i], m_piDims[i]);
            }

            int iSource = (pSource->getDims() - 1);
            bool bPassed = false;
            int *piSourceDims = pSource->getDimsArray();

            for (int i = (iNewDims - 1); i >= m_iDims; i--)
            {
                if ((*_pArgs)[i]->isColon())
                {
                    bIsColon = true;
                    if (pSource->isVector() && ( bPassed == false ))
                    {
                        piNewDims[i] = std::max(piSourceDims[0], piSourceDims[1]);
                    }
                    else if ((iSource < 0) || (bPassed))
                    {
                        piNewDims[i] = 1;
                    }
                    else
                    {
                        piNewDims[i] = piSourceDims[iSource];
                        iSource--;
                    }
                }
                else
                {
                    piNewDims[i] = piMaxDim[i];
                }
            }
        }
    }
    else // _iDims < m_iDims
    {
        if (isVector() || isScalar() || getSize() == 0) //getSize() == 0, on ly for [] and {}
        {
            if (getSize() < piMaxDim[0])
            {
                bNeedToResize = true;
                iNewDims = 2;
                piNewDims = new int[2];

                if (getCols() == 1 || getSize() == 0)
                {
                    //column vector
                    piNewDims[0] = piMaxDim[0];
                    piNewDims[1] = 1;
                }
                else if (getRows() == 1)
                {
                    //row vector
                    piNewDims[0] = 1;
                    piNewDims[1] = piMaxDim[0];
                }
            }
        }
        else
        {
            //each index before last index must be in range of his dimension
            //and last given dimension can not be > prod(last dimensions)
            for (int i = 0 ; i < iDims - 1 ; i++)
            {
                //indexes are always doubles
                double* pIdx = getDoubleArrayFromDouble(pArg[i]);
                //InternalType* pVar = pArg[i];
                //double* pIdx = static_cast<double*>(pVar->getAs<Double>()->get());
                int iSize = pArg[i]->getAs<ArrayOf>()->getSize();
                for (int j = 0 ; j < iSize ; j++)
                {
                    if (pIdx[j] >= m_piDims[i])
                    {
                        //free pArg content
                        cleanIndexesArguments(_pArgs, &pArg);
                        return NULL;
                    }
                }
            }

            //check last dim
            int iMaxLastDim = getVarMaxDim(iDims - 1, iDims);
            double* pIdx = getDoubleArrayFromDouble(pArg[pArg.size() - 1]);
            //InternalType* pVar = pArg[pArg.size() - 1];
            //double* pIdx = static_cast<double*>(pVar->getAs<Double>()->get());
            int iSize = pArg[pArg.size() - 1]->getAs<GenericType>()->getSize();
            for (int i = 0 ; i < iSize ; i++)
            {
                if (pIdx[i] > iMaxLastDim)
                {
                    //free pArg content
                    cleanIndexesArguments(_pArgs, &pArg);
                    return NULL;
                }
            }
        }
    }

    //before resize, check input dimension

    if (bNeedToResize)
    {
        bool bPass = resize(piNewDims, iNewDims);
        if (bPass == false)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }
    else
    {
        piNewDims   = m_piDims;
        iNewDims    = m_iDims;
    }

    //update complexity
    if (pSource->isComplex() && m_pImgData == NULL)
    {
        setComplex(true);
    }

    int* piIndex    = new int[_pArgs->size()];
    int* piCoord    = new int[_pArgs->size()];
    int* piViewDims = new int[iDims];
    memset(piIndex, 0x00, sizeof(int) * _pArgs->size());

    //convert  current dimension to view dimension
    for (int i = 0 ; i < iDims ; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iDims);
    }

    T* pRealData    = pSource->get();
    T* pImgData     = pSource->getImg();
    bool bComplex   = pSource->isComplex();

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        computeTuples(piCountDim, (int)_pArgs->size(), (int)_pArgs->size() - 1, piIndex);

        //std::cout << "[";
        for (int j = 0 ; j < (int)_pArgs->size() ; j++)
        {
            piCoord[j] = getIntValueFromDouble(pArg[j], piIndex[j]) - 1;
            //InternalType* pVar = pArg[j];
            //piCoord[j] = static_cast<int>(pVar->getAs<Double>()->get(piIndex[j]) - 1);
            //std::cout << piCoord[j] << " ";
        }

        //std::cout << "]" << std::endl;

        int iPos = getIndexWithDims(piCoord, piViewDims, iDims);
        if (iPos < 0)
        {
            if (bNeedToResize)
            {
                delete[] piNewDims;
            }

            delete[] piMaxDim;
            delete[] piCountDim;
            delete[] piIndex;
            delete[] piCoord;
            delete[] piViewDims;

            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);

            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("Invalid index.\n").c_str());
            throw ast::ScilabError(szError);
        }

        if (pSource->isScalar())
        {
            //element-wise insertion
            set(iPos, pRealData[0]);
            if (pImgData != NULL && bComplex)
            {
                setImg(iPos, pImgData[0]);
            }
        }
        else
        {
            if ( bIsColon)
            {
                int iPas = 1;
                for (int j = 0; j < iDimsOrigine; j++)
                {
                    iPas = iPas * m_piDims[j];
                }

                for (int iPost = iPos; iPost < this->getSize(); iPost += iPas)
                {
                    set(iPost, pRealData[i]);
                    if (pImgData != NULL && bComplex)
                    {
                        setImg(iPost, pImgData[i]);
                    }
                    i++;
                }
            }
            else
            {
                set(iPos, pRealData[i]);
                if (pImgData != NULL && bComplex)
                {
                    setImg(iPos, pImgData[i]);
                }
            }
        }

        // reset imaginary part
        if (m_pImgData != NULL && bComplex == false)
        {
            setImg(iPos, 0);
        }

        //update index
        piIndex[0]++;
    }

    if (bNeedToResize)
    {
        delete[] piNewDims;
    }

    delete[] piMaxDim;
    delete[] piCountDim;
    delete[] piIndex;
    delete[] piCoord;
    delete[] piViewDims;

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return this;
}

template <typename T>
InternalType* ArrayOf<T>::insertNew(typed_list* _pArgs, InternalType* _pSource)
{
    typed_list pArg;
    InternalType *pOut  = NULL;
    ArrayOf* pSource = _pSource->getAs<ArrayOf>();

    int iDims           = (int)_pArgs->size();
    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];
    bool bComplex       = pSource->getImg() != NULL;
    bool bUndefine      = false;
    bool bIsImpli       = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);

    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmptyDouble();
    }

    if (iSeqCount < 0)
    {
        iSeqCount = -iSeqCount;
        bUndefine = true;
    }

    if (bUndefine)
    {
        //manage : and $ in creation by insertion
        int iSource = (pSource->getDims() - 1);
        int *piSourceDims   = pSource->getDimsArray();
        int iCompteurNull = 0;
        bool bPassed = false;

        for (int i = 0; i < iDims; i++)
        {
            if (pArg[i] == NULL)
            {
                iCompteurNull++;
            }
            else
            {
                if ((*_pArgs)[i]->isImplicitList())
                {
                    bIsImpli = true;
                }
            }
        }

        for (int i = (iDims - 1); i >= 0; i--)
        {
            if (pArg[i] == NULL)
            {
                //undefine value
                if (pSource->isScalar())
                {
                    piMaxDim[i]     = 1;
                    //piCountDim[i]   = 1;
                }
                else if (pSource->isVector() && (iCompteurNull != pSource->getSize()) && (bPassed == false))
                {
                    piMaxDim[i] = std::max(piSourceDims[0], piSourceDims[1]);
                    bPassed = true;
                }
                else if ((iCompteurNull > pSource->getRows()) && (bPassed == false))
                {
                    piMaxDim[i] = pSource->getSize();
                    bPassed = true;
                }
                else if ((iSource < 0) || (bPassed))
                {
                    piMaxDim[i] = 1;
                    iSource = -1;
                }
                else
                {
                    piMaxDim[i]     = piSourceDims[iSource];
                    //piCountDim[i]   = piSourceDims[iSource];
                }

                iSource--;
                //replace pArg value by the new one
                pArg[i] = createDoubleVector(piMaxDim[i]);
            }
        }
    }

    //remove last dimension at size 1
    //remove last dimension if are == 1
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piMaxDim[i] == 1)
        {
            iDims--;
            pArg.back()->killMe();
            pArg.pop_back();
        }
        else
        {
            break;
        }
    }

    if (checkArgValidity(pArg) == false)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //contain bad index, like <= 0, ...
        return NULL;
    }

    if (iDims == 1)
    {
        if (pSource->getCols() == 1)
        {
            int piRealDim[2] = {piMaxDim[0], 1};
            pOut = pSource->createEmpty(2, piRealDim, bComplex);
        }
        else
        {
            //rows == 1
            int piRealDim[2] = {1, piMaxDim[0]};
            pOut = pSource->createEmpty(2, piRealDim, bComplex);
        }
    }
    else
    {
        pOut = pSource->createEmpty(iDims, piMaxDim, bComplex);
    }

    //fill with null item
    ArrayOf* pArrayOut = pOut->getAs<ArrayOf>();
    T* pRealData = pArrayOut->get();
    if (bComplex)
    {
        T* pImgData = pArrayOut->getImg();
        for (int i = 0 ; i < pArrayOut->getSize() ; i++)
        {
            pRealData[i] = pSource->getNullValue();
            pImgData[i]  = pSource->getNullValue();
        }
    }
    else
    {
        for (int i = 0 ; i < pArrayOut->getSize() ; i++)
        {
            pRealData[i] = pSource->getNullValue();
        }
    }

    if (bIsImpli && (pArrayOut->getSize() != _pSource->getAs<types::GenericType>()->getSize()))
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }
    //insert values in new matrix
    InternalType* pOut2 = pArrayOut->insert(&pArg, _pSource);
    if (pOut != pOut2)
    {
        delete pOut;
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut2;
}

template <typename T>
bool ArrayOf<T>::append(int _iRows, int _iCols, InternalType* _poSource)
{
    ArrayOf * pGT = _poSource->getAs<ArrayOf>();
    int iRows = pGT->getRows();
    int iCols = pGT->getCols();

    //insert without resize
    if (iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
    {
        return false;
    }

    //Update complexity if necessary
    if (pGT->isComplex())
    {
        setComplex(true);
    }
    else if (isComplex())
    {
        pGT->setComplex(true);
    }

    if (pGT->isComplex())
    {
        for (int i = 0 ; i < iRows ; i++)
        {
            for (int j = 0 ; j < iCols ; j++)
            {
                set(_iRows + i, _iCols + j, pGT->get(i, j));
                setImg(_iRows + i, _iCols + j, pGT->getImg(i, j));
            }
        }
    }
    else
    {
        for (int i = 0 ; i < iRows ; i++)
        {
            for (int j = 0 ; j < iCols ; j++)
            {
                set(_iRows + i, _iCols + j, pGT->get(i, j));
            }
        }
    }

    return true;
}

template <typename T>
InternalType* ArrayOf<T>::remove(typed_list* _pArgs)
{
    ArrayOf<T>* pOut    = NULL;
    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //no Seq, no change but no error.
        return this;
    }

    bool* pbFull = new bool[iDims];
    //coord must represent all values on a dimension
    for (int i = 0 ; i < iDims ; i++)
    {
        pbFull[i]       = false;
        int iDimToCheck = getVarMaxDim(i, iDims);
        int iIndexSize  = pArg[i]->getAs<GenericType>()->getSize();

        //we can have index more than once
        if (iIndexSize >= iDimToCheck)
        {
            //size is good, now check datas
            double* pIndexes = getDoubleArrayFromDouble(pArg[i]);
            for (int j = 0 ; j < iDimToCheck ; j++)
            {
                bool bFind = false;
                for (int k = 0 ; k < iIndexSize ; k++)
                {
                    if ((int)pIndexes[k] == j + 1)
                    {
                        bFind = true;
                        break;
                    }
                }
                pbFull[i]  = bFind;
            }
        }
    }

    //only one dims can be not full/entire
    bool bNotEntire = false;
    int iNotEntire  = 0;
    bool bTooMuchNotEntire = false;
    for (int i = 0 ; i < iDims ; i++)
    {
        if (pbFull[i] == false)
        {
            if (bNotEntire == false)
            {
                bNotEntire = true;
                iNotEntire = i;
            }
            else
            {
                bTooMuchNotEntire = true;
                break;
            }
        }
    }

    if (bTooMuchNotEntire == true)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    delete[] pbFull;

    //find index to keep
    int iNotEntireSize          = pArg[iNotEntire]->getAs<GenericType>()->getSize();
    double* piNotEntireIndex    = getDoubleArrayFromDouble(pArg[iNotEntire]);
    int iKeepSize               = getVarMaxDim(iNotEntire, iDims);
    bool* pbKeep                = new bool[iKeepSize];

    //fill pbKeep with true value
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        pbKeep[i] = true;
    }

    for (int i = 0 ; i < iNotEntireSize ; i++)
    {
        int idx = (int)piNotEntireIndex[i] - 1;

        //don't care of value out of bounds
        if (idx < iKeepSize)
        {
            pbKeep[idx] = false;
        }
    }

    int iNewDimSize = 0;
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        if (pbKeep[i] == true)
        {
            iNewDimSize++;
        }
    }
    delete[] pbKeep;

    int* piNewDims = new int[iDims];
    for (int i = 0 ; i < iDims ; i++)
    {
        if (i == iNotEntire)
        {
            piNewDims[i] = iNewDimSize;
        }
        else
        {
            piNewDims[i] = getVarMaxDim(i, iDims);
        }
    }

    //remove last dimension if are == 1
    int iOrigDims = iDims;
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piNewDims[i] == 1)
        {
            iDims--;
        }
        else
        {
            break;
        }
    }

    if (iNewDimSize == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        delete[] piNewDims;
        return createEmptyDouble();
    }

    if (iDims == 1)
    {
        //two cases, depends of original matrix/vector
        if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] != 1)
        {
            //special case for row vector
            int piRealDim[2] = {1, iNewDimSize};
            pOut = createEmpty(2, piRealDim, m_pImgData != NULL);
            //in this case we have to care of 2nd dimension
            //iNotEntire = 1;
        }
        else
        {
            int piRealDim[2] = {iNewDimSize, 1};
            pOut = createEmpty(2, piRealDim, m_pImgData != NULL);
        }
    }
    else
    {
        pOut = createEmpty(iDims, piNewDims, m_pImgData != NULL);
    }

    delete[] piNewDims;
    //find a way to copy existing data to new variable ...
    int iNewPos = 0;
    int* piIndexes = new int[iOrigDims];
    int* piViewDims = new int[iOrigDims];
    for (int i = 0 ; i < iOrigDims ; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iOrigDims);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        bool bByPass = false;
        getIndexesWithDims(i, piIndexes, piViewDims, iOrigDims);

        //check if piIndexes use removed indexes
        for (int j = 0 ; j < iNotEntireSize ; j++)
        {
            if ((piNotEntireIndex[j] - 1) == piIndexes[iNotEntire])
            {
                //by pass this value
                bByPass = true;
                break;
            }
        }

        if (bByPass == false)
        {
            //compute new index
            pOut->set(iNewPos, get(i));
            if (m_pImgData != NULL)
            {
                pOut->setImg(iNewPos, getImg(i));
            }
            iNewPos++;
        }
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    delete[] piIndexes;
    delete[] piViewDims;
    return pOut;
}

template <typename T>
InternalType* ArrayOf<T>::extract(typed_list* _pArgs)
{
    ArrayOf<T>* pOut    = NULL;
    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmptyDouble();
    }

    if (iSeqCount < 0)
    {
        return NULL;
    }

    if (iDims < m_iDims)
    {
        for (int i = 0 ; i < iDims ; i++)
        {
            int iDimToCheck = 0;
            if (i == (iDims - 1))
            {
                iDimToCheck = getVarMaxDim(i, iDims);
            }
            else
            {
                iDimToCheck = m_piDims[i];
            }

            if (piMaxDim[i] > iDimToCheck)
            {
                //free pArg content
                cleanIndexesArguments(_pArgs, &pArg);
                return NULL;
            }
        }
    }
    else
    {
        if (iDims > m_iDims)
        {
            for (int i = m_iDims ; i < iDims ; i++)
            {
                if (piMaxDim[i] > 1)
                {
                    //free pArg content
                    cleanIndexesArguments(_pArgs, &pArg);
                    return NULL;
                }
            }
        }

        //check MaxDim
        for (int i = 0 ; i < m_iDims ; i++)
        {
            if (piMaxDim[i] > m_piDims[i])
            {
                //free pArg content
                cleanIndexesArguments(_pArgs, &pArg);
                //exrtact must be in dimension limits
                return NULL;
            }
        }
    }

    //remove last dimension if are == 1
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piCountDim[i] == 1)
        {
            (iDims)--;
        }
        else
        {
            break;
        }
    }

    //vector
    if (iDims == 1)
    {
        if (piCountDim[0] == 0)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return createEmptyDouble();
        }
        else
        {
            //two cases, depends of original matrix/vector
            if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[1] != 1 && m_piDims[0] == 1)
            {
                //special case for row vector
                int piRealDim[2] = {1, piCountDim[0]};
                pOut = createEmpty(2, piRealDim, m_pImgData != NULL);
            }
            else
            {
                if (getSize() == 1)
                {
                    //for extraction on scalar
                    pOut = createEmpty(pArg[0]->getAs<GenericType>()->getDims(), pArg[0]->getAs<GenericType>()->getDimsArray(), m_pImgData != NULL);
                }
                else
                {
                    int piRealDim[2] = {piCountDim[0], 1};
                    pOut = createEmpty(2, piRealDim, m_pImgData != NULL);
                }
            }
        }
    }
    else
    {
        //matrix
        pOut = createEmpty(iDims, piCountDim, m_pImgData != NULL);
    }

    int* piIndex    = new int[_pArgs->size()];
    int* piCoord    = new int[_pArgs->size()];
    int* piViewDims = new int[iDims];
    memset(piIndex, 0x00, sizeof(int) * _pArgs->size());

    for (int i = 0 ; i < iDims ; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iDims);
    }

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        //increment last dimension
        computeTuples(piCountDim, (int)_pArgs->size(), (int)_pArgs->size() - 1, piIndex);

        //std::cout << "[";
        for (int j = 0 ; j < (int)_pArgs->size() ; j++)
        {
            piCoord[j] = getIntValueFromDouble(pArg[j], piIndex[j]) - 1;
            //InternalType* pVar = pArg[i];
            //piCoord[j] = static_cast<int>(pVar->getAs<Double>()->get(piIndex[j]) - 1);
            //std::cout << piCoord[j] << " ";

            // try to access somewhere wrong.
            if (piCoord[j] < 0)
            {
                delete[] piIndex;
                delete[] piCoord;
                delete[] piViewDims;
                delete[] piMaxDim;
                delete[] piCountDim;

                //free pArg content
                cleanIndexesArguments(_pArgs, &pArg);
                return NULL;
            }
        }

        //std::cout << "]" << std::endl;

        int iPos = 0;
        //put vlaue in the new matrix
        if ((int)_pArgs->size() < m_iDims)
        {
            //compute index based on viewed matrix
            iPos = getIndexWithDims(piCoord, piViewDims, iDims);
        }
        else
        {
            //compute vector index
            iPos = getIndex(piCoord);
        }

        //convert flat dimension to 0
        for (int j = 0 ; j < iDims ; j++)
        {
            if (piCountDim[j] == 1)
            {
                piCoord[j] = 0;
            }
        }

        pOut->set(i, get(iPos));
        if (m_pImgData != NULL)
        {
            pOut->setImg(i, getImg(iPos));
        }


        piIndex[0]++;
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    delete[] piIndex;
    delete[] piCoord;
    delete[] piViewDims;
    delete[] piMaxDim;
    delete[] piCountDim;

    return pOut;
}

template <typename T>
bool ArrayOf<T>::resize(int* _piDims, int _iDims)
{
    if (_iDims == m_iDims)
    {
        bool bChange = false;
        for (int i = 0 ; i < _iDims ; i++)
        {
            if (m_piDims[i] != _piDims[i])
            {
                bChange = true;
                break;
            }
        }

        if (bChange == false)
        {
            //nothing to do
            return true;
        }
    }

    //alloc new data array
    T* pRealData = NULL;
    T* pImgData = NULL;

    int iNewSize = 0;
    if (m_pImgData != NULL)
    {
        iNewSize = get_max_size(_piDims, _iDims);
        if (m_iSizeMax < iNewSize)
        {
            //alloc 10% bigger than asked to prevent future resize
            int iOldSizeMax = m_iSizeMax;
            m_iSizeMax = static_cast<int>(iNewSize * 1.1);
            pRealData = allocData(m_iSizeMax);
            pImgData  = allocData(m_iSizeMax);

            //copy values into new one
            int* piIndexes = new int[std::max(m_iDims, _iDims)];
            memset(piIndexes, 0x00, sizeof(int) * std::max(m_iDims, _iDims));
            for (int i = 0; i < _iDims; i++)
            {
                piIndexes[i] = 0;
            }

            int iPreviousNewIdx = 0;
            for (int i = 0; i < m_iSize; i++)
            {
                getIndexes(i, piIndexes);
                int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                pRealData[iNewIdx] = m_pRealData[i];
                pImgData[iNewIdx]  = m_pImgData[i];
                for (int j = iPreviousNewIdx; j < iNewIdx; ++j)
                {
                    T pTemp = getNullValue();
                    pRealData[j] = copyValue(pTemp);
                    pImgData[j]  = copyValue(pTemp);
                    if (pTemp != pRealData[j])
                    {
                        deleteData(pTemp);
                    }
                }

                iPreviousNewIdx = iNewIdx + 1;
            }

            // if it's not the first resize,
            // fill new data with element of last allocation
            if (iPreviousNewIdx < iOldSizeMax)
            {
                for (int i = iPreviousNewIdx; i < iOldSizeMax; ++i)
                {
                    pRealData[i] = m_pRealData[i];
                    pImgData[i] = m_pImgData[i];
                }
            }
            else
            {
                // first resize, iOldSizeMax don't contain the 10%
                iOldSizeMax = iPreviousNewIdx;
            }

            for (int i = iOldSizeMax; i < m_iSizeMax; ++i)
            {
                T pTemp = getNullValue();
                pRealData[i] = copyValue(pTemp);
                pImgData[i]  = copyValue(pTemp);
                if (pTemp != pRealData[i])
                {
                    deleteData(pTemp);
                }
            }

            delete[] piIndexes;
            //delete all array
            delete[] m_pRealData;
            delete[] m_pImgData;
            //replace old array by new one
            m_pRealData = pRealData;
            m_pImgData = pImgData;
        }
        else
        {
            //check if only the last dims change
            bool bNonLastDimChange = false;
            for (int i = 0; i < (m_iDims - 1); i++)
            {
                if (m_piDims[i] != _piDims[i])
                {
                    bNonLastDimChange = true;
                    break;
                }
            }

            //if vector or if row dimension not change, we don't need to shift data
            if (m_iDims != _iDims || (!isVector() && bNonLastDimChange))
            {
                //copy values into new one
                int* piIndexes = new int[std::max(m_iDims, _iDims)];
                memset(piIndexes, 0x00, sizeof(int) * std::max(m_iDims, _iDims));
                for (int i = m_iSize - 1; i >= 0; i--)
                {
                    getIndexes(i, piIndexes);
                    int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                    if (iNewIdx != i)
                    {
                        T pTemp = m_pRealData[iNewIdx];
                        m_pRealData[iNewIdx] = m_pRealData[i];
                        m_pRealData[i] = pTemp;

                        pTemp = m_pImgData[iNewIdx];
                        m_pImgData[iNewIdx] = m_pImgData[i];
                        m_pImgData[i] = pTemp;
                    }
                }
                delete[] piIndexes;
            }
        }
    }
    else
    {
        iNewSize = get_max_size(_piDims, _iDims);
        if (m_iSizeMax < iNewSize)
        {
            //alloc 10% bigger than asked to prevent future resize
            int iOldSizeMax = m_iSizeMax;
            m_iSizeMax = static_cast<int>(iNewSize * 1.1);
            pRealData = allocData(m_iSizeMax);

            //copy values into new one
            int* piIndexes = new int[std::max(m_iDims, _iDims)];
            memset(piIndexes, 0x00, sizeof(int) * std::max(m_iDims, _iDims));
            for (int i = 0 ; i < _iDims ; i++)
            {
                piIndexes[i] = 0;
            }

            int iPreviousNewIdx = 0;
            for (int i = 0 ; i < m_iSize ; i++)
            {
                getIndexes(i, piIndexes);
                int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                pRealData[iNewIdx] = m_pRealData[i];
                for (int j = iPreviousNewIdx; j < iNewIdx; ++j)
                {
                    T pTemp = getNullValue();
                    T pTemp2 = copyValue(pTemp);
                    pRealData[j] = pTemp2;
                    if (pTemp != pTemp2)
                    {
                        deleteData(pTemp);
                    }
                }

                iPreviousNewIdx = iNewIdx + 1;
            }

            // if it's not the first resize,
            // fill new data with element of last allocation
            if (iPreviousNewIdx < iOldSizeMax)
            {
                for (int i = iPreviousNewIdx; i < iOldSizeMax; ++i)
                {
                    pRealData[i] = m_pRealData[i];
                }
            }
            else
            {
                // first resize, iOldSizeMax don't contain the 10%
                iOldSizeMax = iPreviousNewIdx;
            }

            for (int i = iOldSizeMax; i < m_iSizeMax; ++i)
            {
                T pTemp = getNullValue();
                T pTemp2 = copyValue(pTemp);
                pRealData[i] = pTemp2;
                if (pTemp != pTemp2)
                {
                    deleteData(pTemp);
                }
            }

            delete[] piIndexes;
            //delete all array
            delete[] m_pRealData;
            //replace old array by new one
            m_pRealData = pRealData;
        }
        else
        {
            //check if only the last dims change
            bool bNonLastDimChange = false;
            for (int i = 0 ; i < (m_iDims - 1) ; i++)
            {
                if (m_piDims[i] != _piDims[i])
                {
                    bNonLastDimChange = true;
                    break;
                }
            }

            //if vector or if row dimension not change, we don't need to shift data
            if (m_iDims != _iDims || (!isVector() && bNonLastDimChange))
            {
                //copy values into new one
                int* piIndexes = new int[std::max(m_iDims, _iDims)];
                memset(piIndexes, 0x00, sizeof(int) * std::max(m_iDims, _iDims));
                for (int i = m_iSize - 1 ; i >= 0  ; i--)
                {
                    getIndexes(i, piIndexes);
                    int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                    if (iNewIdx != i)
                    {
                        T pTemp = m_pRealData[iNewIdx];
                        m_pRealData[iNewIdx] = m_pRealData[i];
                        m_pRealData[i] = pTemp;
                    }
                }
                delete[] piIndexes;
            }
        }
    }

    if (_iDims != m_iDims)
    {
        //int* piDims = new int[_iDims];
        for (int i = 0 ; i < _iDims ; i++)
        {
            m_piDims[i] = _piDims[i];
        }
        //delete[] m_piDims;
        //m_piDims = piDims;
        m_iDims = _iDims;
    }
    else
    {
        for (int i = 0 ; i < m_iDims ; i++)
        {
            m_piDims[i] = _piDims[i];
        }
    }
    m_iRows = m_piDims[0];
    m_iCols = m_piDims[1];
    m_iSize = iNewSize;
    return true;
}


// used to allow definition of ArrayOf methode in this cpp file.
template class EXTERN_AST ArrayOf<char>;
template class EXTERN_AST ArrayOf<unsigned char>;
template class EXTERN_AST ArrayOf<short>;
template class EXTERN_AST ArrayOf<unsigned short>;
template class EXTERN_AST ArrayOf<int>;
template class EXTERN_AST ArrayOf<unsigned int>;
template class EXTERN_AST ArrayOf<long long>;
template class EXTERN_AST ArrayOf<unsigned long long>;
template class EXTERN_AST ArrayOf<double>;
template class EXTERN_AST ArrayOf<wchar_t*>;
template class EXTERN_AST ArrayOf<SinglePoly*>;
template class EXTERN_AST ArrayOf<SingleStruct*>;
template class EXTERN_AST ArrayOf<InternalType*>; // Cell
}
