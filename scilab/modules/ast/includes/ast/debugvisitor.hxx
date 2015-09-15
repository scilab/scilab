/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_DEBUGVISITOR_HXX
#define AST_DEBUGVISITOR_HXX

#include <iostream>
#include <sstream>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "alltypes.hxx"

namespace ast
{
class EXTERN_AST DebugVisitor : public GenVisitor<const_kind>
{
public:
    DebugVisitor(std::wostream & my_ostr = std::wcerr) : ostr(&my_ostr) { }

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
    virtual void visit (const SeqExp  &e);
    virtual void visit (const SelectExp  &e);
    virtual void visit (const CaseExp  &e);
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

    template<typename T>
    inline void printInternalType(wostringstream & stream, types::InternalType * pIT)
    {
        T * pT = static_cast<T *>(pIT);
        if (pT)
        {
            const int size = pT->getSize();
            if (size == 0)
            {
                stream << L"[]";
            }
            else if (size == 1)
            {
                stream << pT->get(0);
            }
            else
            {
                stream << L"[";
                const int _size = std::min(4, size);
                for (unsigned int i = 0; i < _size - 1; ++i)
                {
                    stream << pT->get(i) << L",";
                }
                stream << pT->get(_size - 1);
                if (size <= _size)
                {
                    stream << L"]";
                }
                else
                {
                    stream << L"...";
                }
            }
        }
    }

protected:
    std::wostream * ostr;

private:

    void START_NODE(const ast::Ast & e);
    void END_NODE(void);
    void print(wstring str);
    void print(wstring str, const Exp & e);
};
}
#endif // !AST_DEBUGVISITOR_HXX
