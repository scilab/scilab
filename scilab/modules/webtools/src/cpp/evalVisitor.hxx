/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __EVALVISITOR_HXX__
#define __EVALVISITOR_HXX__

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "execvisitor.hxx"

/*
** This class defines a read only visitor used to execute
** little scripts without modify the context.
** ie: matrix extraction, struct navigation, ...
*/

namespace ast
{
class EvalVisitor : public ExecVisitor
{
public:
    EvalVisitor(): _iScopeLvl(-1), _strLastError("") {}
    EvalVisitor(int iScopeLvl): _strLastError("")
    {
        _iScopeLvl = iScopeLvl;
    }

    virtual ~EvalVisitor() {}

    EvalVisitor* clone()
    {
        return new EvalVisitor();
    }

    void visit(const SeqExp& e);
    void visit(const SimpleVar& e);
    void visit(const FieldExp& e);
    void visit(const CallExp& e);
    void visit(const ListExp& e);
    void visit(const CellCallExp& e);
    void visit(const NotExp& e); // @ or ~ !

    void visit(const OpExp& e)
    {
        e.getLeft().accept(*this);
        if (getResult())
        {
            e.getRight().accept(*this);
            if (getResult())
            {
                ExecVisitor::visit(e);
            }
        }
    }

    void visit(const LogicalOpExp& e)
    {
        e.getLeft().accept(*this);
        if (getResult())
        {
            e.getRight().accept(*this);
            if (getResult())
            {
                ExecVisitor::visit(e);
            }
        }
    }

    /*** not managed expression **/
    // test(,1)
    void visit(const NilExp& /*e*/)
    {
        clearResult();
    }

    // function declaration
    void visit(const ArrayListVar& /*e*/)
    {
        clearResult();
    }

    void visit(const AssignExp& /*e*/)
    {
        clearResult();
    }

    void visit(const IfExp& /*e*/)
    {
        clearResult();
    }

    void visit(const TryCatchExp& /*e*/)
    {
        clearResult();
    }

    void visit(const WhileExp& /*e*/)
    {
        clearResult();
    }

    void visit(const ForExp& /*e*/)
    {
        clearResult();
    }

    void visit(const BreakExp& /*e*/)
    {
        clearResult();
    }

    void visit(const ContinueExp& /*e*/)
    {
        clearResult();
    }

    void visit(const ReturnExp& /*e*/)
    {
        clearResult();
    }

    void visit(const SelectExp& /*e*/)
    {
        clearResult();
    }

    void visit(const CaseExp& /*e*/)
    {
        clearResult();
    }

    // {1, "e"}
    void visit(const CellExp& /*e*/)
    {
        clearResult();
    }

    // (1,2,3)
    void visit(const ArrayListExp& /*e*/)
    {
        clearResult();
    }

    void visit(const AssignListExp& /*e*/)
    {
        clearResult();
    }

    void visit(const TransposeExp& /*e*/)
    {
        clearResult();
    }

    void visit(const VarDec& /*e*/)
    {
        clearResult();
    }

    void visit(const FunctionDec& /*e*/)
    {
        clearResult();
    }

    const std::string& getLastError()
    {
        return _strLastError;
    }

private:
    int _iScopeLvl;
    std::string _strLastError;
};
}

#endif /* !__EVALVISITOR_HXX__ */
