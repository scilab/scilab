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

#ifndef __MULTIVARIATE_POLYNOMIAL_HXX__
#define __MULTIVARIATE_POLYNOMIAL_HXX__

#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "core_math.h"
#include "tools.hxx"
#include "MultivariateMonomial.hxx"

namespace analysis
{
/**
 * \struct MultivariatePolynomial
 * \brief Represents a multivariate polynomial
 */
struct MultivariatePolynomial
{
    typedef std::unordered_set<MultivariateMonomial, MultivariateMonomial::Hash, MultivariateMonomial::Eq> Polynomial;
    int64_t constant;
    bool valid;
    Polynomial polynomial;

    /**
     * \brief constructor
     * \param var to init polynomial
     */
    MultivariatePolynomial(const uint64_t var) : constant(0), valid(true)
    {
        polynomial.emplace(var);
    }

    /**
     * \brief constructor
     * \param _constant to init polynomial
     */
    MultivariatePolynomial(const int64_t _constant = 0, const bool _valid = true) : constant(_constant), valid(_valid) { }

    /**
     * \brief constructor
     * \param _size the size of the polynomial (used to reserve the unordered_set)
     * \param _constant to init polynomial
     */
    MultivariatePolynomial(const unsigned int _size, const int64_t _constant) : constant(_constant), polynomial(_size), valid(true) { }

    /**
     * \brief copy constructor
     */
    MultivariatePolynomial(const MultivariatePolynomial & mp) : constant(mp.constant), polynomial(mp.polynomial), valid(mp.valid) { }

    /**
     * \brief Get an invalid polynomial (i.e. constant == NaN)
     * \return invalid polynomial
     */
    inline static MultivariatePolynomial getInvalid()
    {
        return MultivariatePolynomial(0, false);
    }

    /**
     * \brief Check if it is valid
     * \return true if it is valid
     */
    inline bool isValid() const
    {
        return valid;
    }

    /**
     * \brief Check if it is invalid
     * \return true if it is invalid
     */
    inline bool isInvalid() const
    {
        return !valid;
    }

    /**
     * \brief Invalidate the polynomial
     */
    inline void invalid()
    {
        constant = 0;
	valid = false;
        polynomial.clear();
    }

    /**
     * \brief Check if a variable is contained in the polynomial
     * \param var an id
     * \return true if the polynomial contains the var
     */
    inline bool contains(const uint64_t var) const
	{
	    for (const auto & m : polynomial)
	    {
		if (m.contains(var))
		{
		    return true;
		}
	    }

	    return false;
	}
    
    /**
     * \brief Check if the variables of the polynomial have an id lower or equal to max
     * \param max an id
     * \return true if all the variables have an id leq to max
     */
    inline bool checkVariable(const uint64_t max) const
    {
        for (const auto & m : polynomial)
        {
            if (!m.checkVariable(max))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Check if the variables of the polynomial have an id greater or equal to min
     * \param min an id
     * \return true if the polynomial contains a var with an id geq than min
     */
    inline bool containsVarsGEq(const uint64_t min) const
    {
        for (const auto & m : polynomial)
        {
	    if (m.monomial.lower_bound(min) != m.monomial.end())
	    {
		return true;
	    }
        }

	return false;
    }
    
    /**
     * \brief Translate the variables of the polynomial which have an id greater or equal to min
     * \param min an id
     * \return a translated polynomial
     */
    inline MultivariatePolynomial translateVariables(const uint64_t t, const uint64_t min) const
    {
	MultivariatePolynomial mp;
        for (const auto & m : polynomial)
        {
	    MultivariateMonomial mm(m);
	    MultivariateMonomial::Monomial::iterator i = mm.monomial.lower_bound(min);
	    if (i != mm.monomial.end())
	    {
		// We don't modify the order in the set, so we can const_cast
		for (MultivariateMonomial::Monomial::iterator j = std::prev(mm.monomial.end()); j != i; --j)
		{
		    const_cast<VarExp &>(*j).var += t;
		}
		const_cast<VarExp &>(*i).var += t;
	    }
	    mp.add(mm);
        }

	return mp;
    }

    /**
     * \brief Add a monomial in the polynomial
     * \param m the monomial to add
     * \param coeff the multiplcative coefficient to applicate to the monomial
     * \return *this
     */
    inline MultivariatePolynomial & add(const MultivariateMonomial & m, const int64_t coeff = 1)
    {
        const int64_t c = m.coeff * coeff;
        if (c)
        {
            Polynomial::iterator i = polynomial.find(m);
            if (i == polynomial.end())
            {
                Polynomial::iterator j = polynomial.insert(m).first;
                j->coeff = c;
            }
            else
            {
                if (i->coeff == -c)
                {
                    polynomial.erase(i);
                }
                else
                {
                    i->coeff += c;
                }
            }
        }
        return *this;
    }

    /**
     * \brief Subtract a monomial in the polynomial
     * \param m the monomial to add
     */
    inline void sub(const MultivariateMonomial & m)
    {
        Polynomial::iterator i = polynomial.find(m);
        if (i == polynomial.end())
        {
            if (m.coeff)
            {
                polynomial.insert(m).first->coeff = -m.coeff;
            }
        }
        else
        {
            if (i->coeff == m.coeff)
            {
                polynomial.erase(i);
            }
            else
            {
                i->coeff -= m.coeff;
            }
        }
    }

    /**
     * \brief Overload of the + operator
     */
    inline MultivariatePolynomial operator+(const MultivariateMonomial & R) const
    {
        if (isValid())
        {
            MultivariatePolynomial res(*this);
            res.add(R);
            return res;
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the += operator
     */
    inline MultivariatePolynomial & operator+=(const MultivariateMonomial & R)
    {
        if (isValid())
        {
            add(R);
        }
        return *this;
    }

    /**
     * \brief Overload of the + operator
     */
    inline MultivariatePolynomial operator+(const int64_t R) const
    {
        if (isValid())
        {
            MultivariatePolynomial res(*this);
            res.constant += R;
            return res;
        }
        return *this;
    }

    /**
     * \brief Overload of the += operator
     */
    inline MultivariatePolynomial & operator+=(const int64_t R)
    {
        if (isValid())
        {
            constant += R;
        }
        return *this;
    }

    /**
     * \brief Overload of the - operator
     */
    inline MultivariatePolynomial operator-(const MultivariateMonomial & R) const
    {
        if (isValid())
        {
            MultivariatePolynomial res(*this);
            res.sub(R);
            return res;
        }
        return *this;
    }

    /**
     * \brief Overload of the -= operator
     */
    inline MultivariatePolynomial & operator-=(const MultivariateMonomial & R)
    {
        if (isValid())
        {
            sub(R);
        }
        return *this;
    }

    /**
     * \brief Overload of the - operator
     */
    inline MultivariatePolynomial operator-(const int64_t R) const
    {
        if (isValid())
        {
            MultivariatePolynomial res(*this);
            res.constant -= R;
            return res;
        }
        return *this;
    }

    /**
     * \brief Overload of the - operator (unary)
     */
    inline MultivariatePolynomial operator-() const
    {
        if (isValid())
        {
            MultivariatePolynomial res(*this);
            res.constant = -res.constant;
            for (auto & m : res.polynomial)
            {
                m.coeff = -m.coeff;
            }
            return res;
        }
        return *this;
    }

    /**
     * \brief Overload of the -= operator
     */
    inline MultivariatePolynomial & operator-=(const int64_t R)
    {
        if (isValid())
        {
            constant -= R;
        }
        return *this;
    }

    /**
     * \brief Overload of the + operator
     */
    inline MultivariatePolynomial operator+(const MultivariatePolynomial & R) const
    {
        if (isValid() && R.isValid())
        {
            MultivariatePolynomial res(*this);
            res.constant += R.constant;
            for (const auto & m : R.polynomial)
            {
                res.add(m);
            }
            return res;
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the += operator
     */
    inline MultivariatePolynomial & operator+=(const MultivariatePolynomial & R)
    {
        if (isValid() && R.isValid())
        {
            constant += R.constant;
            for (const auto & m : R.polynomial)
            {
                add(m);
            }
        }
        else
        {
            invalid();
        }

        return *this;
    }

    /**
     * \brief Overload of the - operator
     */
    inline MultivariatePolynomial operator-(const MultivariatePolynomial & R) const
    {
        if (isValid() && R.isValid())
        {
            MultivariatePolynomial res(*this);
            res.constant -= R.constant;
            for (const auto & m : R.polynomial)
            {
                res.sub(m);
            }
            return res;
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the -= operator
     */
    inline MultivariatePolynomial & operator-=(const MultivariatePolynomial & R)
    {
        if (isValid() && R.isValid())
        {
            constant -= R.constant;
            for (const auto & m : R.polynomial)
            {
                sub(m);
            }
        }
        else
        {
            invalid();
        }
        return *this;
    }

    /**
     * \brief Overload of the * operator
     */
    inline MultivariatePolynomial operator*(const MultivariatePolynomial & R) const
    {
        if (isValid() && R.isValid())
        {
            if (R.isConstant())
            {
                return *this * R.constant;
            }
            else if (isConstant())
            {
                return R * constant;
            }
            else
            {
                MultivariatePolynomial res(static_cast<unsigned int>((polynomial.size() + 1) * (R.polynomial.size() + 1) - 1), constant * R.constant);
                for (const auto & mR : R.polynomial)
                {
                    res.add(mR, constant);
                }
                for (const auto & mL : polynomial)
                {
                    res.add(mL, R.constant);
                    for (const auto & mR : R.polynomial)
                    {
                        res.add(mL * mR);
                    }
                }
                return res;
            }
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the / operator
     */
    inline MultivariatePolynomial operator/(const MultivariatePolynomial & R) const
    {
        if (isValid() && R.isValid())
        {
            if (R.isConstant())
            {
                if (R.constant != 1)
                {
                    return *this / R.constant;
                }
                else
                {
                    return *this;
                }
            }
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the /= operator
     */
    inline MultivariatePolynomial & operator/=(const MultivariatePolynomial & R)
    {
        if (isValid() && R.isValid())
        {
            if (R.polynomial.empty())
            {
                constant /= R.constant;
                for (auto & m : polynomial)
                {
                    m.coeff /= R.constant;
                }
            }
            else
            {
                MultivariatePolynomial res(*this / R);
                polynomial = res.polynomial;
                constant = res.constant;
            }
        }
        else
        {
            invalid();
        }
        return *this;
    }

    /**
     * \brief Overload of the *= operator
     */
    inline MultivariatePolynomial & operator*=(const MultivariatePolynomial & R)
    {
        if (isValid() && R.isValid())
        {
            if (R.polynomial.empty())
            {
                constant *= R.constant;
                for (auto & m : polynomial)
                {
                    m.coeff *= R.constant;
                }
            }
            else
            {
                MultivariatePolynomial res(*this * R);
                polynomial = res.polynomial;
                constant = res.constant;
            }
        }
        else
        {
            invalid();
        }
        return *this;
    }

    /**
     * \brief Overload of the * operator
     */
    inline MultivariatePolynomial operator*(const MultivariateMonomial & R) const
    {
        if (isValid())
        {
            MultivariatePolynomial res(static_cast<unsigned int>(polynomial.size() + 1), int64_t(0));
            res.add(constant * R);
            for (const auto & mL : polynomial)
            {
                res.add(mL * R);
            }
            return res;
        }
        else
        {
            return getInvalid();
        }
    }

    /**
     * \brief Overload of the *= operator
     */
    inline MultivariatePolynomial & operator*=(const MultivariateMonomial & R)
    {
        if (isValid())
        {
            MultivariatePolynomial res = *this * R;
            polynomial = res.polynomial;
            constant = res.constant;
        }
        return *this;
    }

    /**
     * \brief Overload of the * operator
     */
    inline MultivariatePolynomial operator*(const int64_t R) const
    {
        if (isValid())
        {
            if (R)
            {
                if (R == 1)
                {
                    return *this;
                }
                else
                {
                    MultivariatePolynomial res(*this);
                    res.constant *= R;
                    for (auto & m : res.polynomial)
                    {
                        m.coeff *= R;
                    }
                    return res;
                }
            }
            else
            {
                return MultivariatePolynomial(int64_t(0));
            }
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the *= operator
     */
    inline MultivariatePolynomial & operator*=(const int64_t R)
    {
        if (isValid())
        {
            if (R == 0)
            {
                constant = 0;
                polynomial.clear();
            }
            else if (R != 1)
            {
                constant *= R;
                for (auto & m : polynomial)
                {
                    m.coeff *= R;
                }
            }
        }
        return *this;
    }

    /**
     * \brief Overload of the / operator
     */
    inline MultivariatePolynomial operator/(const int64_t R) const
    {
        if (isValid())
        {
            if (R != 1)
            {
                MultivariatePolynomial res(*this);
                res.constant /= R;
                for (auto & m : res.polynomial)
                {
                    m.coeff /= R;
                }
                return res;
            }
        }
        return *this;
    }

    /**
     * \brief Overload of the /= operator
     */
    inline MultivariatePolynomial & operator/=(const int64_t R)
    {
        if (isValid())
        {
            if (R != 1)
            {
                constant /= R;
                for (auto & m : polynomial)
                {
                    m.coeff /= R;
                }
            }
        }
        return *this;
    }

    /**
     * \brief Overload of the ^ operator (exponentiation)
     */
    inline MultivariatePolynomial operator^(unsigned int R) const
    {
        if (isValid())
        {
            if (R == 0)
            {
                return MultivariatePolynomial(int64_t(1));
            }
            else if (R == 1)
            {
                return *this;
            }
            else
            {
                if (constant == 0)
                {
                    if (polynomial.empty())
                    {
                        return MultivariatePolynomial(int64_t(0));
                    }
                    else if (polynomial.size() == 1)
                    {
                        const MultivariateMonomial & m = *polynomial.begin();
                        MultivariatePolynomial res;
                        res.polynomial.emplace(m ^ R);

                        return res;
                    }
                }

                if (polynomial.empty())
                {
                    return MultivariatePolynomial(tools::powui(constant, R));
                }

                MultivariatePolynomial p = *this;
                MultivariatePolynomial y = (R & 1) ? *this : MultivariatePolynomial(int64_t(1));

                while (R >>= 1)
                {
                    p *= p;
                    if (R & 1)
                    {
                        y *= p;
                    }
                }

                return y;
            }
        }
        return getInvalid();
    }

    /**
     * \brief Overload of the ^ operator (exponentiation)
     */
    inline MultivariatePolynomial operator^(const MultivariatePolynomial & R) const
    {
        if (isValid() && R.isValid())
        {
            if (R.isConstant() && R.constant == (unsigned int)R.constant)
            {
                return (*this) ^ ((unsigned int)R.constant);
            }
        }
        return getInvalid();
    }

    /**
     * \brief Evaluate a polynomial
     * \param values an unordered_map<ULL, const MultivariatePolynomial *> containing mapping between var number and polynomial
     *        or a std::vector<const MultivariatePolynomial *> (0->vector[0], 1->vector[1], ...)
     * \return the result of the evaluation
     */
    template<typename T>
    inline MultivariatePolynomial eval(T && values) const
    {
        if (isInvalid())
        {
            return getInvalid();
        }
        if (!MultivariatePolynomial::__isValid(values))
        {
            return getInvalid();
        }

        std::unordered_map<uint64_t, std::set<unsigned int>> expected_exps;
        for (const auto & m : polynomial)
        {
            for (const auto & ve : m.monomial)
            {
                if (ve.exp >= 2 && MultivariatePolynomial::__contains(values, ve.var))
                {
                    expected_exps[ve.var].emplace(ve.exp);
                }
            }
        }

        std::unordered_map<uint64_t, std::unordered_map<unsigned int, MultivariatePolynomial>> exps;
        for (const auto & p : expected_exps)
        {
            if (p.second.size() == 1)
            {
                const unsigned int expo = *p.second.begin();
                auto & map = exps.emplace(p.first, std::unordered_map<unsigned int, MultivariatePolynomial>()).first->second;
                map.emplace(expo, (*__getSafe(values, p.first)) ^ expo);
            }
            else
            {
                unsigned int estim = 0;
                for (const auto i : p.second)
                {
                    estim += tools::popcount(i) + tools::log2(i) - 1;
                }
                unsigned int max = *std::prev(p.second.end());
                auto & map = exps.emplace(p.first, std::unordered_map<unsigned int, MultivariatePolynomial>()).first->second;

                // if we have p^3, p^5, p^6 and p^9 to compute:
                //   i) cost of p^3 is 2 mults
                //   ii) cost of p^5 is 3 mults
                //   iii) cost of p^6 is 3 mults
                //   iv) cost of p^9 is 4 mults
                //   v) total cost is 12
                //   vi) 12 > 9 so we compute p^2, p^3, p^4, ..., p^9 and we retains only the exponents 3, 5, 6, 9
                // if we just have p^3, p^9:
                //   i) total cost is 6
                //   ii) we compute p^3 and p^9 separatly in using fast exponentiation
                if (estim > max)
                {
                    MultivariatePolynomial mp(*__getSafe(values, p.first));
                    auto it = p.second.begin();
                    for (unsigned int i = 2; i <= max; ++i)
                    {
                        mp *= *__getSafe(values, p.first);
                        if (i == *it)
                        {
                            map.emplace(i, mp);
                            ++it;
                        }
                    }
                }
                else
                {
                    for (const auto expo : p.second)
                    {
                        map.emplace(expo, (*__getSafe(values, p.first)) ^ expo);
                    }
                }
            }
        }

        MultivariatePolynomial res(constant);
        for (const auto & m : polynomial)
        {
            MultivariatePolynomial r(m.coeff);
            for (const auto & ve : m.monomial)
            {
                const MultivariatePolynomial * mp = MultivariatePolynomial::__get(values, ve.var);
                if (mp)
                {
                    if (ve.exp == 1)
                    {
                        r *= *mp;
                    }
                    else if (ve.exp > 1)
                    {
                        r *= exps[ve.var][ve.exp];
                    }
                }
                else
                {
                    MultivariateMonomial mm(int64_t(1));
                    r *= mm.add(ve);
                }
            }
            res += r;
        }

        return res;
    }

    /**
     * \brief Check divisibility by an integer
     * \return true if all the coeffs are divisible by n
     */
    inline bool isDivisibleBy(const int64_t n) const
    {
        if (constant % n == 0)
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff % n != 0)
                {
		    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Check divisibility by a polynomial
     * For now the polynomial must be constant.
     * \return true if this is divisible by mp
     */
    inline bool isDivisibleBy(const MultivariatePolynomial & mp) const
    {
	if (mp.polynomial.empty())
	{
	    return isDivisibleBy(mp.constant);
	}

	return false;
    }
    
    /**
     * \brief Check positivity
     * \return true if all the coeffs are positive and the exponents are even
     */
    inline bool isPositive() const
    {
        if (constant >= 0)
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff >= 0)
                {
                    for (const auto & ve : m.monomial)
                    {
                        if (ve.exp % 2) // exp is odd
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Check strict positivity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are strict positive
     */
    inline bool isCoeffStrictPositive(const bool checkConstant = true) const
    {
        if (!checkConstant || (constant > 0))
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff <= 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Check positivity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are positive
     */
    inline bool isCoeffPositive(const bool checkConstant = true) const
    {
        if (!checkConstant || (constant >= 0))
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff < 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Check strict negativity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are strict negative
     */
    inline bool isCoeffStrictNegative(const bool checkConstant = true) const
    {
        if (!checkConstant || (constant < 0))
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff >= 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Check negativity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are negative
     */
    inline bool isCoeffNegative(const bool checkConstant = true) const
    {
        if (!checkConstant || (constant <= 0))
        {
            for (const auto & m : polynomial)
            {
                if (m.coeff > 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * \brief Helper function to print a polynomial
     * \param vars a mapping between vars numbers and wstring representation
     * \return a wstring representing this polynomial
     */
    inline const std::wstring print(const std::map<uint64_t, std::wstring> & vars) const
    {
        std::wostringstream wos;
        wos << constant;
        std::set<MultivariateMonomial, MultivariateMonomial::Compare> s(polynomial.begin(), polynomial.end());
        for (const auto & m : s)
        {
            wos << L" + " << m.print(vars);
        }
        return wos.str();
    }

    /**
     * \brief Overload of << operator
     */
    friend inline std::wostream & operator<<(std::wostream & out, const MultivariatePolynomial & p)
    {
        const std::map<uint64_t, std::wstring> vars;
        out << p.constant;
        std::set<MultivariateMonomial, MultivariateMonomial::Compare> s(p.polynomial.begin(), p.polynomial.end());
        for (const auto & m : s)
        {
            out << L" + " << m.print(vars);
        }
        return out;
    }

    /**
     * \return true if the two polynomials are differing only by a constant
     */
    inline bool isDiffConstant(const MultivariatePolynomial & R) const
    {
        return polynomial == R.polynomial;
    }

    /**
     * \return true if the polynomial is constant
     */
    inline bool isConstant() const
    {
        return polynomial.empty();
    }

    /**
     * \brief Check if a polynomial is constant and equal to a value
     * \param val the constant value to check
     * \return true if the polynomial is constant and equal to val
     */
    inline bool isConstant(const int64_t val) const
    {
        return isConstant() && constant == val;
    }

    /**
     * \brief Get the common coeff for all the monomials
     * \param[out] common the common value
     * \return true if there is a common coeff
     */
    inline bool getCommonCoeff(int64_t & common) const
    {
        if (constant != 0)
        {
            return false;
        }
        if (polynomial.empty())
        {
            common = constant;
            return true;
        }

        common = polynomial.begin()->coeff;
        for (Polynomial::const_iterator i = std::next(polynomial.begin()), e = polynomial.end(); i != e; ++i)
        {
            if (i->coeff != common)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Overload of == operator
     */
    inline bool operator==(const MultivariatePolynomial & R) const
    {
        return constant == R.constant && polynomial == R.polynomial;
    }

    /**
     * \brief Overload of != operator
     */
    inline bool operator!=(const MultivariatePolynomial & R) const
    {
        return !(*this == R);
    }

    /**
     * \brief Overload of == operator
     */
    inline bool operator==(const int64_t R) const
    {
        return polynomial.empty() && constant == R;
    }

    /**
     * \brief Overload of != operator
     */
    inline bool operator!=(const int64_t R) const
    {
        return !(*this == R);
    }

    /**
     * \brief Overload of == operator
     */
    friend inline bool operator==(const int64_t L, const MultivariatePolynomial & R)
    {
        return R == L;
    }

    /**
     * \brief Overload of != operator
     */
    friend inline bool operator!=(const int64_t L, const MultivariatePolynomial & R)
    {
        return R != L;
    }

    /**
     * \return a hash
     */
    inline std::size_t hash() const
    {
        std::size_t h = std::hash<int64_t>()(constant);
        for (const auto & m : polynomial)
        {
            // since the order of the monomials is not always the same
            // we must use a commutative operation to combine the monomial's hashes
            h += tools::hash_combine(std::hash<int64_t>()(m.coeff), MultivariateMonomial::Hash()(m));
        }

        return h;
    }

    /**
     * \struct Hash
     * \brief To be used in an unordered container
     */
    struct Hash
    {
        inline std::size_t operator()(const MultivariatePolynomial & mp) const
        {
            return mp.hash();
        }
    };

    /**
     * \struct Eq
     * \brief To be used in an unordered container
     */
    struct Eq
    {
        inline bool operator()(const MultivariatePolynomial & L, const MultivariatePolynomial & R) const
        {
            return L == R;
        }
    };

private:

    // Helper function to use with eval
    inline static bool __isValid(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values)
    {
        for (const auto & p : values)
        {
            if (p.second->isInvalid())
            {
                return false;
            }
        }
        return true;
    }

    // Helper function to use with eval
    inline static bool __isValid(const std::vector<const MultivariatePolynomial *> & values)
    {
        for (const auto & v : values)
        {
            if (v->isInvalid())
            {
                return false;
            }
        }
        return true;
    }

    // Helper function to use with eval
    inline static bool __isValid(const std::pair<uint64_t, const MultivariatePolynomial *> & values)
    {
	return values.second->isValid();
    }

    // Helper function to use with eval
    inline static bool __contains(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
        return values.find(val) != values.end();
    }

    // Helper function to use with eval
    inline static bool __contains(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
    {
        return val < values.size();
    }

    // Helper function to use with eval
    inline static bool __contains(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
        return values.first == val;
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __get(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
        const auto i = values.find(val);
        if (i != values.end())
        {
            return i->second;
        }
        return nullptr;
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __getSafe(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
	return values.find(val)->second;
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __get(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
    {
	if (val < values.size())
	{
	    return values[val];
	}
	return nullptr;
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __getSafe(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
    {
        return values[val];
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __get(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
	if (values.first == val)
	{
	    return values.second;
	}
        return nullptr;
    }

    // Helper function to use with eval
    inline static const MultivariatePolynomial * __getSafe(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
    {
	return values.second;
    }
};

} // namespace analysis

#endif // __MULTIVARIATE_POLYNOMIAL_HXX__
