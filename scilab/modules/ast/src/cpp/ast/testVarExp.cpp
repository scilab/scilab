/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Antoine ELIAS
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

#include <iostream>
#include <string>
#include <assert.h>
#include "doubleexp.hxx"
#include "commentexp.hxx"
#include "floatexp.hxx"
#include "boolexp.hxx"
#include "intexp.hxx"
#include "stringexp.hxx"
#include "printvisitor.hxx"
#include "symbol.hxx"


using namespace symbol;

static void SimpleVarTest(void)
{
    //	std::string szA = "a";
    Symbol a("a");
    ast::SimpleVar *poVar1	= new ast::SimpleVar(*new Location(), a);
    ast::SimpleVar *poVar2	= new ast::SimpleVar(*new Location(), *new Symbol("1"));
    ast::SimpleVar *poVar3	= new ast::SimpleVar(*new Location(), *new Symbol("é"));
    ast::SimpleVar *poVar4	= new ast::SimpleVar(*new Location(), *new Symbol("a"));
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "------------------" << std::endl;
    std::cout << "  ast::SimpleVar  " << std::endl;
    std::cout << "------------------" << std::endl;



    assert(poVar1->name_get().name_get().compare("a") == 0);
    assert(poVar2->name_get().name_get().compare("1") == 0);
    assert(poVar3->name_get().name_get().compare("é") == 0);
    assert(poVar4->name_get().name_get().compare("a") == 0);

    poVar1->accept(printMe);
    std::cout << std::endl;
    poVar2->accept(printMe);
    std::cout << std::endl;
    poVar3->accept(printMe);
    std::cout << std::endl;
    poVar4->accept(printMe);
    std::cout << std::endl;

}

static void ArrayListVarTest(void)
{
    std::list<ast::Var *> Vars;
    std::list<ast::Var *> *VarCheck;
    ast::SimpleVar *poVar1	= new ast::SimpleVar(*new Location(), *new Symbol("a"));
    ast::SimpleVar *poVar2	= new ast::SimpleVar(*new Location(), *new Symbol("b"));
    ast::SimpleVar *poVar3	= new ast::SimpleVar(*new Location(), *new Symbol("c"));
    ast::SimpleVar *poVar4	= new ast::SimpleVar(*new Location(), *new Symbol("d"));
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "-------------------------" << std::endl;
    std::cout << "  ast::ArrayListVarTest  " << std::endl;
    std::cout << "-------------------------" << std::endl;

    Vars.push_back(poVar1);
    Vars.push_back(poVar2);
    Vars.push_back(poVar3);
    Vars.push_back(poVar4);

    ast::ArrayListVar *poList1 = new ast::ArrayListVar(*new Location(), Vars);
    VarCheck = 	&poList1->vars_get();

    assert(VarCheck->size() == Vars.size());
    std::list<ast::Var*>::iterator theIterator1;
    std::list<ast::Var*>::iterator theIterator2;

    for (theIterator1 = Vars.begin(), theIterator2 = VarCheck->begin() ; theIterator1 != Vars.end() && theIterator2 != VarCheck->end() ; theIterator1++, theIterator2++)
    {
        assert(((ast::SimpleVar*)(*theIterator1))->name_get().name_get().compare(((ast::SimpleVar*)(*theIterator2))->name_get().name_get()) == 0);
    }

    poList1->accept(printMe);
    std::cout << std::endl;
}

static void DollarVarTest(void)
{
    ast::DollarVar *poDollar1 = new ast::DollarVar(*new Location());
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "----------------------" << std::endl;
    std::cout << "  ast::DollarVarTest  " << std::endl;
    std::cout << "----------------------" << std::endl;

    poDollar1->accept(printMe);
    std::cout << std::endl;
}

static void ColonVarTest(void)
{
    ast::ColonVar *poColon1 = new ast::ColonVar(*new Location());
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "---------------------" << std::endl;
    std::cout << "  ast::ColonVarTest  " << std::endl;
    std::cout << "---------------------" << std::endl;

    poColon1->accept(printMe);
    std::cout << std::endl;
}

int main(void)
{
    SimpleVarTest();
    ArrayListVarTest();
    DollarVarTest();
    ColonVarTest();
    return 0;
}
