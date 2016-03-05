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

#ifndef __VAR_DEF_HXX__
#define __VAR_DEF_HXX__

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <stdint.h>

namespace analysis
{

/**
 * \struct VarExp
 * \brief Represents a variable with an exponent
 *
 * This struct is mainly used in MultivariateMonomial.
 */
struct VarExp
{
    uint64_t var;

    // this field is mutable since it is not used in the hash computation
    // and could me modified.
    mutable unsigned int exp;

    /**
     * \brief constructor
     * \param _var the var number
     * \param _exp the exponent
     */
    VarExp(uint64_t _var, unsigned int _exp = 1) : var(_var), exp(_exp) { }

    /**
     * \brief Print this VarExp
     * \param vars a map containing association between var number and wstring
     * \return a wstring containing the representation of this VarExp
     */
    const std::wstring print(const std::map<uint64_t, std::wstring> & vars) const;

    /**
     * \brief Overload of the operator <<
     */
    friend std::wostream & operator<<(std::wostream & out, const VarExp & ve);

    /**
     * \brief Overload of the operator <<
     */
    inline bool operator==(const VarExp & R) const
    {
        return var == R.var && exp == R.exp;
    }

    /**
     * \struct Hash
     * \brief Helper struct to be used in unordered container
     */
    struct Hash
    {
        inline std::size_t operator()(const VarExp & ve) const
        {
            return ve.var;
        }
    };

    /**
     * \struct Eq
     * \brief Helper struct to be used in unordered container
     */
    struct Eq
    {
        inline bool operator()(const VarExp & L, const VarExp & R) const
        {
            return L.var == R.var;
        }
    };

    /**
     * \struct Compare
     * \brief Helper struct to be used in map or set
     */
    struct Compare
    {
        inline bool operator()(const VarExp & L, const VarExp & R) const
        {
            return L.var < R.var;
        }
    };
};

} // namespace analysis

#endif // __VAR_DEF_HXX__
