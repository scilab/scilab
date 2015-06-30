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
    ExistingMacroDef::ExistingMacroDef(types::Macro & _macro) : MacroDef(_macro.outputs_get()->size(), _macro.inputs_get()->size(), _macro.getBody()), name(_macro.getName()), se(nullptr), inputs(MacroDef::asVector(_macro.inputs_get())), outputs(MacroDef::asVector(_macro.outputs_get()))
    {
        GlobalsCollector::collect(*this);
    }

    ExistingMacroDef::ExistingMacroDef(const ExistingMacroDef & emd) : MacroDef(emd.inputs.size(), emd.outputs.size(), emd.original), name(emd.name), se(emd.se->clone()), inputs(emd.inputs), outputs(emd.outputs)
    {
        GlobalsCollector::collect(*this);
    }

    ast::SeqExp & ExistingMacroDef::getBody()
    {
	se = static_cast<ast::SeqExp *>(original)->clone();
        return *se;
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

    DeclaredMacroDef::DeclaredMacroDef(ast::FunctionDec * const _dec) : MacroDef(_dec->getReturns().getVars().size(), _dec->getArgs().getVars().size(), dec), dec(nullptr)
    {
        GlobalsCollector::collect(*this);
    }

    ast::SeqExp & DeclaredMacroDef::getBody()
    {
	dec = static_cast<ast::FunctionDec *>(original)->clone();
        return static_cast<ast::SeqExp &>(dec->getBody());
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
        return new DeclaredMacroDef(dec ? dec : static_cast<ast::FunctionDec *>(original));
    }

} // namespace analysis
