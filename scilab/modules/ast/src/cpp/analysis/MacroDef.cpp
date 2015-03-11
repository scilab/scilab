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

#include "data/MacroDef.hxx"
#include "data/GlobalsCollector.hxx"

namespace analysis
{
ExistingMacroDef::ExistingMacroDef(types::Macro & _macro) : MacroDef(_macro.outputs_get()->size(), _macro.inputs_get()->size()), macro(_macro)
{
    GlobalsCollector::collect(*this);
}

ast::SeqExp & ExistingMacroDef::getBody()
{
    return *macro.getBody();
}

const std::wstring & ExistingMacroDef::getName()
{
    return macro.getName();
}

std::vector<symbol::Symbol> ExistingMacroDef::getIn()
{
    return MacroDef::asVector(macro.inputs_get());
}

std::vector<symbol::Symbol> ExistingMacroDef::getOut()
{
    return MacroDef::asVector(macro.outputs_get());
}

MacroDef * ExistingMacroDef::clone()
{
    return new ExistingMacroDef(macro);
}

DeclaredMacroDef::DeclaredMacroDef(ast::FunctionDec * const _dec) : MacroDef(_dec->getReturns().getVars().size(), _dec->getArgs().getVars().size()), dec(_dec)
{
    GlobalsCollector::collect(*this);
}

ast::SeqExp & DeclaredMacroDef::getBody()
{
    return static_cast<ast::SeqExp &>(dec->getBody());
}

const std::wstring & DeclaredMacroDef::getName()
{
    return dec->getSymbol().getName();
}

std::vector<symbol::Symbol> DeclaredMacroDef::getIn()
{
    return MacroDef::asVector(&dec->getArgs().getVars());
}

std::vector<symbol::Symbol> DeclaredMacroDef::getOut()
{
    return MacroDef::asVector(&dec->getReturns().getVars());
}

MacroDef * DeclaredMacroDef::clone()
{
    return new DeclaredMacroDef(dec);
}

} // namespace analysis
