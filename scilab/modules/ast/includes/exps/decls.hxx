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

/**
 ** \file decls.hxx
 ** \brief Forward declarations of all node-classes of AST
 ** (needed by the visitors)
 */
#ifndef AST_DECLS_HXX
#define AST_DECLS_HXX

#include <list>

namespace ast
{
class Ast;

class Exp;
// Low Level Expressions.

// Const Exp
class StringExp;
class DoubleExp;
class BoolExp;
class CommentExp;
class NilExp;

// Var Exp
class SimpleVar;
class ArrayListVar;
class DollarVar;
class ColonVar;

// Intructions Exp
class CallExp;
class CellCallExp;
class MathExp;
class OpExp;
class LogicalOpExp;
class AssignExp;
class IfExp;
class WhileExp;
class ForExp;
class BreakExp;
class ContinueExp;
class FieldExp;
class TryCatchExp;
class ReturnExp;
class SelectExp;
class CaseExp;

// Single operations expressions
class NotExp;
class TransposeExp;

// List of expressions
class SeqExp;
class ArrayListExp;
class AssignListExp;

// Matrix expression
class MatrixExp;
class MatrixLineExp;

// Cell expression
class CellExp;

// Decs
class VarDec;
class FunctionDec;

// Type dedicated Exp
class ListExp;

class OptimizedExp;
class MemfillExp;
class DAXPYExp;
class FastSelectExp;
class IntSelectExp;
class MapIntSelectExp;
class SmallIntSelectExp;
class StringSelectExp;
class TableIntSelectExp;

} // namespace ast

#endif // !AST_DECLS_HXX
