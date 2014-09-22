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

    virtual void visit (const MatrixExp &e)
    {
        std::list<MatrixLineExp *>::const_iterator i;
        for (i = e.getLines().begin() ; i != e.getLines().end() ; ++i )
        {
            (*i)->accept (*this);
        }
    }

    virtual void visit (const MatrixLineExp &e)
    {
        std::list<Exp *>::const_iterator i;
        for (i = e.getColumns().begin() ; i != e.getColumns().end() ; ++i)
        {
            (*i)->accept (*this);
        }
    }
    /** \} */

    virtual void visit (const CellExp &e)
    {
        std::list<MatrixLineExp *>::const_iterator i;
        for (i = e.getLines().begin() ; i != e.getLines().end() ; ++i )
        {
            (*i)->accept (*this);
        }
    }

    /** \} */

    /** \name Visit Constant Expressions nodes.
     ** \{ */
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
    /** \} */

    /** \name Visit Variable related nodes.
     ** \{ */
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
        std::list<Var *>::const_iterator i;
        for (i = e.getVars().begin() ; i != e.getVars().end() ; ++i)
        {
            (*i)->accept (*this);
        }
    }
    /** \} */

    /** \name Visit Control Expressions or Instructions nodes.
     ** \{ */

    virtual void visit (const FieldExp &e)
    {
        e.getHead()->accept(*this);
        e.getTail()->accept(*this);
    }

    virtual void visit(const OpExp &e)
    {
        e.getLeft().accept(*this);
        //e.oper_get();
        e.getRight().accept(*this);
    }

    virtual void visit(const LogicalOpExp &e)
    {
        e.getLeft().accept(*this);
        //e.oper_get();
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

        std::list<Exp *>::const_iterator i;
        for (i = e.getArgs().begin (); i != e.getArgs().end (); ++i)
        {
            (*i)->accept (*this);
        }
    }

    virtual void visit(const CallExp &e)
    {
        e.getName().accept (*this);

        std::list<Exp *>::const_iterator i;
        for (i = e.getArgs().begin (); i != e.getArgs().end (); ++i)
        {
            (*i)->accept (*this);
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

        ast::cases_t::iterator it;
        for (it = e.getCases()->begin() ; it !=  e.getCases()->end() ; ++it)
        {
            (*it)->accept(*this);
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
        std::list<Exp *>::const_iterator i;
        for (i = e.getExps().begin (); i != e.getExps().end (); ++i)
        {
            (*i)->accept (*this);
        }
    }

    virtual void visit (const ArrayListExp  &e)
    {
        std::list<Exp *>::const_iterator i;
        for (i = e.getExps().begin (); i != e.getExps().end (); ++i)
        {
            (*i)->accept (*this);
        }
    }

    virtual void visit (const AssignListExp  &e)
    {
        std::list<Exp *>::const_iterator i;
        for (i = e.getExps().begin (); i != e.getExps().end (); ++i)
        {
            (*i)->accept (*this);
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
};
}

#endif /* !__DUMMYVISITOR_HXX__ */
