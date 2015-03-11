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
#include "data/FunctionBlock.hxx"
#include "data/DataManager.hxx"

namespace analysis
{
const bool PolymorphicMacroCache::getOutTypes(AnalysisVisitor & visitor, MacroDef * macrodef, std::vector<TIType> & in, std::vector<TIType> & out)
{
    std::vector<TIType> completeIn = PolymorphicMacroCache::getCompleteIn(*macrodef, visitor.getDM(), in);
    MacroSignature signature(*macrodef, out.size(), completeIn);
    MacroSignMap::iterator i = signatures.find(signature);
    if (i == signatures.end())
    {
        i = signatures.emplace(signature, 0).first;
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

    const MacroOut * mout = cms.getOutTypes(visitor, signature, macrodef, visitor.getDM(), in.size(), completeIn, values);
    if (mout)
    {
        std::vector<TIType>::iterator i = out.begin();
        for (const auto & t : mout->tuple.types)
        {
            *i = t;
            MultivariatePolynomial Rmp = t.rows.getValue()->poly->eval(polys);
            MultivariatePolynomial Cmp = t.cols.getValue()->poly->eval(polys);
            i->rows.setValue(visitor.getGVN().getValue(Rmp));
            i->cols.setValue(visitor.getGVN().getValue(Cmp));
            i->invalidScalar();
        }

        //out.assign(mout->tuple.types.begin(), mout->tuple.types.end());
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TIType> PolymorphicMacroCache::getCompleteIn(MacroDef & macrodef, DataManager & dm, const std::vector<TIType> & in)
{
    std::vector<TIType> types;
    const std::set<symbol::Symbol> & globals = macrodef.getGlobals();
    types.reserve(in.size() + globals.size());
    for (const auto & ty : in)
    {
        types.emplace_back(ty);
    }

    const std::vector<symbol::Symbol> & declaredIn = macrodef.getIn();
    const unsigned int size = declaredIn.size();

    if (in.size() < size)
    {
        // we have less rhs than declared rhs:
        // function foo(x,y,z)...endfunction
        // called with foo(x) so y and z are globals
        for (unsigned int i = in.size(); i < size; ++i)
        {
            types.emplace_back(dm.getType(declaredIn[i]));
        }
    }

    for (const auto & sym : globals)
    {
        types.emplace_back(dm.getType(sym));
    }

    return types;
}


} // namespace analysis
