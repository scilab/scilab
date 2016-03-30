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

public:

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
    SymbolicRange(GVN * _gvn, GVN::Value * _start, GVN::Value * _end) : gvn(_gvn), start(_start), end(_end) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicRange(GVN & _gvn, GVN::Value * _start, GVN::Value * _end) : SymbolicRange(&_gvn, _start, _end) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicRange(GVN & _gvn, int64_t _start, int64_t _end) : SymbolicRange(&_gvn, _gvn.getValue(_start), _gvn.getValue(_end)) { }

    inline void set(GVN & _gvn, GVN::Value * _start, GVN::Value * _end)
    {
        gvn = &_gvn;
        start = _start;
        end = _end;
    }

    inline GVN::Value * getStart() const
    {
        return start;
    }

    inline GVN::Value * getEnd() const
    {
        return end;
    }

    /**
     * \brief Get the associated GVN
     * \return the GVN
     */
    inline GVN * getGVN()
    {
        return gvn;
    }

    /**
     * \brief Check if this range is valid
     * \return true if valid
     */
    inline bool isValid() const
    {
        return gvn != nullptr;
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
    inline SymbolicRange operator+(const GVN::Value & R) const
    {
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::PLUS, *start, R),
                             gvn->getValue(OpValue::Kind::PLUS, *end, R));
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
    inline SymbolicRange operator-(const GVN::Value & R) const
    {
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::MINUS, *start, R),
                             gvn->getValue(OpValue::Kind::MINUS, *end, R));
    }

    /**
     * \brief Overload of the - operator
     */
    friend inline SymbolicRange operator-(const GVN::Value & L, const SymbolicRange & R)
    {
        return SymbolicRange(R.gvn,
                             R.gvn->getValue(OpValue::Kind::MINUS, L, *R.end),
                             R.gvn->getValue(OpValue::Kind::MINUS, L, *R.start));
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
    inline SymbolicRange operator*(const GVN::Value & R) const
    {
        // We suppose that all the values are positive or null
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::TIMES, *start, R),
                             gvn->getValue(OpValue::Kind::TIMES, *end, R));
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
    inline SymbolicRange operator/(const GVN::Value & R) const
    {
        // We suppose that all the values are positive or null
        return SymbolicRange(gvn,
                             gvn->getValue(OpValue::Kind::RDIV, *start, R),
                             gvn->getValue(OpValue::Kind::RDIV, *end, R));
    }

    /**
     * \brief Overload of the - operator
     */
    friend inline SymbolicRange operator/(const GVN::Value & L, const SymbolicRange & R)
    {
        return SymbolicRange(R.gvn,
                             R.gvn->getValue(OpValue::Kind::RDIV, L, *R.end),
                             R.gvn->getValue(OpValue::Kind::RDIV, L, *R.start));
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
