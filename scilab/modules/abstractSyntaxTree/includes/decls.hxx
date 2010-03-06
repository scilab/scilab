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

/**
 ** \file decls.hxx
 ** \brief Forward declarations of all node-classes of AST
 ** (needed by the visitors)
 */
#ifndef AST_DECLS_HXX
# define AST_DECLS_HXX

# include <list>

namespace ast
{
  class Ast;

  class Exp;
  typedef std::list<Exp *> exps_t;
  // Low Level Expressions.

  // Const Exp
  class IntExp;
  class StringExp;
  class FloatExp;
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
  class MathExp;
  class OpExp;
  class LogicalOpExp;
  class AssignExp;
  class IfExp;
  class WhileExp;
  class ForExp;
  class BreakExp;
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

} // namespace ast

#endif // !AST_DECLS_HXX
