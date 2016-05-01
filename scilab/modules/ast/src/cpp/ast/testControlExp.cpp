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
#include <list>
#include <assert.h>
#include "doubleexp.hxx"
#include "commentexp.hxx"
#include "floatexp.hxx"
#include "boolexp.hxx"
#include "intexp.hxx"
#include "stringexp.hxx"
#include "printvisitor.hxx"

using namespace symbol;
using namespace ast;
using namespace std;

static void WhileExpTest(void)
{

    //while ( not EOF(A) ) do
    //	B = B + readlength(A, 80)
    //end

    Location *poLoc				 	= new Location();

    //A
    Symbol *poSymbA					= new Symbol("A");
    SimpleVar *poVarA				= new SimpleVar(*poLoc, *poSymbA);

    //B
    Symbol *poSymbB					= new Symbol("B");
    SimpleVar *poVarB				= new SimpleVar(*poLoc, *poSymbB);

    //Param list : A
    list<Exp*> lParamEOF;
    lParamEOF.push_back(poVarA);

    //EOF
    Symbol *poSymbEOF				= new Symbol("EOF");
    SimpleVar *poVarEOF			= new SimpleVar(*poLoc, *poSymbEOF);

    //EOF(A)
    CallExp *poEOF					= new CallExp(*poLoc, *poVarEOF, lParamEOF);

    // not EOF(A)
    NotExp *poNotEOF				= new NotExp(*poLoc, *poEOF);

    //80
    IntExp *poInt80					= new IntExp(*poLoc, IntExp::_32_, 80);

    //Param list : A, 80
    list<Exp*> lParamRL;
    lParamRL.push_back(poVarA);
    lParamRL.push_back(poInt80);

    //readline
    Symbol *poSymbRL				= new Symbol("readlength");
    SimpleVar *poVarRL			= new SimpleVar(*poLoc, *poSymbRL);

    //readlength(A, 80)
    CallExp *poRL						= new CallExp(*poLoc, *poVarRL, lParamRL);

    //B + readlength(A, 80)
    OpExp *poBpRL						= new OpExp(*poLoc, *poVarB, OpExp::plus, *poRL);

    //B = B + readlength(A, 80)
    AssignExp *poBeqBpRL		= new AssignExp(*poLoc, *poVarB, *poBpRL);

    //while ( not EOF(A) do B = B + readlength(A,80) end
    WhileExp *poWhile 			= new WhileExp(*poLoc, *poNotEOF, *poBeqBpRL);

    PrintVisitor *printMe 	= new PrintVisitor(cout);

    cout << "-----------------" << endl;
    cout << "  ast::WhileExp  " << endl;
    cout << "-----------------" << endl << endl;

    poWhile->accept(*printMe);

    delete printMe;
    delete poWhile;
    delete poBeqBpRL;
    delete poBpRL;
    delete poRL;
    delete poVarRL;
    delete poSymbRL;

    lParamRL.clear();

    delete poInt80;
    delete poNotEOF;
    delete poEOF;
    delete poVarEOF;
    delete poSymbEOF;

    lParamEOF.clear();

    delete poVarB;
    delete poSymbB;
    delete poVarA;
    delete poSymbA;
    delete poLoc;

    cout << endl << endl;
}

static void BreakExpTest(void)
{
    Location *poLoc1			= new Location();
    BreakExp *poBreak 		= new BreakExp(*poLoc1);
    PrintVisitor *printMe = new PrintVisitor(cout);

    cout << "-----------------" << endl;
    cout << "  ast::BreakExp  " << endl;
    cout << "-----------------" << endl;

    poBreak->accept(*printMe);
    delete printMe;
    delete poBreak;
    delete poLoc1;
}

static void ForExpTest(void)
{

    char *toto							= new char[23567];
    Symbol *poSymb1					= new Symbol("a");
    Symbol *poSymb2					= new Symbol("b");
    Symbol *poSymb3					= new Symbol("c");
    Location *poLoc					= new Location();
    SimpleVar *poVar1				= new SimpleVar(*poLoc, *poSymb1);
    SimpleVar *poVar2				= new SimpleVar(*poLoc, *poSymb2);
    VarDec *poVarDec				= new VarDec(*poLoc, *poSymb3, *poVar1);
    ForExp *poFor						= new ForExp(*poLoc, *poVarDec, *poVar2);
    PrintVisitor *printMe		= new PrintVisitor(cout);

    cout << "---------------" << endl;
    cout << "  ast::ForExp  " << endl;
    cout << "---------------" << endl;

    //test vardec->name ("c")
    assert(poFor->vardec_get().name_get() == poVarDec->name_get());
    //test vardec->init ("a")
    assert((SimpleVar*)&poFor->vardec_get().init_get() == poVar1);
    //test body ("b")
    assert((SimpleVar*)&poFor->body_get() == poVar2);

    cout << "variable = " << ((SimpleVar*)&poFor->vardec_get())->name_get().name_get() << endl;
    cout << "condition = " << ((SimpleVar*)&poFor->vardec_get().init_get())->name_get() << endl;
    cout << "body = " << ((SimpleVar*)&poFor->body_get())->name_get() << endl;

    poFor->accept(*printMe);

    delete printMe;
    delete poFor;
    delete poVarDec;
    delete poVar2;
    delete poVar1;
    delete poLoc;
    delete poSymb3;
    delete poSymb2;
    delete poSymb1;

}

static void IfExpTest(void)
{

    //if(A == bar(B)) then
    //	//B has a good value
    //	C = B'
    //else
    // //A is better then B
    //	C = A'
    //end

    Location *poLoc					= new Location();

    //B
    Symbol *poSymbB					= new Symbol("B");
    SimpleVar *poVarB				= new SimpleVar(*poLoc, *poSymbB);

    //Param list : B
    list<Exp*> lParamBar;
    lParamBar.push_back(poVarB);

    //bar
    Symbol *poSymbBar				= new Symbol("bar");
    SimpleVar *poVarBar			= new SimpleVar(*poLoc, *poSymbBar);

    //bar(B)
    CallExp *poBar					= new CallExp(*poLoc, *poVarBar, lParamBar);

    //A
    Symbol *poSymbA					= new Symbol("A");
    SimpleVar *poVarA				= new SimpleVar(*poLoc, *poSymbA);

    //A == bar(B)
    OpExp *poAeqBarB				= new OpExp(*poLoc, *poVarA, OpExp::eq, *poBar);

    //C
    Symbol *poSymbC					= new Symbol("C");
    SimpleVar *poVarC				= new SimpleVar(*poLoc, *poSymbC);

    //B' ( non conjugate )
    TransposeExp *poTransB	= new TransposeExp(*poLoc, *poVarB, TransposeExp::_NonConjugate_);

    //C = B'
    AssignExp *poCeqBt			= new AssignExp(*poLoc, *poVarC, *poTransB);

    //A' ( non conjugate )
    TransposeExp *poTransA	= new TransposeExp(*poLoc, *poVarA, TransposeExp::_NonConjugate_);

    //C = A'
    AssignExp *poCeqAt			= new AssignExp(*poLoc, *poVarC, *poTransA);

    //if (poAeqBarB) then poCeqBt else poCeqAt
    IfExp *poIf							= new IfExp(*poLoc, *poAeqBarB, *poCeqBt, *poCeqAt);

    PrintVisitor *printMe		= new PrintVisitor(cout);

    std::cout << "--------------" << std::endl;
    std::cout << "  ast::IfExp  " << std::endl;
    std::cout << "--------------" << std::endl << endl;

    poIf->accept(*printMe);

    delete printMe;
    delete poIf;
    delete poCeqAt;
    delete poTransA;
    delete poCeqBt;
    delete poTransB;
    delete poVarC;
    delete poSymbC;
    delete poAeqBarB;
    delete poVarA;
    delete poSymbA;
    delete poBar;
    delete poVarBar;
    delete poSymbBar;

    lParamBar.clear();

    delete poVarB;
    delete poSymbB;
    delete poLoc;

    cout << endl << endl;

}

static void ReturnExpTest(void)
{
    Symbol *poSymb					= new Symbol("a");
    Location *poLoc1				= new Location();
    Location *poLoc2				= new Location();
    Location *poLoc3				= new Location();
    SimpleVar *poVar				= new SimpleVar(*poLoc1, *poSymb);
    ReturnExp *poReturn1		= new ReturnExp(*poLoc2);// retrun
    ReturnExp *poReturn2		= new ReturnExp(*poLoc3, poVar);// retrun a
    PrintVisitor *printMe		= new PrintVisitor(cout);

    std::cout << "------------------" << std::endl;
    std::cout << "  ast::ReturnExp  " << std::endl;
    std::cout << "------------------" << std::endl;

    //test exp
    assert((SimpleVar*)&poReturn2->exp_get() == poVar);

    poReturn1->accept(*printMe);
    poReturn2->accept(*printMe);

    delete printMe;
    delete poReturn2;
    delete poReturn1;
    delete poVar;
    delete poLoc3;
    delete poLoc2;
    delete poLoc1;
    delete poSymb;
}

static void TryCatchExpTest(void)
{
    //try
    // A = B + 1
    // C = foo(A)
    //catch
    //print_error("dummy msg")

    //dummy location
    Location *poLoc				= new Location();

    //A
    Symbol *poSymbA				= new Symbol("A");
    SimpleVar *poVarA			= new SimpleVar(*poLoc, *poSymbA);

    //B
    Symbol *poSymbB				= new Symbol("B");
    SimpleVar *poVarB			= new SimpleVar(*poLoc, *poSymbB);

    //1
    IntExp *poInt1				= new IntExp(*poLoc, IntExp::_32_, 1);

    //B + 1
    OpExp *poBp1					= new OpExp(*poLoc, *poVarB, OpExp::plus, *poInt1);

    //A = B + 1
    AssignExp *poAeqBp1		= new AssignExp(*poLoc, *poVarA, *poBp1);

    //C
    Symbol *poSymbC				= new Symbol("C");
    SimpleVar *poVarC			= new SimpleVar(*poLoc, *poSymbC);

    //Param list : A
    list<Exp*> lParamFoo;
    lParamFoo.push_back(poVarA);

    //foo
    Symbol *poSymbFoo			= new Symbol("foo");
    SimpleVar *poVarFoo		= new SimpleVar(*poLoc, *poSymbFoo);

    //foo(A)
    CallExp *poFoo				= new CallExp(*poLoc, *poVarFoo, lParamFoo);

    //C = foo(A)
    AssignExp *poCeqFooA	= new AssignExp(*poLoc, *poVarC, *poFoo);

    //A = B +1
    //C = foo(A)
    list<Exp*> lTry;
    lTry.push_back(poAeqBp1);
    lTry.push_back(poCeqFooA);

    SeqExp *poSeqTry			= new SeqExp(*poLoc, lTry);

    // "dummy msg"
    StringExp *poStr			= new StringExp(*poLoc, "dummy msg");

    //Param list : str
    list<Exp*> lParamPrint;
    lParamPrint.push_back(poStr);

    //print_error
    Symbol *poSymbPrint		= new Symbol("print_error");
    SimpleVar *poVarPrint	= new SimpleVar(*poLoc, *poSymbPrint);

    //print_error("dummy msg")
    CallExp *poPrint			= new CallExp(*poLoc, *poVarPrint, lParamPrint);

    list<Exp*> lCatch;
    lCatch.push_back(poPrint);

    SeqExp *poSeqCatch		= new SeqExp(*poLoc, lCatch);


    TryCatchExp *poTry		= new TryCatchExp(*poLoc, *poSeqTry, *poSeqCatch);

    PrintVisitor *printMe = new PrintVisitor(cout);

    std::cout << "--------------------" << std::endl;
    std::cout << "  ast::TryCatchExp  " << std::endl;
    std::cout << "--------------------" << std::endl << endl;

    poTry->accept(*printMe);


    delete printMe;
    delete poTry;
    delete poSeqCatch;

    lCatch.clear();

    delete poPrint;
    delete poVarPrint;
    delete poSymbPrint;

    lParamPrint.clear();

    delete poStr;
    delete poSeqTry;

    lTry.clear();

    delete poCeqFooA;
    delete poFoo;
    delete poVarFoo;
    delete poSymbFoo;

    lParamFoo.clear();

    delete poVarC;
    delete poSymbC;
    delete poAeqBp1;
    delete poBp1;
    delete poInt1;
    delete poVarB;
    delete poSymbB;
    delete poVarA;
    delete poSymbA;
    delete poLoc;

    cout << endl << endl;

}

int main(void)
{
    WhileExpTest();
    BreakExpTest();
    ForExpTest();
    IfExpTest();
    ReturnExpTest();
    TryCatchExpTest();
    return 0;
}
