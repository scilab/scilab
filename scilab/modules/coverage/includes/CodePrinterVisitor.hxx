/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __CODE_PRINTER_VISITOR_HXX__
#define __CODE_PRINTER_VISITOR_HXX__

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "token.hxx"
#include "CodePrinter.hxx"

namespace coverage
{

class CodePrinterVisitor : public ast::GenVisitor<ast::const_kind>
{

    CodePrinter & printer;

public:

    CodePrinterVisitor(CodePrinter & _printer) : printer(_printer) { }

    CodePrinterVisitor * clone()
    {
        return new CodePrinterVisitor(printer);
    }

    virtual void visit(const ast::MatrixExp & e);
    virtual void visit(const ast::MatrixLineExp & e);
    virtual void visit(const ast::CellExp & e);
    virtual void visit(const ast::StringExp & e);
    virtual void visit(const ast::CommentExp & e);
    virtual void visit(const ast::DoubleExp & e);
    virtual void visit(const ast::BoolExp & e);
    virtual void visit(const ast::NilExp & e);
    virtual void visit(const ast::SimpleVar & e);
    virtual void visit(const ast::ColonVar & e);
    virtual void visit(const ast::DollarVar & e);
    virtual void visit(const ast::ArrayListVar & e);
    virtual void visit(const ast::FieldExp & e);
    virtual void visit(const ast::OpExp & e);
    virtual void visit(const ast::LogicalOpExp & e);
    virtual void visit(const ast::AssignExp & e);
    virtual void visit(const ast::CellCallExp & e);
    virtual void visit(const ast::CallExp & e);
    virtual void visit(const ast::IfExp & e);
    virtual void visit(const ast::TryCatchExp & e);
    virtual void visit(const ast::WhileExp & e);
    virtual void visit(const ast::ForExp & e);
    virtual void visit(const ast::BreakExp & e);
    virtual void visit(const ast::ContinueExp & e);
    virtual void visit(const ast::ReturnExp & e);
    virtual void visit(const ast::SelectExp & e);
    virtual void visit(const ast::CaseExp & e);
    virtual void visit(const ast::SeqExp & e);
    virtual void visit(const ast::ArrayListExp & e);
    virtual void visit(const ast::AssignListExp & e);
    virtual void visit(const ast::NotExp & e);
    virtual void visit(const ast::TransposeExp & e);
    virtual void visit(const ast::VarDec & e);
    virtual void visit(const ast::FunctionDec & e);
    virtual void visit(const ast::ListExp & e);
    virtual void visit(const ast::OptimizedExp & e);
    virtual void visit(const ast::DAXPYExp & e);
    virtual void visit(const ast::MemfillExp & e);
    virtual void visit(const ast::IntSelectExp & e);
    virtual void visit(const ast::StringSelectExp & e);

};

} // namespace coverage


#endif // __CODE_PRINTER_VISITOR_HXX__
