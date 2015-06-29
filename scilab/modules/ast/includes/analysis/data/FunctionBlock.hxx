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
#include <map>
#include <set>
#include <vector>

#include "allexp.hxx"
#include "Block.hxx"
#include "MacroDef.hxx"
#include "TypeLocal.hxx"
#include "TemporaryManager.hxx"
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
    std::vector<std::pair<symbol::Symbol, TypeLocal>> types_in;
    std::vector<std::pair<symbol::Symbol, TypeLocal>> types_out;
    std::map<symbol::Symbol, std::set<TypeLocal>> locals;
    std::vector<GVN::Value *> inValues;
    unsigned int lhs;
    unsigned int rhs;
    int maxVarId;
    GVN fgvn;
    ConstraintManager constraintManager;
    TemporaryManager tempManager;

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

    inline const std::vector<std::pair<symbol::Symbol, TypeLocal>> & getTypesIn() const
	{
	    return types_in;
	}

    inline const std::vector<std::pair<symbol::Symbol, TypeLocal>> & getTypesOut() const
	{
	    return types_out;
	}

    inline const std::map<symbol::Symbol, std::set<TypeLocal>> & getTypesLocals() const
	{
	    return locals;
	}

    inline int getMaxVarId() const
    {
        return maxVarId;
    }

    inline void setLhsRhs(const unsigned int _lhs, const unsigned int _rhs)
    {
        lhs = _lhs;
        rhs = _rhs;
    }

    inline const MPolyConstraintSet & getConstraints() const
    {
        return constraintManager.getSet();
    }

    inline const std::set<symbol::Symbol> & getGlobalConstants() const
    {
        return constraintManager.getGlobalConstants();
    }

    inline const std::map<TypeLocal, std::stack<int>> & getTemp() const
	{
	    return tempManager.getTemp();
	}

    inline const std::map<TypeLocal, int> getTempCount(int & total) const
	{
	    int _total = 0;
	    std::map<TypeLocal, int> map;
	    for (const auto & p : getTemp())
	    {
		_total += p.second.size();
		map.emplace(p.first, p.second.size());
	    }

	    total = _total;
	    
	    return map;
	}

    void finalize() override;
    void addGlobal(const symbol::Symbol & sym) override;
    Block * getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it, const bool global) override;
    void addLocal(const symbol::Symbol & sym, const TIType & type, const bool isAnInt) override;
    int getTmpId(const TIType & type, const bool isAnInt) override;
    void releaseTmp(const int id) override;

    bool addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values);
    void setGlobals(const std::set<symbol::Symbol> & v);
    //TITypeSignatureTuple getGlobals(std::vector<symbol::Symbol> & v);
    MacroOut getOuts();
    void setInOut(MacroDef * macrodef, const unsigned int rhs, const std::vector<TIType> & _in);

    friend std::wostream & operator<<(std::wostream & out, const FunctionBlock & fblock);
};

} // namespace analysis

#endif // __FUNCTIONBLOCK_HXX__
