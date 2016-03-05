/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include <string.h>

#include "gvn/TestGVNVisitor.hxx"

#include "parser.hxx"
#include "functions_gw.hxx"
#include "prettyprintvisitor.hxx"
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

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_testGVN(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    ast::Exp * pExp = 0;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "jit" , 1);
        return types::Function::Error;
    }

    if (!in[0]->isString() || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "jit" , 1);
        return types::Function::Error;
    }

    Parser parser;
    parser.parse(in[0]->getAs<types::String>()->get(0));
    if (parser.getExitStatus() != Parser::Succeded)
    {
        char* pst = wide_string_to_UTF8(parser.getErrorMessage());
        Scierror(999, "%s", pst);
        FREE(pst);
        return types::Function::Error;
    }

    pExp = parser.getTree();

    if (!pExp)
    {
        return types::Function::Error;
    }

    analysis::TestGVNVisitor gvn;
    pExp->accept(gvn);
    gvn.print_info();

    types::Struct * pOut = new types::Struct(1, 1);
    std::map<std::wstring, uint64_t> maps = gvn.getSymMap();
    for (const auto & p : maps)
    {
        pOut->addField(p.first);
        pOut->get(0)->set(p.first, new types::Double((double)p.second));
    }

    out.push_back(pOut);

    delete pExp;

    return types::Function::OK;
}
