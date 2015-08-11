/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MULTIVARIATE_MONOMIAL_HXX__
#define __MULTIVARIATE_MONOMIAL_HXX__

#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "tools.hxx"
#include "VarExp.hxx"

namespace analysis
{

/**
 * \struct MultivariateMonomial
 * \brief Represents a multivariate monomial
 */
struct MultivariateMonomial
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
    inline bool contains(const uint64_t var) const
	{
	    return monomial.find(var) != monomial.end();
	}
    
    /**
     * Check if the variables of the monomial have an id lower or equal to max
     * \param max an id
     * \return true if all the variables have an id leq to max
     */
    inline bool checkVariable(const uint64_t max) const
    {
	return std::prev(monomial.end())->var <= max;
    }

    /**
     * \brief Get the sum of the exponents in the monomial
     * \return the total exponent
     */
    inline unsigned int exponent() const
    {
        unsigned int exp = 0;
        for (const auto & ve : monomial)
        {
            exp += ve.exp;
        }
        return exp;
    }

    /**
     * \brief Add a varexp in the monomial
     * \param ve the varexp to add
     * \return *this
     */
    inline MultivariateMonomial & add(const VarExp & ve)
    {
        Monomial::iterator i = monomial.find(ve);
        if (i == monomial.end())
        {
            monomial.insert(ve);
        }
        else
        {
            i->exp += ve.exp;
        }
        return *this;
    }

    /**
     * \brief Add a varexp in the monomial
     * \param ve the varexp to add
     * \return *this
     */
    inline MultivariateMonomial & add(VarExp && ve)
    {
        Monomial::iterator i = monomial.find(ve);
        if (i == monomial.end())
        {
            monomial.emplace(std::move(ve));
        }
        else
        {
            i->exp += ve.exp;
        }
        return *this;
    }

    /**
     * \brief Product of two monomials
     * \param R the RHS monomial
     * \return the product of *this and R
     */
    inline MultivariateMonomial operator*(const MultivariateMonomial & R) const
    {
        MultivariateMonomial res(*this);
        res.coeff *= R.coeff;
        for (const auto & ve : R.monomial)
        {
            res.add(ve);
        }
        return res;
    }

    /**
     * \brief Product-assignment
     */
    inline MultivariateMonomial & operator*=(const MultivariateMonomial & R)
    {
        coeff *= R.coeff;
        for (const auto & ve : R.monomial)
        {
            add(ve);
        }
        return *this;
    }

    /**
     * \brief Product by a int64_t
     */
    friend inline MultivariateMonomial operator*(const int64_t L, const MultivariateMonomial & R)
    {
        return R * L;
    }

    /**
     * \brief Product by a int64_t
     */
    inline MultivariateMonomial operator*(const int64_t R) const
    {
        MultivariateMonomial res(*this);
        res.coeff *= R;
        return res;
    }

    /**
     * \brief Product-assignment by a int64_t
     */
    inline MultivariateMonomial & operator*=(const int64_t R)
    {
        coeff *= R;
        return *this;
    }

    /**
     * \brief Division by a int64_t
     */
    inline MultivariateMonomial operator/(const int64_t R) const
    {
        MultivariateMonomial res(*this);
        res.coeff /= R;
        return res;
    }

    /**
     * \brief Division-assignment by a int64_t
     */
    inline MultivariateMonomial & operator/=(const int64_t R)
    {
        coeff /= R;
        return *this;
    }

    /**
     * \brief Exponentation by an uint
     */
    inline MultivariateMonomial operator^(unsigned int R) const
    {
        MultivariateMonomial res(*this);
        if (R > 1)
        {
            coeff = std::pow(coeff, R);
            for (auto & ve : res.monomial)
            {
                ve.exp *= R;
            }
        }
        return res;
    }

    /**
     * \brief Equality
     */
    inline bool operator==(const MultivariateMonomial & R) const
    {
        return coeff == R.coeff && monomial == R.monomial;
    }

    /**
     * \brief Print a monomial
     * \param vars a map containing var id -> string representation
     * \return the printed monomial
     */
    inline const std::wstring print(const std::map<uint64_t, std::wstring> & vars) const
    {
        std::wostringstream wos;
        wos << coeff;
        for (const auto & ve : monomial)
        {
            wos << L"*" << ve.print(vars);
        }
        return wos.str();
    }

    /**
     * \brief Overload of the << operator
     */
    friend inline std::wostream & operator<<(std::wostream & out, const MultivariateMonomial & m)
    {
        out << m.coeff;
        for (const auto & ve : m.monomial)
        {
            out << L"*" << ve;
        }
        return out;
    }

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
        inline bool operator()(const MultivariateMonomial & L, const MultivariateMonomial & R) const
        {
            const unsigned int le = L.exponent();
            const unsigned int re = R.exponent();
            if (le < re)
            {
                return true;
            }
            else if (le == re)
            {
                const unsigned int ls = static_cast<unsigned int>(L.monomial.size());
                const unsigned int rs = static_cast<unsigned int>(R.monomial.size());
                if (ls > rs)
                {
                    return true;
                }
                else if (ls == rs)
                {
                    for (Monomial::const_iterator i = L.monomial.begin(), j = R.monomial.begin(), e = L.monomial.end(); i != e; ++i, ++j)
                    {
                        if (VarExp::Compare()(*i, *j))
                        {
                            return true;
                        }
                        else if (!VarExp::Eq()(*i, *j))
                        {
                            return false;
                        }
                    }

                    for (Monomial::const_iterator i = L.monomial.begin(), j = R.monomial.begin(), e = L.monomial.end(); i != e; ++i, ++j)
                    {
                        if (i->exp < j->exp)
                        {
                            return true;
                        }
                        else if (i->exp > j->exp)
                        {
                            return false;
                        }
                    }

                }
            }
            return false;
        }
    };
};

} // namespace analysis

#endif // __MULTIVARIATE_MONOMIAL_HXX__
