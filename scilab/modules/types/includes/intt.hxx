/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file intt.hxx
 ** Define Scilab integer datatypes {u}int{8,16,32,64}
 */

#ifndef __INTT_HH__
#define __INTT_HH__

#include "core_math.h"
#include "int.hxx"
#include "types.hxx"
#include "tostring_common.hxx"
namespace types
{
    /**
     ** Class IntT<typename T>
     ** \brief This is a inner class that derivates from Int to define all integers
     ** \param typename T stands for the data representation
     **
     ** Scilab : uint8 -> 8 bytes -> C : unsigned char
     ** Scilab : int8 -> 8 bytes -> C : char
     ** ...
     ** Scilab : uint64 -> 64 bytes -> C : unsigned long long
     ** Scilab : int64 -> 64 bytes -> C : long long
     */

    template <typename T>
    class IntT : public Int
    {
    private :
        T* m_pData;

        void data_set(T *_pData)
        {
            m_pData = _pData;
        }

    public :
        //IntT
        void createInt(int _iRows, int _iCols)
        {
            m_iRows				= _iRows;
            m_iCols				= _iCols;
            m_iSize				= _iRows * _iCols;

            m_pData				= new T[m_iSize];
        }

        IntT()
        {
        }

        IntT(int _iRows, int _iCols)
        {
            createInt(_iRows, _iCols);
        }

        IntT(T _TData)
        {
            createInt(1, 1);
            m_pData[0]		= _TData;
        }

        IntT(int _iRows, int _iCols, T* _pTData)
        {
            createInt(_iRows, _iCols);
            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pData[i] = _pTData[i];
            }
        }

        ~IntT()
        {
            delete[] m_pData;
        }

        virtual Int *clone()
        {
            Int *pOut = Int::createInt(rows_get(), cols_get(), getIntType());

            T* pTData = new T[size_get()];
            memcpy(pTData, m_pData, size_get() * sizeof(T));

            dynamic_cast<IntT<T>*>(pOut)->data_set(pTData);

            return pOut;
        }

        bool data_set(Int* _pTData)
        {
            if(m_iSize != _pTData->size_get())
            {
                return false;
            }

            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pData[i] = static_cast<T>(_pTData->data_get(i));
            }
            return true;
        }

        bool data_set(int _iRows, int _iCols, long long _YData)
        {
            if(_iRows >= m_iRows || _iCols >= m_iRows)
            {
                return false;
            }

            return data_set(_iCols * m_iRows + _iRows, _YData);
        }

        bool data_set(int _iPos, long long _YData)
        {
            if(_iPos >= m_iSize)
            {
                return false;
            }

            m_pData[_iPos] = static_cast<T>(_YData);
            return true;
        }

        void* data_get()
        {
            return m_pData;
        }

        long long data_get(int _iPos)
        {
            return m_pData[_iPos];
        }

        long long data_get(int _iRows, int _iCols)
        {
            return data_get(_iCols * m_iRows + _iRows);
        }

        bool resize(int _iNewRows, int _iNewCols)
        {
            //alloc new data array
            T* pT = NULL;

            pT = new T[_iNewRows * _iNewCols];
            memset(pT, 0x00, sizeof(T) * _iNewRows * _iNewCols);

            for(int i = 0 ; i < m_iRows ; i++)
            {
                for(int j = 0 ; j < m_iCols ; j++)
                {
                    pT[j * _iNewRows + i] = m_pData[j * m_iRows + i];
                }
            }
            delete[] m_pData;
            m_pData	= pT;
            return true;
        }

        GenericType* get_col_value(int _iPos)
        {
            //FIXME
            return NULL;
        }

        Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
        {
            Int* pOut		= NULL;
            int iRowsOut	= 0;
            int iColsOut	= 0;

            if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
            {
                return NULL;
            }

            pOut = Int::createInt(iRowsOut, iColsOut, getIntType());
            return extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
        }


        Int* extract(Int* _poOut, int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
        {
            if(_bAsVector)
            {
                for(int i = 0 ; i < _iSeqCount ; i++)
                {
                    T TTemp = static_cast<T>(data_get(_piSeqCoord[i] - 1));
                    _poOut->data_set(i, TTemp);
                }
            }
            else
            {
                for(int i = 0 ; i < _iSeqCount ; i++)
                {
                    //convert vertical indexes to horizontal indexes
                    int iOutIndex   = (i % _poOut->cols_get()) * _poOut->rows_get() + (i / _poOut->cols_get());
                    int iInIndex    = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
                    _poOut->data_set(iOutIndex,data_get(iInIndex));
                }
            }
            return _poOut;
        }

        bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
        {
            int iNewRows = rows_get();
            int iNewCols = cols_get();

            if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
            {
                return false;
            }

            if(resize(iNewRows, iNewCols) == false)
            {
                return false;
            }

            Int* pIn = _poSource->getAsInt();
            if(pIn->size_get() == 1)
            {//a(?) = x
                if(_bAsVector)
                {//a([]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        m_pData[_piSeqCoord[i] - 1]	= static_cast<T>(pIn->data_get(0));
                    }
                }
                else
                {//a([],[]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
                        m_pData[iPos]	= static_cast<T>(pIn->data_get(0));
                    }
                }
            }
            else
            {//a(?) = [x]
                if(_bAsVector)
                {//a([]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        m_pData[_piSeqCoord[i] - 1]	= static_cast<T>(pIn->data_get(i));
                    }
                }
                else
                {//a([],[]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
                        int iTempR = i / pIn->cols_get();
                        int iTempC = i % pIn->cols_get();
                        int iNew_i = iTempR + iTempC * pIn->rows_get();

                        m_pData[iPos]	= static_cast<T>(pIn->data_get(iNew_i));
                    }
                }
            }
            return true;
        }


        wstring toString(int _iPrecision, int _iLineLen)
        {
           wostringstream ostr;
            if(m_iRows == 1 && m_iCols == 1)
            {//scalar
                int iWidth = 0;
                GetIntFormat(m_pData[0], &iWidth);
                AddIntValue(&ostr, m_pData[0], iWidth);
                ostr << std::endl;
            }
            else if(m_iCols == 1)
            {//column vector
                for(int i = 0 ; i < m_iRows ; i++)
                {
                    int iWidth = 0;
                    GetIntFormat(m_pData[i], &iWidth);
                    AddIntValue(&ostr, m_pData[i], iWidth);
                    ostr << std::endl;
                }
            }
            else if(m_iRows == 1)
            {//row vector
                wostringstream ostemp;
                int iLastVal = 0;

                for(int i = 0 ; i < m_iCols ; i++)
                {
                    int iWidth = 0;
                    GetIntFormat(m_pData[i], &iWidth);
                    int iLen = iWidth + static_cast<int>(ostemp.str().size());
                    if(iLen > _iLineLen)
                    {//Max length, new line
                        ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << i << std::endl << std::endl;
                        ostr << ostemp.str() << std::endl;
                        ostemp.str(L"");
                        iLastVal = i;
                    }

                    AddIntValue(&ostemp, m_pData[i], iWidth);
                }

                if(iLastVal != 0)
                {
                    ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << m_iCols << std::endl << std::endl;
                }
                ostemp << std::endl;
                ostr << ostemp.str();
            }
            else // matrix
            {
                wostringstream ostemp;
                int iLen = 0;
                int iLastCol = 0;

                //Array with the max printed size of each col
                int *piSize = new int[m_iCols];
                memset(piSize, 0x00, m_iCols * sizeof(int));

                //compute the row size for padding for each printed bloc.
                for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)
                {
                    for(int iRows1 = 0 ; iRows1 < m_iRows ; iRows1++)
                    {
                        int iWidth			= 0;
                        int iCurrentLen = 0;

                        GetIntFormat(m_pData[iCols1 * m_iRows + iRows1], &iWidth);
                        iCurrentLen	= iWidth ;

                        if(iCurrentLen > piSize[iCols1])
                        {
                            piSize[iCols1] = iCurrentLen;
                        }
                    }

                    if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidth	= 0;
                                GetIntFormat(m_pData[iCols2 * m_iRows + iRows2], &iWidth);
                                AddIntValue(&ostemp, m_pData[iCols2 * m_iRows + iRows2], piSize[iCols2]);
                            }
                            ostemp << std::endl;
                        }
                        iLen = 0;
                        ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << std::endl << std::endl;;
                        ostr << ostemp.str();
                        ostemp.str(L"");
                        iLastCol = iCols1;

                    }
                    iLen += piSize[iCols1] + SIGN_LENGTH;
                }

                for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < m_iCols ; iCols2++)
                    {
                        int iWidth			= 0;
                        GetIntFormat(m_pData[iCols2 * m_iRows + iRows2], &iWidth);
                        AddIntValue(&ostemp, m_pData[iCols2 * m_iRows + iRows2], piSize[iCols2]);
                    }
                    ostemp << std::endl;
                }
                if(iLastCol != 0)
                {
                    ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << m_iCols << std::endl << std::endl;
                }
                ostr << ostemp.str();
            }
            return ostr.str();
        }
        virtual IntType getIntType() = 0;

    private :
        virtual void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true) = 0;
        virtual void GetIntFormat(T _TVal, int *_piWidth) = 0;

    };

    /*
    ** Class SignedIntT<typename T>
    ** \brief This is a inner class that derivates from IntT to define all __signed__ integers
    ** \param typename T stands for the data representation and forwards IntT templated typename
    **
    ** Scilab : int8 -> 8 bytes -> C : char
    ** ...
    ** Scilab : int64 -> 64 bytes -> C : long long
    */
    template <typename T>
    class SignedIntT : public IntT<T>
    {
    protected :
        SignedIntT(int _iRows, int _iCols) : IntT<T>(_iRows, _iCols) {}

        void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
        {
            AddSignedIntValue(_postr, _TVal, _iWidth, bPrintPlusSign, bPrintOne);
        }

        void GetIntFormat(T _TVal, int *_piWidth)
        {
            GetSignedIntFormat(_TVal, _piWidth);
        }
    };

    /*
    ** Class SignedIntT<typename T>
    ** \brief This is a inner class that derivates from IntT to define all __unsisigned__ integers
    ** \param typename T stands for the data representation and forwards IntT templated typename
    **
    ** Scilab : uint8 -> 8 bytes -> C : unsigned char
    ** ...
    ** Scilab : uint64 -> 64 bytes -> C : unsigned long long
    */
    template <typename T>
    class UnsignedIntT : public IntT<T>
    {
    protected :
        UnsignedIntT(int _iRows, int _iCols) : IntT<T>(_iRows, _iCols) {}
        void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
        {
            AddUnsignedIntValue(_postr, _TVal, _iWidth, bPrintPlusSign, bPrintOne);
        }
        void GetIntFormat(T _TVal, int *_piWidth)
        {
            GetUnsignedIntFormat(_TVal, _piWidth);
        }
    };

    /*
    ** Class Int8 <-> Scilab int8(X)
    */
    class Int8 : public SignedIntT<char>
    {
    public :
        Int8(int _iRows, int _iCols) : SignedIntT<char>(_iRows, _iCols) {}
        IntType getIntType() { return Type8; }
        wstring getTypeStr(){return L"int8";}
    };

    /*
    ** Class UInt8 <-> Scilab uint8(X)
    */
    class UInt8 : public UnsignedIntT<unsigned char>
    {
    public :
        UInt8(int _iRows, int _iCols) : UnsignedIntT<unsigned char>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned8; }
        wstring getTypeStr(){return L"uint8";}
    };

//16 bits
    /*
    ** Class Int16 <-> Scilab int16(X)
    */
    class Int16 : public SignedIntT<short>
    {
    public :
        Int16(int _iRows, int _iCols) : SignedIntT<short>(_iRows, _iCols) {}
        IntType getIntType() { return Type16; }
        wstring getTypeStr(){return L"int16";}
    };

    /*
    ** Class UInt16 <-> Scilab uint16(X)
    */
    class UInt16 : public UnsignedIntT<unsigned short>
    {
    public :
        UInt16(int _iRows, int _iCols) : UnsignedIntT<unsigned short>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned16; }
        wstring getTypeStr(){return L"uint16";}
    };

//32 bits
    /*
    ** Class Int32 <-> Scilab int32(X)
    */
    class Int32 : public SignedIntT<int>
    {
    public :
        Int32(int _iRows, int _iCols) : SignedIntT<int>(_iRows, _iCols) {}
        IntType getIntType() { return Type32; }
        wstring getTypeStr(){return L"int32";}
    };

    /*
    ** Class UInt32 <-> Scilab uint32(X)
    */
    class UInt32 : public UnsignedIntT<unsigned int>
    {
    public :
        UInt32(int _iRows, int _iCols) : UnsignedIntT<unsigned int>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned32; }
        wstring getTypeStr(){return L"uint32";}
    };

//64 bits
    /*
    ** Class Int64 <-> Scilab int64(X) -*- NEW Scilab 6.0 -*-
    */
    class Int64 : public SignedIntT<long long>
    {
    public :
        Int64(int _iRows, int _iCols) : SignedIntT<long long>(_iRows, _iCols) {}
        IntType getIntType() { return Type64; }
        wstring getTypeStr(){return L"int64";}
    };

    /*
    ** Class UInt64 <-> Scilab uint64(X) -*- NEW Scilab 6.0 -*-
    */
    class UInt64 : public UnsignedIntT<unsigned long long>
    {
    public :
        UInt64(int _iRows, int _iCols) : UnsignedIntT<unsigned long long>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned64; }
        wstring getTypeStr(){return L"uint64";}
    };
}
#endif /* __INTT_HH__ */
