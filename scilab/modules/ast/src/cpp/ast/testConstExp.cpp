/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
#include <assert.h>
#include "doubleexp.hxx"
#include "commentexp.hxx"
#include "floatexp.hxx"
#include "boolexp.hxx"
#include "intexp.hxx"
#include "stringexp.hxx"
#include "printvisitor.hxx"

static void doubleExpTest(void)
{
    double _value = 42.51;
    ast::DoubleExp *_double = new ast::DoubleExp(*new Location(), _value);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "------------------" << std::endl;
    std::cout << "  ast::DoubleExp" << std::endl;
    std::cout << "------------------" << std::endl;

    assert(_double->value_get() == _value);
    _double->accept(printMe);
    std::cout << std::endl;
}

static void commentExpTest(void)
{
    std::string *_comment = new std::string("** This is a comment");
    ast::CommentExp *_commentExp = new ast::CommentExp(*new Location(), _comment);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "-------------------" << std::endl;
    std::cout << "  ast::CommentExp" << std::endl;
    std::cout << "-------------------" << std::endl;

    assert(_commentExp->comment_get() == *_comment);
    _commentExp->accept(printMe);
    std::cout << std::endl;
}

static void stringExpTest(void)
{
    std::string _string = *new std::string("** This is a string : &é'(-è_çà)=^$ù*,;:!");
    ast::StringExp *_stringExp = new ast::StringExp(*new Location(), _string);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "------------------" << std::endl;
    std::cout << "  ast::StringExp" << std::endl;
    std::cout << "------------------" << std::endl;

    assert(_stringExp->value_get() == _string);
    _stringExp->accept(printMe);
    std::cout << std::endl;
}

static void floatExpTest(void)
{
    float _value = 42.51F;
    ast::FloatExp *_float = new ast::FloatExp(*new Location(), _value);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "-----------------" << std::endl;
    std::cout << "  ast::FloatExp" << std::endl;
    std::cout << "-----------------" << std::endl;

    assert(_float->value_get() == _value);
    _float->accept(printMe);
    std::cout << std::endl;
}

static void intExpTest(void)
{
    int _value = 1664;
    ast::IntExp *_iVar8 = new ast::IntExp(*new Location(), ast::IntExp::_8_, _value);
    ast::IntExp *_iVar16 = new ast::IntExp(*new Location(), ast::IntExp::_16_, _value);
    ast::IntExp *_iVar32 = new ast::IntExp(*new Location(), ast::IntExp::_32_, _value);
    ast::IntExp *_iVar64 = new ast::IntExp(*new Location(), ast::IntExp::_64_, _value);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "---------------" << std::endl;
    std::cout << "  ast::IntExp" << std::endl;
    std::cout << "---------------" << std::endl;

    assert(_iVar8->value_get() == _value);
    _iVar8->accept(printMe);
    std::cout << std::endl;
    assert(_iVar16->value_get() == _value);
    _iVar16->accept(printMe);
    std::cout << std::endl;
    assert(_iVar32->value_get() == _value);
    _iVar32->accept(printMe);
    std::cout << std::endl;
    assert(_iVar64->value_get() == _value);
    _iVar64->accept(printMe);
    std::cout << std::endl;
}

static void boolExpTest(void)
{
    bool _true = true;
    bool _false = false;

    ast::BoolExp *_trueExp = new ast::BoolExp(*new Location(), _true);
    ast::BoolExp *_falseExp = new ast::BoolExp(*new Location(), _false);
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "----------------" << std::endl;
    std::cout << "  ast::BoolExp" << std::endl;
    std::cout << "----------------" << std::endl;

    assert(_trueExp->value_get() == _true);
    _trueExp->accept(printMe);
    std::cout << std::endl;
    assert(_falseExp->value_get() == _false);
    _falseExp->accept(printMe);
    std::cout << std::endl;
}

static void nilExpTest(void)
{
    ast::NilExp *_nilExp = new ast::NilExp(*new Location());
    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);

    std::cout << "---------------" << std::endl;
    std::cout << "  ast::NilExp" << std::endl;
    std::cout << "---------------" << std::endl;

    _nilExp->accept(printMe);
    std::cout << std::endl;
}


int main(void)
{
    doubleExpTest();
    floatExpTest();
    intExpTest();
    stringExpTest();
    commentExpTest();
    boolExpTest();
    nilExpTest();
    return 0;
}
