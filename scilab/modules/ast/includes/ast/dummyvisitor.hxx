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

#ifndef __DUMMYVISITOR_HXX__
#define __DUMMYVISITOR_HXX__

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

/*
** This class defines a dummy visitor which only purpose is to
** followup itself through the AST.
*/

namespace ast
{
class EXTERN_AST DummyVisitor : public ConstVisitor
{
protected:
    DummyVisitor() {}

    DummyVisitor* clone()
    {
        return new DummyVisitor();
    }

    virtual void visit(const MatrixExp &e)
    {
        exps_t lines = e.getLines();
        for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
        {
            (*it)->accept (*this);
        }
    }

    virtual void visit (const MatrixLineExp &e)
    {
        exps_t columns = e.getColumns();
        for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
        {
            (*it)->accept (*this);
        }
    }

    virtual void visit (const CellExp &e)
    {
        exps_t lines = e.getLines();
        for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
        {
            (*it)->accept (*this);
        }
    }

    virtual void visit (const StringExp &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const CommentExp &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const DoubleExp  &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const BoolExp  &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const NilExp &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const SimpleVar &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const ColonVar &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const DollarVar &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const ArrayListVar &e)
    {
        exps_t vars = e.getVars();
        for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
        {
            (*it)->accept(*this);
        }
    }

    virtual void visit (const FieldExp &e)
    {
        e.getHead()->accept(*this);
        e.getTail()->accept(*this);
    }

    virtual void visit(const OpExp &e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    virtual void visit(const LogicalOpExp &e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    virtual void visit (const AssignExp  &e)
    {
        e.getLeftExp().accept (*this);
        e.getRightExp().accept (*this);
    }

    virtual void visit(const CellCallExp &e)
    {
        e.getName().accept (*this);

        exps_t args = e.getArgs();
        for (auto arg : args)
        {
            arg->accept(*this);
        }
    }

    virtual void visit(const CallExp &e)
    {
        e.getName().accept (*this);

        exps_t args = e.getArgs();
        for (auto arg : args)
        {
            arg->accept(*this);
        }
    }

    virtual void visit (const IfExp  &e)
    {
        e.getTest().accept(*this);
        e.getThen().accept(*this);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
    }

    virtual void visit (const TryCatchExp  &e)
    {
        e.getTry().accept(*this);
        e.getCatch().accept(*this);
    }

    virtual void visit (const WhileExp  &e)
    {
        e.getTest().accept (*this);
        e.getBody().accept (*this);
    }

    virtual void visit (const ForExp  &e)
    {
        e.getVardec().accept(*this);
        e.getBody().accept (*this);
    }

    virtual void visit (const BreakExp &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const ContinueExp &/*e*/)
    {
        // Nothing to follow up ...
    }

    virtual void visit (const ReturnExp &e)
    {
        if (!e.isGlobal())
        {
            e.getExp().accept(*this);
        }
    }

    virtual void visit (const SelectExp &e)
    {
        e.getSelect()->accept(*this);

        exps_t cases = e.getCases();
        for (auto exp : cases)
        {
            exp->accept(*this);
        }

        if (e.getDefaultCase() != NULL)
        {
            e.getDefaultCase()->accept(*this);
        }
    }

    virtual void visit (const CaseExp &e)
    {
        e.getTest()->accept(*this);
        e.getBody()->accept(*this);
    }

    virtual void visit (const SeqExp  &e)
    {
        for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
        {
            (*it)->accept(*this);
        }
    }

    virtual void visit (const ArrayListExp  &e)
    {
        for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
        {
            (*it)->accept (*this);
        }
    }

    virtual void visit (const AssignListExp  &e)
    {
        for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
        {
            (*it)->accept (*this);
        }
    }
    /** \} */

    /** \name Visit Single Operation nodes.
     ** \{ */
    virtual void visit (const NotExp &e)
    {
        e.getExp().accept (*this);
    }

    virtual void visit (const TransposeExp &e)
    {
        e.getExp().accept (*this);
    }
    /** \} */

    /** \name Visit Declaration nodes.
     ** \{ */
    /** \brief Visit Var declarations. */
    virtual void visit (const VarDec  &e)
    {
        e.getInit().accept(*this);
    }

    virtual void visit (const FunctionDec  &e)
    {
        e.getArgs().accept(*this);
        e.getReturns().accept(*this);
        e.getBody().accept(*this);
    }
    /** \} */

    /** \name Visit Type dedicated Expressions related node.
     ** \{ */
    virtual void visit(const ListExp &e)
    {
        e.getStart().accept(*this);
        e.getStep().accept(*this);
        e.getEnd().accept(*this);
    }
    /** \} */

    /* optimized */
    virtual void visit(const OptimizedExp &e)
    {
        e.getOriginal()->accept(*this);
    }

    virtual void visit (const DAXPYExp &e)
    {
        e.getOriginal()->accept(*this);
    }

    virtual void visit(const IntSelectExp &e)
    {
        e.getOriginal()->accept(*this);
    }

    virtual void visit(const StringSelectExp &e)
    {
        e.getOriginal()->accept(*this);
    }

    virtual void visit(const MemfillExp &e)
    {
        e.getOriginal()->accept(*this);
    }

};
}

#endif /* !__DUMMYVISITOR_HXX__ */
