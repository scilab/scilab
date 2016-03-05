/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __INT_HXX__
#define __INT_HXX__

#include "arrayof.hxx"
#include "bool.hxx"
#include "internal.hxx"
#include "types_transposition.hxx"
#include "configvariable.hxx"
#include "type_traits.hxx"

#include <sstream>
#include <string>
#include <limits>       // std::numeric_limits
#include "tostring_common.hxx"

namespace types
{
template<typename T>
class EXTERN_AST Int : public ArrayOf<T>
{
public :

    Int(int _iRows, int _iCols)
    {
        int piDims[2] = {_iRows, _iCols};
        T * pInt = NULL;
        this->create(piDims, 2, &pInt, NULL);
#ifndef NDEBUG
        //Inspector::addItem(this);
#endif
    }

    Int(T _val)
    {
        int piDims[2]   = {1, 1};
        T * pInt = NULL;
        this->create(piDims, 2, &pInt, NULL);
        pInt[0] = _val;
#ifndef NDEBUG
        //Inspector::addItem(this);
#endif
    }

    Int(int _iRows, int _iCols, T** _pData)
    {
        const int piDims[2] = {_iRows, _iCols};
        this->create(piDims, 2, _pData, NULL);
#ifndef NDEBUG
        //Inspector::addItem(this);
#endif
    }

    Int(int _iDims, const int* _piDims)
    {
        T * pInt = NULL;
        this->create(_piDims, _iDims, &pInt, NULL);
#ifndef NDEBUG
        //Inspector::addItem(this);
#endif
    }

    ~Int()
    {
        if (InternalType::isDeletable() == true)
        {
            deleteAll();
        }
#ifndef NDEBUG
        //Inspector::removeItem(this);
#endif
    }

    Int<T>* clone()
    {
        Int<T> *pbClone = new Int<T>(GenericType::getDims(), GenericType::getDimsArray());
        pbClone->set(ArrayOf<T>::get());
        return pbClone;
    }

    /*Config management*/
    void whoAmI();

    bool isInt()
    {
        return true;
    }

    bool neg(InternalType *& out)
    {
        out = new Int<T>(this->getDims(), this->getDimsArray());
        type_traits::bin_neg<T, T>(this->m_iSize, this->m_pRealData, static_cast<Int<T> *>(out)->get());

        return true;
    }

    virtual bool transpose(InternalType *& out)
    {
        return type_traits::transpose(*this, out);
    }

    bool operator==(const InternalType& it)
    {
        if (const_cast<InternalType &>(it).getType() != getType())
        {
            return false;
        }

        Int<T>* pb = const_cast<InternalType &>(it).getAs<typename types::Int<T> >();

        if (pb->getDims() != GenericType::getDims())
        {
            return false;
        }

        for (int i = 0 ; i < GenericType::getDims() ; i++)
        {
            if (pb->getDimsArray()[i] != GenericType::getDimsArray()[i])
            {
                return false;
            }
        }

        if (memcmp(ArrayOf<T>::get(), pb->get(), GenericType::getSize() * sizeof(T)) != 0)
        {
            return false;
        }
        return true;
    }

    bool operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr();

    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"i";
    }

protected :
    inline InternalType::ScilabType getType(void);
    inline InternalType::ScilabId   getId(void);

private :
    virtual bool subMatrixToString(std::wostringstream& ostr, int* _piDims, int /*_iDims*/) override
    {
        int iCurrentLine = 0;
        int iLineLen = ConfigVariable::getConsoleWidth();
        int iMaxLines = ConfigVariable::getConsoleLines();

        if (GenericType::isIdentity())
        {
            ostr << L"eye *" << std::endl << std::endl;
            int iWidth = 0;
            if (isSigned())
            {
                getSignedIntFormat(ArrayOf<T>::get(0), &iWidth);
                addSignedIntValue(&ostr, ArrayOf<T>::get(0), iWidth);
            }
            else
            {
                getUnsignedIntFormat(ArrayOf<T>::get(0), &iWidth);
                addUnsignedIntValue(&ostr, ArrayOf<T>::get(0), iWidth);
            }
            ostr << std::endl;
        }
        else if (GenericType::isScalar())
        {
            //scalar
            int iWidth  = 0;
            _piDims[0]  = 0;
            _piDims[1]  = 0;
            int iPos    = ArrayOf<T>::getIndex(_piDims);

            if (isSigned())
            {
                getSignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                addSignedIntValue(&ostr, ArrayOf<T>::get(iPos), iWidth);
            }
            else
            {
                getUnsignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                addUnsignedIntValue(&ostr, ArrayOf<T>::get(iPos), iWidth);
            }
            ostr << std::endl;
        }
        else if (GenericType::getCols() == 1)
        {
            //column vector
            int iWidthMax = 0;

            //Array with the max printed size of each col
            for (int i = 0 ; i < GenericType::getRows() ; i++)
            {
                _piDims[1] = 0;
                _piDims[0] = i;
                int iWidth = 0;
                int iPos = ArrayOf<T>::getIndex(_piDims);
                if (isSigned())
                {
                    getSignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                }
                else
                {
                    getUnsignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                }
                iWidthMax = (std::max)(iWidthMax, iWidth);
            }

            for (int i = this->m_iRows1PrintState ; i < this->getRows() ; i++)
            {
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    this->m_iRows1PrintState = i;
                    return false;
                }

                _piDims[1]  = 0;
                _piDims[0]  = i;
                int iPos    = ArrayOf<T>::getIndex(_piDims);

                if (isSigned())
                {
                    addSignedIntValue(&ostr, ArrayOf<T>::get(iPos), iWidthMax);
                }
                else
                {
                    addUnsignedIntValue(&ostr, ArrayOf<T>::get(iPos), iWidthMax);
                }
                ostr << std::endl;
            }
        }
        else if (GenericType::getRows() == 1)
        {
            //row vector
            std::wostringstream ostemp;
            int iLastVal = this->m_iCols1PrintState;

            for (int i = this->m_iCols1PrintState ; i < this->getCols() ; i++)
            {
                int iWidth  = 0;
                int iLen    = 0;
                _piDims[0]  = 0;
                _piDims[1]  = i;
                int iPos    = ArrayOf<T>::getIndex(_piDims);

                if (isSigned())
                {
                    getSignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                }
                else
                {
                    getUnsignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                }

                iLen = iWidth + static_cast<int>(ostemp.str().size());
                if (iLen > iLineLen && iLastVal != i)
                {
                    //Max length, new line
                    iCurrentLine += 4; //"column x to Y" + empty line + value + empty line
                    if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                    {
                        this->m_iCols1PrintState = iLastVal;
                        return false;
                    }

                    addColumnString(ostr, iLastVal + 1, i);
                    ostr << ostemp.str() << std::endl;
                    ostemp.str(L"");
                    iLastVal = i;
                }

                if (isSigned())
                {
                    addSignedIntValue(&ostemp, ArrayOf<T>::get(iPos), iWidth);
                }
                else
                {
                    addUnsignedIntValue(&ostemp, ArrayOf<T>::get(iPos), iWidth);
                }
            }

            if (iLastVal != 0)
            {
                addColumnString(ostr, iLastVal + 1, GenericType::getCols());
            }

            ostemp << std::endl;
            ostr << ostemp.str();
        }
        else // matrix
        {
            std::wostringstream ostemp;
            int iLen = 0;
            int iLastCol = this->m_iCols1PrintState;

            //Array with the max printed size of each col
            int *piSize = new int[GenericType::getCols()];
            memset(piSize, 0x00, GenericType::getCols() * sizeof(int));

            //compute the row size for padding for each printed bloc.
            for (int iCols1 = this->m_iCols1PrintState ; iCols1 < this->getCols() ; iCols1++)
            {
                for (int iRows1 = 0 ; iRows1 < this->getRows() ; iRows1++)
                {
                    int iWidth  = 0;
                    _piDims[0]  = iRows1;
                    _piDims[1]  = iCols1;
                    int iPos    = ArrayOf<T>::getIndex(_piDims);

                    if (isSigned())
                    {
                        getSignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                    }
                    else
                    {
                        getUnsignedIntFormat(ArrayOf<T>::get(iPos), &iWidth);
                    }
                    piSize[iCols1] = (std::max)(piSize[iCols1], iWidth);
                }

                if (iLen + piSize[iCols1] > iLineLen && iCols1 != iLastCol)
                {
                    //find the limit, print this part
                    for (int iRows2 = this->m_iRows2PrintState ; iRows2 < this->getRows() ; iRows2++)
                    {
                        iCurrentLine++;
                        if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) ||
                                ( (iMaxLines != 0 && iCurrentLine + 3 >= iMaxLines && iRows2 == this->m_iRows2PrintState) ||
                                  (iMaxLines != 0 && iCurrentLine + 1 >= iMaxLines && iRows2 != this->m_iRows2PrintState)))
                        {
                            if (this->m_iRows2PrintState == 0 && iRows2 != 0)
                            {
                                //add header
                                addColumnString(ostr, iLastCol + 1, iCols1);
                            }

                            ostr << ostemp.str();
                            this->m_iRows2PrintState = iRows2;
                            this->m_iCols1PrintState = iLastCol;
                            return false;
                        }

                        for (int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                        {
                            _piDims[0]  = iRows2;
                            _piDims[1]  = iCols2;
                            int iPos    = ArrayOf<T>::getIndex(_piDims);

                            if (isSigned())
                            {
                                addSignedIntValue(&ostemp, ArrayOf<T>::get(iPos), piSize[iCols2]);
                            }
                            else
                            {
                                addUnsignedIntValue(&ostemp, ArrayOf<T>::get(iPos), piSize[iCols2]);
                            }
                        }
                        ostemp << std::endl;
                    }
                    iLen = 0;

                    iCurrentLine++;
                    if (this->m_iRows2PrintState == 0)
                    {
                        iCurrentLine += 3;
                        addColumnString(ostr, iLastCol + 1, iCols1);
                    }

                    ostr << ostemp.str();
                    ostemp.str(L"");
                    iLastCol = iCols1;
                    this->m_iRows2PrintState = 0;
                    this->m_iCols1PrintState = 0;
                }

                iLen += piSize[iCols1] + SIGN_LENGTH + SIZE_BETWEEN_TWO_VALUES;
            }

            for (int iRows2 = this->m_iRows2PrintState ; iRows2 < this->getRows() ; iRows2++)
            {
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    if (this->m_iRows2PrintState == 0 && iLastCol != 0)
                    {
                        //add header
                        addColumnString(ostr, iLastCol + 1, GenericType::getCols());
                    }

                    ostr << ostemp.str();
                    this->m_iRows2PrintState = iRows2;
                    this->m_iCols1PrintState = iLastCol;
                    return false;
                }

                for (int iCols2 = iLastCol ; iCols2 < GenericType::getCols() ; iCols2++)
                {
                    _piDims[0]  = iRows2;
                    _piDims[1]  = iCols2;
                    int iPos    = ArrayOf<T>::getIndex(_piDims);

                    if (isSigned())
                    {
                        addSignedIntValue(&ostemp, ArrayOf<T>::get(iPos), piSize[iCols2]);
                    }
                    else
                    {
                        addUnsignedIntValue(&ostemp, ArrayOf<T>::get(iPos), piSize[iCols2]);
                    }
                }
                ostemp << std::endl;
            }

            if (this->m_iRows2PrintState == 0 && iLastCol != 0)
            {
                addColumnString(ostr, iLastCol + 1, GenericType::getCols());
            }

            ostr << ostemp.str();
        }

        return true;
    }

    virtual T getNullValue()
    {
        return T(0);
    }

    virtual Int<T>* createEmpty(int _iDims, int* _piDims, bool /*_bComplex*/)
    {
        return new Int<T>(_iDims, _piDims);
    }

    virtual T copyValue(T _data)
    {
        return _data;
    }

    virtual void deleteAll()
    {
        delete[] ArrayOf<T>::m_pRealData;
        ArrayOf<T>::m_pRealData = NULL;
        deleteImg();
    }

    virtual void deleteImg() { }

    virtual T* allocData(int _iSize)
    {
        return new T[_iSize];
    }

    template<typename U, typename V> struct is_same_int
    {
        const static bool value = false;
    };
    template<typename U> struct is_same_int<U, U>
    {
        const static bool value = true;
    };

public:
    bool isInt8()
    {
        return is_same_int<T, char>::value;
    }
    bool isInt16()
    {
        return is_same_int<T, short>::value;
    }
    bool isInt32()
    {
        return is_same_int<T, int>::value;
    }
    bool isInt64()
    {
        return is_same_int<T, long long>::value;
    }
    bool isUInt8()
    {
        return is_same_int<T, unsigned char>::value;
    }
    bool isUInt16()
    {
        return is_same_int<T, unsigned short>::value;
    }
    bool isUInt32()
    {
        return is_same_int<T, unsigned int>::value;
    }
    bool isUInt64()
    {
        return is_same_int<T, unsigned long long>::value;
    }

    bool isUnsigned()
    {
        return !isSigned();
    }

    bool isSigned()
    {
        return std::numeric_limits<T>::is_signed;
    }
};

template<> inline InternalType::ScilabType Int<char>::getType()
{
    return ScilabInt8;
}
template<> inline InternalType::ScilabType Int<unsigned char>::getType()
{
    return ScilabUInt8;
}
template<> inline InternalType::ScilabType Int<short>::getType()
{
    return ScilabInt16;
}
template<> inline InternalType::ScilabType Int<unsigned short>::getType()
{
    return ScilabUInt16;
}
template<> inline InternalType::ScilabType Int<int>::getType()
{
    return ScilabInt32;
}
template<> inline InternalType::ScilabType Int<unsigned int>::getType()
{
    return ScilabUInt32;
}
template<> inline InternalType::ScilabType Int<long long>::getType()
{
    return ScilabInt64;
}
template<> inline InternalType::ScilabType Int<unsigned long long>::getType()
{
    return ScilabUInt64;
}

template<> inline InternalType::ScilabId Int<char>::getId()
{
    return GenericType::isScalar() ? IdScalarInt8 : IdInt8;
}
template<> inline InternalType::ScilabId Int<unsigned char>::getId()
{
    return GenericType::isScalar() ? IdScalarUInt8 : IdUInt8;
}
template<> inline InternalType::ScilabId Int<short>::getId()
{
    return GenericType::isScalar() ? IdScalarInt16 : IdInt16;
}
template<> inline InternalType::ScilabId Int<unsigned short>::getId()
{
    return GenericType::isScalar() ? IdScalarUInt16 : IdUInt16;
}
template<> inline InternalType::ScilabId Int<int>::getId()
{
    return GenericType::isScalar() ? IdScalarInt32 : IdInt32;
}
template<> inline InternalType::ScilabId Int<unsigned int>::getId()
{
    return GenericType::isScalar() ? IdScalarUInt32 : IdUInt32;
}
template<> inline InternalType::ScilabId Int<long long>::getId()
{
    return GenericType::isScalar() ? IdScalarInt64 : IdInt64;
}
template<> inline InternalType::ScilabId Int<unsigned long long>::getId()
{
    return GenericType::isScalar() ? IdScalarUInt64 : IdUInt64;
}

// Specializations
template<> inline std::wstring Int<char>::getTypeStr()
{
    return L"int8";
}

template<> inline std::wstring Int<short>::getTypeStr()
{
    return L"int16";
}

template<> inline std::wstring Int<int>::getTypeStr()
{
    return L"int32";
}

template<> inline std::wstring Int<long long>::getTypeStr()
{
    return L"int64";
}

template<> inline std::wstring Int<unsigned char>::getTypeStr()
{
    return L"uint8";
}

template<> inline std::wstring Int<unsigned short>::getTypeStr()
{
    return L"uint16";
}

template<> inline std::wstring Int<unsigned int>::getTypeStr()
{
    return L"uint32";
}

template<> inline std::wstring Int<unsigned long long>::getTypeStr()
{
    return L"uint64";
}

template<> inline void Int<char>::whoAmI()
{
    std::cout << "types::Int8";
}

template<> inline void Int<short>::whoAmI()
{
    std::cout << "types::Int16";
}

template<> inline void Int<int>::whoAmI()
{
    std::cout << "types::Int32";
}

template<> inline void Int<long long>::whoAmI()
{
    std::cout << "types::Int64";
}

template<> inline void Int<unsigned char>::whoAmI()
{
    std::cout << "types::UInt8";
}

template<> inline void Int<unsigned short>::whoAmI()
{
    std::cout << "types::UInt16";
}

template<> inline void Int<unsigned int>::whoAmI()
{
    std::cout << "types::UInt32";
}

template<> inline void Int<unsigned long long>::whoAmI()
{
    std::cout << "types::UInt64";
}


typedef Int<char> Int8;
typedef Int<short> Int16;
typedef Int<int> Int32;
typedef Int<long long> Int64;

typedef Int<unsigned char> UInt8;
typedef Int<unsigned short> UInt16;
typedef Int<unsigned int> UInt32;
typedef Int<unsigned long long> UInt64;

#ifdef _MSC_VER
template class Int<char>;
template class Int<unsigned char>;
template class Int<short>;
template class Int<unsigned short>;
template class Int<int>;
template class Int<unsigned int>;
template class Int<long long>;
template class Int<unsigned long long>;
#endif
}


#endif /* !__INT_HXX__ */
