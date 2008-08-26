
(*  Scicos *)
(* *)
(*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr> *)
(* *)
(* This program is free software; you can redistribute it and/or modify *)
(* it under the terms of the GNU General Public License as published by *)
(* the Free Software Foundation; either version 2 of the License, or *)
(* (at your option) any later version. *)
(* *)
(* This program is distributed in the hope that it will be useful, *)
(* but WITHOUT ANY WARRANTY; without even the implied warranty of *)
(* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *)
(* GNU General Public License for more details. *)
(* *) 
(* You should have received a copy of the GNU General Public License *)
(* along with this program; if not, write to the Free Software *)
(* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. *)
(*  *)
(* See the file ./license.txt *)

open Num
open SymbolicExpression
open Optimization


module ExpressionElement =
  struct
    type t = SymbolicExpression.t
    let equal t t' = t == t'
    let hash t = hash t
  end

module ExpressionTable = Hashtbl.Make(ExpressionElement)

type model_info =
  {
    model: model;
    final_index_of_parameters: int array;
    final_index_of_variables: int array;
    surfaces: SymbolicExpression.t list;
    occurrence_table: occurrence_record ExpressionTable.t;
    mutable current_index: int;
    mutable max_index: int;
    code_buffer: Buffer.t
  }

and occurrence_record =
  {
    mutable occurrences: int;
    mutable label: int option
  }

let postprocess_residue model =
  let rec add_to xs = function
    | [] -> xs
    | x' :: xs' when List.memq x' xs -> add_to xs xs'
    | x' :: xs' -> add_to (x' :: xs) xs'
  and accumulate_derivatives (ders, nequs) equ =
    if equ.solved then ders, nequs
    else match derivatives_of equ.expression with
      | [] -> ders, nequs
      | ders' -> add_to ders ders', nequs + 1
  in
  let postprocess_residue' () =
    let cj = create_blackBox "Get_Jacobian_parameter" [] in
    Array.iter
      (fun equ ->
        if not equ.solved && derivatives_of equ.expression = [] then
          equ.expression <- symbolic_mult cj equ.expression)
      model.equations
  in
  let ders, nequs = Array.fold_left accumulate_derivatives ([], 0) model.equations in
  if nequs <> 0 && List.length ders = nequs then postprocess_residue' ()

let next_index model_info =
  model_info.current_index <- model_info.current_index + 1;
  model_info.max_index <- max model_info.max_index model_info.current_index;
  model_info.current_index

let create_index_array a p =
  let size = Array.length a in
  let indexes = Array.make size (-1) in
  let j = ref 0 in
  Array.iteri (fun i x -> if p x then begin indexes.(i) <- !j; incr j end) a;
  indexes

let final_index_of_parameters model =
  create_index_array model.parameters (fun parameter -> parameter.main)

let final_index_of_variables model =
  create_index_array model.equations (fun equation -> not equation.solved)

let collect_surfaces model =
  let rec union xs ys =
    List.fold_left (fun xs y -> if List.memq y xs then xs else y :: xs) xs ys
  and surfaces_of expr = match nature expr with
    | ArcCosine expr' | ArcHyperbolicCosine expr' | ArcHyperbolicSine expr' |
      ArcHyperbolicTangent expr' | ArcSine expr' | ArcTangent expr' |
      Cosine expr' | Derivative (expr', _) | Exponential expr' | Floor expr' |
      HyperbolicCosine expr' | HyperbolicSine expr' | HyperbolicTangent expr' |
      Logarithm expr' | Not expr' | RationalPower (expr', _) | Sign expr' |
      Sine expr' | Tangent expr' -> surfaces_of expr'
    | BlackBox ("noEvent", _) -> []
    | Addition exprs' | And exprs' | BlackBox (_, exprs') |
      Multiplication exprs' | Or exprs' ->
        List.fold_left
          (fun surfaces expr -> union surfaces (surfaces_of expr))
          []
          exprs'
    | Equality (expr', expr'') | Greater (expr', expr'') |
      PartialDerivative (expr', expr'') ->
        union (surfaces_of expr') (surfaces_of expr'')
    | If (expr', expr'', expr''') -> surfaces_of_if expr' expr'' expr'''
    | TimeVariable | BooleanValue _ | Constant _ | DiscreteVariable _ |
      Number _ | Parameter _ | Variable _ -> []
  and surfaces_of_if expr expr' expr'' =
    let surfaces = union (surfaces_of expr') (surfaces_of expr'')
    in match nature expr with
      | BlackBox ("noEvent", _) -> surfaces
      | Greater _ -> union [expr] (union (surfaces_of expr) surfaces)
      | Or _ when is_greater_equal expr ->
          union [expr] (union (surfaces_of expr) surfaces)
      | _ -> assert false
  in
  Array.fold_left
    (fun surfaces equation ->
      union surfaces (surfaces_of equation.expression))
    []
    model.equations

let rec is_atomic expr = match nature expr with
  | BooleanValue _ | Constant _ | Derivative _ | DiscreteVariable _ | Number _ |
    Parameter _ | Variable _ -> true
  | _ -> false

let add_to_occurrence_table modes_on expr table =
  let rec add_if_necessary modes_on expr = match nature expr with
    | BlackBox ("noEvent", [expr']) -> add_if_necessary modes_on expr'
    | _ when is_atomic expr -> ()
    | Or [expr1; expr2] when is_greater_equal expr ->
        begin match nature expr1, nature expr2 with
          | Greater (expr', expr''), _ | _, Greater (expr', expr'') ->
              add_if_necessary modes_on (create_blackBox ">=" [expr'; expr''])
              (* this is a hack to simulate >= being a primitive expression. *)
          | _ -> assert false
        end
    | _ ->
        try
          let record = ExpressionTable.find table expr in
          record.occurrences <- record.occurrences + 1
        with
          | Not_found -> add modes_on expr
  and add modes_on expr =
    ExpressionTable.add table expr { occurrences = 1; label = None };
    match nature expr with
      | ArcCosine expr' | ArcHyperbolicCosine expr' | ArcHyperbolicSine expr' |
        ArcHyperbolicTangent expr' | ArcSine expr' | ArcTangent expr' |
        Cosine expr' | Exponential expr' | Floor expr' |
        HyperbolicCosine expr' | HyperbolicSine expr' |
        HyperbolicTangent expr' | Logarithm expr' | Not expr' |
        RationalPower (expr', _) | Sign expr' | Sine expr' | Tangent expr' ->
          add_if_necessary modes_on expr'
      | Addition exprs' | And exprs' | BlackBox (_, exprs') |
        Multiplication exprs' | Or exprs' ->
          List.iter (add_if_necessary modes_on) exprs'
      | Equality (expr', expr'') | Greater (expr', expr'') ->
          add_if_necessary modes_on expr'; add_if_necessary modes_on expr''
      | If (expr', expr'', expr''') ->
          add_if_necessary false expr';
          add_if_necessary modes_on expr''; add_if_necessary modes_on expr'''
      | TimeVariable -> ()
      | _ -> assert false
  in add_if_necessary modes_on expr

let has_multiple_occurrences expr model_info =
  try
    let record = ExpressionTable.find model_info.occurrence_table expr in
    record.occurrences > 1
  with
    | Not_found -> false

let rec has_alias_binding expr model_info = match nature expr with
  | BlackBox ("noEvent", [expr']) -> has_alias_binding expr' model_info
  | _ ->
      try
        let record = ExpressionTable.find model_info.occurrence_table expr in
        record.label <> None
      with
        | Not_found -> false

let bufferize_float f model_info =
  let s = Printf.sprintf "%.16g" f in
  if not (String.contains s '.' || String.contains s 'e') then
    Printf.bprintf model_info.code_buffer "%s.0" s
  else
    Printf.bprintf model_info.code_buffer "%s" s

let rec bufferize_rhs model_info tabs modes_on lhs expr =
  let rec precedence expr =
    if has_alias_binding expr model_info then 14
    else match nature expr with
      | BooleanValue _ | Constant _ -> 14
      | BlackBox ("noEvent", [expr']) -> precedence expr'
      | ArcCosine _ | ArcHyperbolicCosine _ | ArcHyperbolicSine _ |
        ArcHyperbolicTangent _ | ArcSine _ | ArcTangent _ | BlackBox _ |
        Cosine _ | Derivative _ | Exponential _ | Floor _ | HyperbolicCosine _ |
        HyperbolicSine _ | HyperbolicTangent _ | DiscreteVariable _ |
        Logarithm _ | Parameter _ | PartialDerivative _ | Sign _ | Sine _ |
        Tangent _ | TimeVariable | Variable _ -> 13
      | Not _ -> 12
      | Multiplication _ | Number (Ratio _) -> 11
      | Addition _ -> 10
      | Greater _ -> 8
      | Or _ when is_greater_equal expr -> 8
      | Equality _ -> 7
      | And _ -> 3
      | Or _ -> 2
      | If _ -> 1
      | Number num when lt_num num (Int 0) -> 12
      | Number (Int _) | Number (Big_int _) -> 14
      | RationalPower (_, num) when eq_num num (Int (-1)) -> 11
      | RationalPower _ -> 13
  in
  let rec bufferize_expression_under prec expr =
    if precedence expr < prec then Printf.bprintf model_info.code_buffer "(";
    bufferize_expression expr;
    if precedence expr < prec then Printf.bprintf model_info.code_buffer ")"
  and bufferize_expression expr = match nature expr with
    | Addition exprs' -> bufferize_addition expr exprs'
    | And exprs' -> bufferize_infix_operator expr "&&" exprs'
    | ArcCosine expr' -> bufferize_unary_function expr "acos" expr'
    | ArcHyperbolicCosine expr' -> bufferize_unary_function expr "acosh" expr'
    | ArcHyperbolicSine expr' -> bufferize_unary_function expr "asinh" expr'
    | ArcHyperbolicTangent expr' -> bufferize_unary_function expr "atanh" expr'
    | ArcSine expr' -> bufferize_unary_function expr "asin" expr'
    | ArcTangent expr' -> bufferize_unary_function expr "atan" expr'
    | BlackBox ("noEvent", [expr']) -> bufferize_expression expr'
    | BlackBox (name, exprs') -> bufferize_n_ary_function expr name exprs'
    | BooleanValue b ->
        Printf.bprintf model_info.code_buffer "%c" (if b then '1' else '0')
    | Constant s -> Printf.bprintf model_info.code_buffer "%s" s
    | Cosine expr' -> bufferize_unary_function expr "cos" expr'
    | Derivative (expr', num') when num' = Num.Int 1 ->
        begin match nature expr' with
          | Variable i ->
              let j = model_info.final_index_of_variables.(i) in
              Printf.bprintf model_info.code_buffer "xd[%d]" j
          | _ -> assert false
        end
    | Derivative _ -> assert false
    | Equality (expr', expr'') ->
        bufferize_infix_operator expr "==" [expr'; expr'']
    | Exponential expr' -> bufferize_unary_function expr "exp" expr'
    | Floor expr' -> bufferize_unary_function expr "floor" expr'
    | Greater (expr', expr'') ->
        bufferize_infix_operator expr ">" [expr'; expr'']
    | HyperbolicCosine expr' -> bufferize_unary_function expr "cosh" expr'
    | HyperbolicSine expr' -> bufferize_unary_function expr "sinh" expr'
    | HyperbolicTangent expr' -> bufferize_unary_function expr "tanh" expr'
    | If (expr1, expr2, expr3) -> bufferize_if expr expr1 expr2 expr3
    | DiscreteVariable i when i < 0 ->
        Printf.bprintf model_info.code_buffer "u[%d][0]" (-1 - i)
    | DiscreteVariable i -> Printf.bprintf model_info.code_buffer "z[%d]" i
    | Logarithm expr' -> bufferize_unary_function expr "log" expr'
    | Multiplication exprs' -> bufferize_multiplication expr exprs'
    | Not expr' -> bufferize_prefix_operator expr "!" expr'
    | Number num -> bufferize_float (float_of_num num) model_info
    | Or [expr1; expr2] when is_greater_equal expr ->
        begin match nature expr1, nature expr2 with
          | Greater (expr', expr''), _ | _, Greater (expr', expr'') ->
              bufferize_greater_equal expr expr' expr''
          | _ -> assert false
        end
    | Or exprs' -> bufferize_infix_operator expr "||" exprs'
    | Parameter i ->
        Printf.bprintf model_info.code_buffer
          "rpar[%d]"
          model_info.final_index_of_parameters.(i)
    | PartialDerivative (expr', expr'') ->
        bufferize_partial_derivative expr expr' expr''
    | RationalPower (expr', num) -> bufferize_rational_power expr expr' num
    | Sign expr' -> bufferize_unary_function expr "sgn" expr'
    | Sine expr' -> bufferize_unary_function expr "sin" expr'
    | Tangent expr' -> bufferize_unary_function expr "tan" expr'
    | TimeVariable -> bufferize_n_ary_function expr "get_scicos_time" []
    | Variable i ->
        let j = model_info.final_index_of_variables.(i) in
        Printf.bprintf model_info.code_buffer "x[%d]" j
  and bufferize_greater_equal expr expr' expr'' =
    try
      let record =
        ExpressionTable.find
          model_info.occurrence_table
          (create_blackBox ">=" [expr'; expr''])
          (* this is a hack to simulate >= being a primitive expression. *)
      in match record.label with
        | None ->
            bufferize_expression_under (precedence expr) expr';
            Printf.bprintf model_info.code_buffer ">=";
            bufferize_expression_under (precedence expr) expr''
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_unary_function expr name expr' =
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None ->
            Printf.bprintf model_info.code_buffer "%s(" name;
            bufferize_expression expr';
            Printf.bprintf model_info.code_buffer ")"
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_prefix_operator expr name expr' =
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None ->
            Printf.bprintf model_info.code_buffer "%s" name;
            bufferize_expression_under (precedence expr) expr'
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_arguments_under prec name = function
    | [] -> ()
    | [expr'] -> bufferize_expression_under prec expr'
    | expr' :: exprs' ->
        bufferize_expression_under prec expr';
        Printf.bprintf model_info.code_buffer "%s" name;
        bufferize_arguments_under prec name exprs'
  and bufferize_n_ary_function expr name exprs' =
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None when model_info.model.trace <> None ->
            Printf.bprintf model_info.code_buffer "%s_trace(" name;
            bufferize_arguments_under 0 ", " exprs';
            Printf.bprintf model_info.code_buffer ")"            
        | None ->
            Printf.bprintf model_info.code_buffer "%s(" name;
            bufferize_arguments_under 0 ", " exprs';
            Printf.bprintf model_info.code_buffer ")"
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_infix_operator expr name exprs' =
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None -> bufferize_arguments_under (precedence expr) name exprs'
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_if expr expr1 expr2 expr3 =
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None ->
            bufferize_condition_under (precedence expr) expr1;
            Printf.bprintf model_info.code_buffer " ? ";
            bufferize_expression_under (precedence expr) expr2;
            Printf.bprintf model_info.code_buffer " : ";
            bufferize_expression_under (precedence expr) expr3;
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_condition_under prec expr =
    let rec find_mode_index_at i = function
      | [] -> -1
      | cond :: _ when cond == expr -> i
      | _ :: surfaces -> find_mode_index_at (i + 1) surfaces
    in
    if modes_on then
      let i = find_mode_index_at 0 model_info.surfaces in
      if i = -1 then bufferize_expression_under prec expr
      else Printf.bprintf model_info.code_buffer "mode[%d]==1" i
    else bufferize_expression_under prec expr
  and bufferize_addition expr exprs' =
    let bufferize_addition' () =
      let not_negatives, negatives =
        List.partition
          (fun expr ->
            has_multiple_occurrences expr model_info ||
            match nature expr with
              | Number num when lt_num num (Int 0) -> false
              | Multiplication exprs ->
                  begin
                    let not_reciprocals =
                      List.filter
                        (fun expr -> match nature expr with
                          | RationalPower (_, num) when lt_num num (Int 0) ->
                              false
                          | _ -> true)
                        exprs
                    in match not_reciprocals with
                      | [] -> true
                      | expr' :: _ ->
                          begin match nature expr' with
                            | Number num when lt_num num (Int 0) -> false
                            | _ -> true
                          end
                  end
              | BlackBox ("noEvent", _) -> assert false
              | _ -> true)
          exprs'
      in
      bufferize_arguments_under (precedence expr) "+" not_negatives;
      List.iter (bufferize_expression_under (precedence expr)) negatives
    in
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None -> bufferize_addition' ()
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_multiplication expr exprs' =
    let bufferize_multiplication' () =
      let not_reciprocals, reciprocals =
        List.fold_left
          (fun (exprs, exprs') expr -> match nature expr with
            | BlackBox ("noEvent", _) -> assert false
            | RationalPower (expr'', num) when eq_num num (Int (-1)) &&
              not (has_multiple_occurrences expr model_info) ->
                exprs, (expr'' :: exprs')
            | _ -> (expr :: exprs), exprs')
          ([], [])
          exprs'
      in
      begin match List.rev not_reciprocals with
        | [] -> Printf.bprintf model_info.code_buffer "1.0"
        | expr' :: exprs' ->
            begin match nature expr' with
              | Number (Int -1) ->
                  Printf.bprintf model_info.code_buffer "-";
                  begin match exprs' with
                    | [] -> Printf.bprintf model_info.code_buffer "1.0"
                    | exprs' ->
                        bufferize_arguments_under (precedence expr) "*" exprs'
                  end
              | _ ->
                  bufferize_arguments_under
                    (precedence expr)
                    "*"
                    (expr' :: exprs')
            end
      end;
      begin match reciprocals with
        | [] -> ()
        | [expr'] ->
            Printf.bprintf model_info.code_buffer "/";
            bufferize_expression_under (precedence expr + 1) expr'
            (* add one to the precedence to ensure parentheses to be correctly
               placed arroud expr' *)
        | exprs' ->
            Printf.bprintf model_info.code_buffer "/(";
            bufferize_arguments_under (precedence expr) "*" exprs';
            Printf.bprintf model_info.code_buffer ")"
      end
    in
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None -> bufferize_multiplication' ()
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_rational_power expr expr' num =
    let bufferize_rational_power' () =
      if eq_num num (Int (-1)) then begin
        Printf.bprintf model_info.code_buffer "1.0/";
        bufferize_expression_under (precedence expr + 1) expr'
        (* add one to the precedence to ensure parentheses to be correctly
           placed aroud expr' *)
      end else if is_integer_num num then begin
        Printf.bprintf model_info.code_buffer "ipow(";
        bufferize_expression expr';
        Printf.bprintf model_info.code_buffer ", %d)" (int_of_num num);
      end else begin
        Printf.bprintf model_info.code_buffer "pow(";
        bufferize_expression expr';
        Printf.bprintf model_info.code_buffer ", ";
        bufferize_float (float_of_num num) model_info;
        Printf.bprintf model_info.code_buffer ")"
      end
    in
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | None -> bufferize_rational_power' ()
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
    with
      | Not_found -> assert false
  and bufferize_partial_derivative expr expr' expr'' = assert false
  and bufferize_intermediate_variables_if_necessary expr =
    let rec bufferize_children_if_necessary expr = match nature expr with
      | BlackBox ("noEvent", [expr']) -> bufferize_children_if_necessary expr'
      | ArcCosine expr' | ArcHyperbolicCosine expr' | ArcHyperbolicSine expr' |
        ArcHyperbolicTangent expr' | ArcSine expr' | ArcTangent expr' |
        Cosine expr' | Exponential expr' | Floor expr' |
        HyperbolicCosine expr' | HyperbolicSine expr' |
        HyperbolicTangent expr' | Logarithm expr' | Not expr' |
        RationalPower (expr', _) | Sign expr' | Sine expr' | Tangent expr' ->
          bufferize_intermediate_variables_if_necessary expr'
      | Addition exprs' | And exprs' | BlackBox (_, exprs') |
        Multiplication exprs' | Or exprs' ->
          List.iter bufferize_intermediate_variables_if_necessary exprs'
      | Equality (expr1, expr2) | Greater (expr1, expr2) ->
          bufferize_intermediate_variables_if_necessary expr1;
          bufferize_intermediate_variables_if_necessary expr2
      | If (expr1, expr2, expr3) ->
          bufferize_intermediate_variables_if_necessary expr1;
          bufferize_intermediate_variables_if_necessary expr2;
          bufferize_intermediate_variables_if_necessary expr3
      | TimeVariable -> ()
      | _ -> assert false
    in match nature expr with
      | BlackBox ("noEvent", [expr']) -> bufferize_intermediate_variables_if_necessary expr'
      | _ ->
          try
            let record = ExpressionTable.find model_info.occurrence_table expr in
            match record.label with
              | None when record.occurrences > 1 ->
                  let i = next_index model_info in
                  bufferize_children_if_necessary expr;
                  for i = 1 to tabs do
                    Printf.bprintf model_info.code_buffer "\t"
                  done;
                  Printf.bprintf model_info.code_buffer "v%d = " i;
                  bufferize_expression expr;
                  Printf.bprintf model_info.code_buffer ";\n";
                  record.label <- Some i
              | _ -> bufferize_children_if_necessary expr
          with
            | Not_found -> ()
  in
  bufferize_intermediate_variables_if_necessary expr;
  for i = 1 to tabs do
    Printf.bprintf model_info.code_buffer "\t"
  done;
  Printf.bprintf model_info.code_buffer "%s" lhs;
  bufferize_expression expr

let bufferize_equations model_info =
  ExpressionTable.clear model_info.occurrence_table;
  Array.iter
    (fun equation ->
      if not equation.solved then
        add_to_occurrence_table
          true
          equation.expression
          model_info.occurrence_table)
    model_info.model.equations;
  model_info.current_index <- -1;
  Array.iteri
    (fun i equation ->
      if not equation.solved then
        let j = model_info.final_index_of_variables.(i) in
        let lhs = "res[" ^ (string_of_int j) ^ "] = " in
        bufferize_rhs model_info 2 true lhs equation.expression;
        Printf.bprintf model_info.code_buffer ";\n")
    model_info.model.equations

let bufferize_jacobian model_info =
  let model = model_info.model in
  let nx =
    Array.fold_left
      (fun acc equation -> if not equation.solved then acc + 1 else acc)
      0
      model.equations
  and nu = Array.length model.inputs
  and ny =
    Array.fold_left
      (fun acc variable -> if variable.output <> None then acc + 1 else acc)
      0
      model.variables
  in
  let cj = create_blackBox "Get_Jacobian_parameter" []
  and dx = create_blackBox "Get_Scicos_SQUR" [] in
  let jacobian_matrix =
    Array.init (nx + ny) (fun _ -> Array.make (nx + nu) zero)
  in
  let _ =
    Array.fold_left
      (fun i equation ->
        if not equation.solved then begin
          let _ =
            Array.fold_left
              (fun (j, k) variable ->
                if not model.equations.(k).solved then begin
                  let dfdx =
                    symbolic_partial_derivative_with
                      dx
                      (create_variable k)
                      equation.expression
                  in
                  jacobian_matrix.(i).(j) <-
                    if variable.state then
                      let dfdxd =
                        symbolic_partial_derivative_with
                          dx
                          (create_derivative (create_variable k) (Num.Int 1))
                          equation.expression
                      in
                      symbolic_add dfdx (symbolic_mult cj dfdxd)
                    else dfdx;
                  j + 1, k + 1
                end else j, k + 1)
              (0, 0)
              model.variables
          in
          i + 1
        end else i)
      0
      model.equations
  in
  ();
  let _ =
    Array.fold_left
      (fun i equation ->
        if not equation.solved then begin
          for j = -1 downto -Array.length model.inputs do
            jacobian_matrix.(i).(nx - 1 - j) <-
              symbolic_partial_derivative_with
                dx
                (create_discrete_variable j)
                equation.expression
          done;
          i + 1
        end else i)
      0
      model.equations
  in
  ();
  let _ =
    Array.fold_left
      (fun (i, k) equation ->
        if model.variables.(k).output <> None then begin
          let _ =
            Array.fold_left
              (fun (j, l) variable ->
                if not model.equations.(l).solved then begin
                  jacobian_matrix.(nx + i).(j) <-
                    if equation.solved then
                      symbolic_partial_derivative_with
                        dx
                        (create_variable l)
                        equation.expression
                    else if k = l then one
                    else zero;
                  j + 1, l + 1
                end else j, l + 1)
              (0, 0)
              model.variables
          in
          i + 1, k + 1
        end else i, k + 1)
      (0, 0)
      model.equations
  in
  ();
  let _ =
    Array.fold_left
      (fun (i, k) equation ->
        if model.variables.(k).output <> None then begin
          for j = -1 downto -Array.length model.inputs do
            jacobian_matrix.(nx + i).(nx - 1 - j) <-
              if equation.solved then
                symbolic_partial_derivative_with
                  dx
                  (create_discrete_variable j)
                  equation.expression
              else zero
          done;
          i + 1, k + 1
        end else i, k + 1)
      (0, 0)
      model.equations
  in
  ();
  ExpressionTable.clear model_info.occurrence_table;
  Array.iter
    (fun row ->
      Array.iter
        (fun elt ->
          add_to_occurrence_table
            true
            elt
            model_info.occurrence_table)
        row)
    jacobian_matrix;
  model_info.current_index <- -1;
  for j = 0 to nx - 1 do
    for i = 0 to nx - 1 do
      let lhs = "res[" ^ (string_of_int ((j * nx) + i)) ^ "] = " in
      bufferize_rhs model_info 2 true lhs jacobian_matrix.(i).(j);
      Printf.bprintf model_info.code_buffer ";\n"
    done;
  done;
  for j = nx to nx + nu - 1 do
    for i = 0 to nx - 1 do
      let lhs = "res[" ^ (string_of_int ((j * nx) + i)) ^ "] = " in
      bufferize_rhs model_info 2 true lhs jacobian_matrix.(i).(j);
      Printf.bprintf model_info.code_buffer ";\n"
    done;
  done;
  let offset = nx * (nx + nu) in
  for j = 0 to nx - 1 do
    for i = nx to nx + ny - 1 do
      let lhs =
        "res[" ^ (string_of_int (offset + (j * ny) + (i - nx))) ^ "] = "
      in
      bufferize_rhs model_info 2 true lhs jacobian_matrix.(i).(j);
      Printf.bprintf model_info.code_buffer ";\n"
    done;
  done;
  for j = nx to nx + nu - 1 do
    for i = nx to nx + ny - 1 do
      let lhs =
        "res[" ^ (string_of_int (offset + (j * ny) + (i - nx))) ^ "] = "
      in
      bufferize_rhs model_info 2 true lhs jacobian_matrix.(i).(j);
      Printf.bprintf model_info.code_buffer ";\n"
    done;
  done;
  Printf.bprintf model_info.code_buffer "\t\tset_block_error(0);\n"

let bufferize_outputs model_info =
  let bufferize_outputs' modes_on =
    Array.iteri
      (fun i variable ->
        match variable.output with
          | None -> ()
          | Some j ->
              let lhs = "y[" ^ (string_of_int j) ^ "][0] = " in
              let equation = model_info.model.equations.(i) in
              if equation.solved then
                bufferize_rhs model_info 3 modes_on lhs equation.expression
              else begin
                let k = model_info.final_index_of_variables.(i) in
                Printf.bprintf model_info.code_buffer "\t\t\t%sx[%d]" lhs k
              end;
              Printf.bprintf model_info.code_buffer "; /* %s" variable.v_name;
              if variable.v_comment <> "" then
                Printf.bprintf model_info.code_buffer " = %s"
                  variable.v_comment;
              Printf.bprintf model_info.code_buffer " */\n")
      model_info.model.variables
  in
  ExpressionTable.clear model_info.occurrence_table;
  Array.iteri
    (fun i equation ->
      if model_info.model.variables.(i).output <> None then
        add_to_occurrence_table
          false
          equation.expression
          model_info.occurrence_table)
    model_info.model.equations;
  model_info.current_index <- -1;
  Printf.bprintf model_info.code_buffer
    "\t\tif (get_phase_simulation() == 1) {\n";
  bufferize_outputs' false;
  ExpressionTable.clear model_info.occurrence_table;
  Array.iteri
    (fun i equation ->
      if model_info.model.variables.(i).output <> None then
        add_to_occurrence_table
          true
          equation.expression
          model_info.occurrence_table)
    model_info.model.equations;
  model_info.current_index <- -1;
  Printf.bprintf model_info.code_buffer "\t\t} else {\n";
  bufferize_outputs' true;
  Printf.bprintf model_info.code_buffer "\t\t}\n"

let bufferize_surface_expression model_info cond = match nature cond with
  | Greater (expr, expr') ->
      add_to_occurrence_table
        false
        (symbolic_sub expr expr')
        model_info.occurrence_table
  | Or [expr; expr'] ->
      assert (is_greater_equal cond);
      begin match nature expr, nature expr' with
        | Greater (expr, expr'), _ | _, Greater (expr, expr') ->
            add_to_occurrence_table
              false
              (symbolic_sub expr expr')
              model_info.occurrence_table
        | _ -> assert false
      end
  | _ -> ()

let bufferize_surface_equation model_info i cond =
  let bufferize_surface_equation' expr expr' =
    let lhs = "g[" ^ (string_of_int i) ^ "] = " in
    bufferize_rhs model_info 2 false lhs (symbolic_sub expr expr');
    Printf.bprintf model_info.code_buffer ";\n";
  in match nature cond with
    | Greater (expr, expr') -> bufferize_surface_equation' expr expr'
    | Or [expr; expr'] ->
        assert (is_greater_equal cond);
        begin match nature expr, nature expr' with
          | Greater (expr, expr'), _ | _, Greater (expr, expr') ->
              bufferize_surface_equation' expr expr'
          | _ -> assert false
        end
    | _ -> ()

let bufferize_when_equations model_info =
  let _ =
    List.fold_left
      (fun i (_, when_exprs) ->
        Printf.bprintf model_info.code_buffer "\t\tif (jroot[%d] == 1) {\n" i;
        ExpressionTable.clear model_info.occurrence_table;
        List.iter
          (fun (Assign (_, expr) | Reinit (_, expr)) ->
            add_to_occurrence_table false expr model_info.occurrence_table)
          when_exprs;
        model_info.current_index <- -1;
        List.iter
          (function
            | Assign (expr, expr') ->
                begin match nature expr with
                  | DiscreteVariable j ->
                      let lhs = "z[" ^ (string_of_int j) ^ "] = " in
                      bufferize_rhs model_info 3 false lhs expr';
                      Printf.bprintf model_info.code_buffer ";\n"
                  | _ -> failwith "Invalid discrete variable assignment"
                end
            | Reinit (expr, expr') ->
                begin match nature expr with
                  | Variable j ->
                      let k = model_info.final_index_of_variables.(j) in
                      let lhs = "x[" ^ (string_of_int k) ^ "] = " in
                      bufferize_rhs model_info 3 false lhs expr';
                      Printf.bprintf model_info.code_buffer ";\n"
                  | _ -> failwith "Invalid variable reinitialization"
                end)
          when_exprs;
        Printf.bprintf model_info.code_buffer "\t\t}\n";
        i + 1)
      (List.length model_info.surfaces)
      model_info.model.when_clauses
  in ()

let bufferize_surfaces model_info =
  ExpressionTable.clear model_info.occurrence_table;
  List.iter
    (fun cond ->
      add_to_occurrence_table false cond model_info.occurrence_table;
      bufferize_surface_expression model_info cond)
    model_info.surfaces;
  List.iter
    (fun (expr, _) -> bufferize_surface_expression model_info expr)
    model_info.model.when_clauses;
  model_info.current_index <- -1;
  let i =
    List.fold_left
      (fun i cond -> bufferize_surface_equation model_info i cond; i + 1)
      0
      model_info.surfaces
  in
  let _ =
    List.fold_left
      (fun i (expr, _) -> bufferize_surface_equation model_info i expr; i + 1)
      i
      model_info.model.when_clauses
  in ();
  match model_info.surfaces with
    | [] -> ()
    | _ ->
        Printf.bprintf model_info.code_buffer
          "\t\tif (get_phase_simulation() == 1) {\n";
        let _ =
          List.fold_left
            (fun i cond ->
              let lhs = "mode[" ^ (string_of_int i) ^ "] = " in
              bufferize_rhs model_info 3 false lhs cond;
              Printf.bprintf model_info.code_buffer " ? 1 : 2;\n";
              i + 1)
            0
            model_info.surfaces
        in ();
        Printf.bprintf model_info.code_buffer "\t\t}\n"

let bufferize_initializations model_info =
  let start_value = function
    | None -> zero
    | Some expr -> expr
  in
  ExpressionTable.clear model_info.occurrence_table;
  Array.iter
    (fun discrete_variable ->
      add_to_occurrence_table
        false
        (start_value discrete_variable.d_start_value)
        model_info.occurrence_table)
    model_info.model.discrete_variables;
  Array.iter
    (fun discrete_variable ->
      add_to_occurrence_table
        false
        (start_value discrete_variable.start_value)
        model_info.occurrence_table)
    model_info.model.variables;
  Array.iteri
    (fun i discrete_variable ->
      let lhs = "z[" ^ (string_of_int i) ^ "] = " in
      bufferize_rhs model_info 2 false lhs (start_value discrete_variable.d_start_value);
      Printf.bprintf
        model_info.code_buffer
        "; /* %s"
        discrete_variable.d_v_name;
      if discrete_variable.d_v_comment <> "" then
        Printf.bprintf
          model_info.code_buffer
          " = %s"
          discrete_variable.d_v_comment;
      Printf.bprintf model_info.code_buffer " */\n")
    model_info.model.discrete_variables;
  Array.iteri
    (fun i variable ->
      let equation = model_info.model.equations.(i) in
      if not equation.solved then
        let j = model_info.final_index_of_variables.(i) in
        let lhs = "x[" ^ (string_of_int j) ^ "] = " in
        bufferize_rhs model_info 2 true lhs (start_value variable.start_value);
        Printf.bprintf model_info.code_buffer "; /* %s" variable.v_name;
        if variable.v_comment <> "" then
          Printf.bprintf model_info.code_buffer " = %s" variable.v_comment;
        Printf.bprintf model_info.code_buffer " */\n")
    model_info.model.variables

let bufferize_variable_nature model_info =
  let nb_vars =
    Array.fold_left
      (fun acc equation -> if not equation.solved then acc + 1 else acc)
      0
      model_info.model.equations in
  if nb_vars > 0 then begin
    Array.iteri
      (fun i variable ->
        let equation = model_info.model.equations.(i) in
        if not equation.solved then begin
          Printf.bprintf
            model_info.code_buffer
            "\t\tproperty[%d] = %s; /* %s"
            model_info.final_index_of_variables.(i)
            (if variable.state then "1" else "-1")
            variable.v_name;
          if variable.v_comment <> "" then
            Printf.bprintf model_info.code_buffer " = %s" variable.v_comment;
          Printf.bprintf
            model_info.code_buffer
            " (%s variable) */\n"
            (if variable.state then "state" else "algebraic")
        end)
      model_info.model.variables;
    Printf.bprintf model_info.code_buffer "\t\tset_pointer_xproperty(property);\n"
  end


let rec last = function
  | [] -> failwith "last"
  | [x] -> x
  | _ :: xs -> last xs

let generate_trace_info oc model_info = match model_info.model.trace with
  | None -> ()
  | Some filename ->
      Printf.fprintf oc
        "#include <stdio.h>\n\
        \n\
        \n/* Tracing code */\n";
      List.iter
        (fun (name, arity) ->
          Printf.fprintf oc
            "\n\
            double %s_trace("
            (last name);
          for i = 1 to arity - 1 do
            Printf.fprintf oc "double arg%d, " i
          done;
          if arity > 0 then Printf.fprintf oc "double arg%d" arity;
          Printf.fprintf oc
            ")\n\
            {\n\
            \tdouble res;\n\
            \tFILE *fd;\n\
            \tfd = fopen(\"%s\",\"a\");\n\
            \tfprintf(fd, \"%s("
            filename
            (last name);
          for i = 1 to arity - 1 do
            Printf.fprintf oc "%%g, "
          done;
          if arity > 0 then Printf.fprintf oc "%%g";
          Printf.fprintf oc ")\", ";
          for i = 1 to arity - 1 do
            Printf.fprintf oc "arg%d, " i
          done;
          if arity > 0 then Printf.fprintf oc "arg%d" arity;
          Printf.fprintf oc
            ");\n\
            \tfclose(fd);\n\
            \tres = %s("
            (last name);
          for i = 1 to arity - 1 do
            Printf.fprintf oc "arg%d, " i
          done;
          if arity > 0 then Printf.fprintf oc "arg%d" arity;
          Printf.fprintf oc
            ");\n\
            \tfd = fopen(\"%s\",\"a\");\n\
            \tfprintf(fd, \" -> %%g\\n\", res);\n\
            \tfclose(fd);\n\
            \treturn res;\n\
            }\n"
            filename)
      model_info.model.external_functions

let generate_code path filename fun_name model with_jac =
  let rec to_filename = function
    | [] -> ""
    | [s] -> s
    | s :: ss -> s ^ "/" ^ to_filename ss
  in
  let oc = open_out filename in
  postprocess_residue model;
  let model_info =
    {
      model = model;
      final_index_of_parameters = final_index_of_parameters model;
      final_index_of_variables = final_index_of_variables model;
      surfaces = collect_surfaces model;
      occurrence_table = ExpressionTable.create 100;
      current_index = -1;
      max_index = -1;
      code_buffer = Buffer.create 10000
    }
  in
  let nb_vars =
    Array.fold_left
      (fun acc equation -> if not equation.solved then acc + 1 else acc)
      0
      model.equations
  and nb_modes = List.length model_info.surfaces in
  Printf.fprintf oc "/*\n\
    number of discrete variables = %d\n\
    number of variables = %d\n\
    number of inputs = %d\n\
    number of outputs = %d\n\
    number of modes = %d\n\
    number of zero-crossings = %d\n\
    I/O direct dependency = %s\n\
    */\n\n"
    (Array.length model.discrete_variables)
    nb_vars
    (Array.length model.inputs)
    (Array.fold_left
      (fun acc variable -> if variable.output <> None then acc + 1 else acc)
      0
      model.variables)
    nb_modes
    (List.length model.when_clauses + nb_modes)
    (if model.io_dependency then "true" else "false");
  Printf.fprintf oc "#include <math.h>\n#include <scicos_block.h>\n";
  List.iter
    (fun (name, _) ->
      Printf.fprintf oc
        "#include \"%s.h\"\n"
        (String.escaped (Filename.concat path (to_filename name))))
    model.external_functions;
  generate_trace_info oc model_info;
  Printf.fprintf oc "\n\n/* Utility functions */\n\n";
  Printf.fprintf oc
    "double ipow_(double x, int n)\n\
     {\n\
     \tdouble y;\n\
     \ty = n %% 2 ? x : 1;\n\
     \twhile (n >>= 1) {\n\
     \t\tx = x * x;\n\
     \t\tif (n %% 2) y = y * x;\n\
     \t}\n\
     \treturn y;\n\
     }\n\
     \n\
     double ipow(double x, int n)\n\
     {\n\
     \t/* NaNs propagation */\n\
     \tif (x != x || x == 0.0 && n == 0) return exp(x * log((double)n));\n\
     \t/* Normal execution */\n\
     \tif (n < 0) return 1.0 / ipow_(x, -n);\n\
     \treturn ipow_(x, n);\n\
     }\n";
  Printf.fprintf oc "\n\n/* Scicos block's entry point */\n\n";
  Printf.fprintf oc
    "void %s(scicos_block *block, int flag)\n"
    fun_name;
  Printf.fprintf oc
    "{\n\
     \tdouble *rpar = block->rpar;\n\
     \tdouble *z = block->z;\n\
     \tdouble *x = block->x;\n\
     \tdouble *xd = block->xd;\n\
     \tdouble **y = block->outptr;\n\
     \tdouble **u = block->inptr;\n\
     \tdouble *g = block->g;\n\
     \tdouble *res = block->res;\n\
     \tint *jroot = block->jroot;\n\
     \tint *mode = block->mode;\n\
     \tint nevprt = block->nevprt;\n";
  if nb_vars > 0 then
    Printf.fprintf oc "\tint property[%d];\n" nb_vars;
  Printf.fprintf oc "\n";
  Printf.bprintf model_info.code_buffer "\tif (flag == 0) {\n";
  bufferize_equations model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 1) {\n";
  bufferize_outputs model_info;
  Printf.bprintf model_info.code_buffer
    "\t} else if (flag == 2 && nevprt < 0) {\n";
  bufferize_when_equations model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 4) {\n";
  begin match model.trace with
    | None -> ()
    | Some filename ->
        Printf.bprintf model_info.code_buffer
          "\t\tFILE *fd;\n\
          \t\tfd = fopen(\"%s\", \"w\");\n\
          \t\tfclose(fd);\n"
          filename
  end;
  bufferize_initializations model_info;
  if with_jac then begin
    Printf.bprintf model_info.code_buffer "\t\tSet_Jacobian_flag(1);\n";
  end;
  Printf.bprintf model_info.code_buffer
    "\t} else if (flag == 6) {\n";
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 7) {\n";
  bufferize_variable_nature model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 9) {\n";
  bufferize_surfaces model_info;
  if with_jac then begin
    Printf.bprintf model_info.code_buffer "\t} else if (flag == 10) {\n";
    bufferize_jacobian model_info;
  end;
  Printf.bprintf model_info.code_buffer "\t}\n";
  if model_info.max_index >= 0 then begin
    Printf.fprintf oc "\t/* Intermediate variables */\n\tdouble ";
    for i = 0 to model_info.max_index - 1 do
      Printf.fprintf oc "v%d" i;
      if (i + 1) mod 16 = 0 then Printf.fprintf oc ";\n\tdouble "
      else Printf.fprintf oc ", "
    done;
    Printf.fprintf oc "v%d;\n\n" model_info.max_index
  end;
  Buffer.output_buffer oc model_info.code_buffer;
  Printf.fprintf oc "\n\treturn;\n";
  Printf.fprintf oc "}\n";
  close_out oc
