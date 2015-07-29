/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>

#include "AnalysisVisitor.hxx"

#include "parser.hxx"
#include "functions_gw.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "debugvisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "context.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace std;
using namespace types;
using namespace ast;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_testAnalysis(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "testAnalysis", 1);
        return Function::Error;
    }

    // check that arguments are a string
    unsigned int i = 1;
    Location loc;
    ast::exps_t * args = new exps_t();
    args->reserve(in.size() - 1);
    for (const auto arg : in)
    {
        if (!arg->isString() || arg->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "testAnalysis", i);
            return Function::Error;
        }
        if (i > 1)
        {
            symbol::Symbol sym(arg->getAs<types::String>()->get(0));
            args->emplace_back(new ast::SimpleVar(loc, sym));
        }
        ++i;
    }

    symbol::Symbol sym(in[0]->getAs<types::String>()->get(0));
    ast::SimpleVar * var = new ast::SimpleVar(loc, sym);
    ast::CallExp ce(loc, *var, *args);

    analysis::AnalysisVisitor analysis;
    ce.accept(analysis);

    //analysis.print_info();

    analysis::TIType & t = analysis.getResult().getType();
    Struct * pOut = new Struct(1, 1);
    pOut->addField(L"type");
    pOut->get(0)->set(L"type", new String(analysis::TIType::toString(t.type).c_str()));

    pOut->addField(L"rows");
    if (t.rows.isConstant())
    {
        pOut->get(0)->set(L"rows", new Double(t.rows.getConstant()));
    }
    else
    {
        pOut->get(0)->set(L"rows", new Double(analysis::tools::NaN()));
    }

    pOut->addField(L"cols");
    if (t.cols.isConstant())
    {
        pOut->get(0)->set(L"cols", new Double(t.cols.getConstant()));
    }
    else
    {
        pOut->get(0)->set(L"cols", new Double(analysis::tools::NaN()));
    }
    out.push_back(pOut);

    //ast::DebugVisitor debugMe;
    //pExp->accept(debugMe);

    //ast::PrintVisitor printMe(std::wcout);
    //pExp->accept(printMe);

    return Function::OK;
}
