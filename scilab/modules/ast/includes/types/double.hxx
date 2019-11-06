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
    void                        whoAmI() override;
    bool                        isEmpty();

    Double*                     clone() override;
    bool                        fillFromCol(int _iCols, Double *_poSource);
    bool                        fillFromRow(int _iRows, Double *_poSource);
    Double*                     append(int _iRows, int _iCols, InternalType* _poSource) override;

    //bool                        append(int _iRows, int _iCols, Double *_poSource);

    bool                        operator==(const InternalType& it) override;
    bool                        operator!=(const InternalType& it) override;

    bool                        isDouble() override
    {
        return true;
    }

    bool isComplex() override
    {
        return (m_pImgData != NULL) || isViewAsZComplex();
    }

    inline bool isNumericallyComplex(double tolerance = 0)
    {
        if (isComplex())
        {
            int listSize = getSize();
            double* bImg = getImg();
            for (int i = 0; i < listSize; i++)
            {
                if (abs(bImg[i]) > tolerance)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool isTrue() override;

    bool neg(InternalType *& out) override
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
    virtual std::wstring        getTypeStr() const override
    {
        return L"constant";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr() const override
    {
        return L"s";
    }

    inline ScilabType           getType(void) override
    {
        return ScilabDouble;
    }
    inline ScilabId             getId(void) override
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

    virtual bool adjoint(InternalType *& out) override
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

    virtual bool transpose(InternalType *& out) override
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

    virtual ast::Exp*           getExp(const Location& loc) override;

    virtual Double* set(int _iPos, const double _data) override
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

    virtual Double* set(int _iRows, int _iCols, const double _data) override
    {
        return set(_iCols * getRows() + _iRows, _data);
    }

    virtual Double* set(double* _pdata) override
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

    virtual Double* set(const double* _pdata) override
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

    virtual bool isNativeType() override
    {
        return true;
    }

    virtual void fillDefaultValues() override
    {
        int size = getSize();
        memset(m_pRealData, 0x00, sizeof(double) * size);
        if (isComplex())
        {
            memset(m_pImgData, 0x00, sizeof(double) * size);
        }
    }

private:
    virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    virtual double              getNullValue() override;
    virtual Double*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false) override;
    virtual double              copyValue(double _dblData) override;
    virtual void                deleteAll() override;
    virtual void                deleteImg() override;
    virtual double*             allocData(int _iSize) override;
    virtual void                deleteData(double /*data*/) override { }

    bool                        m_bViewAsInteger;
    bool                        m_bViewAsZComplex;

};
}

#ifdef _MSC_VER
template class types::ArrayOf<double>; //Double
#endif
#endif /* !__DOUBLE_HXX__ */
