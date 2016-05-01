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
#include "tools.hxx"

namespace analysis
{

class MacroDef
{

protected:

    const unsigned int lhs;
    const unsigned int rhs;
    ast::Exp * const original;

    tools::SymbolOrdSet globals;

public:

    MacroDef(const unsigned int _lhs, const unsigned int _rhs, ast::Exp * _original) : lhs(_lhs), rhs(_rhs), original(_original) { }
    virtual ~MacroDef() { }

    virtual ast::SeqExp & getBody() = 0;
    virtual const ast::SeqExp & getOriginalBody() = 0;
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

    inline tools::SymbolOrdSet & getGlobals()
    {
        return globals;
    }

    inline const tools::SymbolOrdSet & getGlobals() const
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
    std::vector<symbol::Symbol> inputs;
    std::vector<symbol::Symbol> outputs;

public:

    ExistingMacroDef(types::Macro & _macro);
    ExistingMacroDef(const ExistingMacroDef & emd);

    ~ExistingMacroDef() { }

    ast::SeqExp & getBody();
    const ast::SeqExp & getOriginalBody();
    const std::wstring & getName();
    std::vector<symbol::Symbol> getIn();
    std::vector<symbol::Symbol> getOut();
    MacroDef * clone() const;
};

class DeclaredMacroDef : public MacroDef
{

public:

    DeclaredMacroDef(ast::FunctionDec * const _dec);

    ~DeclaredMacroDef() { }

    ast::SeqExp & getBody();
    const ast::SeqExp & getOriginalBody();
    const std::wstring & getName();
    std::vector<symbol::Symbol> getIn();
    std::vector<symbol::Symbol> getOut();
    MacroDef * clone() const;
};

} // namespace analysis

#endif // __MACRO_DEF_HXX__
