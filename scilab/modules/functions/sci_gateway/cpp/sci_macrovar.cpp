/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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
#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "macrovarvisitor.hxx"
#include "string.hxx"
#include "list.hxx"
#include "macro.hxx"
#include "macrofile.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::InternalType* createString(std::list<std::wstring>& lst);
void addIn(ast::MacrovarVisitor& pVisit, std::list<symbol::Variable*>* pSym);
void addOut(ast::MacrovarVisitor& pVisit, std::list<symbol::Variable*>* pSym);

types::Function::ReturnValue sci_macrovar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "macrovar" , 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "macrovar" , 1);
        return types::Function::Error;
    }

    types::Macro* pM = NULL;
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabMacro:
        {
            pM = in[0]->getAs<types::Macro>();
            break;
        }
        case types::InternalType::ScilabMacroFile:
        {
            types::MacroFile* pMF = in[0]->getAs<types::MacroFile>();
            pM = pMF->getMacro();
            break;
        }
        default :
            Scierror(999, _("%s: Wrong type for input arguments: macro expected.\n"), "macrovar");
            return types::Function::Error;
    }


    ast::MacrovarVisitor visit;

    addIn(visit, pM->getInputs());
    addOut(visit, pM->getOutputs());

    pM->getBody()->accept(visit);

    types::List* pL = new types::List();

    pL->append(createString(visit.getIn()));
    pL->append(createString(visit.getOut()));
    pL->append(createString(visit.getExternal()));
    pL->append(createString(visit.getCalled()));
    pL->append(createString(visit.getLocal()));

    out.push_back(pL);
    return types::Function::OK;
}


types::InternalType* createString(std::list<std::wstring>& lst)
{
    if (lst.size() == 0)
    {
        return types::Double::Empty();
    }

    types::String* pS = new types::String((int)lst.size(), 1);
    std::list<std::wstring>::iterator it = lst.begin();
    for (int i = 0 ; it != lst.end() ; it++, i++)
    {
        pS->set(i, (*it).c_str());
    }

    return pS;
}

void addOut(ast::MacrovarVisitor& visit, std::list<symbol::Variable*>* pSym)
{
    if (pSym == 0 || pSym->size() == 0)
    {
        return;
    }

    std::list<symbol::Variable*>::iterator it = pSym->begin();
    for (int i = 0 ; it != pSym->end() ; it++, i++)
    {
        visit.addOut((*it)->getSymbol().getName().c_str());
    }
}

void addIn(ast::MacrovarVisitor& visit, std::list<symbol::Variable*>* pSym)
{
    if (pSym == 0 || pSym->size() == 0)
    {
        return;
    }

    std::list<symbol::Variable*>::iterator it = pSym->begin();
    for (int i = 0 ; it != pSym->end() ; it++, i++)
    {
        visit.addIn((*it)->getSymbol().getName().c_str());
    }
}