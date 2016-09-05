/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

// This code is separated in double.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __DOUBLE_HXX__
#define __DOUBLE_HXX__

#include "arrayof.hxx"
#include "bool.hxx"
#include "types_transposition.hxx"

namespace types
{
class EXTERN_AST Double : public ArrayOf<double>
{
public :
    virtual						~Double();

    Double(double _dblReal);
    Double(double _dblReal, double _dblImg);
    Double(int _iRows, int _iCols, bool _bComplex = false, bool _bZComplex = false);
    Double(int _iRows, int _iCols, double **_pdblReal);
    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
    Double(int _iDims, const int* _piDims, bool _bComplex = false, bool _bZComplex = false);

    static Double*              Empty();
    static Double*              Identity(int _iRows, int _iCols);
    static Double*              Identity(int _iDims, const int* _piDims);
    static Double*              Identity(int _iDims, const int* _piDims, double _dblReal);
    static Double*              Identity(int _iDims, const int* _piDims, double _dblReal, double _dblImg);


    /*data management*/
    double*                     getReal() const;
    double                      getReal(int _iRows, int _iCols);
    bool                        setInt(int* _piReal); //to translate int to double matrix

    /*zero or one set filler*/
    bool                        setZeros();
    bool                        setOnes();

    /*Config management*/
    void                        whoAmI();
    bool                        isEmpty();

    Double*                     clone();
    bool                        fillFromCol(int _iCols, Double *_poSource);
    bool                        fillFromRow(int _iRows, Double *_poSource);
    Double*                     append(int _iRows, int _iCols, InternalType* _poSource);

    //bool                        append(int _iRows, int _iCols, Double *_poSource);

    bool                        operator==(const InternalType& it);
    bool                        operator!=(const InternalType& it);

    bool                        isDouble()
    {
        return true;
    }

    bool isComplex()
    {
        return (m_pImgData != NULL) || isViewAsZComplex();
    }

    inline bool isNumericallyComplex(double tolerance = 0)
    {
        if(isComplex())
        {
            int listSize = getSize();
            double* bImg = getImg();
            for(int i = 0; i < listSize; i++)
            {
                if(abs(bImg[i]) > tolerance)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool isTrue();

    bool neg(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        return ArrayOf<double>::neg(out);
    }

    void                        setViewAsInteger(bool _bViewAsInteger = true)
    {
        m_bViewAsInteger = _bViewAsInteger;
    }
    bool                        isViewAsInteger()
    {
        return m_bViewAsInteger;
    }

    void                        convertToInteger();
    void                        convertFromInteger();

    void                        setViewAsZComplex(bool _bViewAsZComplex = true)
    {
        m_bViewAsZComplex = _bViewAsZComplex;
    }
    bool                        isViewAsZComplex()
    {
        return m_bViewAsZComplex;
    }

    void                        convertToZComplex();
    void                        convertFromZComplex();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr()
    {
        return L"constant";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"s";
    }

    inline ScilabType           getType(void)
    {
        return ScilabDouble;
    }
    inline ScilabId             getId(void)
    {
        return isIdentity() ? isComplex() ? IdIdentityComplex : IdIdentity
               : isEmpty() ? IdEmpty
               : isComplex() ? isScalar() ? IdScalarDoubleComplex
               : IdDoubleComplex
               : isScalar() ? IdScalarDouble
               : IdDouble;
    }

    inline bool conjugate(InternalType *& out)
    {
        if (isEmpty() || isIdentity() || !isComplex())
        {
            out = clone();
            return true;
        }

        if (isScalar())
        {
            out = new Double(m_pRealData[0], -m_pImgData[0]);
            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), true);
            out = pReturn;

            Transposition::conjugate(getSize(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            return true;
        }

        return false;

    }

    virtual bool adjoint(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        if (isIdentity())
        {
            out = clone();
            return true;
        }

        if (isScalar())
        {
            if (isComplex())
            {
                out = new Double(m_pRealData[0], -m_pImgData[0]);
            }
            else
            {
                out = clone();
            }

            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), isComplex());
            out = pReturn;
            if (isComplex())
            {
                Transposition::adjoint(getRows(), getCols(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            }
            else
            {
                Transposition::adjoint(getRows(), getCols(), m_pRealData, pReturn->m_pRealData);
            }

            return true;
        }

        return false;
    }

    virtual bool transpose(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        if (isIdentity() || isScalar())
        {
            out = clone();
            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), isComplex());
            out = pReturn;
            if (isComplex())
            {
                Transposition::transpose(getRows(), getCols(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            }
            else
            {
                Transposition::transpose(getRows(), getCols(), m_pRealData, pReturn->m_pRealData);
            }

            return true;
        }

        return false;
    }

    virtual ast::Exp*           getExp(const Location& loc);

    virtual Double* set(int _iPos, const double _data)
    {
        if (_iPos >= m_iSize)
        {
            return NULL;
        }

        typedef Double* (Double::*set_t)(int, double);
        Double* pIT = checkRef(this, (set_t)&Double::set, _iPos, _data);
        if (pIT != this)
        {
            return pIT;
        }

        m_pRealData[_iPos] = _data;
        return this;
    }

    virtual Double* set(int _iRows, int _iCols, const double _data)
    {
        return set(_iCols * getRows() + _iRows, _data);
    }

    virtual Double* set(double* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return NULL;
        }

        typedef Double* (Double::*set_t)(double*);
        Double* pIT = checkRef(this, (set_t)&Double::set, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0; i < m_iSize; i++)
        {
            m_pRealData[i] = _pdata[i];
        }

        return this;
    }

    virtual Double* set(const double* _pdata)
    {
        if (m_pRealData == NULL)
        {
            return NULL;
        }

        typedef Double* (Double::*set_t)(const double*);
        Double* pIT = checkRef(this, (set_t)&Double::set, _pdata);
        if (pIT != this)
        {
            return pIT;
        }

        for (int i = 0; i < m_iSize; i++)
        {
            m_pRealData[i] = _pdata[i];
        }

        return this;
    }


private:
    virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    virtual double              getNullValue();
    virtual Double*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual double              copyValue(double _dblData);
    virtual void                deleteAll();
    virtual void                deleteImg();
    virtual double*             allocData(int _iSize);
    virtual void                deleteData(double /*data*/) { }

    bool                        m_bViewAsInteger;
    bool                        m_bViewAsZComplex;

};
}

#ifdef _MSC_VER
template class types::ArrayOf<double>; //Double
#endif
#endif /* !__DOUBLE_HXX__ */
