/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __GLOBALS_COLLECTOR_HXX__
#define __GLOBALS_COLLECTOR_HXX__

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"
#include "MacroDef.hxx"

namespace analysis
{

class GlobalsCollector : public ast::ConstVisitor, public Chrono
{

    MacroDef & macrodef;
    std::set<symbol::Symbol> locals;
    std::set<symbol::Symbol> & globals;

    GlobalsCollector(MacroDef & _macrodef) : macrodef(_macrodef), globals(_macrodef.getGlobals()) { }

public:

    virtual ~GlobalsCollector() { }

    virtual GlobalsCollector* clone()
    {
        return new GlobalsCollector(macrodef);
    }

    static void collect(MacroDef & macrodef);
    const tools::SymbolOrdSet & getGlobals() const;
    void print_info();

    friend std::wostream & operator<<(std::wostream & out, const GlobalsCollector & gc);

private:

    void collect();
    void visit(const ast::SimpleVar & e);
    void visit(const ast::DollarVar & e);
    void visit(const ast::ColonVar & e);
    void visit(const ast::ArrayListVar & e);
    void visit(const ast::DoubleExp & e);
    void visit(const ast::BoolExp & e);
    void visit(const ast::StringExp & e);
    void visit(const ast::CommentExp & e);
    void visit(const ast::NilExp & e);
    void visit(const ast::CallExp & e);
    void visit(const ast::CellCallExp & e);
    void visit(const ast::OpExp & e);
    void visit(const ast::LogicalOpExp & e);
    void visit(const ast::AssignExp & e);
    void visit(const ast::IfExp & e);
    void visit(const ast::WhileExp & e);
    void visit(const ast::ForExp & e);
    void visit(const ast::BreakExp & e);
    void visit(const ast::ContinueExp & e);
    void visit(const ast::TryCatchExp & e);
    void visit(const ast::SelectExp & e);
    void visit(const ast::CaseExp & e);
    void visit(const ast::ReturnExp & e);
    void visit(const ast::FieldExp & e);
    void visit(const ast::NotExp & e);
    void visit(const ast::TransposeExp & e);
    void visit(const ast::MatrixExp & e);
    void visit(const ast::MatrixLineExp & e);
    void visit(const ast::CellExp & e);
    void visit(const ast::SeqExp & e);
    void visit(const ast::ArrayListExp & e);
    void visit(const ast::AssignListExp & e);
    void visit(const ast::VarDec & e);
    void visit(const ast::FunctionDec & e);
    void visit(const ast::ListExp & e);
    void visit(const ast::OptimizedExp & e);
    void visit(const ast::MemfillExp & e);
    void visit(const ast::DAXPYExp & e);
    void visit(const ast::IntSelectExp & e);
    void visit(const ast::StringSelectExp & e);
};

} // namespace analysis

#endif // __GLOBALS_COLLECTOR_HXX__
