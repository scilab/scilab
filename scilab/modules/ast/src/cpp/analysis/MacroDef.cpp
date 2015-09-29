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
ExistingMacroDef::ExistingMacroDef(types::Macro & _macro) : MacroDef(_macro.getOutputs()->size(), _macro.getInputs()->size(), _macro.getBody()), name(_macro.getName()), inputs(MacroDef::asVector(_macro.getInputs())), outputs(MacroDef::asVector(_macro.getOutputs()))
{
    GlobalsCollector::collect(*this);
}

ExistingMacroDef::ExistingMacroDef(const ExistingMacroDef & emd) : MacroDef(emd.inputs.size(), emd.outputs.size(), emd.original), name(emd.name), inputs(emd.inputs), outputs(emd.outputs)
{
    GlobalsCollector::collect(*this);
}

ast::SeqExp & ExistingMacroDef::getBody()
{
    return *static_cast<ast::SeqExp *>(original)->clone();
}

const ast::SeqExp & ExistingMacroDef::getOriginalBody()
{
    return *static_cast<ast::SeqExp *>(original);
}

const std::wstring & ExistingMacroDef::getName()
{
    return name;
}

std::vector<symbol::Symbol> ExistingMacroDef::getIn()
{
    return inputs;
}

std::vector<symbol::Symbol> ExistingMacroDef::getOut()
{
    return outputs;
}

MacroDef * ExistingMacroDef::clone() const
{
    return new ExistingMacroDef(*this);
}

DeclaredMacroDef::DeclaredMacroDef(ast::FunctionDec * const _dec) : MacroDef(_dec->getReturns().getVars().size(), _dec->getArgs().getVars().size(), _dec)
{
    GlobalsCollector::collect(*this);
}

ast::SeqExp & DeclaredMacroDef::getBody()
{
    return *static_cast<ast::SeqExp *>(static_cast<ast::FunctionDec *>(original)->getBody().clone());
}

const ast::SeqExp & DeclaredMacroDef::getOriginalBody()
{
    return static_cast<ast::SeqExp &>(static_cast<ast::FunctionDec *>(original)->getBody());
}

const std::wstring & DeclaredMacroDef::getName()
{
    return static_cast<ast::FunctionDec *>(original)->getSymbol().getName();
}

std::vector<symbol::Symbol> DeclaredMacroDef::getIn()
{
    return MacroDef::asVector(&static_cast<ast::FunctionDec *>(original)->getArgs().getVars());
}

std::vector<symbol::Symbol> DeclaredMacroDef::getOut()
{
    return MacroDef::asVector(&static_cast<ast::FunctionDec *>(original)->getReturns().getVars());
}

MacroDef * DeclaredMacroDef::clone() const
{
    return new DeclaredMacroDef(static_cast<ast::FunctionDec *>(original));
}

} // namespace analysis
