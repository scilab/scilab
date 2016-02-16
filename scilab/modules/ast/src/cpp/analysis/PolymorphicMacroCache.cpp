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
#include "data/FunctionBlock.hxx"
#include "data/DataManager.hxx"

namespace analysis
{

PolymorphicMacroCache::PolymorphicMacroCache() : id(1) { }

PolymorphicMacroCache::~PolymorphicMacroCache() { }

bool PolymorphicMacroCache::getOutTypes(AnalysisVisitor & visitor, MacroDef * macrodef, std::vector<TIType> & in, std::vector<TIType> & out, uint64_t & functionId)
{
    // TODO: handle varargin
    if (in.size() > macrodef->getRhs())
    {
        return false;
    }

    std::vector<TIType> completeIn;
    if (!PolymorphicMacroCache::getCompleteIn(*macrodef, visitor, in, completeIn))
    {
        return false;
    }

    MacroSignature signature(*macrodef, out.size(), completeIn);
    MacroSignMap::iterator i = signatures.find(signature);
    if (i == signatures.end())
    {
        i = signatures.emplace(signature, CompleteMacroSignature(gvn, id)).first;
    }

    CompleteMacroSignature & cms = i->second;
    std::vector<GVN::Value *> values;
    std::vector<const MultivariatePolynomial *> polys;
    for (auto & t : completeIn)
    {
        if (t.isscalar())
        {
            GVN::Value * v = t.rows.getValue();
            values.emplace_back(v);
            polys.emplace_back(v->poly);
        }
        else
        {
            GVN::Value * v = t.rows.getValue();
            values.emplace_back(v);
            polys.emplace_back(v->poly);
            v = t.cols.getValue();
            values.emplace_back(v);
            polys.emplace_back(v->poly);
        }
    }

    const MacroOut * mout = cms.getOutTypes(visitor, signature, macrodef, visitor.getDM(), in.size(), completeIn, values, functionId);
    if (mout)
    {
        std::vector<TIType>::iterator i = out.begin();
        for (const auto & t : mout->tuple.types)
        {
            *i = t;
            GVN::Value * Rv = getValue(t.rows.getValue(), visitor, polys, mout->maxVarId);
            GVN::Value * Cv = getValue(t.cols.getValue(), visitor, polys, mout->maxVarId);
            i->rows.setValue(Rv);
            i->cols.setValue(Cv);
            i->invalidScalar();
            ++i;
        }

        //out.assign(mout->tuple.types.begin(), mout->tuple.types.end());
        return true;
    }
    else
    {
        return false;
    }
}

bool PolymorphicMacroCache::getCompleteIn(MacroDef & macrodef, AnalysisVisitor & visitor, const std::vector<TIType> & in, std::vector<TIType> & completeIn)
{
    for (const auto & ty : in)
    {
        if (ty.type == TIType::UNKNOWN)
        {
            return false;
        }
    }

    const tools::SymbolOrdSet & globals = macrodef.getGlobals();
    completeIn.reserve(in.size() + globals.size());
    for (const auto & ty : in)
    {
        completeIn.emplace_back(ty);
    }

    const std::vector<symbol::Symbol> & declaredIn = macrodef.getIn();
    const unsigned int size = declaredIn.size();
    DataManager & dm = visitor.getDM();

    if (in.size() < size)
    {
        // we have less rhs than declared rhs:
        // function foo(x,y,z)...endfunction
        // called with foo(x) so y and z are globals
        for (unsigned int i = in.size(); i < size; ++i)
        {
            completeIn.emplace_back(dm.getType(declaredIn[i]));
        }
    }

    for (const auto & sym : globals)
    {
        TIType ty = dm.getType(sym, /* global */ true);
        completeIn.emplace_back(ty);
        if (ty.type == TIType::UNKNOWN)
        {
            return false;
        }
    }

    return true;
}

GVN::Value * PolymorphicMacroCache::getValue(const GVN::Value * value, AnalysisVisitor & visitor, const std::vector<const MultivariatePolynomial *> & polys, const int maxVarId) const
{
    if (value->poly->containsVarsGEq(maxVarId + 1))
    {
        // we translate variables with an id which could conflict with ids in the poly
        MultivariatePolynomial mp1 = value->poly->translateVariables(visitor.getGVN().getCurrentValue() + 1, maxVarId + 1);
        MultivariatePolynomial mp2 = mp1.eval(polys);
        if (mp2.containsVarsGEq(visitor.getGVN().getCurrentValue() + 1))
        {
            // after evaluation, we have always values coming from the function block GVN
            return visitor.getGVN().getValue();
        }
        else
        {
            return visitor.getGVN().getValue(mp2);
        }
    }
    else
    {
        // the out poly is only an expression of the in values
        MultivariatePolynomial mp = value->poly->eval(polys);
        return visitor.getGVN().getValue(mp);
    }
}

std::wostream & operator<<(std::wostream & out, const PolymorphicMacroCache & pmc)
{
    out << L"Macro cache" << "\n";
    tools::printMap(pmc.signatures, out, true);
    return out;
}

} // namespace analysis
