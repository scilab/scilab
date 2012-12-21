(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

open ScilabAst
open ScilabContext

let debug = false

let buf_uint8 b n =
  try
    Buffer.add_char b (char_of_int n)
  with e ->
    Printf.fprintf stderr "buf_uint8 %d\n%!" n;
    raise e

let buf_bool b bool =
  buf_uint8 b (if bool then 1 else 0)

let buf_uint32 b n =
  buf_uint8 b (n land 0xff);
  buf_uint8 b ((n lsr 8) land 0xff);
  buf_uint8 b ((n lsr 16) land 0xff);
  buf_uint8 b ((n lsr 24) land 0xff);
  ()

let set_uint32 s pos n =
  s.[pos] <- char_of_int (n land 0xff);
  s.[pos+1] <- char_of_int ((n lsr 8) land 0xff);
  s.[pos+2] <- char_of_int ((n lsr 16) land 0xff);
  s.[pos+3] <- char_of_int ((n lsr 24) land 0xff);
  ()

let buf_uint32_32 b nl =
  let n = Int32.to_int nl in
  buf_uint8 b (n land 0xff);
  buf_uint8 b ((n lsr 8) land 0xff);
  buf_uint8 b ((n lsr 16) land 0xff);
  buf_uint8 b ((Int32.to_int (Int32.shift_right nl 24)) land 0xff);
  ()


external jit_ocaml_set_double : string -> int -> float -> unit =
    "jit_ocaml_set_double_c"
let double_len = 8
let buf_double =
  let s = String.create double_len in
  fun b d ->
    jit_ocaml_set_double s 0 d;
    Buffer.add_string b s;
    ()

let buf_wstring b s =
  buf_uint32 b (String.length s);
  Buffer.add_string b s
let buf_Symbol = buf_wstring

(* We always reuse the same buffer.
   TODO: we might, from time to time, reset it if it grew too big. *)
let b = Buffer.create 10_000

let buf_location b loc =
  buf_uint32 b loc.first_line;
  buf_uint32 b loc.first_column;
  buf_uint32 b loc.last_line;
  buf_uint32 b loc.last_column;
  ()

let buf_ast b exp code =
  if debug then
    Printf.fprintf stderr "buf_ast %d -> %d\n%!" (Buffer.length b) code;
  buf_uint8 b code;
  buf_location b exp.exp_location;
  let i = exp.exp_info in
  buf_bool b i.is_verbose;
  buf_bool b i.is_break;
  buf_bool b i.is_breakable;
  buf_bool b i.is_return;
  buf_bool b i.is_returnable;
  buf_bool b i.is_continue;
  buf_bool b i.is_continuable;
  ()

let buf_IntExp_Prec b prec =
  let code = match prec with
    | IntExp_8 -> 1
    | IntExp_16 -> 2
    | IntExp_32 -> 3
    | IntExp_64 -> 4
  in
  buf_uint8 b code

let buf_LogicalOpExp_Oper b kind =
  let code = match kind with
    | OpLogicalExp_logicalAnd -> 24
    | OpLogicalExp_logicalOr -> 25
    | OpLogicalExp_logicalShortCutAnd -> 26
    | OpLogicalExp_logicalShortCutOr -> 27
  in
  buf_uint8 b code

let buf_OpExp_Oper b kind =
  let code = match kind with
   | OpExp_plus -> 1
    | OpExp_minus -> 2
    | OpExp_times -> 3
    | OpExp_rdivide -> 4
    | OpExp_ldivide -> 5
    | OpExp_power -> 6

    | OpExp_dottimes -> 7
    | OpExp_dotrdivide -> 8
    | OpExp_dotldivide -> 9
    | OpExp_dotpower -> 10

    | OpExp_krontimes -> 11
    | OpExp_kronrdivide -> 12
    | OpExp_kronldivide -> 13

    | OpExp_controltimes -> 14
    | OpExp_controlrdivide -> 15
    | OpExp_controlldivide -> 16

    | OpExp_eq -> 17
    | OpExp_ne -> 18
    | OpExp_lt -> 19
    | OpExp_le -> 20
    | OpExp_gt -> 21
    | OpExp_ge -> 22

    | OpExp_unaryMinus -> 23
  in
  buf_uint8 b code

let buf_OpExp_Kind b kind =
  let code = match kind with
    | OpExp_invalid_kind -> 1
    | OpExp_bool_kind ->  2
    | OpExp_string_kind -> 3
    | OpExp_integer_kind -> 4
    | OpExp_float_kind -> 5
    | OpExp_double_kind -> 6
    | OpExp_float_complex_kind -> 7
    | OpExp_double_complex_kind -> 8

    | OpExp_bool_matrix_kind -> 9
    | OpExp_string_matrix_kind -> 10
    | OpExp_integer_matrix_kind -> 11
    | OpExp_float_matrix_kind -> 12
    | OpExp_double_matrix_kind -> 13
    | OpExp_float_complex_matrix_kind -> 14
    | OpExp_double_complex_matrix_kind -> 15
    | OpExp_matrix_kind -> 16
  in
  buf_uint8 b code

let buf_IfExp_Kind b kind =
  let code = match kind with
    | IfExp_invalid_kind -> 1
    | IfExp_instruction_kind -> 2
    | IfExp_expression_kind -> 3
    | IfExp_untyped_kind -> 4
  in
  buf_uint8 b code

let buf_TransposeExp_Kind b kind =
  let code = match kind with
    | Conjugate -> 1
    | NonConjugate -> 2
  in
  buf_uint8 b code

let buf_VarDec_Kind b kind =
  let code = match kind with
    | VarDec_invalid_kind -> 1
    | VarDec_evaluation_kind -> 2
    | VarDec_assignment_kind -> 3
  in
  buf_uint8 b code

let rec buf_exp b exp =
  match exp.exp_desc with
  | SeqExp items ->
    buf_ast b exp 1;
    buf_exp_list b items
  | ConstExp (StringExp s) ->
    buf_ast b exp 2;
    buf_wstring b s.stringExp_value
  | ConstExp (CommentExp s) ->
    buf_ast b exp 3;
    buf_wstring b s.commentExp_comment
  | ConstExp (IntExp int) ->
    buf_ast b exp 4;
    buf_IntExp_Prec b int.intExp_prec;
    buf_uint32_32 b int.intExp_value
  | ConstExp (FloatExp f) ->
    buf_ast b exp 5;
    buf_double b f.floatExp_value
  | ConstExp (DoubleExp f) ->
    buf_ast b exp 6;
    buf_double b f.doubleExp_value
  | ConstExp (BoolExp bool) ->
    buf_ast b exp 7;
    buf_bool b bool.boolExp_value
  | ConstExp NilExp ->
    buf_ast b exp 8
  | Var { var_desc = SimpleVar symbol } ->
    buf_ast b exp 9;
    buf_Symbol b (symbol_name symbol)
  | Var { var_desc = ColonVar } ->
    buf_ast b exp 10;
  | Var { var_desc = DollarVar } ->
    buf_ast b exp 11;
  | Var { var_desc = ArrayListVar vars } ->
    buf_ast b exp 12;
    buf_vars b vars
  | FieldExp f ->
    buf_ast b exp 13;
    buf_exp b f.fieldExp_head;
    buf_exp b f.fieldExp_tail
  | ControlExp (IfExp ife) ->
    buf_ast b exp 14;
    buf_IfExp_Kind b ife.ifExp_kind;
    buf_bool b (ife.ifExp_else <> None);
    buf_exp b ife.ifExp_test;
    buf_exp b ife.ifExp_then;
    begin match ife.ifExp_else with None -> ()
    | Some exp -> buf_exp b exp end
  | ControlExp (TryCatchExp t) ->
    buf_ast b exp 15;
    buf_location b t.tryCatchExp_tryme_location;
    buf_location b t.tryCatchExp_catchme_location;
    buf_exp_list b t.tryCatchExp_tryme;
    buf_exp_list b t.tryCatchExp_catchme;
  | ControlExp (WhileExp w) ->
    buf_ast b exp 16;
    buf_exp b w.whileExp_test;
    buf_exp b w.whileExp_body
  | ControlExp (ForExp f) ->
    buf_ast b exp 17;
    buf_location b f.forExp_vardec_location;
    buf_varDec b f.forExp_vardec;
    buf_exp b f.forExp_body
  | ControlExp BreakExp ->
    buf_ast b exp 18
  | ControlExp ContinueExp ->
    buf_ast b exp 19
  | ControlExp (ReturnExp r) ->
    buf_ast b exp 20;
    begin match r.returnExp_exp with
      None ->
        buf_bool b true
    | Some returnExp_exp ->
      buf_bool b false;
      buf_exp b returnExp_exp
    end
  | ControlExp (SelectExp s) ->
    buf_ast b exp 21;
    begin match s.selectExp_default with
      None -> buf_bool b false
    | Some (default_location, default) ->
      buf_bool b true;
      buf_location b default_location;
    buf_exp_list b default
    end;
    buf_exp b s.selectExp_selectme;
    buf_cases b s.selectExp_cases
      (* 22 would be for CaseExp, not possible yet *)
  | MathExp (CellExp m) ->
    buf_ast b exp 23;
    buf_MatrixExp b m
  | ArrayListExp array ->
    buf_ast b exp 24;
    buf_exp_array b array
  | AssignListExp array ->
    buf_ast b exp 25;
    buf_exp_array b array
  | MathExp (NotExp n) ->
    buf_ast b exp 26;
    buf_exp b n.notExp_exp
  | MathExp (TransposeExp t) ->
    buf_ast b exp 27;
    buf_TransposeExp_Kind b t.transposeExp_conjugate;
    buf_exp b t.transposeExp_exp
  | Dec (VarDec v) ->
    buf_ast b exp 28;
    buf_varDec b v
  | Dec (FunctionDec f) ->
    buf_ast b exp 29;
    buf_Symbol b (symbol_name f.functionDec_symbol);
    buf_location b f.functionDec_args.arrayListVar_location;
    buf_location b f.functionDec_returns.arrayListVar_location;
    buf_exp b f.functionDec_body;
    buf_vars b f.functionDec_args.arrayListVar_vars;
    buf_vars b f.functionDec_returns.arrayListVar_vars;
  | ListExp l ->
    buf_ast b exp 30;
    buf_exp b l.listExp_start;
    buf_exp b l.listExp_step;
    buf_exp b l.listExp_end
  | AssignExp a ->
    buf_ast b exp 31;
    buf_exp b a.assignExp_left_exp;
    buf_exp b a.assignExp_right_exp
  | MathExp (OpExp (oper, opexp)) ->
    buf_ast b exp 32;
    buf_OpExp_Kind b opexp.opExp_kind;
    buf_OpExp_Oper b oper;
    buf_exp b opexp.opExp_left;
    buf_exp b opexp.opExp_right;
  | MathExp (LogicalOpExp (oper, opexp)) ->
    buf_ast b exp 33;
    buf_OpExp_Kind b opexp.opExp_kind;
    buf_LogicalOpExp_Oper b oper;
    buf_exp b opexp.opExp_left;
    buf_exp b opexp.opExp_right;
  | MathExp (MatrixExp m) ->
    buf_ast b exp 34;
    buf_MatrixExp b m
  | CallExp c ->
    buf_ast b exp 35;
    buf_exp b c.callExp_name;
    buf_exp_array b c.callExp_args
  | CellCallExp c ->
    buf_ast b exp 36;
    buf_exp b c.callExp_name;
    buf_exp_array b c.callExp_args

and buf_MatrixExp b m =
  buf_uint32 b (Array.length m.matrixExp_lines);
  Array.iter (fun line ->
    buf_location b line.matrixLineExp_location;
    buf_exp_array b line.matrixLineExp_columns
  ) m.matrixExp_lines

and buf_cases b cases =
  buf_uint32 b (Array.length cases);
  Array.iter (fun c ->
    buf_location b c.caseExp_location;
    buf_location b c.caseExp_body_location;
    buf_exp b c.caseExp_test;
    buf_exp_list b c.caseExp_body;
  ) cases

and buf_varDec b v =
  buf_Symbol b (symbol_name v.varDec_name);
  buf_VarDec_Kind b v.varDec_kind;
  buf_exp b v.varDec_init

and buf_vars b vars =
  buf_uint32 b (Array.length vars);
  Array.iter (fun var ->
    buf_exp b { exp_desc = Var var;
                exp_location = var.var_location;
                exp_info = dummy_info;
              }
  ) vars

and buf_exp_list b exps =
  buf_uint32 b (List.length exps);
  List.iter (buf_exp b) exps

and buf_exp_array b exps =
  if debug then
    Printf.fprintf stderr "buf_exp_array %d\n%!" (Array.length exps);
  buf_uint32 b (Array.length exps);
  Array.iter (buf_exp b) exps

let string_of_ast ast =
  Buffer.clear b;
  Buffer.add_string b "1234";
  buf_exp b ast;
  let s = Buffer.contents b in
  set_uint32 s 0 (String.length s);
  s
