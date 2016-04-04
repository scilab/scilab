/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __LISTVISITOR_HXX__
#define __LISTVISITOR_HXX__

#include "dummyvisitor.hxx"
#include "tlist.hxx"

//used by macr2tree to generate a tlist from an AST

namespace ast
{
class EXTERN_AST TreeVisitor : public DummyVisitor
{
public:
    TreeVisitor() : l(nullptr), eol(nullptr) {}
    ~TreeVisitor()
    {
        eol->killMe();
        l->killMe();
    }

    virtual void visit(const SeqExp  &e);
    virtual void visit(const CommentExp &e);
    virtual void visit(const SimpleVar &e);
    virtual void visit(const DoubleExp &e);
    virtual void visit(const StringExp &e);
    virtual void visit(const BoolExp &e);
    virtual void visit(const DollarVar &e);
    virtual void visit(const ColonVar &e);
    virtual void visit(const MatrixExp &e);
    virtual void visit(const MatrixLineExp &e);
    virtual void visit(const OpExp &e);
    virtual void visit(const LogicalOpExp &e);
    virtual void visit(const IfExp &e);
    virtual void visit(const ListExp &e);
    virtual void visit(const AssignExp &e);
    virtual void visit(const CallExp &e);
    virtual void visit(const ForExp &e);
    virtual void visit(const FieldExp &e);
    virtual void visit(const TryCatchExp  &e);
    virtual void visit(const WhileExp  &e);
    virtual void visit(const ContinueExp &/*e*/);
    virtual void visit(const BreakExp &/*e*/);
    virtual void visit(const ReturnExp &e);
    virtual void visit(const SelectExp &e);
    virtual void visit(const CaseExp &e);
    virtual void visit(const ArrayListExp &e);
    virtual void visit(const NotExp &e);
    virtual void visit(const TransposeExp &e);
    virtual void visit(const FunctionDec &e);

    inline types::List* getList()
    {
        return l;
    }

    static types::List* createVar(const std::wstring& str);
    static types::List* createConst(types::InternalType* pIT);
    static types::List* createOperation();
    static types::List* createAssign();
    static types::InternalType* getVerbose(const Exp& e);

    types::InternalType* getEOL();
private:

    types::List* l;
    types::List* eol;
};
}
#endif /* !__LISTVISITOR_HXX__ */
