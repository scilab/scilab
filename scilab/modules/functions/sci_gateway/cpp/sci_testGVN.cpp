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

//#include "gvn/TestGVNVisitor.hxx"

#include "parser.hxx"
#include "functions_gw.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "debugvisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
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
Function::ReturnValue sci_testGVN(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    ast::Exp * pExp = 0;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "jit" , 1);
        return Function::Error;
    }

    if (!in[0]->isString() || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "jit" , 1);
        return Function::Error;
    }

    Parser parser;
    parser.parse(in[0]->getAs<types::String>()->get(0));
    if (parser.getExitStatus() != Parser::Succeded)
    {
        char* pst = wide_string_to_UTF8(parser.getErrorMessage());
        Scierror(999, "%s", pst);
        FREE(pst);
        return Function::Error;
    }

    pExp = parser.getTree();

    if (!pExp)
    {
        return Function::Error;
    }

    //analysis::TestGVNVisitor gvn;
    //pExp->accept(gvn);
    ////gvn.print_info();

    //Struct * pOut = new Struct(1, 1);
    //std::map<std::wstring, uint64_t> maps = gvn.getSymMap();
    //for (const auto & p : maps)
    //{
    //    pOut->addField(p.first);
    //    pOut->get(0)->set(p.first, new Double((double) p.second));
    //}

    //out.push_back(pOut);

    delete pExp;

    return Function::OK;
}
