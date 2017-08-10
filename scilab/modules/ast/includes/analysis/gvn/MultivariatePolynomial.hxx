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

#ifndef __MULTIVARIATE_POLYNOMIAL_HXX__
#define __MULTIVARIATE_POLYNOMIAL_HXX__

#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core_math.h"
#include "tools.hxx"
#include "MultivariateMonomial.hxx"
#include "dynlib_ast.h"

namespace analysis
{
/**
 * \struct MultivariatePolynomial
 * \brief Represents a multivariate polynomial
 */
struct EXTERN_AST MultivariatePolynomial
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
    MultivariatePolynomial(const unsigned int _size, const int64_t _constant) : constant(_constant), valid(true), polynomial(_size) { }

    /**
     * \brief copy constructor
     */
    MultivariatePolynomial(const MultivariatePolynomial & mp) : constant(mp.constant), valid(mp.valid), polynomial(mp.polynomial) { }

    /**
     * \brief Get an invalid polynomial (i.e. constant == NaN)
     * \return invalid polynomial
     */
    static MultivariatePolynomial getInvalid();

    /**
     * \brief Check if it is valid
     * \return true if it is valid
     */
    bool isValid() const;

    /**
     * \brief Check if it is invalid
     * \return true if it is invalid
     */
    bool isInvalid() const;

    /**
     * \brief Invalidate the polynomial
     */
    void invalid();

    /**
     * \brief Check if a variable is contained in the polynomial
     * \param var an id
     * \return true if the polynomial contains the var
     */
    bool contains(const uint64_t var) const;

    /**
     * \brief Check if the variables of the polynomial have an id lower or equal to max
     * \param max an id
     * \return true if all the variables have an id leq to max
     */
    bool checkVariable(const uint64_t max) const;

    /**
     * \brief Check if the variables of the polynomial have an id greater or equal to min
     * \param min an id
     * \return true if the polynomial contains a var with an id geq than min
     */
    bool containsVarsGEq(const uint64_t min) const;

    /**
     * \brief Translate the variables of the polynomial which have an id greater or equal to min
     * \param min an id
     * \return a translated polynomial
     */
    MultivariatePolynomial translateVariables(const uint64_t t, const uint64_t min) const;

    /**
     * \brief Add a monomial in the polynomial
     * \param m the monomial to add
     * \param coeff the multiplcative coefficient to applicate to the monomial
     * \return *this
     */
    MultivariatePolynomial & add(const MultivariateMonomial & m, const int64_t coeff = 1);

    /**
     * \brief Subtract a monomial in the polynomial
     * \param m the monomial to add
     */
    void sub(const MultivariateMonomial & m);

    /**
     * \brief Overload of the + operator
     */
    MultivariatePolynomial operator+(const MultivariateMonomial & R) const;

    /**
     * \brief Overload of the += operator
     */
    MultivariatePolynomial & operator+=(const MultivariateMonomial & R);

    /**
     * \brief Overload of the + operator
     */
    MultivariatePolynomial operator+(const int64_t R) const;

    /**
     * \brief Overload of the += operator
     */
    MultivariatePolynomial & operator+=(const int64_t R);

    /**
     * \brief Overload of the - operator
     */
    MultivariatePolynomial operator-(const MultivariateMonomial & R) const;

    /**
     * \brief Overload of the -= operator
     */
    MultivariatePolynomial & operator-=(const MultivariateMonomial & R);

    /**
     * \brief Overload of the - operator
     */
    MultivariatePolynomial operator-(const int64_t R) const;

    /**
     * \brief Overload of the - operator (unary)
     */
    MultivariatePolynomial operator-() const;

    /**
     * \brief Overload of the -= operator
     */
    MultivariatePolynomial & operator-=(const int64_t R);

    /**
     * \brief Overload of the + operator
     */
    MultivariatePolynomial operator+(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of the += operator
     */
    MultivariatePolynomial & operator+=(const MultivariatePolynomial & R);

    /**
     * \brief Overload of the - operator
     */
    MultivariatePolynomial operator-(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of the -= operator
     */
    MultivariatePolynomial & operator-=(const MultivariatePolynomial & R);

    /**
     * \brief Overload of the * operator
     */
    MultivariatePolynomial operator*(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of the / operator
     */
    MultivariatePolynomial operator/(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of the /= operator
     */
    MultivariatePolynomial & operator/=(const MultivariatePolynomial & R);

    /**
     * \brief Overload of the *= operator
     */
    MultivariatePolynomial & operator*=(const MultivariatePolynomial & R);

    /**
     * \brief Overload of the * operator
     */
    MultivariatePolynomial operator*(const MultivariateMonomial & R) const;

    /**
     * \brief Overload of the *= operator
     */
    MultivariatePolynomial & operator*=(const MultivariateMonomial & R);

    /**
     * \brief Overload of the * operator
     */
    MultivariatePolynomial operator*(const int64_t R) const;

    /**
     * \brief Overload of the *= operator
     */
    MultivariatePolynomial & operator*=(const int64_t R);

    /**
     * \brief Overload of the / operator
     */
    MultivariatePolynomial operator/(const int64_t R) const;

    /**
     * \brief Overload of the /= operator
     */
    MultivariatePolynomial & operator/=(const int64_t R);

    /**
     * \brief Overload of the ^ operator (exponentiation)
     */
    MultivariatePolynomial operator^(unsigned int R) const;

    /**
     * \brief Overload of the ^ operator (exponentiation)
     */
    MultivariatePolynomial operator^(const MultivariatePolynomial & R) const;

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
    bool isDivisibleBy(const int64_t n) const;

    /**
     * \brief Check divisibility by a polynomial
     * For now the polynomial must be constant.
     * \return true if this is divisible by mp
     */
    bool isDivisibleBy(const MultivariatePolynomial & mp) const;

    /**
     * \brief Check positivity
     * \return true if all the coeffs are positive and the exponents are even
     */
    bool isPositive() const;

    /**
     * \brief Check strict positivity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are strict positive
     */
    bool isCoeffStrictPositive(const bool checkConstant = true) const;

    /**
     * \brief Check positivity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are positive
     */
    bool isCoeffPositive(const bool checkConstant = true) const;

    /**
     * \brief Check strict negativity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are strict negative
     */
    bool isCoeffStrictNegative(const bool checkConstant = true) const;

    /**
     * \brief Check negativity
     * \param checkConstant if true the constant is checked too
     * \return true if all the coeffs are negative
     */
    bool isCoeffNegative(const bool checkConstant = true) const;

    /**
     * \brief Helper function to print a polynomial
     * \param vars a mapping between vars numbers and wstring representation
     * \return a wstring representing this polynomial
     */
    const std::wstring print(const std::map<uint64_t, std::wstring> & vars) const;

    /**
     * \brief Overload of << operator
     */
    friend std::wostream & operator<<(std::wostream & out, const MultivariatePolynomial & p);

    /**
     * \return true if the two polynomials are differing only by a constant
     */
    bool isDiffConstant(const MultivariatePolynomial & R) const;

    /**
     * \return true if the polynomial is constant
     */
    bool isConstant() const;

    /**
     * \brief Check if a polynomial is constant and equal to a value
     * \param val the constant value to check
     * \return true if the polynomial is constant and equal to val
     */
    bool isConstant(const int64_t val) const;

    /**
     * \brief Get the common coeff for all the monomials
     * \param[out] common the common value
     * \return true if there is a common coeff
     */
    bool getCommonCoeff(int64_t & common) const;

    /**
     * \brief Overload of == operator
     */
    bool operator==(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of != operator
     */
    bool operator!=(const MultivariatePolynomial & R) const;

    /**
     * \brief Overload of == operator
     */
    bool operator==(const int64_t R) const;

    /**
     * \brief Overload of != operator
     */
    bool operator!=(const int64_t R) const;

    /**
     * \brief Overload of == operator
     */
    friend bool operator==(const int64_t L, const MultivariatePolynomial & R);

    /**
     * \brief Overload of != operator
     */
    friend bool operator!=(const int64_t L, const MultivariatePolynomial & R);

    /**
     * \return a hash
     */
    std::size_t hash() const;

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

    // Helper functions to use with eval
    static bool __isValid(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values);
    static bool __isValid(const std::vector<const MultivariatePolynomial *> & values);
    static bool __isValid(const std::pair<uint64_t, const MultivariatePolynomial *> & values);
    static bool __contains(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
    static bool __contains(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val);
    static bool __contains(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __get(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __getSafe(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __get(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __getSafe(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __get(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
    static const MultivariatePolynomial * __getSafe(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val);
};

} // namespace analysis

#endif // __MULTIVARIATE_POLYNOMIAL_HXX__
