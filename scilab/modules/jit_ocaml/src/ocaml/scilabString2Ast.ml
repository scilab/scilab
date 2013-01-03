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

open ScilabAst
open ScilabContext

let debug = false

let get_uint8 s pos =
  int_of_char (String.unsafe_get s pos), pos+1

let get_bool s pos =
  let n, pos = get_uint8 s pos in
  (n <> 0), pos

let get_uint32 s pos =
  let c0, pos = get_uint8 s pos in
  let c1, pos = get_uint8 s pos in
  let c2, pos = get_uint8 s pos in
  let c3, pos = get_uint8 s pos in
  c0 + ((c1 + ((c2 + (c3 lsl 8)) lsl 8)) lsl 8), pos

let get_uint32_32 s pos =
  let c0, pos = get_uint8 s pos in
  let c1, pos = get_uint8 s pos in
  let c2, pos = get_uint8 s pos in
  let c3, pos = get_uint8 s pos in
  Int32.add (Int32.of_int c0)
    (Int32.shift_left (Int32.of_int ((c1 + ((c2 + (c3 lsl 8)) lsl 8)))) 8), pos

let get_location s pos =
  let first_line, pos = get_uint32 s pos in
  let first_column, pos = get_uint32 s pos in
  let last_line, pos = get_uint32 s pos in
  let last_column, pos = get_uint32 s pos in
  let loc = {
    first_line; first_column; last_line; last_column
  } in
  loc, pos

let get_ast s pos =
  let code, pos = get_uint8 s pos in

  let loc, pos = get_location s pos in

  let is_verbose, pos = get_bool s pos in
  let is_break, pos = get_bool s pos in
  let is_breakable, pos = get_bool s pos in
  let is_return, pos = get_bool s pos in
  let is_returnable, pos = get_bool s pos in
  let is_continue, pos = get_bool s pos in
  let is_continuable, pos = get_bool s pos in

  let exp_info = {
    is_verbose;
    is_break; is_breakable;
    is_return; is_returnable;
    is_continue; is_continuable;
  } in
  code, exp_info, loc, pos

let mkexp exp_desc exp_info exp_location =
  { exp_desc; exp_location; exp_info }


let get_wstring s pos =
  let size, pos = get_uint32 s pos in
  let wstring = String.create size in
  String.unsafe_blit s pos wstring 0 size;
  let pos = pos + size in
  wstring, pos

let get_Symbol = get_wstring

let get_OpExp_Kind s pos =
  let code, pos = get_uint8 s pos in
  let kind =
    match code with
    | 1 -> OpExp_invalid_kind
    | 2 -> OpExp_bool_kind
    | 3 -> OpExp_string_kind
    | 4 -> OpExp_integer_kind
    | 5 -> OpExp_float_kind
    | 6 -> OpExp_double_kind
    | 7 -> OpExp_float_complex_kind
    | 8 -> OpExp_double_complex_kind

    | 9 -> OpExp_bool_matrix_kind
    | 10 -> OpExp_string_matrix_kind
    | 11 -> OpExp_integer_matrix_kind
    | 12 -> OpExp_float_matrix_kind
    | 13 -> OpExp_double_matrix_kind
    | 14 -> OpExp_float_complex_matrix_kind
    | 15 -> OpExp_double_complex_matrix_kind
    | 16 -> OpExp_matrix_kind
    | _ -> failwith (Printf.sprintf "get_OpExp_Kind : unknown code %d" code)
  in
  kind, pos

let get_OpExp_Oper s pos =
  let code, pos = get_uint8 s pos in
  let oper =
    match code with
    | 1 -> OpExp_plus
    | 2 -> OpExp_minus
    | 3 -> OpExp_times
    | 4 -> OpExp_rdivide
    | 5 -> OpExp_ldivide
    | 6 -> OpExp_power

    | 7 -> OpExp_dottimes
    | 8 -> OpExp_dotrdivide
    | 9 -> OpExp_dotldivide
    | 10 -> OpExp_dotpower

    | 11 -> OpExp_krontimes
    | 12 -> OpExp_kronrdivide
    | 13 -> OpExp_kronldivide

    | 14 -> OpExp_controltimes
    | 15 -> OpExp_controlrdivide
    | 16 -> OpExp_controlldivide

    | 17 -> OpExp_eq
    | 18 -> OpExp_ne
    | 19 -> OpExp_lt
    | 20 -> OpExp_le
    | 21 -> OpExp_gt
    | 22 -> OpExp_ge

    | 23 -> OpExp_unaryMinus
    | _ -> failwith (Printf.sprintf "get_OpExp_Oper : unknown code %d" code)
  in
  oper, pos

let get_LogicalOpExp_Oper s pos =
  let code, pos = get_uint8 s pos in
  let oper =
    match code with

    | 24 -> OpLogicalExp_logicalAnd
    | 25 -> OpLogicalExp_logicalOr
    | 26 -> OpLogicalExp_logicalShortCutAnd
    | 27 -> OpLogicalExp_logicalShortCutOr
    | _ -> failwith (Printf.sprintf "get_LogicalOpExp_Oper : unknown code %d" code)
  in
  oper, pos

let get_IntExp_Prec s pos =
  let code, pos = get_uint8 s pos in
  let prec =
    match code with

    | 1 -> IntExp_8
    | 2 -> IntExp_16
    | 3 -> IntExp_32
    | 4 -> IntExp_64
    | _ -> failwith (Printf.sprintf "get_IntExp_Prec : unknown code %d" code)
  in
  prec, pos

let get_IfExp_Kind s pos =
  let code, pos = get_uint8 s pos in
  let kind =
    match code with
    | 1 -> IfExp_invalid_kind
    | 2 -> IfExp_instruction_kind
    | 3 -> IfExp_expression_kind
    | 4 -> IfExp_untyped_kind
    | _ -> failwith (Printf.sprintf "get_IfExp_Kind : unknown code %d" code)
  in
  kind, pos

let get_VarDec_Kind s pos =
  let code, pos = get_uint8 s pos in
  let kind =
    match code with
    | 1 -> VarDec_invalid_kind
    | 2 -> VarDec_evaluation_kind
    | 3 -> VarDec_assignment_kind
    | _ -> failwith (Printf.sprintf "get_VarDec_Kind : unknown code %d" code)
  in
  kind, pos

let get_TransposeExp_Kind s pos =
  let code, pos = get_uint8 s pos in
  let kind =
    match code with
    | 1 -> Conjugate
    | 2 -> NonConjugate
    | _ -> failwith (Printf.sprintf "get_TransposeExp_Kind : unknown code %d" code)
  in
  kind, pos


external jit_ocaml_get_double : string -> int -> float =
    "jit_ocaml_get_double_c"
let get_double s pos =
  let d = jit_ocaml_get_double s pos in
  let pos = pos + 8 in
  d, pos

let dummyExp = mkexp (SeqExp []) dummy_info dummy_loc

let warning = ref None
let return_dummyExp = ref false

let indent = ref [""]

let rec get_exp s pos =
  if !return_dummyExp then dummyExp, pos else
    let _ = () in

    let old_pos = pos in
    let code, info, loc, pos = get_ast s pos in
    if debug then begin
      let old_indent = List.hd !indent in
      indent := (Printf.sprintf "%s%d-" old_indent code) :: !indent;
      Printf.fprintf stderr "%s : %d -> %d\n%!" old_indent old_pos code;
    end;
    let exp, pos =
      match code with
      | 1 ->
        let items, pos = get_exps s pos in
        mkexp (SeqExp items) info loc, pos
      | 2 ->
        let stringExp_value, pos = get_wstring s pos in
        mkexp (ConstExp (StringExp {
          stringExp_value;
          stringExp_bigString = ();
        }
        )) info loc, pos
      | 3 ->
        let commentExp_comment, pos = get_wstring s pos in
        mkexp (ConstExp (CommentExp {
          commentExp_comment;
        }
        )) info loc, pos

      | 4 ->
        let intExp_prec, pos = get_IntExp_Prec s pos in
        let intExp_value, pos = get_uint32_32 s pos in
        mkexp (ConstExp (IntExp {
          intExp_value;
          intExp_prec;
        })) info loc, pos

      | 5 ->
        let floatExp_value, pos = get_double s pos in
        mkexp (ConstExp (FloatExp {
          floatExp_value;
        })) info loc, pos

      | 6 ->
        let doubleExp_value, pos = get_double s pos in
        mkexp (ConstExp (DoubleExp {
          doubleExp_value;
          doubleExp_bigDouble = ();
        })) info loc, pos

      | 7 ->
        let boolExp_value, pos = get_uint8 s pos in
        let boolExp_value = boolExp_value <> 0 in
        mkexp (ConstExp (BoolExp {
          boolExp_value;
          boolExp_bigBool = ();
        })) info loc, pos

      | 8 ->
        mkexp (ConstExp NilExp) info loc, pos

      | 9 ->
        let symbol, pos = get_wstring s pos in
        mkexp (Var {
          var_location = loc;
          var_desc = SimpleVar (new_symbol symbol);
        }) info loc, pos

      | 10 ->
        mkexp (Var {
          var_location = loc;
          var_desc = ColonVar;
        }) info loc, pos

      | 11 ->
        mkexp (Var {
          var_location = loc;
          var_desc = DollarVar;
        }) info loc, pos

      | 12 ->
        let vars, pos = get_vars s pos in
        mkexp (Var { var_desc = ArrayListVar vars;
                     var_location = loc }) info loc, pos

      | 13 ->
        let fieldExp_head, pos = get_exp s pos in
        let fieldExp_tail, pos = get_exp s pos in
        mkexp (FieldExp { fieldExp_head; fieldExp_tail }) info loc, pos

      | 14 ->
        let ifExp_kind, pos = get_IfExp_Kind s pos in
        let has_else, pos = get_uint8 s pos in
        let ifExp_test, pos = get_exp s pos in
        let ifExp_then, pos = get_exp s pos in
        let ifExp_else, pos =
          if has_else <> 0 then
            let ifExp_else, pos = get_exp s pos in
            Some ifExp_else, pos
          else
            None, pos
        in
        mkexp (ControlExp (IfExp {
          ifExp_kind; ifExp_test; ifExp_then; ifExp_else
        })) info loc, pos

      | 15 ->
        let tryCatchExp_tryme_location, pos = get_location s pos in
        let tryCatchExp_catchme_location, pos = get_location s pos in
        let tryCatchExp_tryme, pos = get_exps s pos in
        let tryCatchExp_catchme, pos = get_exps s pos in
        mkexp (ControlExp (TryCatchExp {
          tryCatchExp_tryme_location;
          tryCatchExp_catchme_location;
          tryCatchExp_tryme; tryCatchExp_catchme })) info loc, pos

      | 16 ->
        let whileExp_test, pos = get_exp s pos in
        let whileExp_body, pos = get_exp s pos in
        mkexp (ControlExp (WhileExp {
          whileExp_test; whileExp_body })) info loc, pos

      | 17 ->
        let forExp_vardec_location, pos = get_location s pos in
        let forExp_vardec, pos = get_varDec s pos in
        let forExp_body, pos = get_exp s pos in
        mkexp (ControlExp (ForExp {
          forExp_vardec_location;
          forExp_vardec; forExp_body })) info loc, pos

      | 18 ->
        mkexp (ControlExp BreakExp) info loc, pos

      | 19 ->
        mkexp (ControlExp ContinueExp) info loc, pos

      | 20 ->
        let returnExp_is_global, pos = get_bool s pos in
        let returnExp_exp, pos =
          if returnExp_is_global then None, pos else
            let returnExp_exp, pos = get_exp s pos in
            Some returnExp_exp, pos
        in
        mkexp (ControlExp (ReturnExp {
          returnExp_exp
        })) info loc, pos

      | 21 ->
        let has_default, pos = get_bool s pos in
        let selectExp_default, pos =
          if has_default then
            let default_location, pos = get_location s pos in
            let default, pos = get_exps s pos in
            Some (default_location, default), pos
          else
            None, pos
        in
        let selectExp_selectme, pos = get_exp s pos in
        let selectExp_cases, pos = get_cases s pos in
        mkexp (ControlExp (SelectExp {
          selectExp_default;
          selectExp_selectme; selectExp_cases })) info loc, pos

      | 23 ->
        let matrixExp_lines, pos = get_matrixLines s pos in
        mkexp (MathExp (CellExp { matrixExp_lines })) info loc, pos

      | 24 ->
        let exps, pos = get_exps s pos in
        let exps = Array.of_list exps in
        mkexp (ArrayListExp exps) info loc, pos

      | 25 ->
        let exps, pos = get_exps s pos in
        let exps = Array.of_list exps in
        mkexp (AssignListExp exps) info loc, pos

      | 26 ->
        let notExp_exp, pos = get_exp s pos in
        mkexp (MathExp (NotExp { notExp_exp })) info loc, pos

      | 27 ->
        let transposeExp_conjugate, pos = get_TransposeExp_Kind s pos in
        let transposeExp_exp, pos = get_exp s pos in
        mkexp (MathExp (TransposeExp {
          transposeExp_exp; transposeExp_conjugate })) info loc, pos
      | 28 ->
        let vardec, pos = get_varDec s pos in
        mkexp (Dec (VarDec vardec)) info loc, pos

      | 29 ->
        let functionDec_symbol, pos = get_Symbol s pos in
        let functionDec_symbol = new_symbol functionDec_symbol in
        let args_loc, pos = get_location s pos in
        let returns_loc, pos = get_location s pos in
        let functionDec_body, pos = get_exp s pos in
        let args, pos = get_vars s pos in
        let returns, pos = get_vars s pos in
        let functionDec_args = {
          arrayListVar_location = args_loc;
          arrayListVar_vars = args;
        } in
        let functionDec_returns = {
          arrayListVar_location = returns_loc;
          arrayListVar_vars = returns;
        } in
        mkexp (Dec (FunctionDec {
          functionDec_symbol; functionDec_location = loc;
          functionDec_body; functionDec_args; functionDec_returns;
        })) info loc, pos

      | 30 ->
        let listExp_start, pos = get_exp s pos in
        let listExp_step, pos = get_exp s pos in
        let listExp_end, pos = get_exp s pos in
        mkexp (ListExp {
          listExp_start; listExp_step; listExp_end }) info loc, pos

      | 31 ->
        let assignExp_left_exp, pos = get_exp s pos in
        let assignExp_right_exp, pos = get_exp s pos in
        mkexp (AssignExp {
          assignExp_left_exp; assignExp_right_exp
        }) info loc, pos

      | 32 ->
        let opExp_kind, pos = get_OpExp_Kind s pos in
        let opExp_oper, pos = get_OpExp_Oper s pos in
        let opExp_left, pos = get_exp s pos in
        let opExp_right, pos =get_exp s pos in
        mkexp (MathExp (OpExp (opExp_oper,
                               {
                                 opExp_right; opExp_left; opExp_kind
                               }))) info loc, pos

      | 33 ->
        let opExp_kind, pos = get_OpExp_Kind s pos in
        let opExp_oper, pos = get_LogicalOpExp_Oper s pos in
        let opExp_left, pos = get_exp s pos in
        let opExp_right, pos =get_exp s pos in
        mkexp (MathExp (LogicalOpExp (opExp_oper,
                                      {
                                        opExp_right; opExp_left; opExp_kind
                                      }))) info loc, pos

      | 34 ->
        let matrixExp_lines, pos = get_matrixLines s pos in
        mkexp (MathExp (MatrixExp { matrixExp_lines })) info loc, pos

      | 35 ->
        let callExp_name, pos = get_exp s pos in
        let callExp_args, pos = get_exps s pos in
        let callExp_args = Array.of_list callExp_args in
        mkexp (CallExp {
          callExp_name; callExp_args
        }) info loc, pos

      | 37 ->
        let callExp_name, pos = get_exp s pos in
        let callExp_args, pos = get_exps s pos in
        let callExp_args = Array.of_list callExp_args in
        mkexp (CellCallExp {
          callExp_name; callExp_args
        }) info loc, pos

      | _ ->
        warning := Some (
          Printf.sprintf  "ast_of_buffer: code %d unknown" code);
        return_dummyExp := true;
        dummyExp, pos
    in
    if debug then begin
      indent := List.tl !indent;
    end;
    exp, pos

and get_varDec s pos =
  let varDec_name, pos = get_Symbol s pos in
  let varDec_name = new_symbol varDec_name in
  let varDec_kind, pos = get_VarDec_Kind s pos in
  let varDec_init, pos = get_exp s pos in
  { varDec_name; varDec_init; varDec_kind }, pos

and get_exps s pos =
  if !return_dummyExp then [], pos else
    let nitems, pos = get_uint32 s pos in
    let rec iter nitems s pos ritems =
      if nitems > 0 then
        let item, pos = get_exp s pos in
        iter (nitems-1) s pos (item :: ritems)
      else (List.rev ritems, pos)
    in
    iter nitems s pos []

and get_matrixLines s pos =
  if !return_dummyExp then [||], pos else
    let nitems, pos = get_uint32 s pos in
    let rec iter nitems s pos ritems =
      if nitems > 0 then
        let loc, pos = get_location s pos in
        let exps, pos = get_exps s pos in
        let item = {
          matrixLineExp_location = loc;
          matrixLineExp_columns = Array.of_list exps;
        } in
        iter (nitems-1) s pos (item :: ritems)
      else (Array.of_list (List.rev ritems), pos)
    in
    iter nitems s pos []

and get_cases s pos =
  if !return_dummyExp then [||], pos else
    let nitems, pos = get_uint32 s pos in
    let rec iter nitems s pos ritems =
      if nitems > 0 then
        let caseExp_location, pos = get_location s pos in
        let caseExp_body_location, pos = get_location s pos in
        let caseExp_test, pos = get_exp s pos in
        let caseExp_body, pos = get_exps s pos in
        let case = {
          caseExp_location; caseExp_body_location;
          caseExp_body; caseExp_test } in
        iter (nitems-1) s pos (case :: ritems)
      else (Array.of_list (List.rev ritems), pos)
    in
    iter nitems s pos []

and get_vars s pos =
  let exps, pos = get_exps s pos in
  let vars = if !return_dummyExp then [||] else
      Array.map (fun exp ->
        match exp.exp_desc with
          Var var -> var
        | _ -> assert false
      ) (Array.of_list exps)
  in
  vars, pos

let previous_one = ref None

let copy_string s =
  let pos = 0 in
  let buflen, pos = get_uint32 s pos in
  let copy = String.create buflen in
  String.unsafe_blit s 0 copy 0 buflen;
  copy

let diff_strings s1 s2 =
  if s1 <> s2 then
    let len1 = String.length s1 in
    let len2 = String.length s2 in
    Printf.fprintf stderr "len1 = %d <> len2 = %d\n%!" len1 len2;
    for i = 4 to (min len1 len2)-1 do
      if s1.[i] <> s2.[i] then begin
        Printf.fprintf stderr "diff at %d\n%!" i;
      end
    done

let ast_of_string s =
  return_dummyExp := false;
  warning := None;
  let pos = 0 in
  let buflen, pos = get_uint32 s pos in
  if debug then
    Printf.fprintf stderr "buflen : %d\n%!" buflen;
  let ast, pos = get_exp s pos in
  begin
    match !warning with
      None -> ()
    | Some s ->
      Printf.fprintf stderr "Warning: %s\n%!" s
  end;

  if debug then begin
    let s1 = copy_string s in
    begin match !previous_one with
      None -> ()
    | Some (previous_ast, s2) ->
      if previous_ast = ast then
        Printf.fprintf stderr "IDEMPOTENT!!\n%!"
      else
        Printf.fprintf stderr "NOT IDEMPOTENT!!\n%!";
      diff_strings s1 s2;
    end;
    previous_one := Some (ast, s1);
  end;
  ast
