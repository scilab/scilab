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

#ifndef __MULTIVARIATE_MONOMIAL_HXX__
#define __MULTIVARIATE_MONOMIAL_HXX__

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "tools.hxx"
#include "VarExp.hxx"
#include "dynlib_ast.h"

namespace analysis
{

/**
 * \struct MultivariateMonomial
 * \brief Represents a multivariate monomial
 */
struct EXTERN_AST MultivariateMonomial
{
    typedef std::set<VarExp, VarExp::Compare> Monomial;

    // Since the coeff is not used to compute the hash we must set is as mutable to be able to modify it when extract
    // from an unordered_set or unordered_map.
    mutable int64_t coeff;
    Monomial monomial;

    /**
     * \brief constructor
     * \param var the default var to put in the monomial
     */
    MultivariateMonomial(const uint64_t var) : coeff(1)
    {
        monomial.emplace(var);
    }

    /**
     * \brief constructor
     * \param coeff the default coefficient for this empty monomial
     */
    MultivariateMonomial(const int64_t _coeff = 1) : coeff(_coeff) { }

    /**
     * \brief copy-constructor
     * \param mm the monomial to copy
     */
    MultivariateMonomial(const MultivariateMonomial & mm) : coeff(mm.coeff), monomial(mm.monomial) { }

    /**
     * \brief Check if a variable is contained in the monomial
     * \param var an id
     * \return true if the monomial contains the var
     */
    bool contains(const uint64_t var) const;

    /**
     * Check if the variables of the monomial have an id lower or equal to max
     * \param max an id
     * \return true if all the variables have an id leq to max
     */
    bool checkVariable(const uint64_t max) const;

    /**
     * \brief Get the sum of the exponents in the monomial
     * \return the total exponent
     */
    unsigned int exponent() const;

    /**
     * \brief Add a varexp in the monomial
     * \param ve the varexp to add
     * \return *this
     */
    MultivariateMonomial & add(const VarExp & ve);

    /**
     * \brief Add a varexp in the monomial
     * \param ve the varexp to add
     * \return *this
     */
    MultivariateMonomial & add(VarExp && ve);

    /**
     * \brief Product of two monomials
     * \param R the RHS monomial
     * \return the product of *this and R
     */
    MultivariateMonomial operator*(const MultivariateMonomial & R) const;

    /**
     * \brief Product-assignment
     */
    MultivariateMonomial & operator*=(const MultivariateMonomial & R);

    /**
     * \brief Product by a int64_t
     */
    friend MultivariateMonomial operator*(const int64_t L, const MultivariateMonomial & R);

    /**
     * \brief Product by a int64_t
     */
    MultivariateMonomial operator*(const int64_t R) const;

    /**
     * \brief Product-assignment by a int64_t
     */
    MultivariateMonomial & operator*=(const int64_t R);

    /**
     * \brief Division by a int64_t
     */
    MultivariateMonomial operator/(const int64_t R) const;

    /**
     * \brief Division-assignment by a int64_t
     */
    MultivariateMonomial & operator/=(const int64_t R);

    /**
     * \brief Exponentation by an uint
     */
    MultivariateMonomial operator^(unsigned int R) const;

    /**
     * \brief Equality
     */
    bool operator==(const MultivariateMonomial & R) const;

    /**
     * \brief Print a monomial
     * \param vars a map containing var id -> string representation
     * \return the printed monomial
     */
    const std::wstring print(const std::map<uint64_t, std::wstring> & vars) const;

    /**
     * \brief Overload of the << operator
     */
    friend std::wostream & operator<<(std::wostream & out, const MultivariateMonomial & m);

    /**
     * \struct Hash
     * \brief To be used in an unordered container
     */
    struct Hash
    {
        inline std::size_t operator()(const MultivariateMonomial & m) const
        {
            // We don't consider the coefficient of the monomial : take care it is not a bug !!
            // when we add a monomial 1.23*a*b in a polynomial we search in the unordered_set ?*a*b and
            // we add 1.23 to its coefficient
            std::size_t h = 0;
            for (const auto & ve : m.monomial)
            {
                h = tools::hash_combine(h, std::hash<uint64_t>()(ve.var), std::hash<unsigned int>()(ve.exp));
            }
            return h;
        }
    };

    /**
     * \struct Eq
     * \brief To be used in an unordered container
     */
    struct Eq
    {
        inline bool operator()(const MultivariateMonomial & L, const MultivariateMonomial & R) const
        {
            // See the comment above (to see why we ignore the coefficient)
            return L.monomial == R.monomial;
        }
    };

    /**
     * \struct Compare
     * \brief To be used in an ordered container
     */
    struct Compare
    {
        bool operator()(const MultivariateMonomial & L, const MultivariateMonomial & R) const;
    };
};

} // namespace analysis

#endif // __MULTIVARIATE_MONOMIAL_HXX__
