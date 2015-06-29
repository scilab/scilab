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

#include "AnalysisVisitor.hxx"
#include "data/Block.hxx"
#include "data/DataManager.hxx"

#include <algorithm>

namespace analysis
{

    Block::Block(DataManager * _dm) : dm(_dm), parent(nullptr), exp(nullptr), gvn(&dm->getDefaultGVN()), isReturn(false), id(0) { }

    Block::~Block()
    {
        std::for_each(blocks.begin(), blocks.end(), [](Block * b)
                      {
                          delete b;
                      });
    }

    void Block::addGlobal(const symbol::Symbol & sym)
    {
        dm->addGlobal(sym);
    }

    Info & Block::setDefaultData(const symbol::Symbol & sym)
    {
        Info & i = addSym(sym, new Data(false, sym));
        i.local = Info::Local::INFO_FALSE;
        i.type = DataManager::getSymInScilabContext(getGVN(), sym, i.exists);
        addGlobal(sym);
        dm->registerData(i.data);//, __LINE__, __FILE__);

        return i;
    }

    Info & Block::putSymsInScope(const symbol::Symbol & sym, Block * block, Info & info)
    {
        // We put the sym (which is defined in block) in this block and pull all its companions too.
        Info & i = addSym(sym, info);
        Data * data = i.data;
        // emplace called the Info's copy ctor so a new Data has been allocated
        dm->registerData(data);//, __LINE__, __FILE__);
        // We put all the shared syms in this scope
        if (!data->hasOneOwner())
        {
            for (const auto & _sym : data->sharedSyms)
            {
                if (sym != _sym)
                {
                    Info & _i = block->symMap.find(_sym)->second;
                    Data * old = _i.data;
                    // we set _i.data to nullptr to avoid the data copy when cloning _i
                    _i.data = nullptr;
                    addSym(_sym, _i).data = data;
                    _i.data = old;
                }
            }
        }

        return i;
    }

    Info & Block::putSymsInScope(const symbol::Symbol & sym)
    {
        std::map<symbol::Symbol, Info>::iterator it;
	Block * block = getDefBlock(sym, it, false);
        if (!block)
        {
            Info & info = dm->root->setDefaultData(sym);
            block = dm->root;
	    if (block != this)
	    {
		return putSymsInScope(sym, block, info);
	    }
	    return info;
        }

        if (block != this)
        {
            return putSymsInScope(sym, block, it->second);
        }

        return it->second;
    }

    Block * Block::getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it, const bool global)
    {
        it = symMap.find(sym);
        if (it != symMap.end())
        {
            return this;
        }
        else if (parent)
        {
            // search sym in the previous block
            return parent->getDefBlock(sym, it, global);
        }

        return nullptr;
    }

    Info & Block::getInfo(const symbol::Symbol & sym)
    {
        std::map<symbol::Symbol, Info>::iterator i = symMap.find(sym);
        if (i != symMap.end())
        {
            // The sym has been already used in this block
            return i->second;
        }
        else if (parent)
        {
            // search sym in the previous block
            return parent->getInfo(sym);
        }
        else
        {
            // We are in the root block (parent == nullptr) and the sym doesn't exist here
            // so we can presumed that it exists in the previous scope
            return setDefaultData(sym);
        }
    }

    Info & Block::addRead(const symbol::Symbol & sym, ast::Exp * exp)
    {
        /* READ:
           - no type modification
           - no refcount modification
        */
        Info & info = getInfo(sym);
        info.R = true;

        return info;
    }

    Info & Block::addWrite(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp)
    {
        /* WRITE:
           - TODO: if x is scalar and x(2)=3 then x is a matrix !
           - type can be modified: a=1:3; a(1)=%i or $; a is typed complex or polynomial
           - if associated data is shared then we need to clone it
        */
        Info & info = putSymsInScope(sym);
        if (info.exists)
        {
            if (info.type.type == TIType::Type::DOUBLE)
            {
                if (Rtype.type == TIType::Type::COMPLEX)
                {
                    info.type.type = TIType::Type::COMPLEX;
                }
                else if (Rtype.type == TIType::Type::POLYNOMIAL)
                {
                    info.type.type = TIType::Type::POLYNOMIAL;
                }
            }
        }
        else
        {
            info.type = Rtype;
            info.exists = true;
        }

        if (!info.data->hasOneOwner())
        {
            // data is shared between several symbols => we need to clone it
            info.data->rem(sym);
            info.data = new Data(info.isknown(), sym);
            dm->registerData(info.data);//, __LINE__, __FILE__);
        }
        info.W = true;

        return info;
    }

    void Block::addLocal(const symbol::Symbol & sym, const TIType & type, const bool isAnInt)
    {
	if (parent)
	{
	    parent->addLocal(sym, type, isAnInt);
	}
    }

    int Block::getTmpId(const TIType & type, const bool isAnInt)
    {
	if (parent)
	{
	    return parent->getTmpId(type, isAnInt);
	}

	return -1;
    }
    
    void Block::releaseTmp(const int id)
    {
	if (parent)
	{
	    parent->releaseTmp(id);
	}
    }

    Info & Block::addDefine(const symbol::Symbol & sym, const TIType & Rtype, const bool isIntIterator, ast::Exp * exp)
    {
        /* DEFINE:
           - if associated data is shared then we need to clone it
        */
	addLocal(sym, Rtype, isIntIterator);
        Info & info = putAndClear(sym, exp);
        info.cleared = false;
        info.data = new Data(true, sym);
        info.type = Rtype;
        info.exists = true;
        dm->registerData(info.data);//, __LINE__, __FILE__);

        return info;
    }

    Info & Block::addShare(const symbol::Symbol & Lsym, const symbol::Symbol & Rsym, const TIType & Rtype, ast::Exp * exp)
    {
	addLocal(Lsym, Rtype, /* isIntIterator */ false);
        Info & Linfo = putAndClear(Lsym, exp);
        Info & Rinfo = putSymsInScope(Rsym);
        Linfo.cleared = false;
        Linfo.type = Rtype;
        Linfo.data = Rinfo.data;
        Linfo.isint = Rinfo.isint;
        Linfo.data->add(Lsym);
        Linfo.exists = true;

        return Linfo;
    }

    Info & Block::addClear(const symbol::Symbol & sym, ast::Exp * exp)
    {
        return putAndClear(sym, exp);
    }

    Info & Block::addMacroDef(ast::FunctionDec * dec)
    {
        TIType ty(getGVN(), TIType::MACRO);
	Info & i = addDefine(dec->getSymbol(), ty, false, dec);
	i.exp = dec;

	return i;
    }

    std::vector<TIType> Block::addCall(AnalysisVisitor & visitor, const unsigned int lhs, const symbol::Symbol & sym, std::vector<TIType> & in, ast::CallExp * callexp)
    {
        std::map<symbol::Symbol, Info>::iterator it;
        Block * block = getDefBlock(sym, it, false);
        types::InternalType * pIT = nullptr;
        std::vector<TIType> out(lhs, TIType(visitor.getGVN()));
        TIType type;
	
        if (block)
        {
            type = it->second.type;
        }
        else
        {
            type = DataManager::getSymInScilabContext(getGVN(), sym, pIT);
        }

        switch (type.type)
        {
        case TIType::FUNCTION:
        {
            if (lhs > 0)
            {
		TIType ty = Checkers::check(getGVN(), sym.getName(), in);
		if (ty.type != TIType::UNKNOWN && ty.hasInvalidDims())
		{
		    out[0] = ty.asUnknownMatrix();
		}
		else
		{
		    out[0] = ty;
		}
            }
            break;
        }
        case TIType::MACRO:
        {
            if (pIT)
            {
                visitor.getPMC().getOutTypes(visitor, dm->getMacroDef(static_cast<types::Macro *>(pIT)), in, out);
            }
            else
            {
                if (it->second.exp && it->second.exp->isFunctionDec())
                {
                    DeclaredMacroDef macrodef(static_cast<ast::FunctionDec *>(it->second.exp));
                    visitor.getPMC().getOutTypes(visitor, &macrodef, in, out);
                }
                else
                {
                    DataManager::getSymInScilabContext(getGVN(), sym, pIT);
                    visitor.getPMC().getOutTypes(visitor, dm->getMacroDef(static_cast<types::Macro *>(pIT)), in, out);
                }
            }
            break;
        }
        case TIType::MACROFILE:
        {
	    DataManager::getSymInScilabContext(getGVN(), sym, pIT);
	    visitor.getPMC().getOutTypes(visitor, dm->getMacroDef(static_cast<types::MacroFile *>(pIT)->getMacro()), in, out);
            break;
        }
        default:
        {
        }
	}

        return out;
    }

    Info & Block::putAndClear(const symbol::Symbol & sym, ast::Exp * exp)
    {
        std::map<symbol::Symbol, Info>::iterator it;
        Block * block = getDefBlock(sym, it, false);
        if (block)
        {
            Info & info = it->second;
            if (block == this)
            {
                if (info.data->hasOneOwner())
                {
                    info.cleared = true;
                    info.local = Info::Local::INFO_TRUE;
                    return info;
                }
                else
                {
                    info.data->rem(sym);
                    info.data = nullptr;
                    info.cleared = true;
                    info.local = Info::Local::INFO_TRUE;
                    return info;
                }
            }
            else
            {
                if (info.data->hasOneOwner())
                {
                    Data * old = info.data;
                    info.data = nullptr;
                    Info & i = addSym(sym, info);
                    info.data = old;
                    i.cleared = true;
                    i.local = Info::Local::INFO_TRUE;
                    return i;
                }
                else
                {
                    Info & i = putSymsInScope(sym, block, info);
                    i.data->rem(sym);
                    i.data = nullptr;
                    i.cleared = true;
                    i.local = Info::Local::INFO_TRUE;
                    return i;
                }
            }
        }
        else
        {
            Info & i = addSym(sym, nullptr);
            i.local = Info::Local::INFO_TRUE;

            return i;
        }
    }

    Block * Block::addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp)
    {
        Block * b;
        switch (kind)
        {
        case NORMAL:
            b = new Block(id, this, exp);
            break;
        case LOOP:
            b = new LoopBlockHead(id, this, exp);
            break;
        case EXCLUSIVE:
            b = new XBlockHead(id, this, exp);
            break;
        case MACRO:
            b = new FunctionBlock(id, this, exp);
            break;
        }
        blocks.push_back(b);

        return b;
    }

    void Block::finalize()
    {
        pullup(symMap);
    }

    bool Block::requiresAnotherTrip()
    {
        return false;
    }

    void Block::merge(std::map<symbol::Symbol, Info> & M, std::map<symbol::Symbol, Info> & N)
    {
        // TODO: when we merge double and double* we should mark the sym to convert the double into a double*
        // and in the LLVM side, make a phi node to set the correct value !
        for (auto & p : M)
        {
            std::map<symbol::Symbol, Info>::iterator i = N.find(p.first);
            if (i != N.end())
            {
                // sym is common to the two maps
                p.second.merge(i->second);
                N.erase(i);
            }
            else
            {
                // sym is in M and not in N
                Info & i = getInfo(p.first);
                p.second.merge(i);
            }
        }

        // We erased common syms in N, so the remainder is the syms which are in N and not in M
        for (auto & p : N)
        {
            Block::addSym(M, p.first, p.second).merge(getInfo(p.first));
        }
    }

    void Block::pullup(std::map<symbol::Symbol, Info> & M)
    {
        if (parent)
        {
            std::map<symbol::Symbol, Info> & map = parent->symMap;
            std::map<symbol::Symbol, Info>::iterator end = map.end();
            for (auto & p : M)
            {
                std::map<symbol::Symbol, Info>::iterator i = map.find(p.first);
                if (i != end)
                {
                    i->second = p.second;
                }
                else
                {
                    Block::addSym(map, p.first, p.second);
                }
            }
        }
    }

    std::wostream & operator<<(std::wostream & out, const Block & block)
    {
        const unsigned int n = block.blocks.size();
        out << L"Table " << block.id;
	if (block.exp)
	{
	    out << L" at " << block.exp->getLocation();
	}
	out << L" (" << n << ((n >= 2) ? L" children):" : L" child):") << std::endl;

        for (const auto & p : block.symMap)
        {
            out << L" -" << p.first << L"  " << p.second << std::endl;
        }
        //#ifdef DEBUG_DATAMANAGER
        //for (const auto b : block.blocks)
        //{

        //}
        std::for_each(block.blocks.begin(), block.blocks.end(), [&](Block * b)
                      {
                          out << *b << std::endl;
                      });
        //#endif

        return out;
    }

} // namespace analysis
