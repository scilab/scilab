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

#ifndef __INFERENCE_CONSTRAINT_HXX__
#define __INFERENCE_CONSTRAINT_HXX__

#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "GVN.hxx"
#include "tools.hxx"

namespace analysis
{

struct MPolyConstraint;
struct MPolyConstraintSet;

struct InferenceConstraint
{
    enum Result
    {
        RESULT_TRUE, RESULT_FALSE, RESULT_DUNNO
    };

    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const = 0;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const = 0;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const { }

    inline static std::vector<const MultivariatePolynomial *> getArgs(const std::vector<GVN::Value *> & values)
    {
        std::vector<const MultivariatePolynomial *> args;
        args.reserve(values.size());
        for (const auto value : values)
        {
            args.emplace_back(value->poly);
        }
        return args;
    }

    inline static void applyEquality(GVN::Value & x, GVN::Value & y)
    {
        if (x != y)
        {
            if (x.poly->polynomial.size() < y.poly->polynomial.size())
            {
                y = x;
            }
            else
            {
                x = y;
            }
        }
    }
};

struct SameDimsConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct EqualConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct PositiveConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct StrictPositiveConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct GreaterConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct StrictGreaterConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct ValidIndexConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct ValidRangeConstraint : public InferenceConstraint
{
    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const /*override*/;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const /*override*/;
};

struct MPolyConstraint : public InferenceConstraint
{
    enum Kind
    {
        EQ0, NEQ0, GT0, GEQ0
    };

    MultivariatePolynomial poly;
    Kind kind;

    MPolyConstraint(const MultivariatePolynomial & _poly, const Kind _kind) : poly(_poly), kind(_kind)
    {
        int64_t common;
        if (poly.getCommonCoeff(common) && common != 1 && common != 0)
        {
            if (kind == EQ0)
            {
                poly /= common;
            }
            else
            {
                poly /= std::abs(common);
            }
        }
    }

    inline bool isConstant() const
    {
        return poly.isConstant();
    }

    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const override;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const override;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const override;

    struct Hash
    {
        inline std::size_t operator()(const MPolyConstraint & mpc) const
        {
            return tools::hash_combine(mpc.kind, mpc.poly.hash());
        }
    };

    inline bool operator==(const MPolyConstraint & R) const
    {
        return kind == R.kind && poly == R.poly;
    }

    struct Eq
    {
        inline bool operator()(const MPolyConstraint & L, const MPolyConstraint & R) const
        {
            return L == R;
        }
    };

    friend std::wostream & operator<<(std::wostream & out, const MPolyConstraint & mpc);
};

struct MPolyConstraintSet : public InferenceConstraint
{
    std::unordered_set<MPolyConstraint, MPolyConstraint::Hash, MPolyConstraint::Eq> constraints;

    MPolyConstraintSet() { }
    MPolyConstraintSet(const unsigned int size)
    {
        constraints.reserve(size);
    }

    inline void add(MPolyConstraint && mpc)
    {
        if (!mpc.isConstant())
        {
            constraints.emplace(std::move(mpc));
        }
    }

    inline void add(MultivariatePolynomial && poly, MPolyConstraint::Kind kind)
    {
        if (!poly.isConstant())
        {
            constraints.emplace(std::move(poly), kind);
        }
    }

    inline void add(const MultivariatePolynomial & poly, MPolyConstraint::Kind kind)
    {
        if (!poly.isConstant())
        {
            constraints.emplace(poly, kind);
        }
    }

    inline void add(const MPolyConstraintSet & set)
    {
        constraints.insert(set.constraints.begin(), set.constraints.end());
    }

    inline bool empty() const
    {
        return constraints.empty();
    }

    inline std::size_t size() const
    {
        return constraints.size();
    }

    virtual Result check(GVN & gvn, const std::vector<GVN::Value *> & values) const override;
    virtual MPolyConstraintSet getMPConstraints(const std::vector<GVN::Value *> & values) const override;
    virtual void applyConstraints(const std::vector<GVN::Value *> & values) const override;

    friend std::wostream & operator<<(std::wostream & out, const MPolyConstraintSet & mpcs);

    struct Hash
    {
        inline std::size_t operator()(const MPolyConstraintSet & mpcs) const
        {
            std::size_t seed = 0;
            for (const auto & c : mpcs.constraints)
            {
                seed = tools::hash_combine(seed, MPolyConstraint::Hash()(c));
            }
            return seed;
        }
    };

    struct Eq
    {
        inline bool operator()(const MPolyConstraintSet & L, const MPolyConstraintSet & R) const
        {
            return L.constraints == R.constraints;
        }
    };
};


} // namespace analysis

#endif // __INFERENCE_CONSTRAINT_HXX__
