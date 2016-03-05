/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

#ifndef AST_SHORTCUTVISITOR_HXX
#define AST_SHORTCUTVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "dummyvisitor.hxx"
#include "logicalopexp.hxx"

namespace ast
{
class ShortCutVisitor : public DummyVisitor
{
public:
    ShortCutVisitor() {}
    ~ShortCutVisitor() {}

    virtual void visit (const SimpleVar &/*e*/) {}
    virtual void visit (const DollarVar &/*e*/) {}
    virtual void visit (const ColonVar &/*e*/) {}
    virtual void visit (const ArrayListVar &/*e*/) {}
    virtual void visit (const DoubleExp &/*e*/) {}
    virtual void visit (const BoolExp &/*e*/) {}
    virtual void visit (const StringExp &/*e*/) {}
    virtual void visit (const CommentExp &/*e*/) {}
    virtual void visit (const NilExp &/*e*/) {}
    virtual void visit (const CallExp &/*e*/) {}
    virtual void visit (const CellCallExp &/*e*/) {}
    virtual void visit (const OpExp &/*e*/) {}
    virtual void visit (const AssignExp &/*e*/) {}
    virtual void visit (const IfExp &/*e*/) {}
    virtual void visit (const WhileExp &/*e*/) {}
    virtual void visit (const ForExp &/*e*/) {}
    virtual void visit (const BreakExp &/*e*/) {}
    virtual void visit (const ContinueExp &/*e*/) {}
    virtual void visit (const TryCatchExp &/*e*/) {}
    virtual void visit (const SelectExp &/*e*/) {}
    virtual void visit (const CaseExp &/*e*/) {}
    virtual void visit (const ReturnExp &/*e*/) {}
    virtual void visit (const FieldExp &/*e*/) {}
    virtual void visit (const NotExp &/*e*/) {}
    virtual void visit (const TransposeExp &/*e*/) {}
    virtual void visit (const MatrixExp &/*e*/) {}
    virtual void visit (const MatrixLineExp &/*e*/) {}
    virtual void visit (const CellExp &/*e*/) {}
    virtual void visit (const SeqExp &/*e*/) {}
    virtual void visit (const ArrayListExp &/*e*/) {}
    virtual void visit (const AssignListExp &/*e*/) {}
    virtual void visit (const VarDec &/*e*/) {}
    virtual void visit (const FunctionDec &/*e*/) {}
    virtual void visit (const ListExp &/*e*/) {}

    virtual void visit (const LogicalOpExp &e);

};
}
#endif // !AST_SHORTCUTVISITOR_HXX
