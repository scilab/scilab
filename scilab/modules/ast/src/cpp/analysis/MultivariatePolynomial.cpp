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

#include "gvn/MultivariatePolynomial.hxx"

namespace analysis
{

MultivariatePolynomial MultivariatePolynomial::getInvalid()
{
    return MultivariatePolynomial(0, false);
}

bool MultivariatePolynomial::isValid() const
{
    return valid;
}

bool MultivariatePolynomial::isInvalid() const
{
    return !valid;
}

void MultivariatePolynomial::invalid()
{
    constant = 0;
    valid = false;
    polynomial.clear();
}

bool MultivariatePolynomial::contains(const uint64_t var) const
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

bool MultivariatePolynomial::checkVariable(const uint64_t max) const
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

bool MultivariatePolynomial::containsVarsGEq(const uint64_t min) const
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

MultivariatePolynomial MultivariatePolynomial::translateVariables(const uint64_t t, const uint64_t min) const
{
    MultivariatePolynomial mp(polynomial.size(), constant);
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

MultivariatePolynomial & MultivariatePolynomial::add(const MultivariateMonomial & m, const int64_t coeff)
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

void MultivariatePolynomial::sub(const MultivariateMonomial & m)
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

MultivariatePolynomial MultivariatePolynomial::operator+(const MultivariateMonomial & R) const
{
    if (isValid())
    {
        MultivariatePolynomial res(*this);
        res.add(R);
        return res;
    }
    return getInvalid();
}

MultivariatePolynomial & MultivariatePolynomial::operator+=(const MultivariateMonomial & R)
{
    if (isValid())
    {
        add(R);
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator+(const int64_t R) const
{
    if (isValid())
    {
        MultivariatePolynomial res(*this);
        res.constant += R;
        return res;
    }
    return *this;
}

MultivariatePolynomial & MultivariatePolynomial::operator+=(const int64_t R)
{
    if (isValid())
    {
        constant += R;
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator-(const MultivariateMonomial & R) const
{
    if (isValid())
    {
        MultivariatePolynomial res(*this);
        res.sub(R);
        return res;
    }
    return *this;
}

MultivariatePolynomial & MultivariatePolynomial::operator-=(const MultivariateMonomial & R)
{
    if (isValid())
    {
        sub(R);
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator-(const int64_t R) const
{
    if (isValid())
    {
        MultivariatePolynomial res(*this);
        res.constant -= R;
        return res;
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator-() const
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

MultivariatePolynomial & MultivariatePolynomial::operator-=(const int64_t R)
{
    if (isValid())
    {
        constant -= R;
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator+(const MultivariatePolynomial & R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator+=(const MultivariatePolynomial & R)
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

MultivariatePolynomial MultivariatePolynomial::operator-(const MultivariatePolynomial & R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator-=(const MultivariatePolynomial & R)
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

MultivariatePolynomial MultivariatePolynomial::operator*(const MultivariatePolynomial & R) const
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

MultivariatePolynomial MultivariatePolynomial::operator/(const MultivariatePolynomial & R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator/=(const MultivariatePolynomial & R)
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

MultivariatePolynomial & MultivariatePolynomial::operator*=(const MultivariatePolynomial & R)
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

MultivariatePolynomial MultivariatePolynomial::operator*(const MultivariateMonomial & R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator*=(const MultivariateMonomial & R)
{
    if (isValid())
    {
        MultivariatePolynomial res = *this * R;
        polynomial = res.polynomial;
        constant = res.constant;
    }
    return *this;
}

MultivariatePolynomial MultivariatePolynomial::operator*(const int64_t R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator*=(const int64_t R)
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

MultivariatePolynomial MultivariatePolynomial::operator/(const int64_t R) const
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

MultivariatePolynomial & MultivariatePolynomial::operator/=(const int64_t R)
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

MultivariatePolynomial MultivariatePolynomial::operator^(unsigned int R) const
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

MultivariatePolynomial MultivariatePolynomial::operator^(const MultivariatePolynomial & R) const
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

bool MultivariatePolynomial::isDivisibleBy(const int64_t n) const
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

bool MultivariatePolynomial::isDivisibleBy(const MultivariatePolynomial & mp) const
{
    if (mp.polynomial.empty())
    {
        return isDivisibleBy(mp.constant);
    }

    return false;
}

bool MultivariatePolynomial::isPositive() const
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

bool MultivariatePolynomial::isCoeffStrictPositive(const bool checkConstant) const
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

bool MultivariatePolynomial::isCoeffPositive(const bool checkConstant) const
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

bool MultivariatePolynomial::isCoeffStrictNegative(const bool checkConstant) const
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

bool MultivariatePolynomial::isCoeffNegative(const bool checkConstant) const
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

const std::wstring MultivariatePolynomial::print(const std::map<uint64_t, std::wstring> & vars) const
{
    std::wostringstream wos;
    if (polynomial.empty())
    {
        wos << constant;
    }
    else
    {
        std::set<MultivariateMonomial, MultivariateMonomial::Compare> s(polynomial.begin(), polynomial.end());
        auto i = s.begin();
        if (constant)
        {
            wos << constant;
            if (i->coeff >= 0)
            {
                wos << L'+' << i->print(vars);
            }
            else
            {
                wos << i->print(vars);
            }
        }
        else
        {
            wos << i->print(vars);
        }

        for (i = std::next(s.begin()); i != s.end(); ++i)
        {
            if (i->coeff >= 0)
            {
                wos << L'+' << i->print(vars);
            }
            else
            {
                wos << i->print(vars);
            }
        }
    }

    return wos.str();
}

std::wostream & operator<<(std::wostream & out, const MultivariatePolynomial & p)
{
    const std::map<uint64_t, std::wstring> vars;
    out << p.print(vars);

    return out;
}

bool MultivariatePolynomial::isDiffConstant(const MultivariatePolynomial & R) const
{
    return polynomial == R.polynomial;
}

bool MultivariatePolynomial::isConstant() const
{
    return polynomial.empty();
}

bool MultivariatePolynomial::isConstant(const int64_t val) const
{
    return isConstant() && constant == val;
}

bool MultivariatePolynomial::getCommonCoeff(int64_t & common) const
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

bool MultivariatePolynomial::operator==(const MultivariatePolynomial & R) const
{
    return constant == R.constant && polynomial == R.polynomial;
}

bool MultivariatePolynomial::operator!=(const MultivariatePolynomial & R) const
{
    return !(*this == R);
}

bool MultivariatePolynomial::operator==(const int64_t R) const
{
    return polynomial.empty() && constant == R;
}

bool MultivariatePolynomial::operator!=(const int64_t R) const
{
    return !(*this == R);
}

bool operator==(const int64_t L, const MultivariatePolynomial & R)
{
    return R == L;
}

bool operator!=(const int64_t L, const MultivariatePolynomial & R)
{
    return R != L;
}

std::size_t MultivariatePolynomial::hash() const
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

bool MultivariatePolynomial::__isValid(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values)
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

bool MultivariatePolynomial::__isValid(const std::vector<const MultivariatePolynomial *> & values)
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

bool MultivariatePolynomial::__isValid(const std::pair<uint64_t, const MultivariatePolynomial *> & values)
{
    return values.second->isValid();
}

bool MultivariatePolynomial::__contains(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    return values.find(val) != values.end();
}

bool MultivariatePolynomial::__contains(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
{
    return val < values.size();
}

bool MultivariatePolynomial::__contains(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    return values.first == val;
}

const MultivariatePolynomial * MultivariatePolynomial::__get(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    const auto i = values.find(val);
    if (i != values.end())
    {
        return i->second;
    }
    return nullptr;
}

const MultivariatePolynomial * MultivariatePolynomial::__getSafe(const std::unordered_map<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    return values.find(val)->second;
}

const MultivariatePolynomial * MultivariatePolynomial::__get(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
{
    if (val < values.size())
    {
        return values[val];
    }
    return nullptr;
}

const MultivariatePolynomial * MultivariatePolynomial::__getSafe(const std::vector<const MultivariatePolynomial *> & values, const uint64_t val)
{
    return values[val];
}

const MultivariatePolynomial * MultivariatePolynomial::__get(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    if (values.first == val)
    {
        return values.second;
    }
    return nullptr;
}

const MultivariatePolynomial * MultivariatePolynomial::__getSafe(const std::pair<uint64_t, const MultivariatePolynomial *> & values, const uint64_t val)
{
    return values.second;
}

} // namespace analysis
