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

#ifndef __CONSTRAINT_MANAGER_HXX__
#define __CONSTRAINT_MANAGER_HXX__

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "InferenceConstraint.hxx"
#include "dynlib_ast.h"

namespace analysis
{

class FunctionBlock;

class EXTERN_AST ConstraintManager
{

public:

    typedef std::unordered_set<MPolyConstraintSet, MPolyConstraintSet::Hash, MPolyConstraintSet::Eq> UnverifiedSet;

private:

    ConstraintManager * parent;
    FunctionBlock * function;
    MPolyConstraintSet verified;
    std::set<symbol::Symbol> constantConstraints;
    UnverifiedSet unverified;

    static std::vector<std::shared_ptr<InferenceConstraint>> generalConstraints;

public:

    enum Kind { SAMEDIMS = 0, EQUAL, POSITIVE, STRICT_POSITIVE, GREATER, STRICT_GREATER, VALID_INDEX, VALID_RANGE, COUNT };

    ConstraintManager(FunctionBlock * _function, FunctionBlock * _parent = nullptr);
    ConstraintManager(FunctionBlock & _function, FunctionBlock * _parent = nullptr) : ConstraintManager(&_function, _parent) { }
    ConstraintManager() : ConstraintManager(nullptr, nullptr) { }

    inline bool isRoot() const
    {
        return parent == nullptr;
    }

    inline const MPolyConstraintSet & getVerifiedConstraints() const
    {
        return verified;
    }

    inline const UnverifiedSet & getUnverifiedConstraints() const
    {
        return unverified;
    }

    inline const std::set<symbol::Symbol> & getGlobalConstants() const
    {
        return constantConstraints;
    }

    bool check(const MPolyConstraintSet & set, const std::vector<GVN::Value *> & values);
    bool check(Kind kind, const std::vector<GVN::Value *> & values);
    bool checkGlobalConstant(const symbol::Symbol & sym);

    friend std::wostream & operator<<(std::wostream & out, const ConstraintManager & cm);

    static bool checkGlobalConstants(const std::set<symbol::Symbol> & gc);

    template<typename... Args>
    inline bool check(Kind kind, Args... args)
    {
        std::vector<GVN::Value *> v;
        return __check(kind, v, args...);
    }

private:

    inline bool __check(Kind kind, const std::vector<GVN::Value *> & v)
    {
        return check(kind, v);
    }

    template<typename... Args>
    inline bool __check(Kind kind, std::vector<GVN::Value *> & v, GVN::Value & val, Args... args)
    {
        v.emplace_back(&val);
        return __check(kind, v, args...);
    }

    template<typename... Args>
    inline bool __check(Kind kind, std::vector<GVN::Value *> & v, GVN::Value * val, Args... args)
    {
        v.emplace_back(val);
        return __check(kind, v, args...);
    }

    static std::vector<std::shared_ptr<InferenceConstraint>> init();

};

} // namespace analysis

#endif // __CONSTRAINT_MANAGER_HXX__
