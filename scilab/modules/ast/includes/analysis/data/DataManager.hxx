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

#ifndef __DATA_MANAGER_HXX__
#define __DATA_MANAGER_HXX__

#include <iostream>
#include <stack>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

//#define DEBUG_DATAMANAGER

#include "alldec.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "symbol.hxx"
#include "TIType.hxx"
#include "gvn/GVN.hxx"
#include "data/Block.hxx"

namespace analysis
{

class AnalysisVisitor;
class FunctionBlock;
class MacroDef;
class Data;
class Info;

class DataManager
{
    friend class Block;

    Block * root;
    Block * current;
    std::vector<Data *> data;
    unsigned int id;
    tools::SymbolSet globals;
    std::stack<FunctionBlock *> callStack;
    std::unordered_map<types::Macro *, MacroDef *> macroDefCache;
    GVN gvn;

public:

    DataManager();

    ~DataManager();

    GVN & getGVN();
    GVN & getDefaultGVN();
    MacroDef * getMacroDef(types::Macro * macro);
    void addGlobal(const symbol::Symbol & sym);
    void registerData(Data * _data, int line = 0, char * file = nullptr);
    int getTmpId(const TIType & type, const bool isAnInt);
    void releaseTmp(const int id, ast::Exp * exp);
    Info & read(const symbol::Symbol & sym, ast::Exp * exp);
    Info & write(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp);
    Info & define(const symbol::Symbol & sym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp);
    Info & share(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp);
    Info & clear(const symbol::Symbol & sym, ast::Exp * exp);
    Info & macrodef(ast::Exp * exp);
    std::vector<TIType> call(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp, uint64_t & functionId);
    void addBlock(Block::BlockKind kind, ast::Exp * exp);
    Block * getCurrent();
    void finalizeBlock();
    bool requiresAnotherTrip();
    void pushFunction(FunctionBlock * fblock);
    FunctionBlock * poptopFunction();
    FunctionBlock * topFunction();
    void popFunction();
    TIType getType(const symbol::Symbol & sym, const bool global = false);
    Info & getInfo(const symbol::Symbol & sym);
    void reset();
    friend std::wostream & operator<<(std::wostream & out, const DataManager & dm);

    template<typename T>
    bool getTypes(std::vector<TIType> & out, const T & syms)
    {
        Block * parent = getCurrent();
        for (const auto & sym : syms)
        {
            tools::SymbolMap<Info>::iterator it;
            Block * block = parent->getDefBlock(sym, it, false);
            if (block)
            {
                Info & i = it->second;
                out.emplace_back(i.type.type, i.type.isscalar());
            }
            else
            {
                // TODO: get type in Scilab context
                return false;
            }
        }

        return true;
    }

    static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists);
    static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, types::InternalType *& pIT);
    static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists, types::InternalType *& pIT);

};
}

#endif // __DATA_MANAGER_HXX__
