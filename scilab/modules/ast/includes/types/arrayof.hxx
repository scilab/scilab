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

#ifndef __ARRAYOF_HXX__
#define __ARRAYOF_HXX__

#ifdef _MSC_VER
#define NOMINMAX
#endif

#include <sstream>
#include <cstdio>
#include "types.hxx"
#include "scilabexception.hxx"
#include "inspector.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "os_string.h"
}

namespace types
{

template <typename T>
class ArrayOf : public GenericType
{
public:
    typedef T type;

protected :
    T*                      m_pRealData;
    T*                      m_pImgData;


    ArrayOf() : GenericType(), m_pRealData(NULL), m_pImgData(NULL) {}

    virtual                 ~ArrayOf()
    {
        //delete[] m_piDims;
    }


    /*internal constructor*/
    void create(const int* _piDims, int _iDims, T** _pRealData, T** _pImgData)
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

        //eye detection ^^
        if (m_iDims == 2 && _piDims[0] == -1 && _piDims[1] == -1)
        {
            m_iDims = 2;
            m_piDims[0] = -1;
            m_piDims[1] = -1;
            m_iSize = 1;
        }
        else
        {
            for (int i = 0 ; i < m_iDims ; i++)
            {
                // if one of dim is null, create an empty matrix
                if (_piDims[i] <= 0)
                {
                    m_iDims = 2;
                    m_piDims[0] = 0;
                    m_piDims[1] = 0;
                    m_iSize = 0;
                    break;
                }

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
                    os_sprintf(message, _("Can not allocate %.2f MB memory.\n"),  (double) ((double) m_iSize * (double) m_piDims[i] * sizeof(T)) / 1.e6);
                    throw ast::InternalError(message);
                }

                m_iSize = iTmpSize;
            }

            if (m_iSize < 0)
            {
                m_pRealData = NULL;
                m_pImgData = NULL;
                char message[bsiz];
                os_sprintf(message, _("Can not allocate negative size (%d).\n"), m_iSize);
                throw ast::InternalError(message);
            }

        }

        try
        {

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
            }
            else
            {
                m_pImgData = NULL;
            }
        }
        catch (std::bad_alloc & /*e*/)
        {
            char message[bsiz];
            os_sprintf(message, _("Can not allocate %.2f MB memory.\n"), (double)(m_iSize * sizeof(T)) / 1.e6);
            throw ast::InternalError(message);
        }

        m_iSizeMax = m_iSize;
        m_iRows = m_piDims[0];
        m_iCols = m_piDims[1];
    }

    virtual T               getNullValue() = 0;
    virtual ArrayOf<T>*     createEmpty(int _iDims, int* _piDims, bool _bComplex = false) = 0;
    virtual GenericType*    createEmpty();

    virtual T               copyValue(T _data) = 0;
    virtual T*              allocData(int _iSize) = 0;
    virtual void            deleteAll() = 0;
    virtual void            deleteImg() = 0;
public :

    bool isArrayOf()
    {
        return true;
    }

    bool isTrue();

    // The function is not write here because we needs to create a Bool which inherits from ArrayOf<int>
    // so it will create a cyclic dependency... so the body of the function is in bool.hxx after the Bool definition.
    virtual bool neg(InternalType *& out);

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
        return m_pImgData != NULL;
    }

    virtual ArrayOf<T>* setComplex(bool _bComplex)
    {
        typedef ArrayOf<T>* (ArrayOf<T>::*setcplx_t)(bool);
        ArrayOf<T>* pIT = checkRef(this, (setcplx_t)&ArrayOf<T>::setComplex, _bComplex);
        if (pIT != this)
        {
            return pIT;
        }

        if (_bComplex == false)
        {
            if (m_pImgData != NULL)
            {
                deleteImg();
            }
        }
        else // _bComplex == true
        {
            if (m_pImgData == NULL)
            {
                m_pImgData = allocData(m_iSize);
                memset(m_pImgData, 0x00, sizeof(T) * m_iSize);
            }
        }

        return this;
    }

    virtual ArrayOf<T>* set(int _iPos, const T _data)
    {
        if (m_pRealData == NULL || _iPos >= m_iSize)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*set_t)(int, T);
        ArrayOf<T>* pIT = checkRef(this, (set_t)&ArrayOf<T>::set, _iPos, _data);
        if (pIT != this)
        {
            return pIT;
        }

        deleteData(m_pRealData[_iPos]);
        m_pRealData[_iPos] = copyValue(_data);
        return this;
    }

    virtual ArrayOf<T>* set(int _iRows, int _iCols, const T _data)
    {
        //            int piIndexes[2];
        //            piIndexes[0] = _iRows;
        //            piIndexes[1] = _iCols;
        //            return set(getIndex(piIndexes), _data);
        return set(_iCols * getRows() + _iRows, _data);
    }

    virtual ArrayOf<T>* set(T* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*set_t)(T*);
        ArrayOf<T>* pIT = checkRef(this, (set_t)&ArrayOf<T>::set, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            deleteData(m_pRealData[i]);
            m_pRealData[i] = copyValue(_pdata[i]);
        }
        return this;
    }

    virtual ArrayOf<T>* set(const T* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*set_t)(const T*);
        ArrayOf<T>* pIT = checkRef(this, (set_t)&ArrayOf<T>::set, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            deleteData(m_pRealData[i]);
            m_pRealData[i] = copyValue(_pdata[i]);
        }

        return this;
    }

    inline T* get() const
    {
        return m_pRealData;
    }

    inline T get(int _iPos)
    {
        if (m_pRealData)
        {
            return m_pRealData[_iPos];
        }
        return T();
    }

    inline T get(int _iRows, int _iCols)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return get(getIndex(piIndexes));
    }

    /*internal function to manage img part*/
    ArrayOf<T>* setImg(int _iPos, T _data)
    {
        if (m_pImgData == NULL || _iPos >= m_iSize)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*setimg_t)(int, T);
        ArrayOf<T>* pIT = checkRef(this, (setimg_t)&ArrayOf<T>::setImg, _iPos, _data);
        if (pIT != this)
        {
            return pIT;
        }

        m_pImgData[_iPos] = copyValue(_data);
        return this;
    }


    ArrayOf<T>* setImg(int _iRows, int _iCols, T _data)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return setImg(getIndex(piIndexes), copyValue(_data));
    }

    ArrayOf<T>* setImg(T* _pdata)
    {
        if (m_pImgData == NULL)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*setimg_t)(T*);
        ArrayOf<T>* pIT = checkRef(this, (setimg_t)&ArrayOf<T>::setImg, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pImgData[i] = copyValue(_pdata[i]);
        }

        return this;
    }


    ArrayOf<T>* setImg(const T* _pdata)
    {
        if (m_pImgData == NULL)
        {
            return NULL;
        }

        typedef ArrayOf<T>* (ArrayOf<T>::*setimg_t)(const T*);
        ArrayOf<T>* pIT = checkRef(this, (setimg_t)&ArrayOf<T>::setImg, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0 ; i < m_iSize ; i++)
        {
            m_pImgData[i] = copyValue(_pdata[i]);
        }

        return this;
    }

    inline T* getImg() const
    {
        return m_pImgData;
    }

    inline T getImg(int _iPos)
    {
        if (m_pImgData)
        {
            return m_pImgData[_iPos];
        }
        return T();
    }

    inline T getImg(int _iRows, int _iCols)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return getImg(getIndex(piIndexes));
    }

    virtual ArrayOf<T>* insert(typed_list* _pArgs, InternalType* _pSource);
    virtual ArrayOf<T>* append(int _iRows, int _iCols, InternalType* _poSource);
    virtual ArrayOf<T>* resize(int* _piDims, int _iDims);

    // return a GenericType because of [] wich is a types::Double (can't be a ArrayOf<char>)
    virtual GenericType* remove(typed_list* _pArgs);
    virtual GenericType* extract(typed_list* _pArgs);
    virtual GenericType* insertNew(typed_list* _pArgs);

    virtual bool invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e) override;
    virtual bool isInvokable() const;
    virtual bool hasInvokeOption() const;
    virtual int getInvokeNbIn();
    virtual int getInvokeNbOut();

    virtual ArrayOf<T>* reshape(int _iNewRows, int _iNewCols)
    {
        int piDims[2] = {_iNewRows, _iNewCols};
        return reshape(piDims, 2);
    }

    virtual ArrayOf<T>* reshape(int* _piDims, int _iDims);


    virtual ArrayOf<T>* resize(int _iNewRows, int _iNewCols)
    {
        int piDims[2] = {_iNewRows, _iNewCols};
        return resize(piDims, 2);
    }

    virtual void deleteData(T /*data*/)
    {
    }

    /*dimensions functions*/
    int getIndex(const int* _piIndexes)
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

    void getIndexes(int _iIndex, int* _piIndexes);

    ArrayOf<T>* getColumnValues(int _iPos)
    {
        ArrayOf<T>* pOut = NULL;
        if (_iPos < m_iCols)
        {
            int piDims[2] = {m_iRows, 1};
            pOut = createEmpty(2, piDims, m_pImgData != NULL);
            T* pReal    = pOut->get();
            T* pImg     = pOut->getImg();
            for (int i = 0 ; i < m_iRows ; i++)
            {
                pReal[i] = copyValue(get(i, _iPos));
            }

            if (m_pImgData != NULL)
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
