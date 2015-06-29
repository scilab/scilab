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
#include "tools.hxx"
#include "Block.hxx"
#include "FunctionBlock.hxx"
#include "XBlock.hxx"
#include "LoopBlock.hxx"
#include "symbol.hxx"
#include "TIType.hxx"
#include "gvn/GVN.hxx"

namespace analysis
{

    class FunctionBlock;

    class DataManager
    {
        friend class Block;

	Block * root;
        Block * current;
        std::vector<Data *> data;
        unsigned int id;
        std::set<symbol::Symbol> globals;
        std::stack<FunctionBlock *> callStack;
	std::unordered_map<types::Macro *, MacroDef *> macroDefCache;
        GVN gvn;

    public:

        DataManager() : id(0)
            {
                current = root = new Block(this);
            }

        ~DataManager()
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

        inline GVN & getGVN()
            {
                return current->getGVN();
            }

        inline GVN & getDefaultGVN()
            {
                return gvn;
            }

	inline MacroDef * getMacroDef(types::Macro * macro)
	    {
		auto i = macroDefCache.find(macro);
		if (i == macroDefCache.end())
		{
		    i = macroDefCache.emplace(macro, new ExistingMacroDef(*macro)).first;
		}
		return i->second;
	    }
	
        inline void addGlobal(const symbol::Symbol & sym)
            {
                globals.emplace(sym);
            }

        inline void registerData(Data * _data, int line = 0, char * file = nullptr)
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

	inline int getTmpId(const TIType & type, const bool isAnInt)
	    {
		return current->getTmpId(type, isAnInt);
	    }
	
	inline void releaseTmp(const int id)
	    {
		current->releaseTmp(id);
	    }
	
        inline Info & read(const symbol::Symbol & sym, ast::Exp * exp)
            {
                return current->addRead(sym, exp);
            }

        inline Info & write(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp)
            {
                return current->addWrite(sym, Rtype, exp);
            }

        inline Info & define(const symbol::Symbol & sym, const TIType & Rtype, const bool isAnInt, ast::Exp * exp)
            {
                return current->addDefine(sym, Rtype, isAnInt, exp);
            }

        inline Info & share(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, ast::Exp * exp)
            {
                return current->addShare(Lsym, Rsym, Rtype, exp);
            }

        inline Info & clear(const symbol::Symbol & sym, ast::Exp * exp)
            {
                return current->addClear(sym, exp);
            }

        inline Info & macrodef(ast::Exp * exp)
            {
                return current->addMacroDef(static_cast<ast::FunctionDec *>(exp));
            }

        inline std::vector<TIType> call(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp)
            {
                return current->addCall(visitor, lhs, sym, in, callexp);
            }

        inline void addBlock(Block::BlockKind kind, ast::Exp * exp)
            {
                current = current->addBlock(++id, kind, exp);
            }

        inline Block * getCurrent()
            {
                return current;
            }

        inline void finalizeBlock()
            {
#ifndef DEBUG_DATAMANAGER
                current->finalize();
#endif
                current = current->parent;
            }

        inline bool requiresAnotherTrip()
            {
                return current->requiresAnotherTrip();
            }

        inline void pushFunction(FunctionBlock * fblock)
            {
                callStack.push(fblock);
            }

        inline FunctionBlock * poptopFunction()
            {
                FunctionBlock * fblock = callStack.top();
                callStack.pop();
                return fblock;
            }

        inline FunctionBlock * topFunction()
            {
                if (callStack.empty())
                {
                    return nullptr;
                }
                return callStack.top();
            }

        inline void popFunction()
            {
                callStack.pop();
            }

        inline static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists)
            {
                types::InternalType * pIT;
                return getSymInScilabContext(gvn, sym, exists, pIT);
            }

        inline static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, types::InternalType *& pIT)
            {
                bool exists;
                return getSymInScilabContext(gvn, sym, exists, pIT);
            }

        inline static TIType getSymInScilabContext(GVN & gvn, const symbol::Symbol & sym, bool & exists, types::InternalType *& pIT)
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

        template<typename T>
        inline bool getTypes(std::vector<TIType> & out, const T & syms)
            {
                Block * parent = getCurrent();
                for (const auto & sym : syms)
                {
                    std::map<symbol::Symbol, Info>::iterator it;
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

        inline TIType getType(const symbol::Symbol & sym, const bool global = false)
            {
                Block * parent = getCurrent();
                std::map<symbol::Symbol, Info>::iterator it;
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

        inline Info & getInfo(const symbol::Symbol & sym)
            {
                std::map<symbol::Symbol, Info>::iterator it;
                Block * block = current->getDefBlock(sym, it, false);
                if (block)
                {
                    return it->second;
                }

                // The sym is not in this DataManager... try in the Scilab's context
                return current->setDefaultData(sym);
            }

        friend std::wostream & operator<<(std::wostream & out, const DataManager & dm)
            {
                out << L"DataManager:" << std::endl
                    << L"Globals: ";

                tools::printSet(dm.globals, out);
                out << std::endl << *dm.root;

                return out;
            }
    };
}

#endif // __DATA_MANAGER_HXX__
