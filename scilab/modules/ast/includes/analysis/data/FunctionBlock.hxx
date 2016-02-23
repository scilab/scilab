/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __FUNCTIONBLOCK_HXX__
#define __FUNCTIONBLOCK_HXX__

#include <list>
#include <vector>
#include <set>
#include <map>

#include "tools.hxx"
#include "allexp.hxx"
#include "Block.hxx"
#include "MacroDef.hxx"
#include "TypeLocal.hxx"
#include "LocalInfo.hxx"
#include "ArgIOInfo.hxx"
#include "TemporaryManager.hxx"
#include "TITypeSignatureTuple.hxx"
#include "gvn/ConstraintManager.hxx"
#include "LoopAnalyzer.hxx"

namespace analysis
{

struct MacroOut;
class CompleteMacroSignature;

class FunctionBlock : public Block
{

    uint64_t functionId;
    std::wstring name;
    std::vector<symbol::Symbol> in;
    std::vector<symbol::Symbol> out;
    tools::SymbolOrdSet globals;
    std::vector<ArgIOInfo> types_in;
    std::vector<ArgIOInfo> types_out;
    tools::SymbolMap<LocalInfo> locals;
    std::vector<GVN::Value *> inValues;
    unsigned int lhs;
    unsigned int rhs;
    int maxVarId;
    GVN fgvn;
    LoopAnalyzer loopAnalyzer;
    ConstraintManager constraintManager;
    TemporaryManager tempManager;

public:

    FunctionBlock(const unsigned int id, Block * parent, ast::Exp * exp);

    virtual ~FunctionBlock()
    {
        // we got a clone
        delete exp;
    }

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

    inline const std::vector<ArgIOInfo> & getTypesIn() const
    {
        return types_in;
    }

    inline const std::vector<ArgIOInfo> & getTypesOut() const
    {
        return types_out;
    }

    inline const tools::SymbolMap<LocalInfo> & getTypesLocals() const
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

    inline void setFunctionId(const uint64_t id)
    {
        functionId = id;
    }

    inline uint64_t getFunctionId() const
    {
        return functionId;
    }

    inline const MPolyConstraintSet & getVerifiedConstraints() const
    {
        return constraintManager.getVerifiedConstraints();
    }

    inline const ConstraintManager::UnverifiedSet & getUnverifiedConstraints() const
    {
        return constraintManager.getUnverifiedConstraints();
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

    inline const LoopAnalyzer & getLoopAnalyzer() const
    {
        return loopAnalyzer;
    }

    void finalize() override;
    void addGlobal(const symbol::Symbol & sym) override;
    Block * getDefBlock(const symbol::Symbol & sym, tools::SymbolMap<Info>::iterator & it, const bool global) override;
    void addLocal(const symbol::Symbol & sym, const TIType & type, const bool isAnInt) override;
    int getTmpId(const TIType & type, const bool isAnInt) override;
    void releaseTmp(const int id, ast::Exp * exp) override;
    void needRefCount(const tools::SymbolSet & set) override;

    bool addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values);
    void setGlobals(const tools::SymbolOrdSet & v);
    //TITypeSignatureTuple getGlobals(std::vector<symbol::Symbol> & v);
    MacroOut getOuts(CompleteMacroSignature & cms);
    void setInOut(MacroDef * macrodef, const unsigned int rhs, const std::vector<TIType> & _in);

    friend std::wostream & operator<<(std::wostream & out, const FunctionBlock & fblock);
};

} // namespace analysis

#endif // __FUNCTIONBLOCK_HXX__
