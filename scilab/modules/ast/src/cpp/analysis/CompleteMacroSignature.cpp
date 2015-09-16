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
#include "data/CompleteMacroSignature.hxx"
#include "data/FunctionBlock.hxx"
#include "data/DataManager.hxx"
#include "data/GlobalsCollector.hxx"

namespace analysis
{
const MacroOut * CompleteMacroSignature::getOutTypes(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values)
{
    for (const auto & mpcmo : outMap)
    {
        if (mpcmo.mpConstraints.check(visitor.getGVN(), values) == InferenceConstraint::Result::RESULT_TRUE && ConstraintManager::checkGlobalConstants(mpcmo.globalConstants))
        {
            return &mpcmo.out;
        }
    }

    return analyze(visitor, signature, macrodef, dm, rhs, in, values);
}

const MacroOut * CompleteMacroSignature::analyze(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values)
{
    if (signature.lhs <= macrodef->getLhs())
    {
        visitor.getLogger().log(L"Visit macro ", macrodef->getName());
        dm.addBlock(Block::MACRO, &macrodef->getBody());
        FunctionBlock & fblock = *static_cast<FunctionBlock *>(dm.getCurrent());
        fblock.setName(macrodef->getName());
        fblock.setLhsRhs(signature.lhs, rhs);
        fblock.setInOut(macrodef, rhs, in);
        fblock.setGlobals(macrodef->getGlobals());
        if (!fblock.addIn(signature.tuple, values))
        {
            dm.finalizeBlock();
            return nullptr;
        }

        fblock.getExp()->accept(visitor);
        dm.finalizeBlock();
        std::wcerr << fblock << std::endl;
        visitor.emitFunctionBlock(fblock);
        outMap.emplace_back(fblock.getConstraints(), fblock.getGlobalConstants(), fblock.getOuts());

        return &outMap.back().out;
    }

    return nullptr;
}

} // namespace analysis
