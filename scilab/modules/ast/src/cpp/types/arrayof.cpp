/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include "arrayof.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "singlepoly.hxx"
#include "singlestruct.hxx"
#include "type_traits.hxx"
#include "exp.hxx"
#include "types_tools.hxx"

extern "C"
{
#include "dynlib_ast.h"
}

namespace types
{

static int get_max_size(int* _piDims, int _iDims)
{
    if (_iDims == 0)
    {
        return 0;
    }

    int iMax = 1;
    for (int i = 0 ; i < _iDims ; i++)
    {
        iMax *= _piDims[i];
    }
    return iMax;
}

template <typename T>
GenericType* ArrayOf<T>::createEmpty()
{
    return createEmptyDouble();
}

template <typename T>
void ArrayOf<T>::getIndexes(int _iIndex, int* _piIndexes)
{
    getIndexesWithDims(_iIndex, _piIndexes, m_piDims, m_iDims);
}

template <typename T>
ArrayOf<T>* ArrayOf<T>::insert(typed_list* _pArgs, InternalType* _pSource)
{
    ArrayOf<T>* pIT = checkRef(this, &ArrayOf::insert, _pArgs, _pSource);
    if (pIT != this)
    {
        return pIT;
    }

    int index;
    if (getScalarIndex(this, _pArgs, &index))
    {
        ArrayOf* pIns = _pSource->getAs<ArrayOf>();
        int sizeIn = pIns->getSize();
        //only scalar can be used to ".=" operation
        if (sizeIn != 1)
        {
            return NULL;
        }

        T* pRealData = pIns->get();
        T* pImgData = pIns->getImg();

        if (isComplex() == false && pIns->isComplex() == false)
        {
            if (set(index, *pRealData) != NULL)
            {
                return this;
            }
        }

        //if complex continue
    }

    std::vector<int> indexes;
    std::vector<int> dims;
    if (getImplicitIndex(this, _pArgs, indexes, dims))
    {
        if (indexes.size() == 0)
        {
            return this;
        }

        ArrayOf* pIns = _pSource->getAs<ArrayOf>();
        int sizeIn = pIns->getSize();
        int count = static_cast<int>(indexes.size());
        //only scalar can be used to ".=" operation
        if (sizeIn != 1 && count != sizeIn)
        {
            return NULL;
        }

        T* pRealData = pIns->get();
        T* pImgData = pIns->getImg();

        bool status = true;
        if (isComplex() == false && pIns->isComplex() == false)
        {
            if (sizeIn == 1)
            {
                for (int & i : indexes)
                {
                    if (set(i, *pRealData) == NULL)
                    {
                        status = false;
                        break;
                    }
                }
            }
            else
            {
                for (int & i : indexes)
                {
                    if (set(i, *pRealData) == NULL)
                    {
                        status = false;
                        break;
                    }
                    ++pRealData;
                }
            }

            if (status)
            {
                return this;
            }

            //if status is false, continue to entire process
        }
    }

    bool bNeedToResize = false;
    int iDims = (int)_pArgs->size();
    int iDimsOrigine = m_iDims;
    typed_list pArg;

    int* piMaxDim = new int[iDims];
    int* piCountDim = new int[iDims];

    //on case of resize
    int* piNewDims = NULL;
    int iNewDims = 0;
    ArrayOf* pSource = _pSource->getAs<ArrayOf>();

    bool bIsColon = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        delete[] piCountDim;
        delete[] piMaxDim;
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    //only scalar can be used to ".=" operation
    if (iSeqCount != pSource->getSize() && pSource->isScalar() == false)
    {
        return NULL;
    }

    //remove last dimension at size 1
    //remove last dimension if are == 1
    for (int i = (iDims - 1); i >= m_iDims; i--)
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
            for (int i = 0; i < m_iDims; i++)
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
            for (int i = 0; i < m_iDims; i++)
            {
                piNewDims[i] = std::max(piMaxDim[i], m_piDims[i]);
            }

            int iSource = (pSource->getDims() - 1);
            bool bPassed = false;
            int *piSourceDims = pSource->getDimsArray();

            for (int i = m_iDims; i < iNewDims; ++i)
            {
                piNewDims[i] = piMaxDim[i];
            }
        }
    }
    else // _iDims < m_iDims
    {
        if (isVector() || isScalar() || getSize() == 0) //getSize() == 0, only for [] and {}
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
            for (int i = 0; i < iDims - 1; i++)
            {
                //indexes are always doubles
                double* pIdx = getDoubleArrayFromDouble(pArg[i]);
                //InternalType* pVar = pArg[i];
                //double* pIdx = static_cast<double*>(pVar->getAs<Double>()->get());
                int iSize = pArg[i]->getAs<ArrayOf>()->getSize();
                for (int j = 0; j < iSize; j++)
                {
                    if (pIdx[j] > m_piDims[i])
                    {
                        delete[] piCountDim;
                        delete[] piMaxDim;
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
            for (int i = 0; i < iSize; i++)
            {
                if (pIdx[i] > iMaxLastDim)
                {
                    delete[] piCountDim;
                    delete[] piMaxDim;
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
        ArrayOf<T>* pTemp = resize(piNewDims, iNewDims);
        if (pTemp == NULL)
        {
            delete[] piCountDim;
            delete[] piMaxDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }
    else
    {
        piNewDims = m_piDims;
        iNewDims = m_iDims;
    }

    //update complexity
    if (pSource->isComplex() && m_pImgData == NULL)
    {
        setComplex(true);
    }

    int argSize = static_cast<int>(pArg.size());
    int* piIndex = new int[argSize];
    int* piCoord = new int[argSize];
    int* piViewDims = new int[iDims];
    memset(piIndex, 0x00, sizeof(int) * argSize);

    //convert  current dimension to view dimension
    for (int i = 0; i < iDims; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iDims);
    }

    T* pRealData = pSource->get();
    T* pImgData = pSource->getImg();
    bool bComplex = pSource->isComplex();

    for (int i = 0; i < iSeqCount; i++)
    {
        computeTuples(piCountDim, argSize, argSize - 1, piIndex);

        //std::cout << "[";
        for (int j = 0; j < argSize; j++)
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
            throw ast::InternalError(szError);
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
            if (bIsColon)
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
GenericType* ArrayOf<T>::insertNew(typed_list* _pArgs)
{
    typed_list pArg;
    InternalType *pOut = NULL;

    int iDims = (int)_pArgs->size();
    int* piMaxDim = new int[iDims];
    int* piCountDim = new int[iDims];
    bool bComplex = getImg() != NULL;
    bool bUndefine = false;
    bool bIsImpli = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);

    if (iSeqCount == 0)
    {
        delete[] piMaxDim;
        delete[] piCountDim;
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
        int *piSourceDims = getDimsArray();
        int iSourceDims = getDims();
        int iCompteurNull = 0;
        int iLastNull = 0;
        for (int i = 0; i < iDims; i++)
        {
            if (pArg[i] == NULL)
            {
                iCompteurNull++;
                iLastNull = i;
            }
            else
            {
                if ((*_pArgs)[i]->isImplicitList())
                {
                    bIsImpli = true;
                }
            }
        }

        //if all index are : -> a = x
        if (iCompteurNull == pArg.size())
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return this;
        }

        //vector case
        if (isVector() && iCompteurNull == 1)
        {
            piMaxDim[iLastNull] = getSize();
            pArg[iLastNull] = createDoubleVector(piMaxDim[iLastNull]);
        }
        else
        {
            //matrix and hypermatrix case
            if (iCompteurNull < getDims())
            {
                delete[] piMaxDim;
                delete[] piCountDim;
                //free pArg content
                cleanIndexesArguments(_pArgs, &pArg);
                //contain bad index, like <= 0, ...
                return NULL;
            }

            //replace ":" by know source dimensions
            int iSource = 0;
            for (int i = 0; i < iDims; ++i)
            {
                if (pArg[i] == NULL)
                {
                    if (iSource < iSourceDims)
                    {
                        piMaxDim[i] = piSourceDims[iSource];
                        pArg[i] = createDoubleVector(piMaxDim[i]);
                        ++iSource;
                    }
                    else
                    {
                        //fill dimensions after getDimes() with 1
                        piMaxDim[i] = 1;
                        pArg[i] = createDoubleVector(piMaxDim[i]);
                    }
                }
            }
        }
    }

    //remove last dimension at size 1
    //remove last dimension if are == 1
    for (int i = (iDims - 1); i >= 2; i--)
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
        delete[] piMaxDim;
        delete[] piCountDim;
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //contain bad index, like <= 0, ...
        return NULL;
    }

    if (iDims == 1)
    {
        if (getCols() == 1)
        {
            int piRealDim[2] = {piMaxDim[0], 1};
            pOut = createEmpty(2, piRealDim, bComplex);
        }
        else
        {
            //rows == 1
            int piRealDim[2] = {1, piMaxDim[0]};
            pOut = createEmpty(2, piRealDim, bComplex);
        }
    }
    else
    {
        pOut = createEmpty(iDims, piMaxDim, bComplex);
    }

    //fill with null item
    ArrayOf* pArrayOut = pOut->getAs<ArrayOf>();
    T* pRealData = pArrayOut->get();
    if (bComplex)
    {
        T* pImgData = pArrayOut->getImg();
        for (int i = 0; i < pArrayOut->getSize(); i++)
        {
            pArrayOut->deleteData(pRealData[i]);
            pRealData[i] = getNullValue();
            pArrayOut->deleteData(pImgData[i]);
            pImgData[i] = getNullValue();
        }
    }
    else
    {
        for (int i = 0; i < pArrayOut->getSize(); i++)
        {
            pArrayOut->deleteData(pRealData[i]);
            pRealData[i] = getNullValue();
        }
    }

    if (bIsImpli && (pArrayOut->getSize() != getSize()))
    {
        delete[] piMaxDim;
        delete[] piCountDim;
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }
    //insert values in new matrix
    ArrayOf* pOut2 = pArrayOut->insert(&pArg, this);
    if (pOut != pOut2)
    {
        delete pOut;
    }

    delete[] piMaxDim;
    delete[] piCountDim;
    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut2;
}

template <typename T>
ArrayOf<T>* ArrayOf<T>::append(int _iRows, int _iCols, InternalType* _poSource)
{
    ArrayOf<T>* pIT = checkRef(this, &ArrayOf::append, _iRows, _iCols, _poSource);
    if (pIT != this)
    {
        return pIT;
    }

    ArrayOf * pGT = _poSource->getAs<ArrayOf>();
    int iRows = pGT->getRows();
    int iCols = pGT->getCols();

    //insert without resize
    if (iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
    {
        return NULL;
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
        for (int i = 0; i < iRows; i++)
        {
            for (int j = 0; j < iCols; j++)
            {
                set(_iRows + i, _iCols + j, pGT->get(i, j));
                setImg(_iRows + i, _iCols + j, pGT->getImg(i, j));
            }
        }
    }
    else
    {
        for (int i = 0; i < iRows; i++)
        {
            for (int j = 0; j < iCols; j++)
            {
                set(_iRows + i, _iCols + j, pGT->get(i, j));
            }
        }
    }

    return this;
}

template <typename T>
GenericType* ArrayOf<T>::remove(typed_list* _pArgs)
{
    ArrayOf<T>* pOut = NULL;
    int iDims = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim = new int[iDims];
    int* piCountDim = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    delete[] piMaxDim;
    delete[] piCountDim;
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //no Seq, no change but no error.
        return this;
    }

    bool* pbFull = new bool[iDims];
    //coord must represent all values on a dimension
    for (int i = 0; i < iDims; i++)
    {
        pbFull[i] = false;
        int iDimToCheck = getVarMaxDim(i, iDims);
        int iIndexSize = pArg[i]->getAs<GenericType>()->getSize();

        //we can have index more than once
        if (iIndexSize >= iDimToCheck)
        {
            //size is good, now check datas
            double* pIndexes = getDoubleArrayFromDouble(pArg[i]);
            for (int j = 0; j < iDimToCheck; j++)
            {
                bool bFind = false;
                for (int k = 0; k < iIndexSize; k++)
                {
                    if ((int)pIndexes[k] == j + 1)
                    {
                        bFind = true;
                        break;
                    }
                }
                pbFull[i] = bFind;
            }
        }
    }

    //only one dims can be not full/entire
    bool bNotEntire = false;
    int iNotEntire = 0;
    bool bTooMuchNotEntire = false;
    for (int i = 0; i < iDims; i++)
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

    delete[] pbFull;

    if (bTooMuchNotEntire == true)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //find index to keep
    int iNotEntireSize = pArg[iNotEntire]->getAs<GenericType>()->getSize();
    double* piNotEntireIndex = getDoubleArrayFromDouble(pArg[iNotEntire]);
    int iKeepSize = getVarMaxDim(iNotEntire, iDims);
    bool* pbKeep = new bool[iKeepSize];

    //fill pbKeep with true value
    for (int i = 0; i < iKeepSize; i++)
    {
        pbKeep[i] = true;
    }

    for (int i = 0; i < iNotEntireSize; i++)
    {
        int idx = (int)piNotEntireIndex[i] - 1;

        //don't care of value out of bounds
        if (idx < iKeepSize)
        {
            pbKeep[idx] = false;
        }
    }

    int iNewDimSize = 0;
    for (int i = 0; i < iKeepSize; i++)
    {
        if (pbKeep[i] == true)
        {
            iNewDimSize++;
        }
    }
    delete[] pbKeep;

    int* piNewDims = new int[iDims];
    for (int i = 0; i < iDims; i++)
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
    for (int i = (iDims - 1); i >= 2; i--)
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
        return createEmpty();
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

        {
            int iNewPos = 0;
            int size = getSize();

            //try to sort piNotEntireIndex
            std::sort(piNotEntireIndex, piNotEntireIndex + iNotEntireSize);

            int last = 0;
            for (int i = 0; i < iNotEntireSize; ++i)
            {
                int ii = piNotEntireIndex[i] - 1;
                for (int j = last; j < ii; ++j)
                {
                    pOut->set(iNewPos, get(j));
                    if (m_pImgData != NULL)
                    {
                        pOut->setImg(iNewPos, getImg(j));
                    }
                    iNewPos++;
                }

                last = ii + 1;
            }

            for (int i = last; i < size; ++i)
            {
                pOut->set(iNewPos, get(i));
                if (m_pImgData != NULL)
                {
                    pOut->setImg(iNewPos, getImg(i));
                }
                iNewPos++;
            }
        }
    }
    else
    {
        pOut = createEmpty(iDims, piNewDims, m_pImgData != NULL);

        //find a way to copy existing data to new variable ...
        int iNewPos = 0;
        int* piIndexes = new int[iOrigDims];
        int* piViewDims = new int[iOrigDims];
        for (int i = 0; i < iOrigDims; i++)
        {
            piViewDims[i] = getVarMaxDim(i, iOrigDims);
        }

        for (int i = 0; i < getSize(); i++)
        {
            bool bByPass = false;
            getIndexesWithDims(i, piIndexes, piViewDims, iOrigDims);

            //check if piIndexes use removed indexes
            for (int j = 0; j < iNotEntireSize; j++)
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

        delete[] piIndexes;
        delete[] piViewDims;
    }

    delete[] piNewDims;

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut;
}

template <typename T>
GenericType* ArrayOf<T>::extract(typed_list* _pArgs)
{
    ArrayOf<T>* pOut = NULL;
    int iDims = (int)_pArgs->size();
    typed_list pArg;

    int index;
    if (getScalarIndex(this, _pArgs, &index))
    {
        if (index < 0)
        {
            return NULL;
        }

        if (getSize() == 0)
        {
            return createEmpty();
        }

        if (index >= getSize())
        {
            return NULL;
        }

        int dims[2] = {1, 1};
        pOut = createEmpty(2, dims, isComplex());;
        pOut->set(0, get(index));
        if (isComplex())
        {
            pOut->setImg(0, getImg(index));
        }

        return pOut;
    }

    std::vector<double> il;
    if (getScalarImplicitIndex(this, _pArgs, il))
    {
        double start = il[0];
        double step = il[1];
        double end = il[2];
        //index are ":"
        bool isForceColVector = il.size() == 4;

        //std::cout << start << ":" << step << ":" << end << std::endl;
        int size = static_cast<int>((end - start) / step + 1);
        if (size <= 0 || m_iSize == 0)
        {
            return createEmpty();
        }

        if (step > 0 && (size - 1) * step + start > m_iSize ||
                step < 0 && start > m_iSize)
        {
            return NULL;
        }

        bool isRowVector = m_iRows == 1;
        isRowVector = isRowVector && !isForceColVector;
        int dims[2] = {isRowVector ? 1 : size, isRowVector ? size : 1};
        pOut = createEmpty(2, dims, isComplex());
        double idx = start;

        if (isComplex())
        {
            for (int i = 0; i < size; ++i)
            {
                int index = static_cast<int>(idx) - 1;
                pOut->set(i, get(index));
                pOut->setImg(i, getImg(index));
                idx += step;
            }
        }
        else
        {
            for (int i = 0; i < size; ++i)
            {
                pOut->set(i, get(static_cast<int>(idx) - 1));
                idx += step;
            }
        }
        return pOut;
    }

    std::vector<int> indexes;
    std::vector<int> dims;
    if (getImplicitIndex(this, _pArgs, indexes, dims))
    {
        if (indexes.size() == 0)
        {
            return createEmpty();
        }

        if (dims.size() == 1)
        {
            int d[2] = {1, dims[0]};
            pOut = createEmpty(2, d, isComplex());
        }
        else
        {
            pOut = createEmpty(static_cast<int>(dims.size()), dims.data(), isComplex());
        }

        int size = getSize();
        if (isComplex())
        {
            int idx = 0;
            for (int & i : indexes)
            {
                if (i < 0 || i >= size)
                {
                    pOut->killMe();
                    return NULL;
                }

                pOut->set(idx, get(i));
                pOut->setImg(idx, getImg(i));
                ++idx;
            }
        }
        else
        {
            int idx = 0;
            for (int & i : indexes)
            {
                pOut->set(idx, get(i));
                ++idx;
            }
        }

        return pOut;
    }


    int* piMaxDim = new int[iDims];
    int* piCountDim = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        delete[] piMaxDim;
        delete[] piCountDim;
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmpty();
    }

    if (iSeqCount < 0)
    {
        delete[] piMaxDim;
        delete[] piCountDim;
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //a = {};a(1:2, 1:2) -> {}
    if (getSize() == 0)
    {
        delete[] piMaxDim;
        delete[] piCountDim;
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmpty();
    }

    if (iDims < m_iDims)
    {
        for (int i = 0; i < iDims; i++)
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
                delete[] piMaxDim;
                delete[] piCountDim;
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
            for (int i = m_iDims; i < iDims; i++)
            {
                if (piMaxDim[i] > 1)
                {
                    delete[] piMaxDim;
                    delete[] piCountDim;
                    //free pArg content
                    cleanIndexesArguments(_pArgs, &pArg);
                    return NULL;
                }
            }
        }

        //check MaxDim
        for (int i = 0; i < m_iDims; i++)
        {
            if (piMaxDim[i] > m_piDims[i])
            {
                delete[] piMaxDim;
                delete[] piCountDim;
                //free pArg content
                cleanIndexesArguments(_pArgs, &pArg);
                //exrtact must be in dimension limits
                return NULL;
            }
        }
    }

    //remove last dimension if are == 1
    for (int i = (iDims - 1); i >= 2; i--)
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
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return createEmpty();
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

    int* piIndex = new int[_pArgs->size()];
    int* piCoord = new int[_pArgs->size()];
    int* piViewDims = new int[iDims];
    memset(piIndex, 0x00, sizeof(int) * _pArgs->size());

    for (int i = 0; i < iDims; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iDims);
    }

    for (int i = 0; i < iSeqCount; i++)
    {
        //increment last dimension
        computeTuples(piCountDim, (int)_pArgs->size(), (int)_pArgs->size() - 1, piIndex);

        //std::cout << "[";
        for (int j = 0; j < (int)_pArgs->size(); j++)
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
        for (int j = 0; j < iDims; j++)
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
ArrayOf<T>* ArrayOf<T>::reshape(int* _piDims, int _iDims)
{
    typedef ArrayOf<T>* (ArrayOf<T>::*reshape_t)(int*, int);
    ArrayOf<T>* pIT = checkRef(this, (reshape_t)&ArrayOf<T>::reshape, _piDims, _iDims);
    if (pIT != this)
    {
        return pIT;
    }

    int iNewSize = get_max_size(_piDims, _iDims);
    if (iNewSize != m_iSize)
    {
        return NULL;
    }

    for (int i = 0 ; i < _iDims ; i++)
    {
        m_piDims[i] = _piDims[i];
    }

    if (_iDims == 1)
    {
        m_piDims[1] = 1;
        _iDims++;
    }

    int iDims = _iDims;
    for (int i = iDims - 1; i >= 2; --i)
    {
        if (m_piDims[i] == 1)
        {
            _iDims--;
        }
        else
        {
            break;
        }
    }

    m_iRows = m_piDims[0];
    m_iCols = m_piDims[1];
    m_iSize = iNewSize;
    m_iDims = _iDims;

    return this;
}

template <typename T>
ArrayOf<T>* ArrayOf<T>::resize(int* _piDims, int _iDims)
{
    typedef ArrayOf<T>* (ArrayOf<T>::*resize_t)(int*, int);
    ArrayOf<T>* pIT = checkRef(this, (resize_t)&ArrayOf::resize, _piDims, _iDims);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iDims == m_iDims)
    {
        bool bChange = false;
        for (int i = 0; i < _iDims; i++)
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
            return this;
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
            pImgData = allocData(m_iSizeMax);

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
                pImgData[iNewIdx] = m_pImgData[i];
                for (int j = iPreviousNewIdx; j < iNewIdx; ++j)
                {
                    T pTemp = getNullValue();
                    pRealData[j] = copyValue(pTemp);
                    pImgData[j] = copyValue(pTemp);
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
                pImgData[i] = copyValue(pTemp);
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
        if (iNewSize > m_iSizeMax)
        {
            //alloc 10% bigger than asked to prevent future resize
            int iOldSizeMax = m_iSizeMax;
            m_iSizeMax = static_cast<int>(iNewSize * 1.1);
            pRealData = allocData(m_iSizeMax);

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
                m_pRealData[i] = NULL;
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

            //clean section between m_iSize and iOldSizeMax
            for (int i = m_iSize; i < iOldSizeMax; ++i)
            {
                deleteData(m_pRealData[i]);
                m_pRealData[i] = NULL;
            }

            //if (iPreviousNewIdx < iOldSizeMax)
            //{
            //    for (int i = iPreviousNewIdx; i < iOldSizeMax; ++i)
            //    {
            //        pRealData[i] = m_pRealData[i];
            //        m_pRealData[i] = NULL;
            //    }
            //}
            //else
            //{
            //    iOldSizeMax = iPreviousNewIdx;
            //}

            //fill exceeded with NullValue
            for (int i = iPreviousNewIdx; i < m_iSizeMax; ++i)
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
                    }
                }
                delete[] piIndexes;
            }
        }
    }

    if (_iDims != m_iDims)
    {
        //int* piDims = new int[_iDims];
        for (int i = 0; i < _iDims; i++)
        {
            m_piDims[i] = _piDims[i];
        }
        //delete[] m_piDims;
        //m_piDims = piDims;
        m_iDims = _iDims;
    }
    else
    {
        for (int i = 0; i < m_iDims; i++)
        {
            m_piDims[i] = _piDims[i];
        }
    }
    m_iRows = m_piDims[0];
    m_iCols = m_piDims[1];
    m_iSize = iNewSize;
    return this;
}

template <typename T>
bool ArrayOf<T>::isTrue()
{
    return type_traits::isTrue<T>(m_iSize, m_pRealData);
}

template<typename T>
bool ArrayOf<T>::neg(InternalType *& out)
{
    out = new Bool(this->m_iDims, this->m_piDims);
    type_traits::neg<T, int>(this->m_iSize, this->m_pRealData, static_cast<Bool *>(out)->get());

    return true;
}

template<typename T>
bool ArrayOf<T>::invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
    }
    else
    {
        InternalType * _out = extract(&in);
        if (!_out)
        {
            std::wostringstream os;
            os << _W("Invalid index.") << std::endl;
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }
        out.push_back(_out);
    }

    return true;
}

template<typename T>
bool ArrayOf<T>::isInvokable() const
{
    return true;
}

template<typename T>
bool ArrayOf<T>::hasInvokeOption() const
{
    return false;
}

template<typename T>
int ArrayOf<T>::getInvokeNbIn()
{
    return -1;
}

template<typename T>
int ArrayOf<T>::getInvokeNbOut()
{
    return 1;
}


// used to allow definition of ArrayOf methode in this cpp file.
template class EXTERN_AST ArrayOf < char >;
template class EXTERN_AST ArrayOf < unsigned char >;
template class EXTERN_AST ArrayOf < short >;
template class EXTERN_AST ArrayOf < unsigned short >;
template class EXTERN_AST ArrayOf < int >;
template class EXTERN_AST ArrayOf < unsigned int >;
template class EXTERN_AST ArrayOf < long long >;
template class EXTERN_AST ArrayOf < unsigned long long >;
template class EXTERN_AST ArrayOf < double >;
template class EXTERN_AST ArrayOf < wchar_t* >;
template class EXTERN_AST ArrayOf < SinglePoly* >;
template class EXTERN_AST ArrayOf < SingleStruct* >;
template class EXTERN_AST ArrayOf < InternalType* >; // Cell
}
