(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

(* A direct translation of the class hierarchy of modules/ast/includes/ *)

(* Questions: what are the Big... types ? *)


type location = {
  first_line : int;
  first_column : int;
  last_line : int;
  last_column : int;
}

module Location = struct

  type t = location

end

(* Now, we use ScilabContext.symbol instead of Symbol.t
module Symbol = struct
  type t = {
    symbol_name : string;
    symbol_binding : ScilabContext.binding;
  }
end
*)

module BigBool = struct
  type t = unit (* what is a BigBool ? *)
end

module BigDouble = struct
  type t = unit (* what is a BigDouble ? *)
end

module BigString = struct
  type t = unit (* what is a BigString ? *)
end

type ast =
  Decs of decs
| Exp of exp

and decs = {
  desc_location : Location.t;
  decs_decs : dec list;
}

and dec =
| VarDec of varDec
(* | TypeDec  UNUSED ??? *)
| FunctionDec of functionDec

and varDec = {
  varDec_name : ScilabContext.symbol;
  varDec_init : exp;
  varDec_kind : varDec_Kind;
}

and varDec_Kind =
  VarDec_invalid_kind
| VarDec_evaluation_kind
| VarDec_assignment_kind

and functionDec =  {
  functionDec_location : Location.t;
  functionDec_symbol : ScilabContext.symbol;
  functionDec_args : arrayListVar;
  functionDec_returns : arrayListVar;
  functionDec_body : exp;
}

and exp = {
  exp_location : Location.t;
  exp_desc : exp_desc;
  exp_info : exp_info;
}

and exp_info = {
  is_verbose : bool;
  is_break : bool;
  is_breakable : bool;
  is_return : bool;
  is_returnable : bool;
  is_continue : bool;
  is_continuable  : bool;
}

and exp_desc =
  AssignExp of assignExp
| CallExp of callExp
| CellCallExp of callExp
| ConstExp of constExp
| ControlExp of controlExp
| Dec of dec
| FieldExp of fieldExp
| ListExp of listExp
| MathExp of mathExp
| Var of var

| SeqExp of seqExp
| ArrayListExp of exp array
| AssignListExp of exp array

and seqExp = exp list

and assignExp = {
  assignExp_left_exp : exp;
  assignExp_right_exp : exp;
}

and callExp = {
  callExp_name : exp;
  callExp_args : exp array;
}

and constExp =
| BoolExp of boolExp
| CommentExp of commentExp
| DoubleExp of doubleExp
| FloatExp of floatExp
| IntExp of intExp
| NilExp
| StringExp of stringExp

and boolExp = {
  boolExp_value : bool;
  boolExp_bigBool : BigBool.t;
}

and commentExp = {
  commentExp_comment : string;
}

and doubleExp = {
  doubleExp_value : float;
  doubleExp_bigDouble : BigDouble.t;
}

and floatExp = {
  floatExp_value : float; (* warning: float is double in OCaml *)
}

and intExp = {
  intExp_value : int32;
  intExp_prec : intExp_Prec;
}

and intExp_Prec =
|  IntExp_8
|  IntExp_16
|  IntExp_32
|  IntExp_64

and stringExp = {
  stringExp_value : string;
  stringExp_bigString : BigString.t;
}

and controlExp =
| BreakExp
(* | CaseExp -> moved to selectExp *)
| ContinueExp
| ForExp of forExp
| IfExp of ifExp
| ReturnExp of returnExp
| SelectExp of selectExp
| TryCatchExp of tryCatchExp
| WhileExp of whileExp

and selectExp = {
  selectExp_selectme : exp;
  selectExp_cases : caseExp array;
  selectExp_default : (Location.t * seqExp) option;
}

and caseExp = {
  caseExp_location : Location.t;
  caseExp_test : exp;
  caseExp_body_location : Location.t;
  caseExp_body : seqExp;
}

and forExp = {
  forExp_vardec_location : Location.t;
  forExp_vardec : varDec;
  forExp_body : exp;
}

and ifExp = {
  ifExp_test : exp;
  ifExp_then : exp;
  ifExp_else : exp option;
  ifExp_kind : ifExp_Kind;
}

and ifExp_Kind =
  IfExp_invalid_kind
| IfExp_instruction_kind
| IfExp_expression_kind

| IfExp_untyped_kind (* not yet known *)

and returnExp = {
  returnExp_exp : exp option;
}

and tryCatchExp = {
  tryCatchExp_tryme_location : Location.t;
  tryCatchExp_tryme : seqExp;
  tryCatchExp_catchme_location : Location.t;
  tryCatchExp_catchme : seqExp;
}

and whileExp = {
  whileExp_test : exp;
  whileExp_body : exp;
}

and fieldExp = {
  fieldExp_head : exp; (* field name *)
  fieldExp_tail : exp; (* initial value *)
}

and listExp = {
  listExp_start : exp; (* start of the list *)
  listExp_step : exp;  (* step of the list *)
  listExp_end : exp;   (* end of the list *)
}

and mathExp =
| MatrixExp of matrixExp
| CellExp of matrixExp
(* | MatrixLineExp of exp array *)
| NotExp of notExp
| OpExp of opExp_Oper * opExp_args
| LogicalOpExp of opLogicalExp_Oper * opExp_args
| TransposeExp of transposeExp

and matrixExp = {
  matrixExp_lines : matrixLineExp array;
}

and matrixLineExp = {
  matrixLineExp_location : Location.t;
  matrixLineExp_columns : exp array;
}

and notExp = {
  notExp_exp : exp;
}

and opExp_args = {
  opExp_left : exp;
  opExp_right : exp;
  opExp_kind : opExp_Kind;
}

and opExp_Oper =
            (*                       Arithmetics     *)
            (* "+" *)        | OpExp_plus
            (* "-" *)        | OpExp_minus
            (* "*" *)        | OpExp_times
            (* "/" *)        | OpExp_rdivide
            (* \  *)         | OpExp_ldivide
            (* "**" or "^" *)| OpExp_power

            (*                       Unary minus *)
            (* "-" *)        | OpExp_unaryMinus

            (*                       Element Ways     *)
            (* ".*" *)       | OpExp_dottimes
            (* "./" *)       | OpExp_dotrdivide
            (* .\ *)         | OpExp_dotldivide (* not run. used ? *)
            (* ".^" *)       | OpExp_dotpower

            (* Kroneckers *)
            (* ".*." *)      | OpExp_krontimes
            (* "./." *)      | OpExp_kronrdivide
            (* ".\." *)      | OpExp_kronldivide

            (*                       Control *)
            (* FIXME : What the hell is this ??? *)
            (* "*." *)       | OpExp_controltimes (* not run. used ? *)
            (* "/." *)       | OpExp_controlrdivide  (* not run. used ? *)
            (* "\." *)       | OpExp_controlldivide (* not run. used ? *)

            (*                       Comparison     *)
            (* "==" *)       | OpExp_eq
           (* "<>" or "~=" *)| OpExp_ne
            (* "<" *)        | OpExp_lt
            (* "<=" *)       | OpExp_le
            (* "<" *)        | OpExp_gt
            (* ">=" *)       | OpExp_ge

and opLogicalExp_Oper =
            (*                       Logical operators *)
            (* "&" *)   | OpLogicalExp_logicalAnd
            (* "|" *)   | OpLogicalExp_logicalOr
            (* "&&" *)  | OpLogicalExp_logicalShortCutAnd
            (* "||" *)  | OpLogicalExp_logicalShortCutOr



and opExp_Kind =
  (* Invalid kind *)
  OpExp_invalid_kind

(*                          Scalar values *)
(* Boolean result *)
| OpExp_bool_kind
(* String result *)
| OpExp_string_kind
(* Integer result *)
| OpExp_integer_kind
(* Float result *)
| OpExp_float_kind
(* Double result *)
| OpExp_double_kind
(* Float Complex result *)
| OpExp_float_complex_kind
(* Double Complex result *)
| OpExp_double_complex_kind

(*                             Matrix values *)
(* Boolean Matrix result *)
| OpExp_bool_matrix_kind
(* String Matrix result *)
| OpExp_string_matrix_kind
(* Integer Matrix result *)
| OpExp_integer_matrix_kind
(* Float Matrix result *)
| OpExp_float_matrix_kind
(* Double Matrix result *)
| OpExp_double_matrix_kind
(* Float Complex Matrix result *)
| OpExp_float_complex_matrix_kind
(* Double Complex Matrix result *)
| OpExp_double_complex_matrix_kind

(* Heterogeneous Matrix result *)
| OpExp_matrix_kind


and transposeExp = {
  transposeExp_exp : exp;
  transposeExp_conjugate : transposeExp_kind;
}

and transposeExp_kind =
  Conjugate | NonConjugate

and var = {
  var_location : Location.t;
  var_desc : var_desc;
}

and var_desc =
| ColonVar  (* a ; *)
| DollarVar (* a $ *)
| SimpleVar of ScilabContext.symbol
| ArrayListVar of var array  (* should never occur, automatically encoded
    in functionDec *)

and arrayListVar = {
  arrayListVar_location : Location.t;
  arrayListVar_vars : var array;
}

(*
in runvisitor.hxx:
    void visitprivate(const MatrixLineExp &e)
    void visitprivate(const CellExp &e)
    void visitprivate(const StringExp &e)
    void visitprivate(const CommentExp &e)
    void visitprivate(const IntExp  &e)
    void visitprivate(const FloatExp  &e)
    void visitprivate(const DoubleExp  &e)
    void visitprivate(const BoolExp  &e)
    void visitprivate(const NilExp &e)
    void visitprivate(const SimpleVar &e)
    void visitprivate(const ColonVar &e)
    void visitprivate(const DollarVar &e)
    void visitprivate(const ArrayListVar &e)
    void visitprivate(const FieldExp &e)
    void visitprivate(const IfExp  &e)
    void visitprivate(const TryCatchExp  &e)
    void visitprivate(const WhileExp  &e)
    void visitprivate(const ForExp  &e)
    void visitprivate(const BreakExp &e)
    void visitprivate(const ContinueExp &e)
    void visitprivate(const ReturnExp &e)
    void visitprivate(const SelectExp &e)
    void visitprivate(const CaseExp &e)
    void visitprivate(const SeqExp  &e)
    void visitprivate(const ArrayListExp  &e)
    void visitprivate(const AssignListExp  &e)
    void visitprivate(const NotExp &e)
    void visitprivate(const TransposeExp &e)
    void visitprivate(const VarDec  &e)
    void visitprivate(const FunctionDec  &e)
    void visitprivate(const ListExp &e)
in run_AssignExp.hxx
    void visitprivate(const AssignExp  &e)
in run_OpExp.hxx
    void visitprivate(const OpExp &e)
    void visitprivate(const LogicalOpExp &e)
in run_MatrixExp.hxx
    void visitprivate(const MatrixExp &e)
in run_CallExp.hxx
    void visitprivate(const CallExp &e)
*)



let dummy_info = {
  is_verbose = false;
  is_break = false;
  is_breakable = false;
  is_return = false;
  is_returnable = false;
  is_continue = false;
  is_continuable = false;
}
let dummy_loc = {
  first_line = 0;
  first_column = 0;
  last_line = 0;
  last_column = 0;
}
