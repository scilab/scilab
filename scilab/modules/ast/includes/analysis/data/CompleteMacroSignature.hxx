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

#ifndef __COMPLETE_MACRO_SIGNATURE_HXX__
#define __COMPLETE_MACRO_SIGNATURE_HXX__

#include <set>
#include <vector>

#include "alltypes.hxx"
#include "allexp.hxx"
#include "MacroDef.hxx"
#include "MacroSignature.hxx"
#include "TITypeTuple.hxx"
#include "TITypeSignatureTuple.hxx"

namespace analysis
{

struct MacroOut
{
    TITypeTuple tuple;
    int maxVarId;
};

class AnalysisVisitor;
class DataManager;

/**
 * \struct CompleteMacroSignature
 * \brief a MacroSignature is not enought to determinate a macro
 * Since a macro can use variables not defined in its scope and since constraints can be applied to arguments
 * we need to have a more complete signature.
 */
class CompleteMacroSignature
{
    struct __MPCMO
    {
        MPolyConstraintSet mpConstraints;
	std::set<symbol::Symbol> globalConstants;
        MacroOut out;

        __MPCMO(const MPolyConstraintSet & _mpConstraints, const std::set<symbol::Symbol> & _globalConstants, MacroOut && _out) : mpConstraints(_mpConstraints), globalConstants(_globalConstants), out(_out) { }
    };

    // This map contains the signatures corresponding to the global symbols mapped to out types
    std::vector<__MPCMO> outMap;

public:

    CompleteMacroSignature(int) { }

    const MacroOut * getOutTypes(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values);

    const MacroOut * analyze(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values);

};

} // namespace analysis

#endif // __COMPLETE_MACRO_SIGNATURE_HXX__
