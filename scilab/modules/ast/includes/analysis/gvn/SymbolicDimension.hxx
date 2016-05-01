/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __SYMBOLIC_DIMENSION_HXX__
#define __SYMBOLIC_DIMENSION_HXX__

#include <iostream>

#include "GVN.hxx"
#include "tools.hxx"

namespace analysis
{

/**
 * \class SymbolicDimension
 * \brief Manage a symbolic dimension
 */
class SymbolicDimension
{
    GVN * gvn;
    GVN::Value * value;

public:

    /**
     * \brief default constructor
     */
    SymbolicDimension() : gvn(nullptr), value(nullptr) { }

    /**
     * \brief constructor
     * \param _gvn the GVN associated with this dimension
     */
    SymbolicDimension(GVN & _gvn) : gvn(&_gvn), value(_gvn.getValue()) { }

    /**
     * \brief constructor
     * \param _gvn the GVN associated with this dimension
     * \param _value the value of this dimension
     */
    SymbolicDimension(GVN * _gvn, GVN::Value * _value) : gvn(_gvn), value(_value) { }

    /**
     * \brief constructor
     * \param _gvn the GVN associated with this dimension
     * \param _value the value of this dimension
     */
    SymbolicDimension(GVN & _gvn, GVN::Value * _value) : gvn(&_gvn), value(_value) { }

    /**
     * \brief constructor
     * \param _gvn the GVN associated with this dimension
     * \param dim the value of this dimension
     */
    SymbolicDimension(GVN & _gvn, int64_t dim) : gvn(&_gvn), value(dim == -1 ? _gvn.getValue() : _gvn.getValue(dim)) { }

    /**
     * \brief Get the associated GVN
     * \return the GVN
     */
    inline GVN * getGVN()
    {
        return gvn;
    }

    /**
     * \brief Check if this dimension is empty
     * \return true if empty
     */
    inline bool empty() const
    {
        return gvn == nullptr || value == nullptr;
    }

    /**
     * \brief Check if this dimension is valid
     * \return true if valid
     */
    inline bool isValid() const
    {
        return gvn != nullptr;
    }

    /**
     * \brief Check if this dimension is constant (i.e. the associated polynomial is constant)
     * \return true if constant
     */
    inline bool isConstant() const
    {
        return value->poly->isConstant();
    }

    /**
     * \brief Get the constant part of the associated polynomial
     * \return the constant
     */
    inline int64_t getConstant() const
    {
        return value->poly->constant;
    }

    /**
     * \brief Get the associated polynomial
     * \return a polynomial
     */
    inline const MultivariatePolynomial * getPoly() const
    {
        return value->poly;
    }

    /**
     * \brief Get the associated Value
     * \return a Value
     */
    inline const GVN::Value * getValue() const
    {
        return value;
    }

    /**
     * \brief Get the associated Value
     * \return a Value
     */
    inline GVN::Value * getValue()
    {
        return value;
    }

    /**
     * \brief Set the associated Value
     * \param _value a Value
     */
    inline void setValue(GVN::Value * _value)
    {
        value = _value;
    }

    /**
     * \brief Set the associated Value
     * \param _value a Value
     */
    inline void setValue(const int64_t _value)
    {
        value = gvn->getValue(_value);
    }

    /**
     * \brief Set the associated GVN
     * \param _gvn a GVN
     */
    inline void setGVN(GVN * _gvn)
    {
        gvn = _gvn;
    }

    /**
     * \brief merge two dimensions considered as a max (cf Info::maxIndex)
     * \param dim a dim
     */
    inline void mergeAsMax(const SymbolicDimension & dim)
    {
        bool mustInvalidate = true;
        if (isValid() && dim.isValid())
        {
            if (value->poly && dim.value->poly)
            {
                MultivariatePolynomial mp = *value->poly - *dim.value->poly;
                if (mp.isCoeffPositive())
                {
                    value = dim.value;
                    mustInvalidate = false;
                }
                else if (mp.isCoeffNegative())
                {
                    mustInvalidate = false;
                }
            }
        }
        if (mustInvalidate && gvn)
        {
            gvn = nullptr;
            value = nullptr;
        }
    }

    /**
     * \brief Overload of the + operator
     */
    inline SymbolicDimension operator+(const SymbolicDimension & R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::PLUS, *value, *R.value));
    }

    /**
     * \brief Overload of the + operator
     */
    inline SymbolicDimension operator+(const int64_t R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::PLUS, *value, *gvn->getValue(R)));
    }

    /**
     * \brief Overload of the - operator
     */
    inline SymbolicDimension operator-(const SymbolicDimension & R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::MINUS, *value, *R.value));
    }

    /**
     * \brief Overload of the - operator
     */
    inline SymbolicDimension operator-(const int64_t R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::MINUS, *value, *gvn->getValue(R)));
    }

    /**
     * \brief Overload of the * operator
     */
    inline SymbolicDimension operator*(const SymbolicDimension & R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::TIMES, *value, *R.value));
    }

    /**
     * \brief Overload of the * operator
     */
    inline SymbolicDimension operator*(const int64_t R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::TIMES, *value, *gvn->getValue(R)));
    }

    /**
     * \brief Overload of the *= operator
     */
    inline SymbolicDimension & operator*=(const SymbolicDimension & R)
    {
        if (R != 1)
        {
            if (*this != 1)
            {
                value = gvn->getValue(OpValue::Kind::TIMES, *value, *R.value);
            }
            else
            {
                value = R.value;
            }
        }

        return *this;
    }

    /**
     * \brief Overload of the / operator
     */
    inline SymbolicDimension operator/(const SymbolicDimension & R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::RDIV, *value, *R.value));
    }

    /**
     * \brief Overload of the / operator
     */
    inline SymbolicDimension operator/(const int64_t R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::RDIV, *value, *gvn->getValue(R)));
    }

    /**
     * \brief Overload of the ^ operator
     */
    inline SymbolicDimension operator^(const SymbolicDimension & R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::POWER, *value, *R.value));
    }

    /**
     * \brief Overload of the ^ operator
     */
    inline SymbolicDimension operator^(const int64_t R) const
    {
        return SymbolicDimension(gvn, gvn->getValue(OpValue::Kind::POWER, *value, *gvn->getValue(R)));
    }

    /**
     * \brief Overload of the == operator
     */
    inline bool operator==(const SymbolicDimension & R) const
    {
        return value->value == R.value->value;
    }

    /**
     * \brief Overload of the == operator
     */
    inline bool operator==(const int64_t R) const
    {
        return value->poly->isConstant(R);
    }

    /**
     * \brief Overload of the != operator
     */
    inline bool operator!=(const SymbolicDimension & R) const
    {
        return value->value != R.value->value;
    }

    /**
     * \brief Overload of the != operator
     */
    inline bool operator!=(const int64_t R) const
    {
        return !value->poly->isConstant(R);
    }

    /**
     * \brief Overload of the + operator
     */
    inline friend SymbolicDimension operator+(const int64_t L, const SymbolicDimension & R)
    {
        return R + L;
    }

    /**
     * \brief Overload of the - operator
     */
    inline friend SymbolicDimension operator-(const int64_t L, const SymbolicDimension & R)
    {
        return SymbolicDimension(R.gvn, R.gvn->getValue(OpValue::Kind::MINUS, *R.gvn->getValue(L), *R.value));
    }

    /**
     * \brief Overload of the * operator
     */
    inline friend SymbolicDimension operator*(const int64_t L, const SymbolicDimension & R)
    {
        return R * L;
    }

    /**
     * \brief Overload of the / operator
     */
    inline friend SymbolicDimension operator/(const int64_t L, const SymbolicDimension & R)
    {
        return SymbolicDimension(R.gvn, R.gvn->getValue(OpValue::Kind::RDIV, *R.gvn->getValue(L), *R.value));
    }

    /**
     * \brief Overload of the ^ operator
     */
    inline friend SymbolicDimension operator^(const int64_t L, const SymbolicDimension & R)
    {
        return SymbolicDimension(R.gvn, R.gvn->getValue(OpValue::Kind::POWER, *R.gvn->getValue(L), *R.value));
    }

    /**
     * \brief Overload of the == operator
     */
    inline friend bool operator==(const int64_t L, const SymbolicDimension & R)
    {
        return R == L;
    }

    /**
     * \brief Overload of the != operator
     */
    inline friend bool operator!=(const int64_t L, const SymbolicDimension & R)
    {
        return R != L;
    }

    /**
     * \brief Overload of the << operator
     */
    friend inline std::wostream & operator<<(std::wostream & out, const SymbolicDimension & d)
    {
        out << *d.value->poly;

        return out;
    }
};

} // namespace analysis

#endif // __SYMBOLIC_DIMENSION_HXX__
