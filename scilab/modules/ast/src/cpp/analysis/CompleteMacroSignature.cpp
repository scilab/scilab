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

#include "AnalysisVisitor.hxx"
#include "data/CompleteMacroSignature.hxx"
#include "data/FunctionBlock.hxx"
#include "data/DataManager.hxx"
#include "data/GlobalsCollector.hxx"

namespace analysis
{
const MacroOut * CompleteMacroSignature::getOutTypes(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values, uint64_t & functionId)
{
    for (const auto & mpcmo : outMap)
    {
        if (mpcmo.verified.check(visitor.getGVN(), values) == InferenceConstraint::RESULT_TRUE && ConstraintManager::checkGlobalConstants(mpcmo.globalConstants))
        {
            for (const auto & set : mpcmo.unverified)
            {
                if (set.check(visitor.getGVN(), values) != InferenceConstraint::RESULT_FALSE)
                {
                    return analyze(visitor, signature, macrodef, dm, rhs, in, values, functionId);
                }
            }
            functionId = mpcmo.id;
            return &mpcmo.out;
        }
    }

    return analyze(visitor, signature, macrodef, dm, rhs, in, values, functionId);
}

const MacroOut * CompleteMacroSignature::analyze(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values, uint64_t & functionId)
{
    if (signature.lhs <= macrodef->getLhs())
    {
        visitor.getLogger().log("Visit macro ", macrodef->getName());
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
        //std::wcerr << fblock << std::endl;
        const auto p = outMap.emplace(id++, fblock.getVerifiedConstraints(), fblock.getUnverifiedConstraints(), fblock.getGlobalConstants(), fblock.getOuts(*this));
        fblock.setFunctionId(p.first->id);
        functionId = p.first->id;
        visitor.emitFunctionBlock(fblock);

        //std::wcerr << *this << std::endl;

        return &p.first->out;
    }

    return nullptr;
}

std::ostream & operator<<(std::ostream & out, const CompleteMacroSignature & cms)
{
    out << "Complete Macro Cache:\n";
    for (const auto & mpcmo : cms.outMap)
    {
        out << " * Verified constraints: " << mpcmo.verified << "\n"
            << " * Unverified constraints: ";
        tools::printSet(mpcmo.unverified, out);
        out << "\n"
            << " * Globals: ";
        tools::printSet(mpcmo.globalConstants, out);
        out << "\n"
            << "   => " << mpcmo.out.tuple << "\n";
    }
    return out;
}

} // namespace analysis
