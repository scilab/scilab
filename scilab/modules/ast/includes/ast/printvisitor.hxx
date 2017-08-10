/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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

#ifndef AST_PRINTVISITOR_HXX
#define AST_PRINTVISITOR_HXX

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

    PrintVisitor(std::wostream& my_ostr, bool parenthesis_display = true, bool _displayOriginal = true, bool _headerOnly = false) :
        ostr (&my_ostr),
        force_parenthesis (parenthesis_display),
        displayOriginal(_displayOriginal),
        headerOnly(_headerOnly),
        indent(0),
        is_last_matrix_line(false),
        is_last_column_comment(false) { }

    PrintVisitor* clone()
    {
        return new PrintVisitor(*ostr, force_parenthesis, displayOriginal, headerOnly);
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
    virtual void visit (const ContinueExp &e);
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

    /* optimized */
    virtual void visit(const OptimizedExp &e);
    virtual void visit(const MemfillExp &e);
    virtual void visit(const DAXPYExp &e);
    virtual void visit(const IntSelectExp &e);
    virtual void visit(const StringSelectExp &e);
    /** \} */


    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
    std::wostream   *ostr;
    bool            force_parenthesis;
    const bool      displayOriginal;
    const bool      headerOnly;
    int             indent;
    bool            is_last_matrix_line;
    bool            is_last_column_comment;

    void            apply_indent(void);
    void            enable_force_parenthesis(void);
    void            set_force_parenthesis(bool new_state);

private:

    void printString(const std::wstring & value)
    {
        std::wstring::const_iterator it;
        *ostr << SCI_OPEN_STRING;
        for (it = value.begin() ; it != value.end() ; ++it)
        {
            if ( *it == '\'' || *it == '"')
            {
                // ' -> '' and " -> "" in scilab strings
                *ostr << *it;
            }
            *ostr << *it;
        }
        *ostr << SCI_CLOSE_STRING;
    }
};
}
#endif // !AST_PRINTVISITOR_HXX
