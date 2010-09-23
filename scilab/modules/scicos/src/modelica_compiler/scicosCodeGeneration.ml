(*
 *  Modelicac
 *
 *  Copyright (C) 2005 - 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

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

let eq_array nodes nodes' =
  let l = Array.length nodes in
  let rec eq_array_from i =
    i = l || nodes.(i) == nodes'.(i) && eq_array_from (i + 1) in
  l = Array.length nodes' && eq_array_from 0

module ArrayElement = struct
  type t = int list * SymbolicExpression.t array
  let equal (dims, exprs) (dims', exprs') =
    dims = dims' && eq_array exprs exprs'
  let hash (dims, exprs) =
    (Hashtbl.hash dims lxor
    Array.fold_left
      (fun acc expr -> acc lsl 2 + hash expr)
      0x32fb7a88
      exprs) land max_int
end

module ArrayStore = Hashtbl.Make(ArrayElement)

type model_info =
  {
    model: model;
    final_index_of_integer_parameters: int array;
    final_index_of_string_parameters: int array;
    final_index_of_real_parameters: int array;
    final_index_of_variables: int array;
    surfaces: SymbolicExpression.t list;
    occurrence_table: occurrence_record ExpressionTable.t;
    mutable current_index: int;
    mutable max_index: int;
    code_buffer: Buffer.t;
    real_array_store_size: int;
    real_arrays: int ArrayStore.t
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

let final_index_of_integer_parameters model =
  create_index_array model.parameters (fun par -> par.p_type = IntegerType)

let final_index_of_string_parameters model =
  create_index_array model.parameters (fun par -> par.p_type = StringType)

let final_index_of_real_parameters model =
  create_index_array model.parameters (fun par -> par.p_type = RealType)

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
      Logarithm expr' | Pre expr' | RationalPower (expr', _) | Sign expr' |
      Sine expr' | Tangent expr' -> surfaces_of expr'
    | BlackBox ("noEvent", _) -> []
    | Addition exprs' | Multiplication exprs' ->
        List.fold_left
          (fun surfaces expr -> union surfaces (surfaces_of expr))
          []
          exprs'
    | BlackBox (_, args) ->
        List.fold_left
          (fun surfaces arg -> union surfaces (surfaces_of_argument arg))
          []
          args
    | PartialDerivative (expr', expr'') ->
        union (surfaces_of expr') (surfaces_of expr'')
    | If (expr', expr'', expr''') -> surfaces_of_if expr' expr'' expr'''
    | Equality _ | Greater _ | GreaterEqual _ |
      And _ | Or _ | Not _ | TimeVariable | BooleanValue _ | Constant _ |
      DiscreteVariable _ | Number _ | Parameter _ | Variable _ | Integer _ |
      String _ -> []
  and surfaces_of_argument = function
    | ScalarArgument expr -> surfaces_of expr
    | ArrayArgument (_, exprs) -> 
        Array.fold_left
          (fun surfaces expr -> union surfaces (surfaces_of expr))
          []
          exprs
  and surfaces_of_if expr expr' expr'' =
    let surfaces = union (surfaces_of expr') (surfaces_of expr'')
    in match nature expr with
      | BlackBox ("noEvent", _) -> surfaces
      | _ -> union [expr] surfaces
  in
  Array.fold_left
    (fun surfaces equation ->
      union surfaces (surfaces_of equation.expression))
    []
    model.equations

let rec is_atomic expr = match nature expr with
  | BooleanValue _ | Constant _ | Derivative _ | DiscreteVariable _ | Number _ |
    Parameter _ | Variable _ | Integer _ | String _ -> true
  | _ -> false

let add_to_occurrence_table modes_on expr model_info =
  let rec add_if_necessary modes_on expr = match nature expr with
    | BlackBox ("noEvent", [ScalarArgument expr']) ->
        add_if_necessary modes_on expr'
    | _ when is_atomic expr -> ()
    | _ ->
        begin try
          let record = ExpressionTable.find model_info.occurrence_table expr in
          record.occurrences <- record.occurrences + 1
        with
          | Not_found -> add modes_on expr
        end
  and add modes_on expr =
    ExpressionTable.add
      model_info.occurrence_table
      expr
      { occurrences = 1; label = None };
    match nature expr with
      | ArcCosine expr' | ArcHyperbolicCosine expr' | ArcHyperbolicSine expr' |
        ArcHyperbolicTangent expr' | ArcSine expr' | ArcTangent expr' |
        Cosine expr' | Exponential expr' | Floor expr' |
        HyperbolicCosine expr' | HyperbolicSine expr' |
        HyperbolicTangent expr' | Logarithm expr' | Not expr' | Pre expr' |
        RationalPower (expr', _) | Sign expr' | Sine expr' | Tangent expr' ->
          add_if_necessary modes_on expr'
      | Addition exprs' | And exprs' | Multiplication exprs' | Or exprs' ->
          List.iter (add_if_necessary modes_on) exprs'
      | BlackBox (_, args) ->
          List.iter (add_argument_if_necessary modes_on) args
      | Equality (expr', expr'') | Greater (expr', expr'') |
        GreaterEqual (expr', expr'') ->
          add_if_necessary modes_on expr'; add_if_necessary modes_on expr''
      | If (expr', expr'', expr''') ->
          add_if_necessary false expr';
          add_if_necessary modes_on expr''; add_if_necessary modes_on expr'''
      | BooleanValue _ | Constant _ | Derivative _ | DiscreteVariable _ |
        Number _ | Parameter _ | Variable _ | Integer _ | String _ |
        TimeVariable -> ()
      | PartialDerivative _ -> assert false
  and add_argument_if_necessary modes_on = function
    | ScalarArgument expr -> add_if_necessary modes_on expr
    | ArrayArgument (_, exprs) ->
        Array.iter (add_if_necessary modes_on) exprs in
  add_if_necessary modes_on expr

let has_multiple_occurrences expr model_info =
  try
    let record = ExpressionTable.find model_info.occurrence_table expr in
    record.occurrences > 1
  with
    | Not_found -> false

let rec has_alias_binding expr model_info = match nature expr with
  | BlackBox ("noEvent", [ScalarArgument expr']) ->
      has_alias_binding expr' model_info
  | _ ->
      try
        let record = ExpressionTable.find model_info.occurrence_table expr in
        record.label <> None
      with
        | Not_found -> false

let real_array_store_size model =
  let rec required_space expr = match nature expr with
    | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> 0
    | ArcCosine node | ArcHyperbolicCosine node |
      ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
      ArcTangent node | Cosine node | Derivative (node, _) |
      Exponential node | Floor node | HyperbolicCosine node |
      HyperbolicSine node | HyperbolicTangent node | Logarithm node |
      Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
      Tangent node -> required_space node
    | Equality (node1, node2) | Greater (node1, node2) |
      GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
        max (required_space node1) (required_space node2)
    | If (node1, node2, node3) ->
        max
          (required_space node1)
          (max (required_space node2) (required_space node3))
    | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
        List.fold_left (fun acc node -> max acc (required_space node)) 0 nodes
    | BlackBox (_, args) ->
        List.fold_left
          (fun acc arg -> acc + argument_required_space arg)
          0
          args
  and argument_required_space = function
    | ScalarArgument expr -> required_space expr
    | ArrayArgument (_, exprs) ->
        max
          (Array.length exprs)
          (Array.fold_left
            (fun acc expr -> max acc (required_space expr))
            0
            exprs) in
  let option_required_space = function
    | None -> 0
    | Some expr -> required_space expr
  and when_expression_required_space = function
    | Assign (_, expr) | Reinit (_, expr) -> required_space expr in
  Array.fold_left
    (fun acc par -> max acc (required_space par.value))
    (Array.fold_left
      (fun acc dvar -> max acc (option_required_space dvar.d_start_value))
      (Array.fold_left
        (fun acc var -> max acc (option_required_space var.start_value))
        (Array.fold_left
          (fun acc equ -> max acc (required_space equ.expression))
          (List.fold_left
            (fun acc (expr, wexprs) ->
              max
                (required_space expr)
                (List.fold_left 
                  (fun acc wexpr ->
                    max acc (when_expression_required_space wexpr))
                  acc
                  wexprs))
            0
            model.when_clauses)
          model.equations)
        model.variables)
      model.discrete_variables)
    model.parameters

let bufferize_float f model_info =
  let s = Printf.sprintf "%.16g" f in
  if not (String.contains s '.' || String.contains s 'e') then
    Printf.bprintf model_info.code_buffer "%s.0" s
  else
    Printf.bprintf model_info.code_buffer "%s" s

let rec bufferize_rhs model_info tabs modes_on lhs expr =
  let bufferize_parameter i =
    let j = model_info.final_index_of_integer_parameters.(i) in
    if j <> -1 then Printf.bprintf model_info.code_buffer "ipar[%d]" j
    else
      let j = model_info.final_index_of_string_parameters.(i) in
      if j <> -1 then Printf.bprintf model_info.code_buffer "spar[%d]" j
      else
        let j = model_info.final_index_of_real_parameters.(i) in
        if j <> -1 then Printf.bprintf model_info.code_buffer "rpar[%d]" j
        else assert false in
  let rec precedence expr =
    if has_alias_binding expr model_info then 14
    else match nature expr with
      | BooleanValue _ | Constant _ | String _ -> 14
      | BlackBox ("noEvent", [ScalarArgument expr']) -> precedence expr'
      | ArcCosine _ | ArcHyperbolicCosine _ | ArcHyperbolicSine _ |
        ArcHyperbolicTangent _ | ArcSine _ | ArcTangent _ | BlackBox _ |
        Cosine _ | Derivative _ | Exponential _ | Floor _ |
        HyperbolicCosine _ | HyperbolicSine _ | HyperbolicTangent _ |
        DiscreteVariable _ | Logarithm _ | Parameter _ | PartialDerivative _ |
        Pre _ | Sign _ | Sine _ | Tangent _ | TimeVariable | Variable _ -> 13
      | Not _ -> 12
      | Multiplication _ | Number (Ratio _) -> 11
      | Addition _ -> 10
      | Greater _ -> 8
      | GreaterEqual _ -> 8
      | Equality _ -> 7
      | And _ -> 3
      | Or _ -> 2
      | If _ -> 1
      | Integer i when i < 0l -> 12
      | Integer _ -> 14
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
    | BlackBox ("noEvent", [ScalarArgument expr']) ->
        bufferize_expression expr'
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
    | GreaterEqual (expr', expr'') ->
        bufferize_infix_operator expr ">=" [expr'; expr'']
    | HyperbolicCosine expr' -> bufferize_unary_function expr "cosh" expr'
    | HyperbolicSine expr' -> bufferize_unary_function expr "sinh" expr'
    | HyperbolicTangent expr' -> bufferize_unary_function expr "tanh" expr'
    | If (expr1, expr2, expr3) -> bufferize_if expr expr1 expr2 expr3
    | Integer i -> Printf.bprintf model_info.code_buffer "%ld" i
    | DiscreteVariable i when i < 0 ->
        Printf.bprintf model_info.code_buffer "u[%d][0]" (-1 - i)
    | DiscreteVariable i -> Printf.bprintf model_info.code_buffer "z[%d]" i
    | Pre expr' ->
        begin match nature expr' with
          | DiscreteVariable i when i >= 0 ->
              Printf.bprintf model_info.code_buffer "z[%d]" i
          | _ -> assert false
        end
    | Logarithm expr' -> bufferize_unary_function expr "log" expr'
    | Multiplication exprs' -> bufferize_multiplication expr exprs'
    | Not expr' -> bufferize_prefix_operator expr "!" expr'
    | Number num -> bufferize_float (float_of_num num) model_info
    | Or exprs' -> bufferize_infix_operator expr "||" exprs'
    | Parameter i -> bufferize_parameter i
    | PartialDerivative (expr', expr'') ->
        bufferize_partial_derivative expr expr' expr''
    | RationalPower (expr', num) -> bufferize_rational_power expr expr' num
    | Sign expr' -> bufferize_unary_function expr "sgn" expr'
    | Sine expr' -> bufferize_unary_function expr "sin" expr'
    | String s -> Printf.bprintf model_info.code_buffer "\"%s\"" s
    | Tangent expr' -> bufferize_unary_function expr "tan" expr'
    | TimeVariable -> bufferize_n_ary_function expr "get_scicos_time" []
    | Variable i ->
        let j = model_info.final_index_of_variables.(i) in
        Printf.bprintf model_info.code_buffer "x[%d]" j
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
  and bufferize_n_ary_function expr name args =
    let rec bufferize_arguments = function
      | [] -> ()
      | [arg] -> bufferize_argument arg
      | arg :: args ->
          bufferize_argument arg;
          Printf.bprintf model_info.code_buffer ", ";
          bufferize_arguments args
    and bufferize_argument arg =
      let rec bufferize_dimensions = function
        | [] -> assert false
        | [dim] -> Printf.bprintf model_info.code_buffer "%d" dim
        | dim :: dims ->
            Printf.bprintf model_info.code_buffer "%d, " dim;
            bufferize_dimensions dims in
      match arg with
      | ScalarArgument expr -> bufferize_expression_under 0 expr
      | ArrayArgument (dims, exprs) ->
          assert (Array.length exprs > 0);
          begin try
            let offset =
              ArrayStore.find model_info.real_arrays (dims, exprs) in
            Printf.bprintf model_info.code_buffer "&real_buffer[%d], " offset;
            bufferize_dimensions dims
          with Not_found -> assert false
          end in
    try
      let record = ExpressionTable.find model_info.occurrence_table expr in
      match record.label with
        | Some i -> Printf.bprintf model_info.code_buffer "v%d" i
        | None ->
            Printf.bprintf model_info.code_buffer "%s(" name;
            bufferize_arguments args;
            Printf.bprintf model_info.code_buffer ")"
    with
      | Not_found -> assert false
  and bufferize_expressions_under prec name = function
    | [] -> ()
    | [expr'] -> bufferize_expression_under prec expr'
    | expr' :: exprs' ->
        bufferize_expression_under prec expr';
        Printf.bprintf model_info.code_buffer "%s" name;
        bufferize_expressions_under prec name exprs'
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
      | BlackBox ("noEvent", [ScalarArgument expr']) ->
          bufferize_children_if_necessary expr'
      | BlackBox (_, args) ->
          List.iter bufferize_argument_intermediate_variables_if_necessary args
      | ArcCosine expr' | ArcHyperbolicCosine expr' | ArcHyperbolicSine expr' |
        ArcHyperbolicTangent expr' | ArcSine expr' | ArcTangent expr' |
        Cosine expr' | Exponential expr' | Floor expr' |
        HyperbolicCosine expr' | HyperbolicSine expr' |
        HyperbolicTangent expr' | Logarithm expr' | Not expr' |
        RationalPower (expr', _) | Sign expr' | Sine expr' | Tangent expr' ->
          bufferize_intermediate_variables_if_necessary expr'
      | Addition exprs' | And exprs' | Multiplication exprs' | Or exprs' ->
          List.iter bufferize_intermediate_variables_if_necessary exprs'
      | Equality (expr1, expr2) | Greater (expr1, expr2) |
        GreaterEqual (expr1, expr2) ->
          bufferize_intermediate_variables_if_necessary expr1;
          bufferize_intermediate_variables_if_necessary expr2
      | If (expr1, expr2, expr3) ->
          bufferize_intermediate_variables_if_necessary expr1;
          bufferize_intermediate_variables_if_necessary expr2;
          bufferize_intermediate_variables_if_necessary expr3
      | _ -> ()
    and bufferize_argument_intermediate_variables_if_necessary = function
      | ScalarArgument expr ->
          bufferize_intermediate_variables_if_necessary expr
      | ArrayArgument (_, exprs) ->
          Array.iter bufferize_intermediate_variables_if_necessary exprs
    and bufferize_arrays pointer = function
      | ScalarArgument _ -> pointer
      | ArrayArgument (dims, exprs)
        when ArrayStore.mem model_info.real_arrays (dims, exprs) ->
          pointer
      | ArrayArgument (dims, exprs) ->
          ArrayStore.add model_info.real_arrays (dims, exprs) pointer;
          Array.iteri
            (fun i expr ->
              for i = 1 to tabs do
                Printf.bprintf model_info.code_buffer "\t"
              done;
              Printf.bprintf
                model_info.code_buffer
                "real_buffer[%d] = "
                (pointer + i);
              bufferize_expression expr;
              Printf.bprintf model_info.code_buffer ";\n")
            exprs;
            pointer + List.fold_left ( * ) 1 dims in
    match nature expr with
    | BlackBox ("noEvent", [ScalarArgument expr']) ->
        bufferize_intermediate_variables_if_necessary expr'
    | BlackBox (_, args) ->
        (* Special case of exteral calls: we always generate an intermediate
           variable in order to avoid nested intermediate array
           construction *)
        begin try
          let record = ExpressionTable.find model_info.occurrence_table expr in
          match record.label with
            | None ->
                let i = next_index model_info in
                bufferize_children_if_necessary expr;
                let _ =
                  List.fold_left
                    (fun acc arg -> bufferize_arrays acc arg)
                    0
                    args in
                for i = 1 to tabs do
                  Printf.bprintf model_info.code_buffer "\t"
                done;
                Printf.bprintf model_info.code_buffer "v%d = " i;
                bufferize_expression expr;
                Printf.bprintf model_info.code_buffer ";\n";
                record.label <- Some i
            | _ -> bufferize_children_if_necessary expr
        with
          | Not_found -> assert false
        end
    | _ ->
        begin try
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
        end in
  ArrayStore.clear model_info.real_arrays;
  bufferize_intermediate_variables_if_necessary expr;
  for i = 1 to tabs do
    Printf.bprintf model_info.code_buffer "\t"
  done;
  Printf.bprintf model_info.code_buffer "%s" lhs;
  bufferize_expression expr;
  ArrayStore.clear model_info.real_arrays

let bufferize_equations model_info =
  ExpressionTable.clear model_info.occurrence_table;
  Array.iter
    (fun equation ->
      if not equation.solved then
        add_to_occurrence_table
          true
          equation.expression
          model_info)
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

let bufferize_jacobian nb_vars model_info =
  let model = model_info.model in
  let nx = nb_vars
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
            model_info)
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
          model_info)
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
          model_info)
    model_info.model.equations;
  model_info.current_index <- -1;
  Printf.bprintf model_info.code_buffer "\t\t} else {\n";
  bufferize_outputs' true;
  Printf.bprintf model_info.code_buffer "\t\t}\n"

let bufferize_surface_expression model_info cond = match nature cond with
  | Greater (expr, expr') | GreaterEqual (expr, expr') ->
      add_to_occurrence_table
        false
        (symbolic_sub expr expr')
        model_info
  | _ -> ()

let bufferize_surface_equation model_info i cond =
  let bufferize_surface_equation' expr expr' =
    let lhs = "g[" ^ (string_of_int i) ^ "] = " in
    bufferize_rhs model_info 2 false lhs (symbolic_sub expr expr');
    Printf.bprintf model_info.code_buffer ";\n";
  in match nature cond with
    | Greater (expr, expr') | GreaterEqual (expr, expr') ->
        bufferize_surface_equation' expr expr'
    | _ -> ()

let bufferize_when_equations model_info =
  let _ =
    List.fold_left
      (fun i (_, when_exprs) ->
        Printf.bprintf model_info.code_buffer "\t\tif (jroot[%d] == 1) {\n" i;
        ExpressionTable.clear model_info.occurrence_table;
        List.iter
          (fun (Assign (_, expr) | Reinit (_, expr)) ->
            add_to_occurrence_table false expr model_info)
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
      add_to_occurrence_table false cond model_info;
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

let bufferize_initializations init nb_pars nb_dvars nb_vars nb_ders model_info =
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
        model_info)
    model_info.model.discrete_variables;
  Array.iter
    (fun discrete_variable ->
      add_to_occurrence_table
        false
        (start_value discrete_variable.start_value)
        model_info)
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
    model_info.model.variables;
  match fst init with
  | Some xml_filename ->
      begin
        if nb_pars > 0 then
          Printf.bprintf
            model_info.code_buffer
            "\t\tif ((*work = scicos_malloc(%d * sizeof(double))) == NULL) \
            {\n\
            \t\t\tset_block_error(-16);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_pars;
        if nb_vars > 0 then
          Printf.bprintf
            model_info.code_buffer
            "\t\tif (read_xml_initial_states(%d, \"%s\", var_ids, x)) {\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_vars
            xml_filename;
        if nb_dvars > 0 then
          Printf.bprintf
            model_info.code_buffer
            "\t\tif (read_xml_initial_states(%d, \"%s\", disc_var_ids, z)) {\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_dvars
            xml_filename;
        if nb_ders > 0 then begin
          Printf.bprintf
            model_info.code_buffer
            "\t\tif (read_xml_initial_states(%d, \"%s\", der_ids, der_values)) {\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_ders
            xml_filename;
          let k = ref 0 in
          Array.iteri
            (fun i variable ->
              if
                not model_info.model.equations.(i).solved && variable.state
              then begin
                let j = model_info.final_index_of_variables.(i) in
                Printf.bprintf
                  model_info.code_buffer
                  "\t\txd[%d] = der_values[%d];\n" j !k;
                incr k
              end)
            model_info.model.variables;
        end;
        if nb_pars > 0 then
          Printf.bprintf
            model_info.code_buffer
            "\t\tspars = *work;\n\
            \t\tif (read_xml_initial_states(%d, \"%s\", par_ids, spars)) {\n\
            \t\t\tscicos_free(*work);\n\
            \t\t\t*work = NULL;\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_pars
            xml_filename
        else
          Printf.bprintf
            model_info.code_buffer
            "\t\t*work = NULL;\n"
      end
  | None -> ()

let bufferize_variable_store init nb_pars nb_dvars nb_ders model_info =
  match snd init with
  | Some xml_filename ->
      begin
        if Array.length model_info.model.variables + nb_dvars > 0 then begin
          ExpressionTable.clear model_info.occurrence_table;
          Array.iteri
            (fun i equation ->
              if equation.solved then
                add_to_occurrence_table
                  false
                  equation.expression
                  model_info)
            model_info.model.equations;
          model_info.current_index <- -1;
          Array.iteri
            (fun i equation ->
              let lhs = Printf.sprintf "all_var_values[%d] = " i in
              if equation.solved then begin
                bufferize_rhs model_info 2 false lhs equation.expression;
                Printf.bprintf model_info.code_buffer ";\n"
              end else
                let j = model_info.final_index_of_variables.(i) in
                Printf.bprintf model_info.code_buffer "\t\t%sx[%d];\n" lhs j)
            model_info.model.equations;
          Array.iteri
            (fun i _ ->
              let lhs =
                Printf.sprintf "all_var_values[%d] = "
                (i + Array.length model_info.model.equations) in
              Printf.bprintf model_info.code_buffer "\t\t%sz[%i];\n" lhs i)
            model_info.model.discrete_variables;
          Printf.bprintf
            model_info.code_buffer
            "\t\tif (write_xml_states(%d, \"%s\", all_var_ids, all_var_values)) {\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            (Array.length model_info.model.variables)
            xml_filename
        end;
        if nb_ders > 0 then begin
          let k = ref 0 in
          Array.iteri
            (fun i variable ->
              if
                not model_info.model.equations.(i).solved && variable.state
              then begin
                let j = model_info.final_index_of_variables.(i) in
                Printf.bprintf
                  model_info.code_buffer
                  "\t\tder_values[%d] = xd[%d];\n" !k j;
                incr k
              end)
            model_info.model.variables;
          Printf.bprintf
            model_info.code_buffer
            "\t\tif (write_xml_states(%d, \"%s\", der_ids, der_values)) {\n\
            \t\t\tset_block_error(-19);\n\
            \t\t\treturn;\n\
            \t\t}\n"
            nb_ders
            xml_filename
        end
      end
  | None -> ()

let bufferize_work_deallocation init nb_pars model_info =
  if fst init <> None && nb_pars > 0 then
    Printf.bprintf
      model_info.code_buffer
      "\t\tif (*work != NULL) {\n\
      \t\t\tspars = *work;\n\
      \t\t\tfor (i = 0; i < %d; i++) block->rpar[i] = spars[i];\n\
      \t\t\tscicos_free(*work);\n\
      \t\t\t*work = NULL;\n\
      \t\t}\n"
      nb_pars

let bufferize_parameter_value init nb_pars model_info =
  if fst init <> None then begin
    if nb_pars > 0 then
      Printf.bprintf
        model_info.code_buffer
        "\t\tspars = *work;\n\
        \t\tfor (i = 0; i < %d; i++) block->rpar[i] = spars[i];\n"
        nb_pars
  end

let bufferize_variable_nature nb_vars model_info =
  if nb_vars > 0 then begin
    Array.iteri
      (fun i variable ->
        let equation = model_info.model.equations.(i) in
        if not equation.solved then begin
          Printf.bprintf
            model_info.code_buffer
            "\t\tblock->xprop[%d] = %s; /* %s"
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
      model_info.model.variables
  end

let rec last = function
  | [] -> failwith "last"
  | [x] -> x
  | _ :: xs -> last xs

let string_of_c_type = function
  | Instantiation.BooleanType [||] -> "int "
  | Instantiation.IntegerType [||] -> "int "
  | Instantiation.RealType [||] -> "double "
  | Instantiation.RealType dims ->
      Array.fold_left (fun acc _ -> acc ^ ", int ") "double *" dims
  | Instantiation.StringType [||] -> "char *"
  | Instantiation.BooleanType _ | Instantiation.CartesianProduct _ |
    Instantiation.CompoundType _ | Instantiation.IntegerType _ |
    Instantiation.StringType _ ->
      failwith "string_of_c_type: unsupported type"

let generate_c_function_prototype oc name in_types out_types =
  let rec generate_c_function_input_arguments = function
    | [] -> ()
    | [in_type] ->
        Printf.fprintf oc "%s" (string_of_c_type in_type)
    | in_type :: in_types ->
        Printf.fprintf oc "%s, " (string_of_c_type in_type);
        generate_c_function_input_arguments in_types in
  match out_types with
  | [Instantiation.RealType [||]] ->
      Printf.fprintf oc "\nextern double %s(" name;
      generate_c_function_input_arguments in_types;
      Printf.fprintf oc ");\n"
  | _ -> failwith "generate_c_function_prototype: unsupported function type" 

let generate_code path filename fun_name model with_jac _ init =
  let rec to_filename = function
    | [] -> ""
    | [s] -> s
    | s :: ss -> s ^ "/" ^ to_filename ss
  and to_der_id s =
    let i = try String.index s '[' - 2 with Not_found -> String.length s - 1 in
    try
      let n = String.rindex_from s i '.' + 1 in
      String.sub s 0 n ^ "__der_" ^ String.sub s n (String.length s - n)
    with Not_found -> "__der_" ^ s
  and is_derivated expr exprs =
    let is_derivated' expr' = match nature expr' with
      | Derivative (expr'', _) -> expr'' == expr
      | _ -> assert false in
    List.exists is_derivated' exprs in
  let oc = open_out filename
  and oc' = open_out (Filename.chop_extension filename ^ "_incidence_matrix.xml") in
  postprocess_residue model;
  let model_info =
    {
      model = model;
      final_index_of_integer_parameters =
        final_index_of_integer_parameters model;
      final_index_of_string_parameters =
        final_index_of_string_parameters model;
      final_index_of_real_parameters = final_index_of_real_parameters model;
      final_index_of_variables = final_index_of_variables model;
      surfaces = collect_surfaces model;
      occurrence_table = ExpressionTable.create 100;
      current_index = -1;
      max_index = -1;
      code_buffer = Buffer.create 10000;
      real_array_store_size = real_array_store_size model;
      real_arrays = ArrayStore.create 13
    }
  in
  let nb_vars =
    Array.fold_left
      (fun acc equation -> if not equation.solved then acc + 1 else acc)
      0
      model.equations
  and nb_dvars = Array.length model.discrete_variables
  and nb_pars = 
    Array.fold_left
      (fun acc parameter -> if parameter.main then acc + 1 else acc)
      0
      model.parameters
  and nb_modes = List.length model_info.surfaces
  and _, nb_ders =
    Array.fold_left
      (fun (i, acc) variable ->
        i + 1,
        if not model.equations.(i).solved && variable.state then acc + 1
        else acc)
      (0, 0)
      model.variables in
  Printf.fprintf oc "/*\n\
    number of discrete variables = %d\n\
    number of variables = %d\n\
    number of inputs = %d\n\
    number of outputs = %d\n\
    number of modes = %d\n\
    number of zero-crossings = %d\n\
    I/O direct dependency = %s\n\
    */\n\n"
    nb_dvars
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
    (fun (name, in_types, out_types) ->
      generate_c_function_prototype oc (last name) in_types out_types)
    model.external_functions;
  Printf.fprintf oc "\n\n/* Utility functions */\n\n";
  Printf.fprintf oc
    "static double ipow_(double x, int n)\n\
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
     static double ipow(double x, int n)\n\
     {\n\
     \t/* NaNs propagation */\n\
     \tif (x != x || (x == 0.0 && n == 0)) return exp((double)n * log(x));\n\
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
     \tint *ipar = block->ipar;\n\
     \tdouble *rpar = block->rpar;\n\
     \tdouble *z = block->z;\n\
     \tdouble *x = block->x;\n\
     \tdouble *xd = block->xd;\n\
     \tdouble **y = block->outptr;\n\
     \tdouble **u = block->inptr;\n\
     \tdouble **work = (double **)block->work;\n\
     \tdouble *g = block->g;\n\
     \tdouble *res = block->res;\n\
     \tint *jroot = block->jroot;\n\
     \tint *mode = block->mode;\n\
     \tint nevprt = block->nevprt;\n";
  if model_info.real_array_store_size > 0 then
    Printf.fprintf oc "\tdouble real_buffer[%d];\n"
      model_info.real_array_store_size;
  if fst init <> None || snd init <> None then begin
    if nb_pars > 0 then begin
      Printf.fprintf oc
        "\tint i;\n\
        \tdouble *spars;\n\
        \tchar *par_ids[] =\n\
        \t\t{\n";
      let first = ref true in
      Array.iter
        (fun parameter ->
          if parameter.main then begin
            if !first then first := false else Printf.fprintf oc ",\n";
            Printf.fprintf oc "\t\t\t\"%s\"" parameter.p_name
          end)
        model.parameters;
      Printf.fprintf oc
        "\n\
        \t\t};\n"
    end;
    if Array.length model.variables + nb_dvars > 0 then begin
      Printf.fprintf oc
        "\tchar *all_var_ids[] =\n\
        \t\t{\n";
      let first = ref true in
      Array.iter
        (fun variable ->
          if !first then first := false else Printf.fprintf oc ",\n";
          Printf.fprintf oc "\t\t\t\"%s\"" variable.v_name)
        model.variables;
      Array.iter
        (fun discrete_variable ->
          if !first then first := false else Printf.fprintf oc ",\n";
          Printf.fprintf oc "\t\t\t\"%s\"" discrete_variable.d_v_name)
        model.discrete_variables;
      Printf.fprintf oc
        "\n\
        \t\t};\n\
        \tdouble all_var_values[%d];\n"
        (Array.length model.variables + nb_dvars)
    end;
    if nb_vars > 0 then begin
      Printf.fprintf oc
        "\tchar *var_ids[] =\n\
        \t\t{\n";
      let first = ref true in
      Array.iteri
        (fun i variable ->
          if not model.equations.(i).solved then begin
            if !first then first := false else Printf.fprintf oc ",\n";
            Printf.fprintf oc "\t\t\t\"%s\"" variable.v_name
          end)
        model.variables;
      Printf.fprintf oc
        "\n\
        \t\t};\n";
    end;
    if nb_dvars > 0 then begin
      Printf.fprintf oc
        "\tchar *disc_var_ids[] =\n\
        \t\t{\n";
      let first = ref true in
      Array.iter
        (fun discrete_variable ->
          if !first then first := false else Printf.fprintf oc ",\n";
          Printf.fprintf oc "\t\t\t\"%s\"" discrete_variable.d_v_name)
        model.discrete_variables;
      Printf.fprintf oc
        "\n\
        \t\t};\n";
    end;
    if nb_ders > 0 then begin
      Printf.fprintf oc
        "\tchar *der_ids[] =\n\
        \t\t{\n";
      let first = ref true in
      Array.iteri
        (fun i variable ->
          if not model.equations.(i).solved && variable.state then begin
            if !first then first := false else Printf.fprintf oc ",\n";
            Printf.fprintf oc "\t\t\t\"%s\"" (to_der_id variable.v_name)
          end)
        model.variables;
      Printf.fprintf oc
        "\n\
        \t\t};\n\
        \tdouble der_values[%d];\n"
        nb_ders
    end
  end;
  Printf.fprintf oc "\n";
  Printf.bprintf model_info.code_buffer "\tif (flag == 0) {\n";
  bufferize_parameter_value init nb_pars model_info;
  bufferize_equations model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 1) {\n";
  bufferize_parameter_value init nb_pars model_info;
  bufferize_outputs model_info;
  Printf.bprintf model_info.code_buffer
    "\t} else if (flag == 2 && nevprt < 0) {\n";
  bufferize_parameter_value init nb_pars model_info;
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
  bufferize_initializations init nb_pars nb_dvars nb_vars nb_ders model_info;
  if with_jac then begin
    Printf.bprintf model_info.code_buffer "\t\tSet_Jacobian_flag(1);\n";
  end;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 5) {\n";
  bufferize_work_deallocation init nb_pars model_info;
  bufferize_variable_store init nb_pars nb_dvars nb_ders model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 6) {\n";
  bufferize_parameter_value init nb_pars model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 7) {\n";
  bufferize_parameter_value init nb_pars model_info;
  bufferize_variable_nature nb_vars model_info;
  Printf.bprintf model_info.code_buffer "\t} else if (flag == 9) {\n";
  bufferize_parameter_value init nb_pars model_info;
  bufferize_surfaces model_info;
  if with_jac then begin
    Printf.bprintf model_info.code_buffer "\t} else if (flag == 10) {\n";
    bufferize_parameter_value init nb_pars model_info;
    bufferize_jacobian nb_vars model_info;
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
  Printf.fprintf oc' "<model>\n";
  Printf.fprintf oc'
    "\t<model_info>\n\
    \t\t<number_of_integer_parameters>%d</number_of_integer_parameters>\n\
    \t\t<number_of_real_parameters>%d</number_of_real_parameters>\n\
    \t\t<number_of_string_parameters>%d</number_of_string_parameters>\n\
    \t\t<number_of_discrete_variables>%d</number_of_discrete_variables>\n\
    \t\t<number_of_continuous_variables>%d</number_of_continuous_variables>\n\
    \t\t<number_of_continuous_unknowns>%d</number_of_continuous_unknowns>\n\
    \t\t<number_of_continuous_states>%d</number_of_continuous_states>\n\
    \t\t<number_of_inputs>%d</number_of_inputs>\n\
    \t\t<number_of_outputs>%d</number_of_outputs>\n\
    \t\t<number_of_modes>%d</number_of_modes>\n\
    \t\t<number_of_zero_crossings>%d</number_of_zero_crossings>\n\
    \t</model_info>\n"
    (Array.fold_left max (-1) model_info.final_index_of_integer_parameters + 1)
    nb_pars
    (Array.fold_left max (-1) model_info.final_index_of_string_parameters + 1)
    nb_dvars
    (Array.length model.variables)
    nb_vars
    nb_ders
    (Array.length model.inputs)
    (Array.fold_left
      (fun acc variable -> if variable.output <> None then acc + 1 else acc)
      0
      model.variables)
    nb_modes
    (List.length model.when_clauses + nb_modes);
  Printf.fprintf oc' "\t<identifiers>\n";
  Array.iter
    (fun par ->
      if par.main then
        Printf.fprintf oc' "\t\t<parameter>%s</parameter>\n" par.p_name)
    model_info.model.parameters;
  Array.iteri
    (fun i var ->
      let equ = model_info.model.equations.(i) in
      if equ.solved then
        Printf.fprintf
          oc'
          "\t\t<explicit_variable>%s</explicit_variable>\n"
          var.v_name
      else
        Printf.fprintf
          oc'
          "\t\t<implicit_variable>%s</implicit_variable>\n"
          var.v_name)
    model_info.model.variables;
  Array.iter
    (fun s -> Printf.fprintf oc' "\t\t<input>%s</input>\n" s)
    model_info.model.inputs;
  Printf.fprintf oc' "\t</identifiers>\n";
  Printf.fprintf oc' "\t<implicit_relations>\n";
  Array.iter
    (fun equ ->
      if not equ.solved then begin
        Printf.fprintf oc' "\t\t<implicit_relation>\n";
        List.iter
          (fun expr ->
            match nature expr with
            | Parameter i when model_info.model.parameters.(i).main ->
                Printf.fprintf oc'
                  "\t\t\t<parameter>%s</parameter>\n"
                  model_info.model.parameters.(i).p_name
            | Parameter _ -> ()
            | _ -> assert false)
          (assignable_parameters_of equ.expression);
        List.iter
          (fun expr ->
            match nature expr with
            | Variable i when is_derivated expr equ.inner_derivatives ->
                Printf.fprintf oc'
                  "\t\t\t<derivative>%s</derivative>\n"
                  model_info.model.variables.(i).v_name
            | Variable i ->
                Printf.fprintf oc'
                  "\t\t\t<variable>%s</variable>\n"
                  model_info.model.variables.(i).v_name
            | _ -> assert false)
          equ.assignable_variables;
        Printf.fprintf oc' "\t\t</implicit_relation>\n"
      end)
    model_info.model.equations;
  Printf.fprintf oc' "\t</implicit_relations>\n";
  Printf.fprintf oc' "\t<outputs>\n";
  Array.iteri
    (fun i variable ->
      match variable.output with
        | None -> ()
        | Some j ->
            Printf.fprintf oc' "\t\t<output>\n";
            Printf.fprintf oc' "\t\t\t<name>%s</name>\n" variable.v_name;
            Printf.fprintf oc' "\t\t\t<order>%d</order>\n" j;
            Printf.fprintf oc' "\t\t\t<dependencies>\n";
            let inputs, vars =
              let equ = model_info.model.equations.(i) in
              if equ.solved then
                inputs_of equ.expression, variables_of equ.expression
              else [], [create_variable i] in
            List.iter
              (fun expr ->
                match nature expr with
                | DiscreteVariable i ->
                    Printf.fprintf oc' "\t\t\t\t<input>%s</input>\n"
                      model_info.model.inputs.(-1 - i)
                | _ -> assert false)
              inputs;
            List.iter
              (fun expr ->
                match nature expr with
                | Variable i ->
                    Printf.fprintf oc'
                      "\t\t\t\t<variable>%s</variable>\n"
                      model_info.model.variables.(i).v_name
                | _ -> assert false)
              vars;
              Printf.fprintf oc' "\t\t\t</dependencies>\n";
              Printf.fprintf oc' "\t\t</output>\n")
      model_info.model.variables;
  Printf.fprintf oc' "\t</outputs>\n";
  Printf.fprintf oc' "</model>\n";
  close_out oc;
  close_out oc'
