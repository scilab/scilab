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

#ifndef __POLYMORPHIC_MACRO_CACHE_HXX__
#define __POLYMORPHIC_MACRO_CACHE_HXX__

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "allexp.hxx"
#include "CompleteMacroSignature.hxx"
#include "MacroDef.hxx"
#include "MacroSignature.hxx"
#include "FunctionBlock.hxx"
#include "TIType.hxx"

namespace analysis
{

class GVN;
class AnalysisVisitor;

class PolymorphicMacroCache
{
    typedef std::unordered_map<MacroSignature, CompleteMacroSignature, MacroSignature::Hash, MacroSignature::Eq> MacroSignMap;
    uint64_t id;
    MacroSignMap signatures;
    GVN gvn;

public:

    PolymorphicMacroCache();
    ~PolymorphicMacroCache();

    const bool getOutTypes(AnalysisVisitor & visitor, MacroDef * macrodef, std::vector<TIType> & in, std::vector<TIType> & out, uint64_t & functionId);

    friend std::ostream & operator<<(std::ostream & out, const PolymorphicMacroCache & pmc);

private:

    GVN::Value * getValue(const GVN::Value * value, AnalysisVisitor & visitor, const std::vector<const MultivariatePolynomial *> & polys, const int maxVarId) const;

    static bool getCompleteIn(MacroDef & macrodef, AnalysisVisitor & visitor, const std::vector<TIType> & in, std::vector<TIType> & completeIn);

};

} // namespace analysis

#endif // __POLYMORPHIC_MACRO_CACHE_HXX__
