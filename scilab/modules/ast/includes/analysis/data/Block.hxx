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

#ifndef __BLOCK_HXX__
#define __BLOCK_HXX__

#include <iostream>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "Data.hxx"
#include "Info.hxx"
#include "symbol.hxx"
#include "TIType.hxx"
#include "gvn/GVN.hxx"
#include "tools.hxx"
#include "dynlib_ast.h"

namespace analysis
{
class AnalysisVisitor;
class DataManager;

class EXTERN_AST Block
{

    friend class DataManager;

protected:

    DataManager * dm;
    ast::Exp * exp;
    Block * parent;
    tools::SymbolMap<Info> symMap;
    std::vector<Block *> blocks;
    GVN * gvn;
    bool isReturn;
    const unsigned int id;

public:

    enum BlockKind
    {
        NORMAL, LOOP, EXCLUSIVE, MACRO
    };

    Block(DataManager * _dm);
    Block(const unsigned int _id, Block * _parent, ast::Exp * _exp) : dm(_parent->dm), exp(_exp), parent(_parent), symMap(), blocks(), gvn(&_parent->getGVN()), isReturn(false), id(_id) { }

    virtual ~Block();

    inline tools::SymbolMap<Info> & getMap()
    {
        return symMap;
    }

    inline Info & addSym(const symbol::Symbol & sym, Data * data)
    {
#ifdef DEBUG_DATAMANAGER
        if (symMap.find(sym) != symMap.end())
        {
            std::wcerr << L"Table " << id << L" already contains " << sym << std::endl;
        }
#endif
        return symMap.emplace(sym, data).first->second;
    }

    inline Info & addSym(const symbol::Symbol & sym, const Info & info)
    {
#ifdef DEBUG_DATAMANAGER
        if (symMap.find(sym) != symMap.end())
        {
            std::wcerr << L"Table " << id << L" already contains " << sym << std::endl;
        }
#endif
        return symMap.emplace(sym, info).first->second;
    }

    inline ast::Exp * getExp()
    {
        return exp;
    }

    inline void setReturn(const bool _isReturn)
    {
        isReturn = _isReturn;
    }

    inline bool getReturn() const
    {
        return isReturn;
    }

    static Info & addSym(tools::SymbolMap<Info> & M, const symbol::Symbol & sym, Info & info);

    Info & setDefaultData(const symbol::Symbol & sym);
    void merge(tools::SymbolMap<Info> & M, tools::SymbolMap<Info> & N);
    void pullup(tools::SymbolMap<Info> & M);
    Info & putSymsInScope(const symbol::Symbol & sym, Block * block, Info & info);
    Info & putSymsInScope(const symbol::Symbol & sym);
    Info & putAndClear(const symbol::Symbol & sym, ast::Exp * exp);
    void clone(Info & info, const symbol::Symbol & sym, ast::Exp * exp);

    virtual void addLocal(const symbol::Symbol & sym, const TIType & type, const bool isIntIterator);
    virtual int getTmpId(const TIType & type, const bool isIntIterator);
    virtual void releaseTmp(const int id, ast::Exp * exp);
    virtual Info & addRead(const symbol::Symbol & sym, ast::Exp * exp);
    virtual Info & addWrite(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp);
    virtual Info & addDefine(const symbol::Symbol & sym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp);
    virtual Info & addShare(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp);
    virtual Info & addMacroDef(ast::FunctionDec * dec);
    virtual std::vector<TIType> addCall(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp, uint64_t & functionId);
    virtual Info & addClear(const symbol::Symbol & sym, ast::Exp * exp);

    virtual Block * getDefBlock(const symbol::Symbol & sym, tools::SymbolMap<Info>::iterator & it, const bool global);
    virtual Info & getInfo(const symbol::Symbol & sym);
    virtual Block * addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp);
    virtual void finalize();
    virtual bool requiresAnotherTrip();
    virtual void addGlobal(const symbol::Symbol & sym);
    virtual GVN & getGVN();
    virtual void clone(const symbol::Symbol & sym, ast::Exp * exp);
    virtual void needRefCount(const tools::SymbolSet & set);

    friend std::wostream & operator<<(std::wostream & out, const Block & block);

private:

    void needRefCount(const tools::SymbolSet & set1, const tools::SymbolSet & set2);

};

} // namespace analysis

#endif // __BLOCK_HXX__
