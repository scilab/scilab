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

#include "tools.hxx"
#include "AnalysisVisitor.hxx"
#include "data/FunctionBlock.hxx"
#include "data/Data.hxx"
#include "data/DataManager.hxx"
#include "data/Info.hxx"

namespace analysis
{

DataManager::DataManager() : id(0)
{
    current = root = new Block(this);
}

DataManager::~DataManager()
{
    for (const auto d : data)
    {
        delete d;
    }
    delete root;
    for (const auto & p : macroDefCache)
    {
        delete p.second;
    }
}

void DataManager::reset()
{
    for (const auto d : data)
    {
        delete d;
    }
    data.clear();
    delete root;
    current = root = new Block(this);
    globals.clear();
    while (!callStack.empty())
    {
        callStack.pop();
    }
    for (const auto & p : macroDefCache)
    {
        delete p.second;
    }
    macroDefCache.clear();
}

GVN & DataManager::getGVN()
{
    return current->getGVN();
}

GVN & DataManager::getDefaultGVN()
{
    return gvn;
}

MacroDef * DataManager::getMacroDef(types::Macro * macro)
{
    auto i = macroDefCache.find(macro);
    if (i == macroDefCache.end())
    {
        i = macroDefCache.emplace(macro, new ExistingMacroDef(*macro)).first;
    }
    return i->second;
}

void DataManager::addGlobal(const symbol::Symbol & sym)
{
    globals.emplace(sym);
}

void DataManager::registerData(Data * _data, int line, char * file)
{
    if (_data)
    {
        /*              bool ok = true;
                        for (const auto d : data)
                        {
                        if (d == _data)
                        {
                        ok =  false;
                        break;
                        }
                        }

                        std::cout << "ptr added=" << _data << " line:" << line << " file:" << file << " doubloon:" << ok << std::endl;
        */
        data.push_back(_data);
    }
}

int DataManager::getTmpId(const TIType & type, const bool isAnInt)
{
    return current->getTmpId(type, isAnInt);
}

void DataManager::releaseTmp(const int id, ast::Exp * exp)
{
    current->releaseTmp(id, exp);
}

Info & DataManager::read(const symbol::Symbol & sym, ast::Exp * exp)
{
    return current->addRead(sym, exp);
}

Info & DataManager::write(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp)
{
    return current->addWrite(sym, Rtype, exp);
}

Info & DataManager::define(const symbol::Symbol & sym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp)
{
    return current->addDefine(sym, Rtype, isAnInt, exp);
}

Info & DataManager::share(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp)
{
    return current->addShare(Lsym, Rsym, Rtype, isAnInt, exp);
}

Info & DataManager::clear(const symbol::Symbol & sym, ast::Exp * exp)
{
    return current->addClear(sym, exp);
}

Info & DataManager::macrodef(ast::Exp * exp)
{
    return current->addMacroDef(static_cast<ast::FunctionDec *>(exp));
}

std::vector<TIType> DataManager::call(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp, uint64_t & functionId)
{
    return current->addCall(visitor, lhs, sym, in, callexp, functionId);
}

void DataManager::addBlock(Block::BlockKind kind, ast::Exp * exp)
{
    current = current->addBlock(++id, kind, exp);
}

Block * DataManager::getCurrent()
{
    return current;
}

void DataManager::finalizeBlock()
{
#ifndef DEBUG_DATAMANAGER
    current->finalize();
#endif
    current = current->parent;
}

bool DataManager::requiresAnotherTrip()
{
    return current->requiresAnotherTrip();
}

void DataManager::pushFunction(FunctionBlock * fblock)
{
    callStack.push(fblock);
}

FunctionBlock * DataManager::poptopFunction()
{
    FunctionBlock * fblock = callStack.top();
    callStack.pop();
    return fblock;
}

FunctionBlock * DataManager::topFunction()
{
    if (callStack.empty())
    {
        return nullptr;
    }
    return callStack.top();
}

void DataManager::popFunction()
{
    callStack.pop();
}

TIType DataManager::getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists)
{
    types::InternalType * pIT;
    return getSymInScilabContext(gvn, sym, exists, pIT);
}

TIType DataManager::getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, types::InternalType *& pIT)
{
    bool exists;
    return getSymInScilabContext(gvn, sym, exists, pIT);
}

TIType DataManager::getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists, types::InternalType *& pIT)
{
    pIT = symbol::Context::getInstance()->get(sym);
    if (pIT)
    {
        exists = true;
        if (pIT->isGenericType())
        {
            types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
            switch (pIT->getType())
            {
                case types::InternalType::ScilabInt8:
                    return TIType(gvn, TIType::Type::INT8, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabUInt8:
                    return TIType(gvn, TIType::Type::UINT8, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabInt16:
                    return TIType(gvn, TIType::Type::INT16, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabUInt16:
                    return TIType(gvn, TIType::Type::UINT16, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabInt32:
                    return TIType(gvn, TIType::Type::INT32, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabUInt32:
                    return TIType(gvn, TIType::Type::UINT32, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabInt64:
                    return TIType(gvn, TIType::Type::INT64, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabUInt64:
                    return TIType(gvn, TIType::Type::UINT64, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabString:
                    return TIType(gvn, TIType::Type::STRING, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabDouble:
                {
                    types::Double * pDbl = static_cast<types::Double *>(pGT);
                    if (pDbl->isEmpty())
                    {
                        return TIType(gvn, TIType::Type::EMPTY, 0, 0);
                    }
                    else if (pDbl->isComplex())
                    {
                        return TIType(gvn, TIType::Type::COMPLEX, pGT->getRows(), pGT->getCols());
                    }
                    else
                    {
                        return TIType(gvn, TIType::Type::DOUBLE, pGT->getRows(), pGT->getCols());
                    }
                }
                case types::InternalType::ScilabBool:
                    return TIType(gvn, TIType::Type::BOOLEAN, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabPolynom:
                    return TIType(gvn, TIType::Type::POLYNOMIAL, pGT->getRows(), pGT->getCols());
                case types::InternalType::ScilabSparse:
                    return TIType(gvn, TIType::Type::SPARSE, pGT->getRows(), pGT->getCols());
                default:
                    return TIType(gvn, TIType::Type::UNKNOWN, pGT->getRows(), pGT->getCols());
            }
        }
        else if (pIT->isCallable())
        {
            switch (pIT->getType())
            {
                case types::InternalType::ScilabFunction:
                    return TIType(gvn, TIType::Type::FUNCTION);
                case types::InternalType::ScilabMacro:
                    return TIType(gvn, TIType::Type::MACRO);
                case types::InternalType::ScilabMacroFile:
                    return TIType(gvn, TIType::Type::MACROFILE);
                case types::InternalType::ScilabLibrary:
                    return TIType(gvn, TIType::Type::LIBRARY);
            }
        }
    }

    exists = false;
    return TIType(gvn);
}

TIType DataManager::getType(const symbol::Symbol & sym, const bool global)
{
    Block * parent = getCurrent();
    tools::SymbolMap<Info>::iterator it;
    Block * block = parent->getDefBlock(sym, it, global);
    if (block)
    {
        return it->second.type;
    }
    else
    {
        bool exists;
        return DataManager::getSymInScilabContext(getGVN(), sym, exists);
    }
}

Info & DataManager::getInfo(const symbol::Symbol & sym)
{
    tools::SymbolMap<Info>::iterator it;
    Block * block = current->getDefBlock(sym, it, false);
    if (block)
    {
        return it->second;
    }

    // The sym is not in this DataManager... try in the Scilab's context
    return current->setDefaultData(sym);
}

std::wostream & operator<<(std::wostream & out, const DataManager & dm)
{
    out << L"DataManager:" << std::endl
        << L"Globals: ";

    tools::printSet(dm.globals, out);
    out << std::endl << *dm.root;

    return out;
}

} // namespace analysis
