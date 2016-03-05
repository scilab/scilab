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

#include "gvn/ConstraintManager.hxx"
#include "data/FunctionBlock.hxx"

namespace analysis
{
std::vector<std::shared_ptr<InferenceConstraint>> ConstraintManager::generalConstraints = init();

std::vector<std::shared_ptr<InferenceConstraint>> ConstraintManager::init()
{
    std::vector<std::shared_ptr<InferenceConstraint>> v;
    v.reserve(Kind::COUNT);
    // Same dims => equality of 2 pairs of values
    v.emplace_back(new SameDimsConstraint());
    // Equality of values
    v.emplace_back(new EqualConstraint());
    // Positivity of a value
    v.emplace_back(new PositiveConstraint());
    // Strict positivity of a value
    v.emplace_back(new StrictPositiveConstraint());
    // Is a value greater than an other ?
    v.emplace_back(new GreaterConstraint());
    // Is a value strict greater than an other ?
    v.emplace_back(new StrictGreaterConstraint());
    // Valid index
    v.emplace_back(new ValidIndexConstraint());
    // Valid range
    v.emplace_back(new ValidRangeConstraint());

    return v;
}

ConstraintManager::ConstraintManager(FunctionBlock * _function, FunctionBlock * _parent) : parent(_parent ? & _parent->getConstraintManager() : nullptr), function(_function) { }

bool ConstraintManager::check(const MPolyConstraintSet & set, const std::vector<GVN::Value *> & values)
{
    /*std::wcerr << set.constraints.begin()->poly << "::" << set.constraints.begin()->kind << std::endl;
    for (const auto & v : values)
    {
    std::wcerr << "DEBUG1=" << *v << std::endl;
    }*/
    InferenceConstraint::Result res = set.check((parent && parent->function) ? parent->function->getGVN() : function->getGVN(), values);
    switch (res)
    {
        case InferenceConstraint::RESULT_TRUE:
        {
            if (!set.empty())
            {
                verified.add(set);
                set.applyConstraints(values);
            }
            return true;
        }
        case InferenceConstraint::RESULT_FALSE:
            if (!set.empty())
            {
                unverified.emplace(set);
            }
            return false;
        case InferenceConstraint::RESULT_DUNNO:
        {
            if (parent && parent->function)
            {
                const bool ret = parent->check(set.getMPConstraints(values), parent->function->getInValues());
                if (!set.empty())
                {
                    if (ret)
                    {
                        verified.add(set);
                        set.applyConstraints(values);
                    }
                    else
                    {
                        unverified.emplace(set);
                    }
                }
                return ret;
            }
            else
            {
                return false;
            }
        }
    }
}

bool ConstraintManager::check(Kind kind, const std::vector<GVN::Value *> & values)
{
    if (function)
    {
        const InferenceConstraint & ic = *generalConstraints[kind];
        InferenceConstraint::Result res = ic.check(function->getGVN(), values);
        const MPolyConstraintSet set = ic.getMPConstraints(values);
        //std::wcerr << "DEBUG2=" << res << std::endl;

        switch (res)
        {
            case InferenceConstraint::RESULT_TRUE:
            {
                if (!set.empty())
                {
                    verified.add(set);
                    ic.applyConstraints(values);
                }
                return true;
            }
            case InferenceConstraint::RESULT_FALSE:
                if (!set.empty())
                {
                    unverified.emplace(set);
                }
                return false;
            case InferenceConstraint::RESULT_DUNNO:
            {
                MPolyConstraintSet set = ic.getMPConstraints(values);
                const bool ret = check(set, function->getInValues());

                if (!set.empty())
                {
                    if (ret)
                    {
                        verified.add(set);
                        ic.applyConstraints(values);
                    }
                    else
                    {
                        unverified.emplace(set);
                    }
                }
                return ret;
            }
        }
    }

    return false;
}

bool ConstraintManager::checkGlobalConstant(const symbol::Symbol & sym)
{
    if (constantConstraints.find(sym) == constantConstraints.end())
    {
        // TODO: fix that !!!
        const bool ret = true; //symbol::Context::getInstance()->isOriginalSymbol(sym);
        if (ret)
        {
            ConstraintManager * cm = this;
            while (cm)
            {
                cm->constantConstraints.emplace(sym);
                cm = cm->parent;
            }
        }
        return ret;
    }
    else
    {
        return true;
    }
}

bool ConstraintManager::checkGlobalConstants(const std::set<symbol::Symbol> & gc)
{
    for (const auto sym : gc)
    {
        if (!symbol::Context::getInstance()->isOriginalSymbol(sym))
        {
            return false;
        }
    }
    return true;
}

std::wostream & operator<<(std::wostream & out, const ConstraintManager & cm)
{
    if (!cm.verified.empty())
    {
        out << L"Verified: " << cm.verified << L'\n';
    }
    if (!cm.unverified.empty())
    {
        out << L"Unverified: ";
        for (const auto & unv : cm.unverified)
        {
            out << unv << L' ';
        }
        out << L'\n';
    }
    if (!cm.constantConstraints.empty())
    {
        out << L"Constants: ";
        tools::printSet(cm.constantConstraints, out);
        out << L'\n';
    }
    return out;
}
}
