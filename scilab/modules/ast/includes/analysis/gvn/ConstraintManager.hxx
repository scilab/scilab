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

#ifndef __CONSTRAINT_MANAGER_HXX__
#define __CONSTRAINT_MANAGER_HXX__

#include <memory>
#include <vector>

#include "InferenceConstraint.hxx"
#include "dynlib_ast.h"

namespace analysis
{

class FunctionBlock;

class EXTERN_AST ConstraintManager
{
    ConstraintManager * parent;
    FunctionBlock * function;
    MPolyConstraintSet mpConstraints;

    static std::vector<std::shared_ptr<InferenceConstraint>> generalConstraints;

public:

    enum Kind { SAMEDIMS = 0, EQUAL, POSITIVE, GREATER, COUNT };

    ConstraintManager(FunctionBlock * _function, FunctionBlock * _parent = nullptr);
    ConstraintManager(FunctionBlock & _function, FunctionBlock * _parent = nullptr) : ConstraintManager(&_function, _parent) { }
    ConstraintManager() : ConstraintManager(nullptr, nullptr) { }

    inline bool isRoot() const
    {
        return parent == nullptr;
    }

    inline const MPolyConstraintSet & getSet() const
    {
        return mpConstraints;
    }

    bool check(const MPolyConstraintSet & set, const std::vector<GVN::Value *> & values);
    bool check(Kind kind, const std::vector<GVN::Value *> & values);

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
