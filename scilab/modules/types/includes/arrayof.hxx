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

#ifndef __ARRAYOF_HXX__
#define __ARRAYOF_HXX__

#include <sstream>
#include <cstdio>
#include "types.hxx"
#include "types_tools.hxx"
#include "scilabexception.hxx"
#include "inspector.hxx"

extern "C"
{
#include "core_math.h"
#include "dynlib_types.h"
#include "localization.h"
}

namespace types
{

//commom function
TYPES_IMPEXP int computeTuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex);
TYPES_IMPEXP InternalType* createEmptyDouble();
TYPES_IMPEXP InternalType* createDoubleVector(int _iSize);
TYPES_IMPEXP int getIntValueFromDouble(InternalType* _pIT, int _iPos);
TYPES_IMPEXP double* getDoubleArrayFromDouble(InternalType* _pIT);
TYPES_IMPEXP bool checkArgValidity(typed_list& _pArg);

template <typename T>
class TYPES_IMPEXP ArrayOf : public GenericType
{
protected :
    bool                    m_bComplex;
    T*                      m_pRealData;
    T*                      m_pImgData;


    ArrayOf() : GenericType(), m_bComplex(false), m_pRealData(NULL), m_pImgData(NULL) {}

    virtual                 ~ArrayOf()
    {
        //delete[] m_piDims;
    }


    /*internal constructor*/
    void create(int* _piDims, int _iDims, T** _pRealData, T** _pImgData)
    {
        m_iSize     = 1;
        m_iDims     = _iDims;

        //reduce dims if it's possible
        for (int i = _iDims - 1 ; i > 1 ; i--)
        {
            if (_piDims[i] == 1)
            {
                //remove dimension equal to 1
                m_iDims--;
            }
            else
            {
                break;
            }
        }

        //m_piDims    = new int[m_iDims];

        for (int i = 0 ; i < m_iDims ; i++)
        {
            m_piDims[i] = _piDims[i];

            /*
            ** Manage overflow on size
            ** a = b * c is in overflow if a / b != c
            ** check b is not 0 (empty matrix case)
            */
            int iTmpSize = m_iSize * m_piDims[i];
            if (m_iSize != 0 && iTmpSize / m_iSize != m_piDims[i])
            {
                char message[bsiz];
                sprintf(message, _("Can not allocate %.2f MB memory.\n"),  (double) ((double) m_iSize * (double) m_piDims[i] * sizeof(T)) / 1.e6);
                throw(ast::ScilabError(message));
            }

            m_iSize = iTmpSize;
        }

        if (_pRealData)
        {
            m_pRealData = allocData(m_iSize);
            *_pRealData = m_pRealData;
        }
        else
        {
            m_pRealData = NULL;
        }

        if (_pImgData)
        {
            m_pImgData = allocData(m_iSize);
            *_pImgData = m_pImgData;
            m_bComplex = true;
        }
        else
        {
            m_pImgData = NULL;
        }

        m_iSizeMax = m_iSize;
        m_iRows = m_piDims[0];
        m_iCols = m_piDims[1];
    }

    virtual T               getNullValue() = 0;
    virtual ArrayOf<T>*     createEmpty(int _iDims, int* _piDims, bool _bComplex = false) = 0;
    virtual T               copyValue(T _data) = 0;
    virtual T*              allocData(int _iSize) = 0;
    virtual void            deleteAll() = 0;
    virtual void            deleteImg() = 0;
public :

    bool isArrayOf()
    {
        return true;
    }

    virtual bool isVector() //only one dim must be != 1
    {
        bool bFirstChance = false;

        for (int i = 0 ; i < m_iDims ; i++)
        {
            if (m_piDims[i] != 1)
            {
                if (bFirstChance == true)
                {
                    return false;
                }
                else
                {
                    bFirstChance = true;
                }
            }
        }
        return true;
    }

    virtual bool isComplex()
    {
        return m_bComplex;
    }

    virtual bool isScalar() //2 dims and each dim == 1
    {
        if (m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] == 1)
        {
            return true;
        }
        return false;
    }

    virtual void setComplex(bool _bComplex)
    {
        if (_bComplex == false)
        {
            if (isComplex() == true)
            {
                deleteImg();
                m_bComplex = false;
            }
        }
        else // _bComplex == true
        {
            if (isComplex() == false)
            {
                m_pImgData = allocData(m_iSize);
                memset(m_pImgData, 0x00, sizeof(T) * m_iSize);
                m_bComplex = true;
            }
        }
    }

    virtual bool set(int _iPos, T _data)
    {
        if (m_pRealData == NULL || _iPos >= m_iSize)
        {
            return false;
        }
        m_pRealData[_iPos] = copyValue(_data);
        return true;
    }

    virtual bool set(int _iRows, int _iCols, T _data)
    {
        //            int piIndexes[2];
        //            piIndexes[0] = _iRows;
        //            piIndexes[1] = _iCols;
        //            return set(getIndex(piIndexes), _data);
        return set(_iCols * getRows() + _iRows, _data);
    }

    virtual bool set(T* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return false;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pRealData[i] = copyValue(_pdata[i]);
        }
        return true;
    }

    virtual bool set(const T* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return false;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pRealData[i] = copyValue(_pdata[i]);
        }
        return true;
    }

    T* get() const
    {
        return m_pRealData;
    }

    T get(int _iPos)
    {
        if (m_pRealData)
        {
            return m_pRealData[_iPos];
        }
        return T();
    }

    T get(int _iRows, int _iCols)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return get(getIndex(piIndexes));
    }

    /*internal function to manage img part*/
    bool setImg(int _iPos, T _data)
    {
        if (isComplex() == false || m_pImgData == NULL || _iPos >= m_iSize)
        {
            return false;
        }
        m_pImgData[_iPos] = copyValue(_data);
        return true;
    }


    bool setImg(int _iRows, int _iCols, T _data)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return setImg(getIndex(piIndexes), copyValue(_data));
    }

    bool setImg(T* _pdata)
    {
        if (m_pImgData == NULL)
        {
            return false;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pImgData[i] = copyValue(_pdata[i]);
        }
        return true;
    }


    bool setImg(const T* _pdata)
    {
        if (m_pImgData == NULL)
        {
            return false;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pImgData[i] = copyValue(_pdata[i]);
        }
        return true;
    }

    T* getImg() const
    {
        return m_pImgData;
    }

    T getImg(int _iPos)
    {
        if (m_pImgData)
        {
            return m_pImgData[_iPos];
        }
        return T();
    }

    T getImg(int _iRows, int _iCols)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return getImg(getIndex(piIndexes));
    }

    InternalType* insert(typed_list* _pArgs, InternalType* _pSource)
    {
        bool bNeedToResize  = false;
        int iDims           = (int)_pArgs->size();
        typed_list pArg;

        int* piMaxDim       = new int[iDims];
        int* piCountDim     = new int[iDims];

        //on case of resize
        int* piNewDims      = NULL;
        int iNewDims        = 0;
        ArrayOf* pSource = _pSource->getAs<ArrayOf>();

        //evaluate each argument and replace by appropriate value and compute the count of combinations
        int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
        if (iSeqCount == 0)
        {
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
                    piNewDims[i] = Max(piMaxDim[i], m_piDims[i]);
                }

                for (int i = m_iDims ; i < iNewDims ; i++)
                {
                    piNewDims[i] = piMaxDim[i];
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
                        return NULL;
                    }
                }
            }
        }

        //before resize, check input dimension
        if (pSource->isScalar() == false)
        {
            if (pSource->getSize() != iSeqCount)
            {
                return NULL;
            }
        }

        if (bNeedToResize)
        {
            bool bPass = resize(piNewDims, iNewDims);
            if (bPass == false)
            {
                return NULL;
            }
        }
        else
        {
            piNewDims   = m_piDims;
            iNewDims    = m_iDims;
        }

        //update complexity
        if (pSource->isComplex() && isComplex() == false)
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

            if (pSource->isScalar())
            {
                //element-wise insertion
                set(iPos, pRealData[0]);
                if (bComplex)
                {
                    setImg(iPos, pImgData[0]);
                }
            }
            else
            {
                set(iPos, pRealData[i]);
                if (bComplex)
                {
                    setImg(iPos, pImgData[i]);
                }
            }

            // reset imaginary part
            if (isComplex() && bComplex == false)
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
        for (int iArg = 0 ; iArg < pArg.size() ; iArg++)
        {
            if (pArg[iArg] != (*_pArgs)[iArg] && pArg[iArg]->isDeletable())
            {
                delete pArg[iArg];
            }
        }

        return this;
    }

    static InternalType* insertNew(typed_list* _pArgs, InternalType* _pSource)
    {
        typed_list pArg;
        InternalType *pOut  = NULL;
        ArrayOf* pSource = _pSource->getAs<ArrayOf>();

        int iDims           = (int)_pArgs->size();
        int* piMaxDim       = new int[iDims];
        int* piCountDim     = new int[iDims];
        bool bComplex       = pSource->isComplex();
        bool bUndefine      = false;

        //evaluate each argument and replace by appropriate value and compute the count of combinations
        int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);
        if (iSeqCount == 0)
        {
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
            int iSource         = 0;
            int *piSourceDims   = pSource->getDimsArray();

            for (int i = 0 ; i < iDims ; i++)
            {
                if (pArg[i] == NULL)
                {
                    //undefine value
                    if (pSource->isScalar())
                    {
                        piMaxDim[i]     = 1;
                        piCountDim[i]   = 1;
                    }
                    else
                    {
                        piMaxDim[i]     = piSourceDims[iSource];
                        piCountDim[i]   = piSourceDims[iSource];
                    }
                    iSource++;
                    //replace pArg value by the new one
                    pArg[i] = createDoubleVector(piMaxDim[i]);
                }
                //else
                //{
                //    piMaxDim[i] = piCountDim[i];
                //}
            }
        }

        //remove last dimension at size 1
        //remove last dimension if are == 1
        for (int i = (iDims - 1) ; i >= 2 ; i--)
        {
            if (piMaxDim[i] == 1)
            {
                iDims--;
                if (pArg.back()->isDeletable())
                {
                    delete pArg.back();
                }
                pArg.pop_back();
            }
            else
            {
                break;
            }
        }

        if (checkArgValidity(pArg) == false)
        {
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

        for (int i = 0 ; i < pArrayOut->getSize() ; i++)
        {
            //TODO: check if copyValue is mandatory
            pArrayOut->set(i, pSource->getNullValue());
            if (bComplex)
            {
                pArrayOut->setImg(i, pSource->getNullValue());
            }
        }

        //insert values in new matrix
        InternalType* pOut2 = pArrayOut->insert(&pArg, _pSource);
        if (pOut != pOut2)
        {
            delete pOut;
        }

        //free pArg content
        for (int iArg = 0 ; iArg < pArg.size() ; iArg++)
        {
            if (pArg[iArg] != (*_pArgs)[iArg] && pArg[iArg]->isDeletable())
            {
                delete pArg[iArg];
            }
        }

        return pOut2;
    }

    virtual bool append(int _iRows, int _iCols, InternalType* _poSource)
    {
        ArrayOf* pGT = _poSource->getAs<ArrayOf>();
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

        if (isComplex())
        {
            pGT->setComplex(true);
        }


        for (int i = 0 ; i < iRows ; i++)
        {
            for (int j = 0 ; j < iCols ; j++)
            {
                set(_iRows + i, _iCols + j, pGT->get(i, j));
                if (pGT->isComplex())
                {
                    setImg(_iRows + i, _iCols + j, pGT->getImg(i, j));
                }
            }
        }

        return true;
    }

    InternalType* remove(typed_list* _pArgs)
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

        if (iDims == 1)
        {
            if (iNewDimSize == 0)
            {
                return createEmptyDouble();
            }
            else
            {
                //two cases, depends of original matrix/vector
                if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] != 1)
                {
                    //special case for row vector
                    int piRealDim[2] = {1, iNewDimSize};
                    pOut = createEmpty(2, piRealDim, isComplex());
                    //in this case we have to care of 2nd dimension
                    //iNotEntire = 1;
                }
                else
                {
                    int piRealDim[2] = {iNewDimSize, 1};
                    pOut = createEmpty(2, piRealDim, isComplex());
                }
            }
        }
        else
        {
            pOut = createEmpty(iDims, piNewDims, isComplex());
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
                if (isComplex())
                {
                    pOut->setImg(iNewPos, getImg(i));
                }
                iNewPos++;
            }
        }

        //free pArg content
        for (int iArg = 0 ; iArg < pArg.size() ; iArg++)
        {
            if (pArg[iArg] != (*_pArgs)[iArg] && pArg[iArg]->isDeletable())
            {
                delete pArg[iArg];
            }
        }

        delete[] piIndexes;
        delete[] piViewDims;
        return pOut;
    }

    InternalType* extract(typed_list* _pArgs)
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
            return createEmptyDouble();
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
                        return NULL;
                    }
                }
            }

            //check MaxDim
            for (int i = 0 ; i < m_iDims ; i++)
            {
                if (piMaxDim[i] > m_piDims[i])
                {
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
                //pOut = createEmptyDouble()->getAs<Double>();
                return createEmptyDouble();
            }
            else
            {
                //two cases, depends of original matrix/vector
                if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[1] != 1 && m_piDims[0] == 1)
                {
                    //special case for row vector
                    int piRealDim[2] = {1, piCountDim[0]};
                    pOut = createEmpty(2, piRealDim, isComplex());
                }
                else
                {
                    if (getSize() == 1)
                    {
                        //for extraction on scalar
                        pOut = createEmpty(pArg[0]->getAs<GenericType>()->getDims(), pArg[0]->getAs<GenericType>()->getDimsArray(), isComplex());
                    }
                    else
                    {
                        int piRealDim[2] = {piCountDim[0], 1};
                        pOut = createEmpty(2, piRealDim, isComplex());
                    }
                }
            }
        }
        else
        {
            //matrix
            pOut = createEmpty(iDims, piCountDim, isComplex());
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
            if (isComplex())
            {
                pOut->setImg(i, getImg(iPos));
            }


            piIndex[0]++;
        }

        //free pArg content
        for (int iArg = 0 ; iArg < pArg.size() ; iArg++)
        {
            if (pArg[iArg] != (*_pArgs)[iArg] && pArg[iArg]->isDeletable())
            {
                delete pArg[iArg];
            }
        }

        delete[] piIndex;
        delete[] piCoord;
        delete[] piViewDims;
        delete[] piMaxDim;
        delete[] piCountDim;

        return pOut;
    }

    bool reshape(int _iNewRows, int _iNewCols)
    {
        int piDims[2] = {_iNewRows, _iNewCols};
        return reshape(piDims, 2);
    }

    bool reshape(int* _piDims, int _iDims)
    {
        int iNewSize = get_max_size(_piDims, _iDims);
        if (iNewSize != m_iSize)
        {
            return false;
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

        m_iRows = m_piDims[0];
        m_iCols = m_piDims[1];
        m_iSize = iNewSize;
        m_iDims = _iDims;

        return true;
    }

    bool resize(int _iNewRows, int _iNewCols)
    {
        int piDims[2] = {_iNewRows, _iNewCols};
        return resize(piDims, 2);
    }

    bool resize(int* _piDims, int _iDims)
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
        if (isComplex())
        {
            iNewSize = get_max_size(_piDims, _iDims);
            if (m_iSizeMax < iNewSize)
            {
                //alloc 10% bigger than asked to prevent future resize
                m_iSizeMax = static_cast<int>(iNewSize * 1.1);
                pRealData = allocData(m_iSizeMax);
                pImgData = allocData(m_iSizeMax);

                //set value to (null) value
                for (int i = 0 ; i < m_iSizeMax ; i++)
                {
                    pRealData[i]    = getNullValue();
                    pImgData[i]     = getNullValue();
                }

                //copy values into new one
                int* piIndexes = new int[Max(m_iDims, _iDims)];
                memset(piIndexes, 0x00, Max(m_iDims, _iDims) * sizeof(int));
                for (int i = 0 ; i < m_iSize ; i++)
                {
                    getIndexes(i, piIndexes);
                    int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                    pRealData[iNewIdx] = copyValue(m_pRealData[i]);
                    pImgData[iNewIdx] = copyValue(m_pImgData[i]);
                }

                delete[] piIndexes;
                //delete all array
                deleteAll();
                //replace old array by new one
                m_pRealData	= pRealData;
                m_pImgData	= pImgData;
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
                    int* piIndexes = new int[Max(m_iDims, _iDims)];
                    memset(piIndexes, 0x00, sizeof(int) * Max(m_iDims, _iDims));
                    for (int i = m_iSize - 1 ; i >= 0  ; i--)
                    {
                        getIndexes(i, piIndexes);
                        int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                        m_pRealData[iNewIdx] = m_pRealData[i];
                        m_pImgData[iNewIdx] = m_pImgData[i];
                        if (iNewIdx != i)
                        {
                            m_pRealData[i]  = getNullValue();
                            m_pImgData[i]   = getNullValue();
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
                m_iSizeMax = static_cast<int>(iNewSize * 1.1);
                pRealData = allocData(m_iSizeMax);

                //set value to (null) value
                for (int i = 0 ; i < m_iSizeMax ; i++)
                {
                    pRealData[i] = copyValue(getNullValue());
                }

                //copy values into new one
                int* piIndexes = new int[Max(m_iDims, _iDims)];
                memset(piIndexes, 0x00, sizeof(int) * Max(m_iDims, _iDims));
                for (int i = 0 ; i < _iDims ; i++)
                {
                    piIndexes[i] = 0;
                }

                for (int i = 0 ; i < m_iSize ; i++)
                {
                    getIndexes(i, piIndexes);
                    int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                    pRealData[iNewIdx] = copyValue(m_pRealData[i]);
                }

                delete[] piIndexes;
                //delete all array
                deleteAll();
                //replace old array by new one
                m_pRealData	= pRealData;
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
                    int* piIndexes = new int[Max(m_iDims, _iDims)];
                    memset(piIndexes, 0x00, sizeof(int) * Max(m_iDims, _iDims));
                    for (int i = m_iSize - 1 ; i >= 0  ; i--)
                    {
                        getIndexes(i, piIndexes);
                        int iNewIdx = getIndexWithDims(piIndexes, _piDims, _iDims);
                        m_pRealData[iNewIdx] = m_pRealData[i];
                        if (iNewIdx != i)
                        {
                            m_pRealData[i] = 0;
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
        m_iCols	= m_piDims[1];
        m_iSize = iNewSize;
        return true;
    }

    /* ArrayOf */
    template <typename V>
    V* getAs(void)
    {
        return dynamic_cast<V*>(this);
    }

    /*dimensions functions*/
    int getIndex(int* _piIndexes)
    {
        int idx = 0;
        int iMult = 1;
        for (int i = 0 ; i < m_iDims ; i++)
        {
            idx += _piIndexes[i] * iMult;
            iMult *= m_piDims[i];
        }
        return idx;
    }

    void getIndexes(int _iIndex, int* _piIndexes)
    {
        getIndexesWithDims(_iIndex, _piIndexes, m_piDims, m_iDims);
    }

    ArrayOf<T>* getColumnValues(int _iPos)
    {
        ArrayOf<T>* pOut = NULL;
        if (_iPos < m_iCols)
        {
            int piDims[2] = {m_iRows, 1};
            pOut = createEmpty(2, piDims, isComplex());
            T* pReal    = pOut->get();
            T* pImg     = pOut->getImg();
            for (int i = 0 ; i < m_iRows ; i++)
            {
                pReal[i] = copyValue(get(i, _iPos));
            }

            if (isComplex())
            {
                for (int i = 0 ; i < m_iRows ; i++)
                {
                    pImg[i] = copyValue(getImg(i, _iPos));
                }
            }
        }
        return pOut;
    }

    virtual bool toString(std::wostringstream& ostr)
    {
        int* piDims = new int[m_iDims];
        bool bFinish = parseSubMatrix(ostr, piDims, m_iDims, m_iDims - 1);
        delete[] piDims;
        return bFinish;
    }

    bool parseSubMatrix(std::wostringstream& ostr, int* _piDims, int _iDims, int _iDim)
    {
        bool bFinish = false;
        if (_iDim == 1)
        {
            //we have reach 2-dim matrix

            if (m_iDims > 2 && m_bPrintFromStart)
            {
                //only print for dims > 2
                ostr << L"(:,:";
                for (int i = 2 ; i < _iDims ; i++)
                {
                    ostr << L"," << (_piDims[i] + 1);
                }
                ostr << L")" << std::endl << std::endl;
            }

            //reset flag to print dims on next call
            m_bPrintFromStart = true;

            bFinish = subMatrixToString(ostr, _piDims, _iDims);
            if (bFinish == false)
            {
                //save print status
                m_bPrintFromStart = false;
                return false;
            }
        }
        else
        {
            //draw, continue to dig
            for (int i = m_iSavePrintState ; i < m_piDims[_iDim] ; i++)
            {
                _piDims[_iDim] = i;
                bFinish = parseSubMatrix(ostr, _piDims, _iDims, _iDim - 1);
                if (bFinish == false)
                {
                    //save print status
                    m_iSavePrintState = i;
                    return false;
                }
            }

            //reset state to print from state
            m_iSavePrintState = 0;
            m_iRows1PrintState = 0;
            m_iCols1PrintState = 0;
            m_iRows2PrintState = 0;
            m_iCols2PrintState = 0;
        }

        return true;
    }

    virtual bool subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) = 0;

    virtual std::wstring toStringInLine()
    {
        std::wostringstream ostr;
        ostr << L"[";

        for (int i = 0 ; i < m_iDims ; i++)
        {
            if (i > 0)
            {
                ostr << L"x";
            }

            ostr << m_piDims[i];
        }

        ostr << L" " << getTypeStr() << L"]";
        return ostr.str();
    }
};
}

#endif /* !__ARRAYOF_HXX__ */
