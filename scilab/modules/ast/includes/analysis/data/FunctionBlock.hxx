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
#include "TITypeSignatureTuple.hxx"
#include "gvn/ConstraintManager.hxx"

namespace analysis
{

struct MacroOut;

class FunctionBlock : public Block
{

    struct __TypeLocal
    {
	TIType::Type type;
	int rows;
	int cols;

	__TypeLocal(const TIType::Type _type, const int _rows, const int _cols) : type(_type), rows(_rows), cols(_cols) { }

	inline bool operator<(const __TypeLocal & R) const
	    {
		return type < R.type || (type == R.type && (rows < R.rows || (rows == R.rows && cols < R.cols)));
	    }

	inline bool isScalar() const
	    {
		return rows == 1 && cols == 1;
	    }
    };
    
    std::wstring name;
    std::vector<symbol::Symbol> in;
    std::vector<symbol::Symbol> out;
    std::set<symbol::Symbol> globals;
    std::map<symbol::Symbol, std::set<__TypeLocal>> locals;
    std::vector<GVN::Value *> inValues;
    unsigned int lhs;
    unsigned int rhs;
    int maxVarId;
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

    inline int getMaxVarId() const
    {
        return maxVarId;
    }

    void finalize() override;
    void addGlobal(const symbol::Symbol & sym) override;
    Info & addDefine(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp) override;
    Block * getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it, const bool global) override;

    bool addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values);
    void setGlobals(const std::set<symbol::Symbol> & v);
    //TITypeSignatureTuple getGlobals(std::vector<symbol::Symbol> & v);
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

    inline const std::set<symbol::Symbol> & getGlobalConstants() const
    {
        return constraintManager.getGlobalConstants();
    }
};

} // namespace analysis

#endif // __FUNCTIONBLOCK_HXX__
