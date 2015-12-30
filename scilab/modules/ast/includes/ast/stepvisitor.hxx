/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __AST_STEPVISITOR_HXX__
#define __AST_STEPVISITOR_HXX__

#include <iostream>

#include "runvisitor.hxx"
#include "printvisitor.hxx"

namespace ast
{
class EXTERN_AST StepVisitor : public RunVisitorT<StepVisitor>
{
    StepVisitor* clone()
    {
        return new StepVisitor();
    }

    void visit(const SeqExp &e)
    {
        for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
        {
            PrintVisitor *pv = new PrintVisitor(std::cerr);
            (*it)->accept(*pv);
            std::wcerr << std::endl;
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

    /* optimized */
    void visit(const OptimizedExp &e)
    {
        visitprivate(e);
    }

    void visit(const MemfillExp &e)
    {
        visitprivate(e);
    }

    void visit(const DAXPYExp &e)
    {
        visitprivate(e);
    }

    void visit(const IntSelectExp &e)
    {
        visitprivate(e);
    }

    void visit(const StringSelectExp &e)
    {
        visitprivate(e);
    }
};
}

#endif /* !__AST_STEPVISITOR_HXX__ */
