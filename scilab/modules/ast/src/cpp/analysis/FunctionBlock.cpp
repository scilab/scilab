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

#include "data/DataManager.hxx"
#include "data/FunctionBlock.hxx"
#include "data/CompleteMacroSignature.hxx"
#include "gvn/ConstraintManager.hxx"

namespace analysis
{

    FunctionBlock::FunctionBlock(const unsigned int id, Block * parent, ast::Exp * exp) : Block(id, parent, exp), constraintManager(*this, dm->topFunction())
    {
        gvn = &fgvn;
        dm->pushFunction(this);
    }

    bool FunctionBlock::addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values)
    {
        std::vector<TITypeSignature>::const_iterator i = tuple.types.begin();
        for (const auto & sym : in)
        {
            Info & info = addSym(sym, new Data(false, sym));
            info.type = TIType(fgvn, i->type, i->scalar);
            dm->registerData(info.data);//, __LINE__, __FILE__);
            ++i;
        }

        for (const auto & sym : globals)
        {
            Info & info = addSym(sym, new Data(false, sym));
            info.type = TIType(fgvn, i->type, i->scalar);
            dm->registerData(info.data);//, __LINE__, __FILE__);
            ++i;
        }
        inValues = values;
	maxVarId = fgvn.getCurrentValue() - 1;

        return true;
    }

    void FunctionBlock::addGlobal(const symbol::Symbol & sym)
    {
        //globals.emplace(sym);
        Block::addGlobal(sym);
    }

    void FunctionBlock::setGlobals(const std::set<symbol::Symbol> & v)
    {
	globals = v;
    }

/*    TITypeSignatureTuple FunctionBlock::getGlobals(std::vector<symbol::Symbol> & v)
    {
        TITypeSignatureTuple tuple;
        std::vector<TITypeSignature> & t = tuple.types;
        t.reserve(globals.size());
        v.reserve(globals.size());

        for (const auto & sym : globals)
        {
            v.emplace_back(sym);
            std::map<symbol::Symbol, Info>::iterator it;
            Block * block = parent->getDefBlock(sym, it);
            if (block)
            {
                t.emplace_back(it->second.type.type, it->second.type.isscalar());
            }
            else
            {
                bool exists;
                TIType ty = DataManager::getSymInScilabContext(fgvn, sym, exists);
                t.emplace_back(ty.type, ty.isscalar());
            }
        }

        return tuple;
    }
*/
    MacroOut FunctionBlock::getOuts()
    {
        MacroOut mo;
	mo.maxVarId = maxVarId;
        std::vector<TIType> & v = mo.tuple.types;
        v.reserve(lhs);
        unsigned int i = 0;
        for (std::vector<symbol::Symbol>::const_iterator s = out.begin(); i < lhs; ++i, ++s)
        {
            std::map<symbol::Symbol, Info>::iterator it;
            Block * block = getDefBlock(*s, it, false);
            if (block == this)
            {
                v.emplace_back(it->second.type);
            }
            else
            {
                addGlobal(*s);
                if (block)
                {
                    v.emplace_back(it->second.type);
                }
                else
                {
                    // TODO: if exists is false then it is an error
                    bool exists;
                    v.emplace_back(DataManager::getSymInScilabContext(fgvn, *s, exists));
                }
            }
        }

        return mo;
    }

    void FunctionBlock::finalize()
    {
        dm->popFunction();
    }

    Info & FunctionBlock::addDefine(const symbol::Symbol & sym, const TIType & Rtype, ast::Exp * exp)
    {
	Info & info = Block::addDefine(sym, Rtype, exp);
	auto i = locals.find(sym);
	if (i == locals.end())
	{
	    i = locals.emplace(sym, std::set<__TypeLocal>()).first;
	}
	if (Rtype.isConstantDims())
	{
	    i->second.emplace(Rtype.type, Rtype.rows.getConstant(), Rtype.cols.getConstant());
	}
	else
	{
	    i->second.emplace(Rtype.type, -1, -1);
	}
	  
        return info;
    }

    Block * FunctionBlock::getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it, const bool global)
    {
	it = symMap.find(sym);
	if (it == symMap.end())
	{
	    if (!global && globals.find(sym) == globals.end())
	    {
		// we have looked for a symbol which is not in this function
		// so we add it and give it an unknown type
		it = symMap.emplace(sym, new Data(false, sym)).first;
		Info & info = it->second;
		info.local = Info::Local::INFO_UNKNOWN;
		info.type = TIType(fgvn);
		dm->registerData(info.data);
	    }
	    else
	    {
		return parent->getDefBlock(sym, it, true);
	    }
	}
	return this;
    }
}
