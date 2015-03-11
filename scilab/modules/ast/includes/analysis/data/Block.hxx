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

#ifndef __BLOCK_HXX__
#define __BLOCK_HXX__

#include <iostream>
#include <map>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "Data.hxx"
#include "Info.hxx"
#include "symbol.hxx"
#include "TIType.hxx"
#include "gvn/GVN.hxx"
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
    std::map<symbol::Symbol, Info> symMap;
    std::vector<Block *> blocks;
    GVN * gvn;
    const unsigned int id;

public:

    enum BlockKind
    {
        NORMAL, LOOP, EXCLUSIVE, MACRO
    };

    Block(DataManager * _dm);
    Block(const unsigned int _id, Block * _parent, ast::Exp * _exp) : dm(_parent->dm), parent(_parent), exp(_exp), gvn(&_parent->getGVN()), id(_id) { }

    virtual ~Block();

    inline std::map<symbol::Symbol, Info> & getMap()
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

    inline static Info & addSym(std::map<symbol::Symbol, Info> & M, const symbol::Symbol & sym, Info & info)
    {
        Data * old = info.data;
        info.data = nullptr;
        Info & i = M.emplace(sym, info).first->second;
        i.data = old;
        info.data = old;

        return i;
    }

    inline ast::Exp * getExp()
    {
        return exp;
    }

    Info & setDefaultData(const symbol::Symbol & sym);
    void merge(std::map<symbol::Symbol, Info> & M, std::map<symbol::Symbol, Info> & N);
    void pullup(std::map<symbol::Symbol, Info> & M);
    Info & putSymsInScope(const symbol::Symbol & sym, Block * block, Info & info);
    Info & putSymsInScope(const symbol::Symbol & sym);
    Info & addRead(const symbol::Symbol & sym, ast::Exp * exp);
    Info & addWrite(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp);
    Info & addDefine(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp);
    Info & addShare(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, ast::Exp * exp);
    Info & addMacroDef(ast::FunctionDec * dec);
    std::vector<TIType> addCall(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp);
    Info & addClear(const symbol::Symbol & sym, ast::Exp * exp);
    Info & putAndClear(const symbol::Symbol & sym, ast::Exp * exp);

    virtual Block * getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it);
    virtual Info & getInfo(const symbol::Symbol & sym);
    virtual Block * addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp);
    virtual void finalize();
    virtual bool requiresAnotherTrip();
    virtual void addGlobal(const symbol::Symbol & sym);

    virtual GVN & getGVN()
    {
        return *gvn;
    }

    friend std::wostream & operator<<(std::wostream & out, const Block & block);
};

} // namespace analysis

#endif // __BLOCK_HXX__
