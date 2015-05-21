/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

using namespace types;
using namespace ast;
using namespace std;

InternalType* createString(std::list<std::wstring>& lst);
void addIn(MacrovarVisitor& pVisit, std::list<symbol::Variable*>* pSym);
void addOut(MacrovarVisitor& pVisit, std::list<symbol::Variable*>* pSym);

Function::ReturnValue sci_macrovar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "macrovar" , 1);
        return Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "macrovar" , 1);
        return Function::Error;
    }

    Macro* pM = NULL;
    switch (in[0]->getType())
    {
        case InternalType::ScilabMacro :
        {
            pM = in[0]->getAs<Macro>();
            break;
        }
        case InternalType::ScilabMacroFile :
        {
            MacroFile* pMF = in[0]->getAs<MacroFile>();
            pM = pMF->getMacro();
            break;
        }
        default :
            Scierror(999, _("%s: Wrong type for input arguments: macro expected.\n"), "macrovar");
            return Function::Error;
    }


    MacrovarVisitor visit;

    addIn(visit, pM->getInputs());
    addOut(visit, pM->getOutputs());

    pM->getBody()->accept(visit);

    List* pL = new List();

    pL->append(createString(visit.getIn()));
    pL->append(createString(visit.getOut()));
    pL->append(createString(visit.getExternal()));
    pL->append(createString(visit.getCalled()));
    pL->append(createString(visit.getLocal()));

    out.push_back(pL);
    return Function::OK;
}


InternalType* createString(std::list<std::wstring>& lst)
{
    if (lst.size() == 0)
    {
        return Double::Empty();
    }

    String* pS = new String((int)lst.size(), 1);
    std::list<std::wstring>::iterator it = lst.begin();
    for (int i = 0 ; it != lst.end() ; it++, i++)
    {
        pS->set(i, (*it).c_str());
    }

    return pS;
}

void addOut(MacrovarVisitor& visit, std::list<symbol::Variable*>* pSym)
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

void addIn(MacrovarVisitor& visit, std::list<symbol::Variable*>* pSym)
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