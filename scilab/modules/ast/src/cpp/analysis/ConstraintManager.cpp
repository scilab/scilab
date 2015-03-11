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
    // Is a value greater than an other ?
    v.emplace_back(new GreaterConstraint());

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
    InferenceConstraint::Result res = set.check(values);
    switch (res)
    {
        case InferenceConstraint::Result::RESULT_TRUE:
        {
            mpConstraints.add(set);
            set.applyConstraints(values);
            return true;
        }
        case InferenceConstraint::Result::RESULT_FALSE:
            return false;
        case InferenceConstraint::Result::RESULT_DUNNO:
        {
            if (parent && parent->function)
            {
                const bool ret = parent->check(set.getMPConstraints(values), parent->function->getInValues());
                if (ret)
                {
                    mpConstraints.add(set);
                    set.applyConstraints(values);
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
    const InferenceConstraint & ic = *generalConstraints[kind];
    InferenceConstraint::Result res = ic.check(values);
    //std::wcerr << "DEBUG2=" << res << std::endl;

    switch (res)
    {
        case InferenceConstraint::Result::RESULT_TRUE:
        {
            MPolyConstraintSet set = ic.getMPConstraints(values);
            mpConstraints.add(set);
            ic.applyConstraints(values);
            return true;
        }
        case InferenceConstraint::Result::RESULT_FALSE:
            return false;
        case InferenceConstraint::Result::RESULT_DUNNO:
        {
            if (function)
            {
                MPolyConstraintSet set = ic.getMPConstraints(values);
                const bool ret = check(set, function->getInValues());
                if (ret)
                {
                    mpConstraints.add(set);
                    ic.applyConstraints(values);
                }
                return ret;
            }
            return false;
        }
    }
}
}
