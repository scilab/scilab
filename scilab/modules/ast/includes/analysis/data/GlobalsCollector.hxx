/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GLOBALS_COLLECTOR_HXX__
#define __GLOBALS_COLLECTOR_HXX__

#include <iostream>
#include <string>
#include <set>
#include <sstream>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"
#include "MacroDef.hxx"

namespace analysis
{

class GlobalsCollector : public ast::Visitor, public Chrono
{

    MacroDef & macrodef;
    std::set<symbol::Symbol> locals;
    std::set<symbol::Symbol> & globals;

    GlobalsCollector(MacroDef & _macrodef) : macrodef(_macrodef), globals(_macrodef.getGlobals()) { }

public:

    virtual ~GlobalsCollector() { }

    static void collect(MacroDef & macrodef);
    const std::set<symbol::Symbol> & getGlobals() const;
    void print_info();

    friend std::wostream & operator<<(std::wostream & out, const GlobalsCollector & gc);

private:

    void collect();
    void visit(ast::SimpleVar & e);
    void visit(ast::DollarVar & e);
    void visit(ast::ColonVar & e);
    void visit(ast::ArrayListVar & e);
    void visit(ast::DoubleExp & e);
    void visit(ast::BoolExp & e);
    void visit(ast::StringExp & e);
    void visit(ast::CommentExp & e);
    void visit(ast::NilExp & e);
    void visit(ast::CallExp & e);
    void visit(ast::CellCallExp & e);
    void visit(ast::OpExp & e);
    void visit(ast::LogicalOpExp & e);
    void visit(ast::AssignExp & e);
    void visit(ast::IfExp & e);
    void visit(ast::WhileExp & e);
    void visit(ast::ForExp & e);
    void visit(ast::BreakExp & e);
    void visit(ast::ContinueExp & e);
    void visit(ast::TryCatchExp & e);
    void visit(ast::SelectExp & e);
    void visit(ast::CaseExp & e);
    void visit(ast::ReturnExp & e);
    void visit(ast::FieldExp & e);
    void visit(ast::NotExp & e);
    void visit(ast::TransposeExp & e);
    void visit(ast::MatrixExp & e);
    void visit(ast::MatrixLineExp & e);
    void visit(ast::CellExp & e);
    void visit(ast::SeqExp & e);
    void visit(ast::ArrayListExp & e);
    void visit(ast::AssignListExp & e);
    void visit(ast::VarDec & e);
    void visit(ast::FunctionDec & e);
    void visit(ast::ListExp & e);
    void visit(ast::OptimizedExp & e);
    void visit(ast::MemfillExp & e);
    void visit(ast::DAXPYExp & e);
    void visit(ast::IntSelectExp & e);
    void visit(ast::StringSelectExp & e);
};

} // namespace analysis

#endif // __GLOBALS_COLLECTOR_HXX__
