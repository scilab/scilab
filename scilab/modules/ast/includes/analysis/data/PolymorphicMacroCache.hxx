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

    bool getOutTypes(AnalysisVisitor & visitor, MacroDef * macrodef, std::vector<TIType> & in, std::vector<TIType> & out, uint64_t & functionId);

    friend std::wostream & operator<<(std::wostream & out, const PolymorphicMacroCache & pmc);

private:

    GVN::Value * getValue(const GVN::Value * value, AnalysisVisitor & visitor, const std::vector<const MultivariatePolynomial *> & polys, const int maxVarId) const;

    static bool getCompleteIn(MacroDef & macrodef, AnalysisVisitor & visitor, const std::vector<TIType> & in, std::vector<TIType> & completeIn);

};

} // namespace analysis

#endif // __POLYMORPHIC_MACRO_CACHE_HXX__
