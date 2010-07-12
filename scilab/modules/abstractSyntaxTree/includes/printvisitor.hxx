/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_PRINTVISITOR_HXX
# define AST_PRINTVISITOR_HXX

#include <string.h>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "token.hxx"

namespace ast
{
    class EXTERN_AST PrintVisitor : public GenVisitor<const_kind>
    {
    public:
        PrintVisitor(std::wostream& my_ostr) :
          ostr (&my_ostr)
          {
              indent = 0;
              force_parenthesis = true;
          }

          PrintVisitor(std::wostream& my_ostr, bool parenthesis_display) :
          ostr (&my_ostr),
              force_parenthesis (parenthesis_display)
          {
              indent = 0;
          }

          /** \name Visit Matrix Expressions nodes.
          ** \{ */
    public :
        virtual void visit (const MatrixExp &e);
        virtual void visit (const MatrixLineExp &e);
        /** \} */

        /** \name Visit Cell Expressions nodes.
        ** \{ */
    public :
        virtual void visit (const CellExp &e);
        /** \} */

        /** \name Visit Constant Expressions nodes.
        ** \{ */
    public :
        virtual void visit (const StringExp &e);
        virtual void visit (const CommentExp &e);
        virtual void visit (const IntExp  &e);
        virtual void visit (const FloatExp  &e);
        virtual void visit (const DoubleExp  &e);
        virtual void visit (const BoolExp  &e);
        virtual void visit (const NilExp &e);
        /** \} */

        /** \name Visit Variable related nodes.
        ** \{ */
        virtual void visit (const SimpleVar &e);
        virtual void visit (const ColonVar &e);
        virtual void visit (const DollarVar &e);
        virtual void visit (const ArrayListVar &e);
        /** \} */

        /** \name Visit Control Expressions or Instructions nodes.
        ** \{ */
        virtual void visit (const FieldExp &e);
        virtual void visit (const OpExp &e);
        virtual void visit (const LogicalOpExp &e);
        virtual void visit (const AssignExp  &e);
        virtual void visit (const CellCallExp &e);
        virtual void visit (const CallExp &e);
        virtual void visit (const IfExp  &e);
        virtual void visit (const TryCatchExp  &e);
        virtual void visit (const WhileExp  &e);
        virtual void visit (const ForExp  &e);
        virtual void visit (const BreakExp &e);
        virtual void visit (const ReturnExp &e);
        virtual void visit (const SelectExp &e);
        virtual void visit (const CaseExp &e);
        virtual void visit (const SeqExp  &e);
        virtual void visit (const ArrayListExp  &e);
        virtual void visit (const AssignListExp  &e);
        /** \} */

        /** \name Visit Single Operation nodes.
        ** \{ */
        virtual void visit (const NotExp &e);
        virtual void visit (const TransposeExp &e);
        /** \} */

        /** \name Visit Declaration nodes.
        ** \{ */
        /** \brief Visit Var declarations. */
        virtual void visit (const VarDec  &e);
        virtual void visit (const FunctionDec  &e);
        /** \} */

        /** \name Visit Type dedicated Expressions related node.
        ** \{ */
    public:
        virtual void visit(const ListExp &e);
        /** \} */


    /*-------------.
    | Attributes.  |
    `-------------*/
    protected:
        std::wostream   *ostr;
        int             indent;
        bool            force_parenthesis;

        void            apply_indent(void);
        void            enable_force_parenthesis(void);
        void            set_force_parenthesis(bool new_state);
    };
}
#endif // !AST_PRINTVISITOR_HXX
