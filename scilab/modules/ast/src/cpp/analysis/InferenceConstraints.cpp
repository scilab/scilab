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

#include "gvn/InferenceConstraint.hxx"

namespace analysis
{
InferenceConstraint::Result SameDimsConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
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

InferenceConstraint::Result EqualConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
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

InferenceConstraint::Result MPolyConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    const std::vector<const MultivariatePolynomial *> & args = InferenceConstraint::getArgs(values);
    MultivariatePolynomial mp;

    if (poly.containsVarsGEq(args.size()))
    {
        mp = poly.translateVariables(gvn.getCurrentValue() + 1, args.size()).eval(args);
    }
    else
    {
        mp = poly.eval(args);
    }

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

InferenceConstraint::Result MPolyConstraintSet::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    for (const auto & constraint : constraints)
    {
        Result res = constraint.check(gvn, values);
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

InferenceConstraint::Result PositiveConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
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

InferenceConstraint::Result StrictPositiveConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & x = *values[0];

    if (x.poly->isCoeffStrictPositive())
    {
        return Result::RESULT_TRUE;
    }
    else if (x.poly->isConstant() && x.poly->constant <= 0)
    {
        return Result::RESULT_FALSE;
    }

    return Result::RESULT_DUNNO;
}

void StrictPositiveConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet StrictPositiveConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    const GVN::Value & x = *values[0];

    set.add(*x.poly, MPolyConstraint::Kind::GT0);

    return set;
}

InferenceConstraint::Result StrictGreaterConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
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

void StrictGreaterConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet StrictGreaterConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(1);
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    set.add(*x.poly - *y.poly, MPolyConstraint::Kind::GT0);

    return set;
}

InferenceConstraint::Result GreaterConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & x = *values[0];
    const GVN::Value & y = *values[1];

    if (x.value == y.value)
    {
        return Result::RESULT_TRUE;
    }

    MultivariatePolynomial mp = *x.poly - *y.poly;
    if (mp.isCoeffPositive(true))
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

    set.add(*x.poly - *y.poly, MPolyConstraint::Kind::GEQ0);

    return set;
}

InferenceConstraint::Result ValidIndexConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & index = *values[0];
    const GVN::Value & max = *values[1];
    if (index.poly->constant > 0 && index.poly->isCoeffPositive(false))
    {
        // the index is geq than 1
        MultivariatePolynomial mp = *max.poly - *index.poly;
        if (mp.isCoeffPositive())
        {
            // max - index >= 0
            return Result::RESULT_TRUE;
        }
        else if (mp.isConstant() && mp.constant < 0)
        {
            return Result::RESULT_FALSE;
        }
    }
    else if (index.poly->isConstant() && index.poly->constant < 1)
    {
        return Result::RESULT_FALSE;
    }

    return Result::RESULT_DUNNO;
}

void ValidIndexConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet ValidIndexConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(2);
    const GVN::Value & index = *values[0];
    const GVN::Value & max = *values[1];

    set.add(*max.poly - *index.poly, MPolyConstraint::Kind::GEQ0);
    set.add(*index.poly - 1, MPolyConstraint::Kind::GEQ0);

    return set;
}

InferenceConstraint::Result ValidRangeConstraint::check(GVN & gvn, const std::vector<GVN::Value *> & values) const
{
    const GVN::Value & index_min = *values[0];
    const GVN::Value & index_max = *values[1];
    const GVN::Value & min = *values[2];
    const GVN::Value & max = *values[3];

    MultivariatePolynomial mp_min = *index_min.poly - *min.poly;
    if (mp_min.isCoeffPositive())
    {
        MultivariatePolynomial mp_max = *max.poly - *index_max.poly;
        if (mp_max.isCoeffPositive())
        {
            return Result::RESULT_TRUE;
        }
        else if (mp_max.isConstant() && mp_max.constant < 0)
        {
            return Result::RESULT_FALSE;
        }
    }
    else if (mp_min.isConstant() && mp_min.constant < 0)
    {
        return Result::RESULT_FALSE;
    }

    return Result::RESULT_DUNNO;
}

void ValidRangeConstraint::applyConstraints(const std::vector<GVN::Value *> & values) const { }

MPolyConstraintSet ValidRangeConstraint::getMPConstraints(const std::vector<GVN::Value *> & values) const
{
    MPolyConstraintSet set(4);
    const GVN::Value & index_min = *values[0];
    const GVN::Value & index_max = *values[1];
    const GVN::Value & min = *values[2];
    const GVN::Value & max = *values[3];

    set.add(*index_min.poly - *min.poly, MPolyConstraint::Kind::GEQ0);
    set.add(*max.poly - *index_max.poly, MPolyConstraint::Kind::GEQ0);

    return set;
}

std::wostream & operator<<(std::wostream & out, const MPolyConstraint & mpc)
{
    out << mpc.poly;
    switch (mpc.kind)
    {
        case MPolyConstraint::Kind::EQ0:
            out << L" = 0";
            break;
        case MPolyConstraint::Kind::NEQ0:
            out << L" != 0";
            break;
        case MPolyConstraint::Kind::GT0:
            out << L" > 0";
            break;
        case MPolyConstraint::Kind::GEQ0:
            out << L" >= 0";
            break;
    }

    return out;
}

std::wostream & operator<<(std::wostream & out, const MPolyConstraintSet & mpcs)
{
    tools::printSet(mpcs.constraints, out);
    return out;
}
}
