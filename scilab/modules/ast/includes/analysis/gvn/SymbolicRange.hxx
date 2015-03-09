/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SYMBOLIC_RANGE_HXX__
#define __SYMBOLIC_RANGE_HXX__

#include <iostream>

#include "GVN.hxx"
#include "tools.hxx"

namespace analysis
{

/**
 * \class SymbolicRange
 * \brief Manage a symbolic range
 */
class SymbolicRange
{
    GVN * gvn;
    GVN::Value * start;
    GVN::Value * end;

public :

    /**
     * \brief default constructor
     */
    SymbolicRange() : gvn(nullptr), start(nullptr), end(nullptr) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicRange(GVN & _gvn, GVN::Value * _start, GVN::Value * _end) : gvn(&_gvn), start(_start), end(_end) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicRange(GVN & _gvn, double _start, double _end) : gvn(&_gvn), start(_gvn.getValue(_start)), end(_gvn.getValue(_end)) { }

    /**
     * \brief Get the associated GVN
     * \return the GVN
     */
    inline GVN * getGVN()
    {
        return gvn;
    }

    /**
     * \brief Invalidate this range
     */
    inline void invalid()
    {
        start = end = gvn->getInvalid();
    }

    /**
     * \brief Check if this range is valid
     * \return true if valid
     */
    inline bool isValid() const
    {
        return start->poly->isValid() && end->poly->isValid();
    }

    /**
     * \brief Overload of the + operator
     */
    inline SymbolicRange operator+(const SymbolicRange & R) const
    {
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::PLUS, *start, *R.start),
                             gvn->getValue(OpValue::Kind::PLUS, *end, *R.end));
    }

    /**
     * \brief Overload of the + operator
     */
    inline SymbolicRange operator+(const double R) const
    {
        GVN::Value * const val = gvn->getValue(R);
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::PLUS, *start, *val),
                             gvn->getValue(OpValue::Kind::PLUS, *end, *val));
    }

    /**
     * \brief Overload of the - operator
     */
    inline SymbolicRange operator-(const SymbolicRange & R) const
    {
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::MINUS, *start, *R.end),
                             gvn->getValue(OpValue::Kind::MINUS, *end, *R.start));
    }

    /**
     * \brief Overload of the - operator
     */
    inline SymbolicRange operator-(const double R) const
    {
        GVN::Value * const val = gvn->getValue(R);
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::MINUS, *start, *val),
                             gvn->getValue(OpValue::Kind::MINUS, *end, *val));
    }

    /**
     * \brief Overload of the * operator
     */
    inline SymbolicRange operator*(const SymbolicRange & R) const
    {
        // We suppose that all the values are positive or null
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::TIMES, *start, *R.start),
                             gvn->getValue(OpValue::Kind::TIMES, *end, *R.end));
    }

    /**
     * \brief Overload of the * operator
     */
    inline SymbolicRange operator*(const double R) const
    {
        GVN::Value * const val = gvn->getValue(R);
        if (R >= 0)
        {
            return SymbolicRange(gvn,
                                 gvn->getValue(OpValue::Kind::TIMES, *start, *val),
                                 gvn->getValue(OpValue::Kind::TIMES, *end, *val));
        }
        else
        {
            return SymbolicRange(gvn,
                                 gvn->getValue(OpValue::Kind::TIMES, *end, *val),
                                 gvn->getValue(OpValue::Kind::TIMES, *start, *val));
        }
    }

    /**
     * \brief Overload of the / operator
     */
    inline SymbolicRange operator/(const SymbolicRange & R) const
    {
        // We suppose that all the values are positive or null
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::RDIV, *start, *R.start),
                             gvn->getValue(OpValue::Kind::RDIV, *end, *R.end));
    }

    /**
     * \brief Overload of the / operator
     */
    inline SymbolicRange operator/(const double R) const
    {
        GVN::Value * const val = gvn->getValue(R);
        if (R >= 0)
        {
            return SymbolicRange(gvn,
                                 gvn->getValue(OpValue::Kind::RDIV, *start, *val),
                                 gvn->getValue(OpValue::Kind::RDIV, *end, *val));
        }
        else
        {
            return SymbolicRange(gvn,
                                 gvn->getValue(OpValue::Kind::RDIV, *end, *val),
                                 gvn->getValue(OpValue::Kind::RDIV, *start, *val));
        }
    }

    /**
     * \brief Overload of the == operator
     */
    inline bool operator==(const SymbolicRange & R) const
    {
        return start->value == R.start->value && end->value == R.end->value;
    }

    /**
     * \brief Overload of the != operator
     */
    inline bool operator!=(const SymbolicRange & R) const
    {
        return start->value != R.start->value || end->value != R.end->value;
    }

    /**
     * \brief Overload of the + operator
     */
    inline friend SymbolicRange operator+(const double L, const SymbolicRange & R)
    {
        return R + L;
    }

    /**
     * \brief Overload of the - operator
     */
    inline friend SymbolicRange operator-(const double L, const SymbolicRange & R)
    {
        GVN::Value * const val = gvn->getValue(R);
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::MINUS, *val, *end),
                             gvn->getValue(OpValue::Kind::MINUS, *val, *start));
    }

    /**
     * \brief Overload of the * operator
     */
    inline friend SymbolicRange operator*(const double L, const SymbolicRange & R)
    {
        return R * L;
    }

    /**
     * \brief Overload of the << operator
     */
    friend inline std::wostream & operator<<(std::wostream & out, const SymbolicRange & r)
    {
        out << L"[" << *r.start->poly << L" ; " << *r.end->poly << L"]";

        return out;
    }
};

} // namespace analysis

#endif // __SYMBOLIC_RANGE_HXX__
