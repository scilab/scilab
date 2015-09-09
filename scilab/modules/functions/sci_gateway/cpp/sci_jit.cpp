/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
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
#include "JITVisitor.hxx"

#include "parser.hxx"
#include "functions_gw.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "debugvisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
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

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_jit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "testJIT", 1);
        return Function::Error;
    }

    // check that arguments are a string
    unsigned int i = 1;
    Location loc;
    ast::exps_t * args = new ast::exps_t();
    args->reserve(in.size() - 1);
    for (const auto arg : in)
    {
        if (!arg->isString() || arg->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "testJIT", i);
            return Function::Error;
        }
        if (i > 1)
        {
            symbol::Symbol sym(arg->getAs<types::String>()->get(0));
            args->emplace_back(new ast::SimpleVar(loc, sym));
        }
        ++i;
    }

    std::vector<types::InternalType *> _out;
    std::vector<types::InternalType *> _in;
    _in.reserve(args->size());
    for (ast::Exp * arg : *args)
    {
	types::InternalType * pIT = symbol::Context::getInstance()->get(static_cast<ast::SimpleVar *>(arg)->getSymbol());
	_in.push_back(pIT);
    }

    const std::wstring name = in[0]->getAs<types::String>()->get(0);
    symbol::Symbol sym(name);
    ast::SimpleVar * var = new ast::SimpleVar(loc, sym);
    ast::CallExp ce(loc, *var, *args);
    
    analysis::AnalysisVisitor analysis;
    jit::JITVisitor jit(analysis);

    analysis.registerFBlockEmittedListener(&jit);

    ce.accept(analysis);

    jit.makeCall(name, _in, _out);
    for (auto pIT : _out)
    {
	out.push_back(pIT);
    }

    return Function::OK;
}
