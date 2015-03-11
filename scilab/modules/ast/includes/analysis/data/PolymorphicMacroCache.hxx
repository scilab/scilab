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

#ifndef __MACROINFO_HXX__
#define __MACROINFO_HXX__

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

class AnalysisVisitor;

class PolymorphicMacroCache
{
    typedef std::unordered_map<MacroSignature, CompleteMacroSignature, MacroSignature::Hash, MacroSignature::Eq> MacroSignMap;
    MacroSignMap signatures;

public:

    PolymorphicMacroCache() { }

    const bool getOutTypes(AnalysisVisitor & visitor, MacroDef * macrodef, std::vector<TIType> & in, std::vector<TIType> & out);

    static std::vector<TIType> getCompleteIn(MacroDef & macrodef, DataManager & dm, const std::vector<TIType> & in);
};

} // namespace analysis

#endif // __MACROINFO_HXX__
