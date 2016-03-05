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

#ifndef __OPVALUE_HXX__
#define __OPVALUE_HXX__

#include <iostream>

#include "tools.hxx"

namespace analysis
{

/**
 * \struct OpValue
 * \brief OpValue represents an operation between one or two operands
 *
 * This is struct is mainly used by the GVN.
 */
struct OpValue
{
    enum Kind : uint8_t
    {
        UNARYMINUS = 0, UNARYNEG, PLUS, MINUS, TIMES, DOTTIMES, RDIV, DOTRDIV, POWER, DOTPOWER
    };
    const Kind kind;
    uint64_t lnum : 60;
    uint64_t rnum : 60;

    /**
     * \brief constructor for unary operation
     * \param _kind the operation kind
     * \param _lnum the value of the operand
     */
    OpValue(Kind _kind, uint64_t _lnum) : kind(_kind), lnum(_lnum), rnum(0) { }

    /**
     * \brief constructor for binary operation
     * \param _kind the operation kind
     * \param _lnum the value of the left operand
     * \param _rnum the value of the right operand
     */
    OpValue(Kind _kind, uint64_t _lnum, uint64_t _rnum) : kind(_kind), lnum(_lnum), rnum(_rnum)
    {
        if (isCommutative() && lnum > rnum)
        {
            const uint64_t x = lnum;
            lnum = rnum;
            rnum = x;
        }
    }

    /**
     * \brief Check if the operation is commutative
     * \return true if the operation is commutative
     */
    inline bool isCommutative() const
    {
        return kind == PLUS || kind == TIMES || kind == DOTTIMES;
    }

    /**
     * \brief Check if the operation is unary
     * \return true if the operation is unary
     */
    inline bool isUnary() const
    {
        return kind == UNARYMINUS || kind == UNARYNEG;
    }

    /**
     * \brief Compute the hash
     * \return the hash
     */
    inline std::size_t hash() const
    {
        return tools::hash_combine(kind, lnum, rnum);
    }

    /**
     * \brief Overload of the operator ==
     */
    inline bool operator==(const OpValue & R) const
    {
        if (kind == R.kind)
        {
            if (isUnary())
            {
                return lnum == R.lnum;
            }
            else
            {
                return lnum == R.lnum && rnum == R.rnum;
            }
        }
        return false;
    }

    /**
     * \brief Overload of the operator <<
     */
    friend std::wostream & operator<<(std::wostream & out, const OpValue & ov);

    /**
     * \struct Hash
     * \brief Helper struct to be used in unordered_map
     */
    struct Hash
    {
        inline std::size_t operator()(const OpValue & ov) const
        {
            return ov.hash();
        }
    };

    /**
     * \struct Eq
     * \brief Helper struct to be used in unordered_map
     */
    struct Eq
    {
        inline bool operator()(const OpValue & L, const OpValue & R) const
        {
            return L == R;
        }
    };
};

} // namespace analysis

#endif // __OPVALUE_HXX__
