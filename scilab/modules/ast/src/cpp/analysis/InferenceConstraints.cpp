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

#include "gvn/InferenceConstraint.hxx"

namespace analysis
{
InferenceConstraint::Result SameDimsConstraint::check(const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & R1 = *values[0];
    const GVN::Value & C1 = *values[1];
    const GVN::Value & R2 = *values[2];
    const GVN::Value & C2 = *values[3];

    if (R1.value == R2.value)
    {
        if (C1.value == C2.value)
        {
            return Result::RESULT_TRUE;
        }

        MultivariatePolynomial mp = *C1.poly - *C2.poly;
        if (mp.constant != 0 && mp.isCoeffPositive(false))
        {
            return Result::RESULT_FALSE;
        }
    }
    else
    {
        MultivariatePolynomial mp = *R1.poly - *R2.poly;
        if (mp.constant > 0 && mp.isCoeffPositive(false))
        {
            return Result::RESULT_FALSE;
        }
    }
    return Result::RESULT_DUNNO;
}

MPolyConstraintSet SameDimsConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(2);
    const GVN::Value & R1 = *values[0];
    const GVN::Value & C1 = *values[1];
    const GVN::Value & R2 = *values[2];
    const GVN::Value & C2 = *values[3];

    set.add(*R1.poly - *R2.poly, MPolyConstraint::Kind::EQ0);
    set.add(*C1.poly - *C2.poly, MPolyConstraint::Kind::EQ0);

    return set;
}

void SameDimsConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const
{
    GVN::Value & R1 = *values[0];
    GVN::Value & C1 = *values[1];
    GVN::Value & R2 = *values[2];
    GVN::Value & C2 = *values[3];

    applyEquality(R1, R2);
    applyEquality(C1, C2);
}

InferenceConstraint::Result EqualConstraint::check(const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    if (x.value == y.value)
    {
        return Result::RESULT_TRUE;
    }
    else
    {
        MultivariatePolynomial mp = *x.poly - *y.poly;
        if (mp.constant > 0 && mp.isCoeffPositive(false))
        {
            return Result::RESULT_FALSE;
        }
    }
    return Result::RESULT_DUNNO;
}

void EqualConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const
{
    GVN::Value & x = *values[0];
    GVN::Value & y = *values[1];

    applyEquality(x, y);
}

MPolyConstraintSet EqualConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    set.add(*x.poly - *y.poly, MPolyConstraint::Kind::EQ0);

    return set;
}

InferenceConstraint::Result MPolyConstraint::check(const std::vector<GVN::Value *> & values) const
{
    MultivariatePolynomial mp = poly.eval(InferenceConstraint::getArgs(values));
    //std::wcerr << "MPolyConstraint=" << poly << "::" << mp << std::endl;
    switch (kind)
    {
        case EQ0:
        {
            if (mp.isConstant(0))
            {
                // for all X, P(X) == 0
                return Result::RESULT_TRUE;
            }
            else if (mp.constant != 0 && mp.isCoeffPositive(false))
            {
                // P(X) = Q(X) + K where K != 0 and Q with positive coeffs
                return Result::RESULT_FALSE;
            }
            else
            {
                return Result::RESULT_DUNNO;
            }
        }
        case NEQ0:
            if (mp.constant != 0 && mp.isCoeffPositive(false))
            {
                return Result::RESULT_TRUE;
            }
            else if (mp.isConstant(0))
            {
                return Result::RESULT_FALSE;
            }
            else
            {
                return Result::RESULT_DUNNO;
            }
        case GT0:
            if (mp.isCoeffStrictPositive())
            {
                return Result::RESULT_TRUE;
            }
            else if (mp.constant < 0 && mp.isCoeffNegative(false))
            {
                return Result::RESULT_FALSE;
            }
            else
            {
                return Result::RESULT_DUNNO;
            }
        case GEQ0:
        {
            if (mp.isCoeffPositive())
            {
                return Result::RESULT_TRUE;
            }
            else if (mp.isConstant() && mp.constant < 0)
            {
                return Result::RESULT_FALSE;
            }
            else
            {
                return Result::RESULT_DUNNO;
            }
        }
    }
}

MPolyConstraintSet MPolyConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    set.add(poly.eval(InferenceConstraint::getArgs(values)), kind);

    return set;
}

void MPolyConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const
{
    if (kind == EQ0)
    {
        if (poly.constant == 0 && poly.polynomial.size() == 2)
        {
            const MultivariateMonomial & m1 = *poly.polynomial.begin();
            const MultivariateMonomial & m2 = *std::next(poly.polynomial.begin());

            if (((m1.coeff == 1 && m2.coeff == -1) || (m1.coeff == -1 && m2.coeff == 1)) && (m1.monomial.size() == 1 && m2.monomial.size() == 1))
            {
                // We have a polynomial P such as P(X,Y)=X-Y
                GVN::Value & x = *values[m1.monomial.begin()->var];
                GVN::Value & y = *values[m2.monomial.begin()->var];

                applyEquality(x, y);
            }
        }
    }
}

InferenceConstraint::Result MPolyConstraintSet::check(const std::vector<GVN::Value *> & values) const
{
    for (const auto & constraint : constraints)
    {
        Result res = constraint.check(values);
        if (res != Result::RESULT_TRUE)
        {
            return res;
        }
    }
    return Result::RESULT_TRUE;
}

MPolyConstraintSet MPolyConstraintSet::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(constraints.size());
    const std::vector<const MultivariatePolynomial *> args = InferenceConstraint::getArgs(values);
    for (const auto & constraint : constraints)
    {
        set.add(constraint.poly.eval(args), constraint.kind);
    }
    return set;
}

void MPolyConstraintSet::applyConstraints(const std::vector<GVN::Value *> & values) const
{
    for (const auto & mpc : constraints)
    {
        mpc.applyConstraints(values);
    }
}

InferenceConstraint::Result PositiveConstraint::check(const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & x = *values[0];

    if (x.poly->isCoeffPositive())
    {
        return Result::RESULT_TRUE;
    }
    else if (x.poly->isConstant() && x.poly->constant < 0)
    {
        return Result::RESULT_FALSE;
    }

    return Result::RESULT_DUNNO;
}

void PositiveConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet PositiveConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    const GVN::Value & x = *values[0];

    set.add(*x.poly, MPolyConstraint::Kind::GEQ0);

    return set;
}

InferenceConstraint::Result GreaterConstraint::check(const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    if (x.value == y.value)
    {
        return Result::RESULT_FALSE;
    }

    MultivariatePolynomial mp = *x.poly - *y.poly;
    if (mp.constant > 0 && mp.isCoeffPositive(false))
    {
        return Result::RESULT_TRUE;
    }
    else if (mp.constant < 0 && mp.isCoeffNegative(false))
    {
        return Result::RESULT_FALSE;
    }

    return Result::RESULT_DUNNO;
}

void GreaterConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet GreaterConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    set.add(*x.poly - *y.poly, MPolyConstraint::Kind::GT0);

    return set;
}
}
