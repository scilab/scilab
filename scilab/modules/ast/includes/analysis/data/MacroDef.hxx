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

#ifndef __MACRO_DEF_HXX__
#define __MACRO_DEF_HXX__

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "alldec.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"
#include "symbol.hxx"

namespace analysis
{

class MacroDef
{

protected:
    
    const unsigned int lhs;
    const unsigned int rhs;
    ast::Exp * const original;

    std::set<symbol::Symbol> globals;

public:

    MacroDef(const unsigned int _lhs, const unsigned int _rhs, ast::Exp * _original) : lhs(_lhs), rhs(_rhs), original(_original) { }
    virtual ~MacroDef() { }

    virtual ast::SeqExp & getBody() = 0;
    virtual const std::wstring & getName() = 0;
    virtual std::vector<symbol::Symbol> getIn() = 0;
    virtual std::vector<symbol::Symbol> getOut() = 0;
    virtual MacroDef * clone() const = 0;

    inline unsigned int getLhs() const
    {
        return lhs;
    }

    inline unsigned int getRhs() const
    {
        return rhs;
    }

    inline ast::Exp * getOriginal() const
	{
	    return original;
	}
    
    inline std::set<symbol::Symbol> & getGlobals()
    {
        return globals;
    }

    inline const std::set<symbol::Symbol> & getGlobals() const
    {
        return globals;
    }

    inline static std::vector<symbol::Symbol> asVector(const ast::exps_t * const l)
    {
        std::vector<symbol::Symbol> _l;
        _l.reserve(l->size());
        for (const auto var : *l)
        {
            _l.emplace_back(static_cast<ast::SimpleVar * const>(var)->getSymbol());
        }
        return _l;
    }

    inline static std::vector<symbol::Symbol> asVector(const std::list<symbol::Variable *> * const l)
    {
        std::vector<symbol::Symbol> _l;
        _l.reserve(l->size());
        for (const auto var : *l)
        {
            _l.emplace_back(var->getSymbol());
        }
        return _l;
    }
};

class ExistingMacroDef : public MacroDef
{
    const std::wstring name;
    ast::SeqExp * se;
    std::vector<symbol::Symbol> inputs;
    std::vector<symbol::Symbol> outputs;
    
public:

    ExistingMacroDef(types::Macro & _macro);
    ExistingMacroDef(const ExistingMacroDef & emd);

    ~ExistingMacroDef()
	{
	    delete se;
	}
    
    ast::SeqExp & getBody();
    const std::wstring & getName();
    std::vector<symbol::Symbol> getIn();
    std::vector<symbol::Symbol> getOut();
    MacroDef * clone() const;
};

class DeclaredMacroDef : public MacroDef
{
    ast::FunctionDec * dec;

public:

    DeclaredMacroDef(ast::FunctionDec * const _dec);

    ~DeclaredMacroDef()
	{
	    delete dec;
	}

    ast::SeqExp & getBody();
    const std::wstring & getName();
    std::vector<symbol::Symbol> getIn();
    std::vector<symbol::Symbol> getOut();
    MacroDef * clone() const;
};

} // namespace analysis

#endif // __MACRO_DEF_HXX__
