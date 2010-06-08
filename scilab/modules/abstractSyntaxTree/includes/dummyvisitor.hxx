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
    class DummyVisitor : public ConstVisitor
    {
    protected:
        DummyVisitor() {}

        virtual void visit (const MatrixExp &e)
        {
            std::list<MatrixLineExp *>::const_iterator i;
            for (i = e.lines_get().begin() ; i != e.lines_get().end() ; ++i )
            {
                (*i)->accept (*this);
            }
        }

        virtual void visit (const MatrixLineExp &e)
        {
            std::list<Exp *>::const_iterator i;
            for (i = e.columns_get().begin() ; i != e.columns_get().end() ; ++i)
            {
                (*i)->accept (*this);
            }
        }
        /** \} */

        virtual void visit (const CellExp &e)
        {
            std::list<MatrixLineExp *>::const_iterator i;
            for (i = e.lines_get().begin() ; i != e.lines_get().end() ; ++i )
            {
                (*i)->accept (*this);
            }
        }

        /** \} */

        /** \name Visit Constant Expressions nodes.
         ** \{ */
        virtual void visit (const StringExp &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const CommentExp &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const IntExp  &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const FloatExp  &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const DoubleExp  &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const BoolExp  &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const NilExp &e)
        {
             // Nothing to follow up ...
        }
        /** \} */

        /** \name Visit Variable related nodes.
         ** \{ */
        virtual void visit (const SimpleVar &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const ColonVar &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const DollarVar &e)
        {
            // Nothing to follow up ...
        }

        virtual void visit (const ArrayListVar &e)
        {
            std::list<Var *>::const_iterator i;
            for (i = e.vars_get().begin() ; i != e.vars_get().end() ; ++i)
            {
                (*i)->accept (*this);
            }
        }
        /** \} */

        /** \name Visit Control Expressions or Instructions nodes.
         ** \{ */

        virtual void visit (const FieldExp &e)
        {
            e.head_get()->accept(*this);
            e.tail_get()->accept(*this);
        }

        virtual void visit(const OpExp &e)
        {
            e.left_get().accept(*this);
            //e.oper_get();
            e.right_get().accept(*this);
        }

        virtual void visit(const LogicalOpExp &e)
        {
            e.left_get().accept(*this);
            //e.oper_get();
            e.right_get().accept(*this);
        }

        virtual void visit (const AssignExp  &e)
        {
            e.left_exp_get().accept (*this);
            e.right_exp_get().accept (*this);
        }

        virtual void visit(const CellCallExp &e)
        {
            e.name_get().accept (*this);

            std::list<Exp *>::const_iterator i;
            for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
            {
                (*i)->accept (*this);
            }
        }

        virtual void visit(const CallExp &e)
        {
            e.name_get().accept (*this);

            std::list<Exp *>::const_iterator i;
            for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
            {
                (*i)->accept (*this);
            }
        }

        virtual void visit (const IfExp  &e)
        {
            e.test_get().accept(*this);
            e.then_get().accept(*this);
            if (e.has_else())
            {
                e.else_get().accept(*this);
            }
        }

        virtual void visit (const TryCatchExp  &e)
        {
            e.try_get ().accept(*this);
            e.catch_get ().accept(*this);
        }

        virtual void visit (const WhileExp  &e)
        {
            e.test_get().accept (*this);
            e.body_get().accept (*this);
        }

        virtual void visit (const ForExp  &e)
        {
            e.vardec_get().accept(*this);
            e.body_get().accept (*this);
        }

        virtual void visit (const BreakExp &e)
        {

        }

        virtual void visit (const ReturnExp &e)
        {
            if (!e.is_global())
            {
                e.exp_get().accept(*this);
            }
        }

        virtual void visit (const SelectExp &e)
        {
            e.select_get()->accept(*this);

            ast::cases_t::iterator it;
            for (it = e.cases_get()->begin() ; it !=  e.cases_get()->end() ; ++it)
            {
                (*it)->accept(*this);
            }
            if (e.default_case_get() != NULL)
            {
                e.default_case_get()->accept(*this);
            }
        }

        virtual void visit (const CaseExp &e)
        {
            e.test_get()->accept(*this);
            e.body_get()->accept(*this);
        }

        virtual void visit (const SeqExp  &e)
        {
            std::list<Exp *>::const_iterator i;
            for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
            {
                (*i)->accept (*this);
            }
        }

        virtual void visit (const ArrayListExp  &e)
        {
            std::list<Exp *>::const_iterator i;
            for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
            {
                (*i)->accept (*this);
            }
        }

        virtual void visit (const AssignListExp  &e)
        {
            std::list<Exp *>::const_iterator i;
            for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
            {
                (*i)->accept (*this);
            }
        }
        /** \} */

        /** \name Visit Single Operation nodes.
         ** \{ */
        virtual void visit (const NotExp &e)
        {
            e.exp_get().accept (*this);
        }

        virtual void visit (const TransposeExp &e)
        {
            e.exp_get().accept (*this);
        }
        /** \} */

        /** \name Visit Declaration nodes.
         ** \{ */
        /** \brief Visit Var declarations. */
        virtual void visit (const VarDec  &e)
        {
            e.init_get().accept(*this);
        }

        virtual void visit (const FunctionDec  &e)
        {
            e.args_get().accept(*this);
            e.returns_get().accept(*this);
            e.body_get().accept(*this);
        }
        /** \} */

        /** \name Visit Type dedicated Expressions related node.
         ** \{ */
        virtual void visit(const ListExp &e)
        {
            e.start_get().accept(*this);
            e.step_get().accept(*this);
            e.end_get().accept(*this);
        }
        /** \} */
    };
}

#endif /* !__DUMMYVISITOR_HXX__ */
