/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO- Antoine ELIAS
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

#include "core_gw.hxx"
#include "function.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "configvariable.hxx"
#include "treevisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_macr2tree(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected."), "macr2tree", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "macr2tree", 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = in[0];
    if (pIT->isMacro() == false && pIT->isMacroFile() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: macro expected.\n"), "macr2tree", 1);
        return types::Function::Error;
    }

    types::Macro* macro = NULL;
    if (pIT->isMacroFile())
    {
        types::MacroFile* pMF = pIT->getAs<types::MacroFile>();
        macro = pMF->getMacro();
    }
    else
    {
        macro = pIT->getAs<types::Macro>();
    }

    std::list<symbol::Variable*>* outputs = macro->getOutputs();
    std::list<symbol::Variable*>* inputs = macro->getInputs();
    ast::SeqExp* body = macro->getBody();

    //create a tlist "program"
    types::TList* l = new types::TList();
    types::String* s = new types::String(1, 6);
    s->set(0, L"program");
    s->set(1, L"name");
    s->set(2, L"outputs");
    s->set(3, L"inputs");
    s->set(4, L"statements");
    s->set(5, L"nblines");

    //header
    l->append(s);
    //name
    l->append(new types::String(macro->getName().c_str()));

    //outputs
    types::List* o = new types::List();
    for (auto p : *outputs)
    {
        types::List* var = ast::TreeVisitor::createVar(p->getSymbol().getName());
        o->append(var);
        var->killMe();
    }

    l->append(o);
    o->killMe();

    //inputs
    types::List* i = new types::List();
    for (auto p : *inputs)
    {
        types::List* var = ast::TreeVisitor::createVar(p->getSymbol().getName());
        i->append(var);
        var->killMe();
    }

    l->append(i);
    i->killMe();

    //statement
    ast::TreeVisitor v;
    body->accept(v);
    //add fake return
    // is deleted with v
    types::List* statement = v.getList();

    types::TList* funcall = new types::TList();
    types::String* sf = new types::String(1, 4);
    sf->set(0, L"funcall");
    sf->set(1, L"rhs");
    sf->set(2, L"name");
    sf->set(3, L"lhsnb");

    funcall->append(sf);
    funcall->append(types::Double::Empty());
    funcall->append(new types::String(L"return"));
    funcall->append(new types::Double(0));

    statement->append(funcall);
    funcall->killMe();

    statement->append(v.getEOL());

    l->append(statement);

    //nb lines
    l->append(new types::Double(macro->getLastLine() - macro->getFirstLine() + 1));
    out.push_back(l);

    return types::Function::OK;
}
