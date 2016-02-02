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
