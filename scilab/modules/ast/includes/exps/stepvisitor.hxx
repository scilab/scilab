/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __AST_STEPVISITOR_HXX__
#define __AST_STEPVISITOR_HXX__

#include <iostream>

#include "runvisitor.hxx"
#include "execvisitor.hxx"
#include "printvisitor.hxx"

namespace ast
{
class StepVisitor : public RunVisitorT<StepVisitor>
{
    void visit(const SeqExp &e)
    {
        std::list<Exp*>::const_iterator it;
        for (it = e.exps_get().begin() ; it != e.exps_get().end() ; ++it)
        {
            PrintVisitor *pv = new PrintVisitor(std::wcerr);
            (*it)->accept(*pv);
            std::wcerr << std::endl;
            //ExecVisitor *pe = new ast::ExecVisitor();
            //(*it)->accept(*pe);
        }
        visitprivate(e);
    }

    void visit (const MatrixExp &e)
    {
        visitprivate(e);
    }

    void visit (const MatrixLineExp &e)
    {
        visitprivate(e);
    }

    void visit (const CellExp &e)
    {
        visitprivate(e);
    }

    void visit (const StringExp &e)
    {
        visitprivate(e);
    }

    void visit (const CommentExp &e)
    {
        visitprivate(e);
    }

    void visit (const DoubleExp &e)
    {
        visitprivate(e);
    }

    void visit (const BoolExp &e)
    {
        visitprivate(e);
    }

    void visit (const NilExp &e)
    {
        visitprivate(e);
    }

    void visit (const SimpleVar &e)
    {
        visitprivate(e);
    }

    void visit (const ColonVar &e)
    {
        visitprivate(e);
    }

    void visit (const DollarVar &e)
    {
        visitprivate(e);
    }

    void visit (const ArrayListVar &e)
    {
        visitprivate(e);
    }

    void visit (const FieldExp &e)
    {
        visitprivate(e);
    }

    void visit (const OpExp &e)
    {
        visitprivate(e);
    }

    void visit (const LogicalOpExp &e)
    {
        visitprivate(e);
    }

    void visit (const AssignExp &e)
    {
        visitprivate(e);
    }

    void visit (const CellCallExp &e)
    {
        visitprivate(e);
    }

    void visit (const CallExp &e)
    {
        visitprivate(e);
    }

    void visit (const IfExp &e)
    {
        visitprivate(e);
    }

    void visit (const TryCatchExp &e)
    {
        visitprivate(e);
    }

    void visit (const WhileExp &e)
    {
        visitprivate(e);
    }

    void visit (const ForExp &e)
    {
        visitprivate(e);
    }

    void visit (const BreakExp &e)
    {
        visitprivate(e);
    }

    void visit (const ContinueExp &e)
    {
        visitprivate(e);
    }

    void visit (const ReturnExp &e)
    {
        visitprivate(e);
    }

    void visit (const SelectExp &e)
    {
        visitprivate(e);
    }

    void visit (const CaseExp &e)
    {
        visitprivate(e);
    }

    void visit (const ArrayListExp &e)
    {
        visitprivate(e);
    }

    void visit (const AssignListExp &e)
    {
        visitprivate(e);
    }

    void visit (const NotExp &e)
    {
        visitprivate(e);
    }

    void visit (const TransposeExp &e)
    {
        visitprivate(e);
    }

    void visit (const VarDec &e)
    {
        visitprivate(e);
    }

    void visit (const FunctionDec &e)
    {
        visitprivate(e);
    }

    void visit(const ListExp &e)
    {
        visitprivate(e);
    }
};
}

#endif /* !__AST_STEPVISITOR_HXX__ */
