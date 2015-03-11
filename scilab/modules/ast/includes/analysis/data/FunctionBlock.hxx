/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FUNCTIONBLOCK_HXX__
#define __FUNCTIONBLOCK_HXX__

#include <list>
#include <set>
#include <vector>

#include "allexp.hxx"
#include "Block.hxx"
#include "MacroDef.hxx"
#include "TITypeSignatureTuple.hxx"
#include "gvn/ConstraintManager.hxx"

namespace analysis
{

struct MacroOut;

class FunctionBlock : public Block
{
    std::wstring name;
    std::vector<symbol::Symbol> in;
    std::vector<symbol::Symbol> out;
    std::set<symbol::Symbol> globals;
    std::vector<GVN::Value *> inValues;
    unsigned int lhs;
    unsigned int rhs;
    GVN fgvn;
    ConstraintManager constraintManager;

public:

    FunctionBlock(const unsigned int id, Block * parent, ast::Exp * exp);
    virtual ~FunctionBlock() { }

    inline ConstraintManager & getConstraintManager()
    {
        return constraintManager;
    }
    inline const std::vector<GVN::Value *> & getInValues() const
    {
        return inValues;
    }
    inline void setName(const std::wstring & _name)
    {
        name = _name;
    }
    inline const std::wstring & getName() const
    {
        return name;
    }
    inline unsigned int getLHS() const
    {
        return lhs;
    }
    inline unsigned int getRHS() const
    {
        return rhs;
    }

    void finalize() override;
    void addGlobal(const symbol::Symbol & sym) override;
    bool addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values);
    TITypeSignatureTuple getGlobals(std::vector<symbol::Symbol> & v);
    MacroOut getOuts();

    inline void setLhsRhs(const unsigned int _lhs, const unsigned int _rhs)
    {
        lhs = _lhs;
        rhs = _rhs;
    }

    inline void setInOut(MacroDef * macrodef)
    {
        in = macrodef->getIn();
        out = macrodef->getOut();
    }

    inline const MPolyConstraintSet & getConstraints() const
    {
        return constraintManager.getSet();
    }
};

} // namespace analysis

#endif // __FUNCTIONBLOCK_HXX__
