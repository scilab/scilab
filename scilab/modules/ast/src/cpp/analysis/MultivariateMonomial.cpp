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

#include <cmath>
#include <functional>

#include "gvn/MultivariateMonomial.hxx"

namespace analysis
{

bool MultivariateMonomial::contains(const uint64_t var) const
{
    return monomial.find(var) != monomial.end();
}

bool MultivariateMonomial::checkVariable(const uint64_t max) const
{
    return std::prev(monomial.end())->var <= max;
}

unsigned int MultivariateMonomial::exponent() const
{
    unsigned int exp = 0;
    for (const auto & ve : monomial)
    {
        exp += ve.exp;
    }
    return exp;
}

MultivariateMonomial & MultivariateMonomial::add(const VarExp & ve)
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

MultivariateMonomial & MultivariateMonomial::add(VarExp && ve)
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

MultivariateMonomial MultivariateMonomial::operator*(const MultivariateMonomial & R) const
{
    MultivariateMonomial res(*this);
    res.coeff *= R.coeff;
    for (const auto & ve : R.monomial)
    {
        res.add(ve);
    }
    return res;
}

MultivariateMonomial & MultivariateMonomial::operator*=(const MultivariateMonomial & R)
{
    coeff *= R.coeff;
    for (const auto & ve : R.monomial)
    {
        add(ve);
    }
    return *this;
}

MultivariateMonomial operator*(const int64_t L, const MultivariateMonomial & R)
{
    return R * L;
}

MultivariateMonomial MultivariateMonomial::operator*(const int64_t R) const
{
    MultivariateMonomial res(*this);
    res.coeff *= R;
    return res;
}

MultivariateMonomial & MultivariateMonomial::operator*=(const int64_t R)
{
    coeff *= R;
    return *this;
}

MultivariateMonomial MultivariateMonomial::operator/(const int64_t R) const
{
    MultivariateMonomial res(*this);
    res.coeff /= R;
    return res;
}

MultivariateMonomial & MultivariateMonomial::operator/=(const int64_t R)
{
    coeff /= R;
    return *this;
}

MultivariateMonomial MultivariateMonomial::operator^(unsigned int R) const
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

bool MultivariateMonomial::operator==(const MultivariateMonomial & R) const
{
    return coeff == R.coeff && monomial == R.monomial;
}

const std::wstring MultivariateMonomial::print(const std::map<uint64_t, std::wstring> & vars) const
{
    std::wostringstream wos;
    if (coeff == -1 || coeff == 1)
    {
        if (coeff == -1)
        {
            wos << L'-';
        }
        if (!monomial.empty())
        {
            wos << monomial.begin()->print(vars);
            for (auto i = std::next(monomial.begin()), e = monomial.end(); i != e; ++i)
            {
                wos << L"*" << i->print(vars);
            }
        }
    }
    else
    {
        wos << coeff;
        for (const auto & ve : monomial)
        {
            wos << L"*" << ve.print(vars);
        }
    }
    return wos.str();
}

std::wostream & operator<<(std::wostream & out, const MultivariateMonomial & m)
{
    const std::map<uint64_t, std::wstring> vars;
    out << m.print(vars);

    return out;
}

bool MultivariateMonomial::Compare::operator()(const MultivariateMonomial & L, const MultivariateMonomial & R) const
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

} // namespace analysis
