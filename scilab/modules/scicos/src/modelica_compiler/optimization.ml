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

open SymbolicExpression


exception Can't_perform_inversion

let union xs ys =
  List.fold_left (fun xs y -> if List.memq y xs then xs else y :: xs) xs ys

module StringMap = Map.Make (struct type t = string let compare = compare end)

let map_length map = StringMap.fold (fun _ _ acc -> acc + 1) map 0

module IntegerElement =
  struct
    type t = Infinity | Int of int
    let zero = Int 0
    let infinity = Infinity
    let equal = ( = )
    let compare x y = match x, y with
      | Int i, Int i' -> compare i i'
      | Int _, Infinity -> -1
      | Infinity, Int _ -> 1
      | Infinity, Infinity -> 0
    let add x y = match x, y with
      | Int i, Int i' -> Int (i + i')
      | _, Infinity | Infinity, _ -> Infinity
    let sub x y = match x, y with
      | Int i, Int i' -> Int (i - i')
      | Infinity, Int _ -> Infinity
      | _, Infinity -> assert false
  end

module IntegerHungarianMethod =
  HungarianMethod.Make
    (IntegerElement)
    (SquareSparseMatrix.Make (IntegerElement))
    (BipartiteGraph)

type identifier_maps =
  {
    parameters_map: parameter StringMap.t Lazy.t;
    inputs_map: input StringMap.t Lazy.t;
    discrete_variables_map: variable StringMap.t Lazy.t;
    variables_map: variable StringMap.t Lazy.t
  }

and parameter =
  {
    parameter_kind: Instantiation.parameter_kind;
    parameter_type: parameter_type;
    parameter_id: int;
    parameter_comment: string;
    parameter_start: t Lazy.t
  }

and parameter_type = IntegerType | StringType | RealType

and input =
  {
    input_id: int;
    input_name: string;
    input_comment: string;
  }

and variable =
  {
    variable_id: int;
    variable_comment: string;
    variable_start: t Lazy.t option
  }

and model =
  {
    mutable inputs: string array;
    mutable parameters: parameter_description array;
    mutable discrete_variables: discrete_variable_description array;
    mutable variables: variable_description array;
    mutable equations: equation_description array;
    mutable reinitializable_variables: t list;
    mutable when_clauses: (t * when_expression list) list;
    mutable io_dependency: bool;
    mutable external_functions:
      (string list *
      Instantiation.expression_type list *
      Instantiation.expression_type list) list;
    trace: string option;
    variables_infos: (string * Compilation.variable_infos) list
  }

and parameter_description =
  {
    mutable main: bool;
    mutable p_type: parameter_type;
    mutable p_name: string;
    mutable p_comment: string;
    mutable value: t
  }

and discrete_variable_description =
  {
    mutable d_output: int option;
    mutable d_v_name: string;
    mutable d_v_comment: string;
    mutable d_start_value: t option
  }

and variable_description =
  {
    mutable output: int option;
    mutable state: bool;
    mutable v_name: string;
    mutable depth_in_hierarchy: int;
    mutable v_comment: string;
    mutable start_value: t option
  }

and equation_description =
  {
    mutable solved: bool;
    mutable inner_variables: t list;
    mutable inner_derivatives: t list;
    mutable assignable_variables: t list;
    mutable expression: t
  }

and when_expression =
  | Assign of t * t
  | Reinit of t * t


let scaling_factor = Num.power_num (Num.Int 10) (Num.Int 16)

let exists_array p xs =
  let l = Array.length xs in
  let rec exists_array_from i =
    i < l && (p xs.(i) || exists_array_from (i + 1)) in
  exists_array_from 0

let num_of_float f =
  let num_of_positive_float f =
    let m, e = frexp f in
    let sm = string_of_float m in
    let s = String.make 16 '0' in
    String.blit sm 2 s 0 (String.length sm - 2);
    let e' = Num.power_num (Num.Int 2) (Num.num_of_int e) in
    Num.div_num (Num.mult_num (Num.num_of_string s) e') scaling_factor
  in
  if f = 0.0 then Num.Int 0
  else if f < 0.0 then
    let num = num_of_positive_float (abs_float f) in
    Num.minus_num num
  else num_of_positive_float f

let string_of_reference = function
  | [(name, [||])] -> name
  | _ -> failwith "string_of_reference: bad reference"

let is_main_parameter id parameters_map =
  let parameter = StringMap.find id parameters_map in
  match parameter.parameter_kind with
    | Instantiation.Main -> true
    | Instantiation.Sub -> false

let get_parameter_start id parameters_map =
  let parameter = StringMap.find id parameters_map in
  Lazy.force parameter.parameter_start

let get_start_value id variables_map =
  let variable = StringMap.find id variables_map in
  match variable.variable_start with
    | None -> zero
    | Some lexpr -> Lazy.force lexpr

let rec symbolic_expression_of_expression inl_par maps iexpr =
  let rec symbolic_argument_of_argument = function
      | Instantiation.ScalarArgument iexpr ->
          ScalarArgument (symbolic_expression_of_expression' iexpr)
      | Instantiation.ArrayArgument (dims, iexprs) ->
          ArrayArgument
            (dims, Array.map symbolic_expression_of_expression' iexprs)
  and symbolic_expression_of_expression' iexpr =
    match iexpr.Instantiation.tex_expression with
      | None -> assert false
      | Some expr -> symbolic_expression_of_expression'' expr
  and symbolic_expression_of_expression'' = function
      | Instantiation.Abs iexpr ->
          let expr = symbolic_expression_of_expression' iexpr in
          symbolic_if
            (symbolic_ge expr zero)
            expr
            (symbolic_minus expr)
      | Instantiation.Acos iexpr ->
          symbolic_acos (symbolic_expression_of_expression' iexpr)
      | Instantiation.Acosh iexpr ->
          symbolic_acosh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Addition (iexpr, iexpr') ->
          symbolic_add
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.And (iexpr, iexpr') ->
          symbolic_and
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Asin iexpr ->
          symbolic_asin (symbolic_expression_of_expression' iexpr)
      | Instantiation.Asinh iexpr ->
          symbolic_asinh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Atan iexpr ->
          symbolic_atan (symbolic_expression_of_expression' iexpr)
      | Instantiation.Atanh iexpr ->
          symbolic_atanh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Boolean false -> false_value
      | Instantiation.Boolean true -> true_value
      | Instantiation.Cardinality _ ->
          invalid_arg "symbolic_expression_of_expression'"
      | Instantiation.CompoundElement _ ->
          invalid_arg "symbolic_expression_of_expression'"
      | Instantiation.Cos iexpr ->
          symbolic_cos (symbolic_expression_of_expression' iexpr)
      | Instantiation.Cosh iexpr ->
          symbolic_cosh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Der iexpr ->
          symbolic_derivative (symbolic_expression_of_expression' iexpr)
      | Instantiation.Division (iexpr, iexpr') ->
          symbolic_div
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Equals (iexpr, iexpr') ->
          symbolic_eq
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Exp iexpr ->
          symbolic_exp (symbolic_expression_of_expression' iexpr)
      | Instantiation.ExternalFunctionCall (path, _, _, iargs) ->
          symbolic_blackBox
            (function_name_of path)
            (List.map symbolic_argument_of_argument iargs)
      | Instantiation.Floor iexpr ->
          symbolic_floor (symbolic_expression_of_expression' iexpr)
      | Instantiation.GreaterEqualThan (iexpr, iexpr') ->
          symbolic_ge
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.GreaterThan (iexpr, iexpr') ->
          symbolic_gt
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.If (iif_exprs, iexpr) ->
          List.fold_right
            (fun (iexpr, iexpr') expr ->
              symbolic_if
                (symbolic_expression_of_expression' iexpr)
                (symbolic_expression_of_expression' iexpr')
                expr)
            iif_exprs
            (symbolic_expression_of_expression' iexpr)
      | Instantiation.Integer i -> create_integer i
      | Instantiation.Log iexpr ->
          symbolic_log (symbolic_expression_of_expression' iexpr)
      | Instantiation.Max (iexpr, iexpr') ->
          symbolic_max
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Min (iexpr, iexpr') ->
          symbolic_min
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Minus iexpr ->
          symbolic_minus (symbolic_expression_of_expression' iexpr)
      | Instantiation.Mod (iexpr, iexpr') ->
          let expr = symbolic_expression_of_expression' iexpr
          and expr' = symbolic_expression_of_expression' iexpr'
          in
          symbolic_sub
            expr
            (symbolic_mult (symbolic_floor (symbolic_div expr expr')) expr')
      | Instantiation.Multiplication (iexpr, iexpr') ->
          symbolic_mult
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.NoEvent iexpr ->
          create_blackBox
            "noEvent"
            [ScalarArgument (symbolic_expression_of_expression' iexpr)]
      | Instantiation.Not iexpr ->
          symbolic_not (symbolic_expression_of_expression' iexpr)
      | Instantiation.NotEquals (iexpr, iexpr') ->
          symbolic_neq
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Or (iexpr, iexpr') ->
          symbolic_or
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.ParameterValue (_, iref) when inl_par ->
          let id = string_of_reference iref in
          if is_main_parameter id (Lazy.force maps.parameters_map) then
            create_parameter
              (StringMap.find id (Lazy.force maps.parameters_map)).parameter_id
          else
            get_parameter_start id (Lazy.force maps.parameters_map)
      | Instantiation.ParameterValue (_, iref) ->
          let id = string_of_reference iref in
            create_parameter
              (StringMap.find id (Lazy.force maps.parameters_map)).parameter_id
      | Instantiation.Power (iexpr, iexpr') ->
          symbolic_power
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Pre iexpr ->
          symbolic_pre (symbolic_expression_of_expression' iexpr)
      | Instantiation.Real f -> create_number (num_of_float f)
      | Instantiation.Sin iexpr ->
          symbolic_sin (symbolic_expression_of_expression' iexpr)
      | Instantiation.Sinh iexpr ->
          symbolic_sinh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Sqrt iexpr ->
          symbolic_sqrt (symbolic_expression_of_expression' iexpr)
      | Instantiation.String s -> create_string s
      | Instantiation.Subtraction (iexpr, iexpr') ->
          symbolic_sub
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Tan iexpr ->
          symbolic_tan (symbolic_expression_of_expression' iexpr)
      | Instantiation.Tanh iexpr ->
          symbolic_tanh (symbolic_expression_of_expression' iexpr)
      | Instantiation.Time -> time
      | Instantiation.VariableStart (_, iref) ->
          let id = string_of_reference iref in
          begin try get_start_value id (Lazy.force maps.variables_map) with
            | Not_found ->
                get_start_value id (Lazy.force maps.discrete_variables_map)
          end
      | Instantiation.VariableValue (_, iref) -> create_variable_value iref
      | Instantiation.Vector iexprs ->
          invalid_arg "symbolic_expression_of_expression'"
  and create_variable_value iref =
    let id = string_of_reference iref in
    try
      create_variable
        (StringMap.find id (Lazy.force maps.variables_map)).variable_id
    with
      | _ ->
          try
            let dvar =
              StringMap.find id (Lazy.force maps.discrete_variables_map)
            in create_discrete_variable dvar.variable_id
          with
            | _ ->
                try
                  let inp = StringMap.find id (Lazy.force maps.inputs_map) in
                  create_discrete_variable (-1 - (inp.input_id))
                  (* Use of strictly negative values to be able to distinguish
                     inputs from other discrete variables. *)
                with
                  | _ -> assert false
  and function_name_of = function
    | [] -> assert false
    | [s] -> s
    | s :: ss -> function_name_of ss
  in symbolic_expression_of_expression' iexpr

let collect_external_function_names iequs =
  let rec add_if_not_in (name, in_types, out_types) = function
    | [] -> [name, in_types, out_types]
    | ((name', _, _) :: _) as name_and_types when name = name' ->
        name_and_types
    | name_and_type :: name_and_types ->
        name_and_type ::
        add_if_not_in (name, in_types, out_types) name_and_types in
  let rec collect_in_equations funcalls = function
    | [] -> funcalls
    | Instantiation.Equation (iexpr, iexpr') :: iequs' ->
        let funcalls = collect_in_expressions funcalls iexpr in
        let funcalls = collect_in_expressions funcalls iexpr' in
        collect_in_equations funcalls iequs'
    | Instantiation.ConditionalEquation (iif_equs, iequs) :: iequs' ->
        let funcalls = collect_in_if_clauses funcalls iif_equs in
        let funcalls = collect_in_equations funcalls iequs in
        collect_in_equations funcalls iequs'
    | Instantiation.When iwhen_clauses :: iequs' ->
        let funcalls = collect_in_when_clauses funcalls iwhen_clauses in
        collect_in_equations funcalls iequs'
    | Instantiation.FlowConnection _ :: _-> assert false
  and collect_in_if_clauses funcalls = function
    | [] -> funcalls
    | (iexpr, iequs) :: iif_equs ->
        let funcalls = collect_in_expressions funcalls iexpr in
        let funcalls = collect_in_equations funcalls iequs in
        collect_in_if_clauses funcalls iif_equs
  and collect_in_when_clauses funcalls = function
    | [] -> funcalls
    | (iexpr, iwhen_equs) :: iwhen_clauses ->
        let funcalls = collect_in_expressions funcalls iexpr in
        let funcalls =
          List.fold_left
            (fun
              funcalls
              (Instantiation.Reinit (iexpr, iexpr') |
              Instantiation.Assign (iexpr, iexpr')) ->
              let funcalls =
                collect_in_expressions funcalls iexpr
              in collect_in_expressions funcalls iexpr')
            funcalls
            iwhen_equs
        in collect_in_when_clauses funcalls iwhen_clauses
  and collect_in_expressions funcalls iexpr =
    match iexpr.Instantiation.tex_expression with
      | None -> funcalls
      | Some expr -> collect_in_expressions' funcalls expr
  and collect_in_expressions' funcalls = function
      | Instantiation.Addition (iexpr, iexpr') |
        Instantiation.And (iexpr, iexpr') |
        Instantiation.Division (iexpr, iexpr') |
        Instantiation.Equals (iexpr, iexpr') |
        Instantiation.GreaterEqualThan (iexpr, iexpr') |
        Instantiation.GreaterThan (iexpr, iexpr') |
        Instantiation.Max (iexpr, iexpr') | Instantiation.Min (iexpr, iexpr') |
        Instantiation.Mod (iexpr, iexpr') |
        Instantiation.Multiplication (iexpr, iexpr') |
        Instantiation.NotEquals (iexpr, iexpr') |
        Instantiation.Or (iexpr, iexpr') | Instantiation.Power (iexpr, iexpr') |
        Instantiation.Subtraction (iexpr, iexpr') ->
          let funcalls = collect_in_expressions funcalls iexpr in
          collect_in_expressions funcalls iexpr'
      | Instantiation.ExternalFunctionCall (name, in_types, out_types, iargs) ->
          let funcalls = add_if_not_in (name, in_types, out_types) funcalls in
          List.fold_left collect_in_arguments funcalls iargs
      | Instantiation.If (iif_exprs, iexpr) ->
          let funcalls =
            List.fold_left
              (fun funcalls (iexpr, iexpr') ->
                let funcalls = collect_in_expressions funcalls iexpr in
                collect_in_expressions funcalls iexpr')
              funcalls
              iif_exprs
          in collect_in_expressions funcalls iexpr
      | Instantiation.Minus iexpr | Instantiation.NoEvent iexpr |
        Instantiation.Not iexpr | Instantiation.Abs iexpr |
        Instantiation.Acos iexpr | Instantiation.Acosh iexpr |
        Instantiation.Cos iexpr | Instantiation.Cosh iexpr |
        Instantiation.Exp iexpr |
        Instantiation.Floor iexpr | Instantiation.Log iexpr |
        Instantiation.Asin iexpr | Instantiation.Asinh iexpr |
        Instantiation.Sin iexpr | Instantiation.Sinh iexpr |
        Instantiation.Sqrt iexpr |
        Instantiation.Atan iexpr | Instantiation.Atanh iexpr |
        Instantiation.Tan iexpr | Instantiation.Tanh iexpr |
        Instantiation.Pre iexpr -> collect_in_expressions funcalls iexpr
      | Instantiation.ParameterValue _ | Instantiation.Real _ |
        Instantiation.String _ | Instantiation.Time |
        Instantiation.VariableStart _ | Instantiation.VariableValue _ |
        Instantiation.Boolean _ | Instantiation.Der _ | Instantiation.Integer _ ->
          funcalls
      | Instantiation.Cardinality _ | Instantiation.CompoundElement _ |
        Instantiation.Vector _ -> assert false
  and collect_in_arguments funcalls = function
    | Instantiation.ScalarArgument iexpr ->
        collect_in_expressions funcalls iexpr
    | Instantiation.ArrayArgument (_, iexprs) ->
        Array.fold_left collect_in_expressions funcalls iexprs
  in collect_in_equations [] iequs

let separate_parameters_from_variables icpnts =
  let is_parameter = function
    | _, Instantiation.InstantiatedParameter _ -> true
    | _, Instantiation.InstantiatedVariable _ -> false
  in List.partition is_parameter icpnts

let separate_inputs_from_others icpnts =
  let is_input = function
    | Instantiation.InstantiatedIntegerVariable
        (_, Compilation.Input, _, _) |
      Instantiation.InstantiatedStringVariable
        (_, Compilation.Input, _, _) |
      Instantiation.InstantiatedDiscreteVariable
        (_, Compilation.Input, _, _) |
      Instantiation.InstantiatedRealVariable
        (_, Compilation.Input, _, _, _) ->
        true
    | Instantiation.InstantiatedIntegerVariable _ |
      Instantiation.InstantiatedStringVariable _ |
      Instantiation.InstantiatedDiscreteVariable _ |
      Instantiation.InstantiatedRealVariable _ |
      Instantiation.InstantiatedCompoundVariable _ -> false
  in
  let filter_variable = function
    | _, Instantiation.InstantiatedParameter _ -> false
    | _, Instantiation.InstantiatedVariable ivar -> is_input ivar
  in List.partition filter_variable icpnts

let separate_discrete_variables_from_others icpnts =
  let is_discrete = function
    | Instantiation.InstantiatedDiscreteVariable _ -> true
    | Instantiation.InstantiatedIntegerVariable _ |
      Instantiation.InstantiatedStringVariable _ |
      Instantiation.InstantiatedRealVariable _ |
      Instantiation.InstantiatedCompoundVariable _ -> false
  in
  let filter_variable = function
    | _, Instantiation.InstantiatedParameter _ -> false
    | _, Instantiation.InstantiatedVariable ivar -> is_discrete ivar
  in List.partition filter_variable icpnts

let separate_outputs_from_others icpnts =
  let is_output = function
    | Instantiation.InstantiatedIntegerVariable
        (_, Compilation.Output, _, _) |
      Instantiation.InstantiatedStringVariable
        (_, Compilation.Output, _, _) |
      Instantiation.InstantiatedDiscreteVariable
        (_, Compilation.Output, _, _) |
      Instantiation.InstantiatedRealVariable
        (_, Compilation.Output, _, _, _) ->
        true
    | Instantiation.InstantiatedIntegerVariable _ |
      Instantiation.InstantiatedStringVariable _ |
      Instantiation.InstantiatedDiscreteVariable _ |
      Instantiation.InstantiatedRealVariable _ |
      Instantiation.InstantiatedCompoundVariable _ -> false
  in
  let filter_variable = function
    | _, Instantiation.InstantiatedParameter _ -> false
    | _, Instantiation.InstantiatedVariable ivar -> is_output ivar
  in List.partition filter_variable icpnts

let create_dictionary get_contents maps icpnts =
  let rec add_entries i map = function
    | [] -> map
    | (s, icpnt) :: icpnts ->
        add_entries
          (i + 1)
          (StringMap.add s (get_contents maps s i icpnt) map)
          icpnts
  in add_entries 0 StringMap.empty icpnts

let separate_whens_from_equations iequs =
  let rec separate_whens_from_equations' whens equations = function
    | [] -> whens, equations
    | (Instantiation.Equation _ as iequ) :: iequs' ->
        separate_whens_from_equations' whens (iequ :: equations) iequs'
    | Instantiation.When iwhen_clauses :: iequs' ->
        separate_whens_from_equations' (whens @ iwhen_clauses) equations iequs'
    | Instantiation.ConditionalEquation _ :: _ |
      Instantiation.FlowConnection _ :: _ -> assert false
  in separate_whens_from_equations' [] [] iequs

let rewrite_when_condition expr =
  let rec contains_time expr = match nature expr with
    | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | Variable _ | Integer _ | String _ -> false
    | TimeVariable  -> true
    | ArcCosine node | ArcHyperbolicCosine node |
      ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
      ArcTangent node | Cosine node | Derivative (node, _) |
      Exponential node | Floor node | HyperbolicCosine node |
      HyperbolicSine node | HyperbolicTangent node | Logarithm node |
      Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
      Tangent node -> contains_time node
    | Equality (node1, node2) | Greater (node1, node2) |
      GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
        contains_time node1 || contains_time node2
    | If (node1, node2, node3) ->
        contains_time node1 || contains_time node2 || contains_time node3
    | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
        List.exists contains_time nodes
    | BlackBox (_, args) -> List.exists contains_time_argument args
  and contains_time_argument = function
    | ScalarArgument node -> contains_time node
    | ArrayArgument (_, nodes) -> exists_array contains_time nodes in
  let is_primary_condition expr =
    assignable_variables_of expr <> [] || contains_time expr in
  let rec rewrite expr = match nature expr with
    | Addition nodes -> apply_addition (List.map rewrite nodes)
    | And nodes -> apply_and (List.map rewrite nodes)
    | ArcCosine node -> symbolic_acos (rewrite node)
    | ArcHyperbolicCosine node -> symbolic_acosh (rewrite node)
    | ArcHyperbolicSine node -> symbolic_asinh (rewrite node)
    | ArcHyperbolicTangent node -> symbolic_atanh (rewrite node)
    | ArcSine node -> symbolic_asin (rewrite node)
    | ArcTangent node -> symbolic_atan (rewrite node)
    | BlackBox (s, args) -> apply_blackBox s (List.map rewrite_argument args)
    | Cosine node -> symbolic_cos (rewrite node)
    | Derivative (node, num) -> symbolic_derive (rewrite node) num
    | Equality (node, node') -> symbolic_eq (rewrite node) (rewrite node')
    | Exponential node -> symbolic_exp (rewrite node)
    | Floor node -> symbolic_floor (rewrite node)
    | Greater (node, node') -> symbolic_gt (rewrite node) (rewrite node')
    | GreaterEqual (node, node') -> symbolic_ge (rewrite node) (rewrite node')
    | HyperbolicCosine node -> symbolic_cosh (rewrite node)
    | HyperbolicSine node -> symbolic_sinh (rewrite node)
    | HyperbolicTangent node -> symbolic_tanh (rewrite node)
    | If (node, node', node'') ->
        symbolic_if (rewrite node) (rewrite node') (rewrite node'')
    | Logarithm node -> symbolic_log (rewrite node)
    | Multiplication nodes -> apply_multiplication (List.map rewrite nodes)
    | Not node -> symbolic_not (rewrite node)
    | Or nodes -> apply_or (List.map rewrite nodes)
    | PartialDerivative (node, node') ->
        create_partialDerivative (rewrite node) (rewrite node')
    | Pre node -> symbolic_pre (rewrite node)
    | RationalPower (node, num) -> symbolic_rationalPower (rewrite node) num
    | Sign node -> symbolic_sgn (rewrite node)
    | Sine node -> symbolic_sin (rewrite node)
    | Tangent node -> symbolic_tan (rewrite node)
    | BooleanValue _ | Constant _ | Number _ | Parameter _ |
      TimeVariable | Variable _ | Integer _ | String _ -> expr
    | DiscreteVariable _ -> symbolic_pre expr
  and rewrite_argument = function
    | ScalarArgument expr -> ScalarArgument (rewrite expr)
    | ArrayArgument (dims, exprs) ->
        ArrayArgument (dims, Array.map rewrite exprs) in
  if is_primary_condition expr then rewrite expr else expr

let symbolic_equation inl_par maps = function
  | Instantiation.Equation (iexpr, iexpr') ->
      let expr =
        symbolic_sub
          (symbolic_expression_of_expression inl_par maps iexpr)
          (symbolic_expression_of_expression inl_par maps iexpr')
      in
      {
        solved = false;
        inner_variables = variables_of expr;
        inner_derivatives = derivatives_of expr;
        assignable_variables = assignable_variables_of expr;
        expression = expr
      }
  | _ -> assert false

let symbolic_surfaces inl_par maps when_clauses =
  List.map
    (fun (iexpr, surfaces) ->
      let expr = symbolic_expression_of_expression inl_par maps iexpr in
      rewrite_when_condition expr,
      List.map
        (function
          | Instantiation.Reinit (iexpr, iexpr') ->
            let var = symbolic_expression_of_expression inl_par maps iexpr in
            begin match nature var with
              | Variable i ->
                  Reinit (var, symbolic_expression_of_expression inl_par maps iexpr')
              | _ -> assert false
            end
          | Instantiation.Assign (iexpr, iexpr') ->
            let var = symbolic_expression_of_expression inl_par maps iexpr in
            begin match nature var with
              | DiscreteVariable i ->
                  Assign (var, symbolic_expression_of_expression inl_par maps iexpr')
              | Variable i ->
                  Reinit (var, symbolic_expression_of_expression inl_par maps iexpr')
              | _ -> assert false
            end)
        surfaces)
    when_clauses

let propagate_noEvent expr =
  (* such that 'noEvent' only appears in conditions *)
  let rec propagate_noEvent' no_event expr = match nature expr with
    | And exprs' ->
        create_and (sort (List.map (propagate_noEvent' no_event) exprs'))
    | ArcCosine expr' -> create_arcCosine (propagate_noEvent' no_event expr')
    | ArcHyperbolicCosine expr' ->
        create_arcHyperbolicCosine (propagate_noEvent' no_event expr')
    | ArcHyperbolicSine expr' ->
        create_arcHyperbolicSine (propagate_noEvent' no_event expr')
    | ArcHyperbolicTangent expr' ->
        create_arcHyperbolicTangent (propagate_noEvent' no_event expr')
    | ArcSine expr' -> create_arcSine (propagate_noEvent' no_event expr')
    | ArcTangent expr' -> create_arcTangent (propagate_noEvent' no_event expr')
    | Cosine expr' -> create_cosine (propagate_noEvent' no_event expr')
    | Derivative (expr', num) ->
        create_derivative (propagate_noEvent' no_event expr') num
    | Equality (expr', expr'') ->
        create_equality
          (propagate_noEvent' no_event expr')
          (propagate_noEvent' no_event expr'')
    | Exponential expr' ->
        create_exponential (propagate_noEvent' no_event expr')
    | Floor expr' -> create_floor (propagate_noEvent' no_event expr')
    | Greater (expr', expr'') ->
        create_greater
          (propagate_noEvent' no_event expr')
          (propagate_noEvent' no_event expr'')
    | GreaterEqual (expr', expr'') ->
        create_greater_equal
          (propagate_noEvent' no_event expr')
          (propagate_noEvent' no_event expr'')
    | HyperbolicCosine expr' ->
        create_hyperbolicCosine (propagate_noEvent' no_event expr')
    | HyperbolicSine expr' ->
        create_hyperbolicSine (propagate_noEvent' no_event expr')
    | HyperbolicTangent expr' ->
        create_hyperbolicTangent (propagate_noEvent' no_event expr')
    | Logarithm expr' -> create_logarithm (propagate_noEvent' no_event expr')
    | Pre expr' -> create_pre (propagate_noEvent' no_event expr')
    | RationalPower (expr', num) ->
        create_rationalPower (propagate_noEvent' no_event expr') num
    | Sign expr' -> create_sign (propagate_noEvent' no_event expr')
    | Sine expr' -> create_sine (propagate_noEvent' no_event expr')
    | Tangent expr' -> create_tangent (propagate_noEvent' no_event expr')
    | Addition exprs' ->
        create_addition (sort (List.map (propagate_noEvent' no_event) exprs'))
    | BlackBox ("noEvent", [ScalarArgument expr']) -> propagate_noEvent' true expr'
    | BlackBox (name, args) ->
        create_blackBox
          name
          (List.map (propagate_noEvent_into_argument no_event) args)
    | Multiplication exprs' ->
        create_multiplication
          (sort (List.map (propagate_noEvent' no_event) exprs'))
    | Not expr' -> create_not (propagate_noEvent' no_event expr')
    | Or exprs' ->
        create_or (sort (List.map (propagate_noEvent' no_event) exprs'))
    | PartialDerivative (expr', expr'') ->
        create_partialDerivative
          (propagate_noEvent' no_event expr')
          (propagate_noEvent' no_event expr'')
    | If (expr', expr'', expr''') ->
        propagate_noEvent_into_if no_event expr' expr'' expr'''
    | BooleanValue _ | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> expr
  and propagate_noEvent_into_argument no_event = function
    | ScalarArgument expr -> ScalarArgument (propagate_noEvent' no_event expr)
    | ArrayArgument (dims, exprs) ->
        ArrayArgument (dims, Array.map (propagate_noEvent' no_event) exprs)
  and propagate_noEvent_into_if no_event expr expr' expr'' =
    let cond =
      if no_event then
        create_blackBox "noEvent" [ScalarArgument (propagate_noEvent' false expr)]
      else begin match nature expr with
        | BlackBox ("noEvent", [ScalarArgument expr']) ->
            create_blackBox
              "noEvent"
              [ScalarArgument (propagate_noEvent' false expr)]
        | _ -> propagate_noEvent' false expr
      end
    in
    create_if
      cond
      (propagate_noEvent' no_event expr')
      (propagate_noEvent' no_event expr'')
  in propagate_noEvent' false expr

let create_model' trace inl_par iexpr =
  let lazy_symbolic_expression_of_expression maps iexpr =
    match iexpr.Instantiation.tex_expression with
    | None -> None
    | Some _ ->
        Some (lazy (symbolic_expression_of_expression inl_par maps iexpr))
  in
  let get_parameter_info maps s i = function
    | Instantiation.InstantiatedParameter (
        Instantiation.InstantiatedIntegerParameter (s', kind, iexpr, _)) ->
        {
          parameter_kind = kind;
          parameter_type = IntegerType;
          parameter_id = i;
          parameter_comment = s';
          parameter_start = lazy
            (symbolic_expression_of_expression inl_par maps iexpr)
        }
    | Instantiation.InstantiatedParameter (
        Instantiation.InstantiatedStringParameter (s', kind, iexpr, _)) ->
        {
          parameter_kind = kind;
          parameter_type = StringType;
          parameter_id = i;
          parameter_comment = s';
          parameter_start = lazy
            (symbolic_expression_of_expression inl_par maps iexpr)
        }
    | Instantiation.InstantiatedParameter (
        Instantiation.InstantiatedRealParameter (s', kind, iexpr, _)) ->
        {
          parameter_kind = kind;
          parameter_type = RealType;
          parameter_id = i;
          parameter_comment = s';
          parameter_start = lazy
            (symbolic_expression_of_expression inl_par maps iexpr)
        }
    | _ -> assert false
  and get_input_info maps s i = function
    | Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedDiscreteVariable (s', _, _, _)) |
      Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedRealVariable (s', _, _, _, _)) ->
        {
          input_id = i;
          input_name = s;
          input_comment = s'
        }
    | _ -> assert false
  and get_variable_info maps s i = function
    | Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedDiscreteVariable (s', _, iexpr, _)) |
      Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedRealVariable (s', _, _, iexpr, _)) ->
        {
          variable_id = i;
          variable_comment = s';
          variable_start = lazy_symbolic_expression_of_expression maps iexpr
        }
    | _ -> assert false
  in
  let derived_variables ders =
    List.fold_left
      (fun vars der ->
        match nature der with
          | Derivative (expr, num) when num = Num.Int 1 ->
              begin match nature expr with
                | Variable _ -> expr :: vars
                | _ -> assert false
              end
          | _ -> assert false)
      []
      ders
  in
  let add_variable_infos acc (id, icpnt) = match icpnt with
    | Instantiation.InstantiatedVariable
        (Instantiation.InstantiatedDiscreteVariable (_, _, _, var_infos) |
         Instantiation.InstantiatedRealVariable (_, _, _, _, var_infos)) ->
        (id, var_infos) :: acc
    | Instantiation.InstantiatedParameter
        (Instantiation.InstantiatedIntegerParameter (_, _, _, pat_infos) |
         Instantiation.InstantiatedRealParameter (_, _, _, pat_infos)) ->
        (id, pat_infos) :: acc
    | _ -> acc
  in
  let icpnts, iinit_equs, iequs = Instantiation.expand_class iexpr in
  let variables_infos =
    List.fold_left add_variable_infos [] icpnts in
  let parameters, variables = separate_parameters_from_variables icpnts in
  let inputs, non_inputs = separate_inputs_from_others variables in
  let discrete_variables, others =
    separate_discrete_variables_from_others non_inputs in
  let outputs, _ = separate_outputs_from_others non_inputs in
  let name_and_types =
    collect_external_function_names iequs @
    collect_external_function_names iinit_equs in
  let rec maps =
    {
      parameters_map =
        lazy (create_dictionary get_parameter_info maps parameters);
      inputs_map =
        lazy (create_dictionary get_input_info maps inputs);
      discrete_variables_map =
       lazy (create_dictionary get_variable_info maps discrete_variables);
      variables_map =
        lazy (create_dictionary get_variable_info maps others)
    }
  in
  let when_clauses, equations = separate_whens_from_equations iequs in
  let nb_parameters = map_length (Lazy.force maps.parameters_map)
  and nb_inputs = map_length (Lazy.force maps.inputs_map)
  and nb_discrete_vars = map_length (Lazy.force maps.discrete_variables_map)
  and nb_vars = map_length (Lazy.force maps.variables_map)
  and nb_equs = List.length equations in
  if nb_equs <> nb_vars then
    failwith
      ("The number of equations doesn't match the number of variables: " ^
      string_of_int nb_equs ^ " equations and " ^ string_of_int nb_vars ^
      " variables.")
  else
    let parameters_array =
      Array.init
        nb_parameters
        (fun _ ->
          {
            main = false;
            p_type = IntegerType;
            p_name = "";
            p_comment = "";
            value = zero
          })
    and inputs_array = Array.make nb_inputs ""
    and discrete_variables_array =
      Array.init
        nb_discrete_vars
        (fun _ ->
          {
            d_output = None;
            d_v_name = "";
            d_v_comment = "";
            d_start_value = Some zero
          })
    and variables_array =
      Array.init
        nb_vars
        (fun _ ->
          {
            output = None;
            state = true;
            v_name = "";
            depth_in_hierarchy = 0;
            v_comment = "";
            start_value = Some zero
          })
    and equations_array =
      Array.init
        nb_equs
        (fun _ ->
          {
            solved = false;
            inner_variables = [];
            inner_derivatives = [];
            assignable_variables = [];
            expression = zero
          })
    in
    let output_index s outputs =
      let rec output_index' i = function
        | [] -> None
        | (s', _) :: _ when s' = s -> Some i
        | _ :: outputs' -> output_index' (i + 1) outputs'
      in output_index' 0 outputs
    in
    let number_of_dots s =
      let count = ref 0 in
      for i = 0 to String.length s - 1 do
        if s.[i] = '.' then incr count;
      done;
      !count
    in
    let _ =
      List.fold_left
        (fun i equ ->
          assert (i < Array.length equations_array);
          equations_array.(i) <- symbolic_equation inl_par maps equ; i + 1)
        0
        equations
    in ();
    let derived_variables =
      Array.fold_left
        (fun vars equation ->
          union vars (derived_variables equation.inner_derivatives))
        []
        equations_array
    in
    StringMap.iter
      (fun s param ->
        assert (param.parameter_id < Array.length parameters_array);
        let parameter = parameters_array.(param.parameter_id) in
        parameter.main <- param.parameter_kind = Instantiation.Main;
        parameter.p_type <- param.parameter_type;
        parameter.p_name <- s;
        parameter.p_comment <- param.parameter_comment;
        parameter.value <- Lazy.force param.parameter_start)
      (Lazy.force maps.parameters_map);
    StringMap.iter
      (fun _ inp ->
        assert (inp.input_id < Array.length inputs_array);
        inputs_array.(inp.input_id) <- inp.input_name)
      (Lazy.force maps.inputs_map);
    StringMap.iter
      (fun s dvar ->
        assert (dvar.variable_id < Array.length discrete_variables_array);
        let variable = discrete_variables_array.(dvar.variable_id) in
        variable.d_output <- output_index s outputs;
        variable.d_v_name <- s;
        variable.d_v_comment <- dvar.variable_comment;
        variable.d_start_value <-
          match dvar.variable_start with
            | None -> None
            | Some lexpr -> Some (Lazy.force lexpr))
      (Lazy.force maps.discrete_variables_map);
    StringMap.iter
      (fun s var ->
        assert (var.variable_id < Array.length variables_array);
        let variable = variables_array.(var.variable_id) in
        variable.output <- output_index s outputs;
        variable.state <-
          List.memq (create_variable var.variable_id) derived_variables;
        variable.v_name <- s;
        variable.depth_in_hierarchy <- number_of_dots s;
        variable.v_comment <- var.variable_comment;
        variable.start_value <-
          match var.variable_start with
            | None -> None
            | Some lexpr -> Some (Lazy.force lexpr))
      (Lazy.force maps.variables_map);
    let when_clauses_list = symbolic_surfaces inl_par maps when_clauses in
    let reinitializable_variables =
      let add_non_discrete_variables vars = function
        | Reinit (var, _) when not (List.memq var vars) -> var :: vars
        | _ -> vars
      in
      List.fold_left
        (fun vars (_, when_expr) ->
          List.fold_left add_non_discrete_variables vars when_expr)
        []
        when_clauses_list
    in
    Array.iter
      (fun equation ->
        equation.expression <- propagate_noEvent equation.expression)
      equations_array;
    {
      parameters = parameters_array;
      inputs = inputs_array;
      discrete_variables = discrete_variables_array;
      variables = variables_array;
      equations = equations_array;
      reinitializable_variables = reinitializable_variables;
      when_clauses = when_clauses_list;
      io_dependency = false;
      external_functions = name_and_types;
      trace = trace;
      variables_infos = variables_infos
    }

let create_model_with_parameters trace iexpr = create_model' trace false iexpr

let create_model trace iexpr = create_model' trace true iexpr

let print_model oc model =
  Printf.fprintf
    oc
    "Number of variables before simplifications: %d\n"
    (Array.length model.variables);
  Printf.fprintf
    oc
    "Number of variables after simplifications: %d\n"
    (Array.fold_left
      (fun n equation -> if equation.solved then n else n + 1)
      0
      model.equations);
  Printf.fprintf
    oc
    "Direct input/ouput dependency: %s\n"
    (if model.io_dependency then "yes" else "no");
  Array.iteri
    (fun i variable ->
      Printf.fprintf
        oc
        "variable (%d) (%s): %s %s variable (%ssolved)\n"
        i
        variable.v_name
        (if variable.output <> None then "output" else "intermediate")
        (if variable.state then "state" else "algebraic")
        (if model.equations.(i).solved then "" else "not "))
    model.variables;
  Array.iteri
    (fun i equation ->
      Printf.fprintf oc "equation(%d): " i;
      if equation.solved then output oc (create_variable i)
      else Printf.fprintf oc "0";
      Printf.fprintf oc " = ";
      output oc equation.expression;
      Printf.fprintf oc "\n")
    model.equations

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

let permute_equations model assocs =
  let equations = Array.copy model.equations in
  List.iter
    (function
      | i, Some j -> equations.(i) <- model.equations.(j)
      | _, None -> assert false)
    assocs;
  model.equations <- equations

let perform_then_propagate_inversion model i =
  let update_clauses var expr' clauses =
    List.map
      (fun (expr, updates) ->
        replace var expr' expr,
        List.map
          (function
            | Assign (expr1, expr2) ->
                Assign (expr1, replace var expr' expr2)
            | Reinit (expr1, expr2) ->
                Reinit (expr1, replace var expr' expr2))
          updates)
      clauses
  in
  let var = create_variable i
  and equation = model.equations.(i) in
  let expr = equation.expression in
  try match invert_if_possible_with_respect_to var expr zero with
    | None -> false
    | Some expr' ->
        if not (List.memq var model.reinitializable_variables) then begin
          equation.expression <- expr';
          equation.solved <- true;
          let additional_variables = variables_of expr' in
          Array.iteri
            (fun j equation ->
              if i <> j && List.memq var equation.inner_variables then begin
                equation.expression <- replace var expr' equation.expression;
                equation.inner_variables <-
                  union equation.inner_variables additional_variables
              end)
            model.equations;
          model.when_clauses <- update_clauses var expr' model.when_clauses
        end;
        true
  with Invalid_argument _ -> raise Can't_perform_inversion

let compute_io_dependency model =
  let rec compute_io_dependency' i =
    if not (i = Array.length model.variables) then
      if
        model.variables.(i).output <> None &&
        exists
          (fun node -> match nature node with
            | DiscreteVariable i when i < 0 -> true
            | _ -> false)
          model.equations.(i).expression
      then
        model.io_dependency <- true
      else
        compute_io_dependency' (i + 1)
  in compute_io_dependency' 0

let perform_hungarian_method model =
  let size =
    Array.fold_left
      (fun acc equation -> if equation.solved then acc else acc + 1)
      0
      model.equations
  in
  let () =
    Array.iter
      (fun equation ->
        if not equation.solved then begin
          equation.inner_variables <- variables_of equation.expression;
          equation.inner_derivatives <- derivatives_of equation.expression;
          equation.assignable_variables <-
            assignable_variables_of equation.expression
        end)
      model.equations
  in
  let table = Array.make size 0 in
  let i = ref 0 in
  for j = 0 to Array.length model.equations - 1 do
    if not model.equations.(j).solved then begin
      table.(!i) <- j; incr i
    end
  done;
  let weight i j =
    let m = table.(i)
    and n = table.(j) in
    let var = create_variable m in
    let discontinuous_state () =
      let not_derivative_of_current_variable e = match nature e with
        | Derivative (var', _) -> var' != var
        | _ -> true in
      List.for_all
        not_derivative_of_current_variable
        model.equations.(n).inner_derivatives &&
      List.memq var model.reinitializable_variables in
    if not (List.memq var model.equations.(n).assignable_variables) then
      IntegerElement.Infinity
    else if discontinuous_state () then
      IntegerElement.Int (size + 1)
    else match inversion_difficulty var model.equations.(n).expression zero with
        | 0 -> IntegerElement.zero
        | 1 -> IntegerElement.Int 1
        | 2 -> IntegerElement.Int (size + 1)
        | _ -> IntegerElement.Infinity
  in
  let strct = IntegerHungarianMethod.init size weight in
  let assocs = IntegerHungarianMethod.perform strct in
  let assocs' =
    List.map
      (function
        | i, Some j -> table.(i), Some table.(j)
        | _, None ->
            failwith "perform_hungarian_method: jacobian is structurally singular")
      assocs
  in
  assert (
    let rec check_results1 = function
      | [] -> true
      | (_, x) :: xs when List.exists (fun (_, y) -> x = y) xs -> false
      | _ :: xs -> check_results1 xs
    in check_results1 assocs');
  assert (
    let check_results2 = function
      | (i, Some j) :: assocs ->
          let var = create_variable i in
          List.memq var model.equations.(j).inner_variables
      | _ -> true
    in check_results2 assocs');
  assocs'

let eliminate_trivial_relations max_simplifs model =
  let max_simplifs_ref = ref max_simplifs in
  let choose_variable i j =
    let sti = model.variables.(i).state
    and depi = model.variables.(i).depth_in_hierarchy
    and stj = model.variables.(j).state
    and depj = model.variables.(j).depth_in_hierarchy in
    match sti, stj with
      | true, false -> j
      | false, true -> i
      | true, true | false, false ->
          let svi = model.variables.(i).start_value
          and svj = model.variables.(j).start_value in
          begin match svi, svj with
            | None, Some _ -> i
            | Some _, None -> j
            | _ when depj > depi -> i
            | _ -> j
          end
  in
  let permute_equations i j =
    let equation = model.equations.(i) in
    model.equations.(i) <- model.equations.(j);
    model.equations.(j) <- equation
  in
  let update_variable_attributes i j =
    let svi = model.variables.(i).start_value
    and svj = model.variables.(j).start_value in
    match svi, svj with
      | None, _ -> model.variables.(i).start_value <- svj
      | _, None -> model.variables.(j).start_value <- svi
      | _ -> ()
  in
  let simplify_trivial_relation n =
    match nature model.equations.(n).expression with
      | Addition [node; node'] when !max_simplifs_ref >= 0 ->
          begin match nature node, nature node' with
            | Variable i, Number _ | Number _, Variable i ->
                permute_equations i n;
                ignore (perform_then_propagate_inversion model i);
                decr max_simplifs_ref
            | Variable i, Multiplication [node; node'] |
              Multiplication [node; node'], Variable i ->
                begin match nature node, nature node' with
                  | Number (Num.Int (-1)), Variable j |
                    Variable j, Number (Num.Int (-1)) ->
                      let k = choose_variable i j in
                      update_variable_attributes i j;
                      permute_equations k n;
                      ignore (perform_then_propagate_inversion model k);
                      decr max_simplifs_ref
                  | _ -> ()
                end
            | _ -> ()
          end
      | _ -> ()
  in
  for i = 0 to Array.length model.equations - 1 do
    simplify_trivial_relation i
  done;
  !max_simplifs_ref

let eliminate_explicit_variables max_simplifs model =
  let is_continuous_state var =
    not (List.memq var model.reinitializable_variables) in
  let is_state_variable expr = match nature expr with
    | Variable j -> model.variables.(j).state
    | _ -> false in
  let try_reducing_state var i =
    let vars = model.equations.(i).assignable_variables
    and ders = model.equations.(i).inner_derivatives in
    if not (List.memq (symbolic_derive var (Num.Int 1)) ders) then begin
      Printf.eprintf "\nTrying to reduce state...";
      match ders with
      | [] when List.for_all is_state_variable vars ->
          let success = perform_then_propagate_inversion model i in
          if success then Printf.eprintf " One state variable removed.%!"
          else Printf.eprintf " Failed.%!"
      | _ ->  Printf.eprintf " Failed.%!"
    end in
  let rec eliminate_explicit_variables' simplifs =
    let assocs = perform_hungarian_method model in
    permute_equations model assocs;
    let bad_variable_choice, success, simplifs =
      List.fold_left
        (fun (bad_variable_choice, success, simplifs) assoc ->
          match assoc with
            | _, None -> assert false
            | i, Some j when simplifs >= 0 ->
                begin try
                  if not model.variables.(i).state then
                    ignore (perform_then_propagate_inversion model i)
                  else begin
                    let var = create_variable i in
                    if is_continuous_state var then try_reducing_state var i
                  end;
                  bad_variable_choice, model.equations.(i).solved, simplifs - 1
                with
                  | Can't_perform_inversion -> true, success, simplifs
                end
            | _ -> bad_variable_choice, success, simplifs)
        (false, false, simplifs)
        assocs
    in
    if bad_variable_choice || success then
      eliminate_explicit_variables' simplifs
  in eliminate_explicit_variables' max_simplifs

let rec rewrite_conditions_in no_event expr =
  let rec remove_no_event expr = match nature expr with
    | ArcCosine expr' -> create_arcCosine (remove_no_event expr')
    | ArcHyperbolicCosine expr' ->
        create_arcHyperbolicCosine (remove_no_event expr')
    | ArcHyperbolicSine expr' ->
        create_arcHyperbolicSine (remove_no_event expr')
    | ArcHyperbolicTangent expr' ->
        create_arcHyperbolicTangent (remove_no_event expr')
    | ArcSine expr' -> create_arcSine (remove_no_event expr')
    | ArcTangent expr' -> create_arcTangent (remove_no_event expr')
    | Cosine expr' -> create_cosine (remove_no_event expr')
    | Derivative (expr', num) ->
        create_derivative (remove_no_event expr') num
    | Exponential expr' -> create_exponential (remove_no_event expr')
    | Floor expr' -> create_floor (remove_no_event expr')
    | HyperbolicCosine expr' ->
        create_hyperbolicCosine (remove_no_event expr')
    | HyperbolicSine expr' ->
        create_hyperbolicSine (remove_no_event expr')
    | HyperbolicTangent expr' ->
        create_hyperbolicTangent (remove_no_event expr')
    | Logarithm expr' -> create_logarithm (remove_no_event expr')
    | Pre expr' -> create_pre (remove_no_event expr')
    | RationalPower (expr', num) ->
        create_rationalPower (remove_no_event expr') num
    | Sign expr' -> create_sign (remove_no_event expr')
    | Sine expr' -> create_sine (remove_no_event expr')
    | Tangent expr' -> create_tangent (remove_no_event expr')
    | Addition exprs' ->
        create_addition (sort (List.map remove_no_event exprs'))
    | BlackBox ("noEvent", [ScalarArgument expr']) ->
        rewrite_conditions_in true expr'
    | BlackBox (name, args) ->
        create_blackBox name (List.map remove_no_event_in_argument args)
    | Multiplication exprs' ->
        create_multiplication (sort (List.map remove_no_event exprs'))
    | PartialDerivative (expr', expr'') ->
        create_partialDerivative
          (remove_no_event expr')
          (remove_no_event expr'')
    | If (expr', expr'', expr''') ->
        create_if
          (remove_no_event expr')
          (remove_no_event expr'')
          (remove_no_event expr''')
    | Equality (expr', expr'') ->
        create_equality
          (remove_no_event expr')
          (remove_no_event expr'')
    | Greater (expr', expr'') ->
        create_greater
          (remove_no_event expr')
          (remove_no_event expr'')
    | GreaterEqual (expr', expr'') ->
        create_greater_equal
          (remove_no_event expr')
          (remove_no_event expr'')
    | And exprs -> create_and (List.map remove_no_event exprs)
    | Or exprs ->  create_or (List.map remove_no_event exprs)
    | Not expr' -> create_not (remove_no_event expr')
    | BooleanValue _ | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> expr
  and remove_no_event_in_argument = function
    | ScalarArgument expr -> ScalarArgument (remove_no_event expr)
    | ArrayArgument (dims, exprs) ->
        ArrayArgument (dims, Array.map remove_no_event exprs) in
  match nature expr with
    | ArcCosine expr' -> create_arcCosine (rewrite_conditions_in no_event expr')
    | ArcHyperbolicCosine expr' ->
        create_arcHyperbolicCosine (rewrite_conditions_in no_event expr')
    | ArcHyperbolicSine expr' ->
        create_arcHyperbolicSine (rewrite_conditions_in no_event expr')
    | ArcHyperbolicTangent expr' ->
        create_arcHyperbolicTangent (rewrite_conditions_in no_event expr')
    | ArcSine expr' -> create_arcSine (rewrite_conditions_in no_event expr')
    | ArcTangent expr' -> create_arcTangent (rewrite_conditions_in no_event expr')
    | Cosine expr' -> create_cosine (rewrite_conditions_in no_event expr')
    | Derivative (expr', num) ->
        create_derivative (rewrite_conditions_in no_event expr') num
    | Exponential expr' -> create_exponential (rewrite_conditions_in no_event expr')
    | Floor expr' -> create_floor (rewrite_conditions_in no_event expr')
    | HyperbolicCosine expr' ->
        create_hyperbolicCosine (rewrite_conditions_in no_event expr')
    | HyperbolicSine expr' ->
        create_hyperbolicSine (rewrite_conditions_in no_event expr')
    | HyperbolicTangent expr' ->
        create_hyperbolicTangent (rewrite_conditions_in no_event expr')
    | Logarithm expr' -> create_logarithm (rewrite_conditions_in no_event expr')
    | Pre expr' -> create_pre (rewrite_conditions_in no_event expr')
    | RationalPower (expr', num) ->
        create_rationalPower (rewrite_conditions_in no_event expr') num
    | Sign expr' -> create_sign (rewrite_conditions_in no_event expr')
    | Sine expr' -> create_sine (rewrite_conditions_in no_event expr')
    | Tangent expr' -> create_tangent (rewrite_conditions_in no_event expr')
    | Addition exprs' ->
        create_addition (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | BlackBox ("noEvent", [ScalarArgument expr']) ->
        rewrite_conditions_in true expr'
    | BlackBox (name, args) ->
        create_blackBox
          name
          (List.map (rewrite_conditions_in_argument no_event) args)
    | Multiplication exprs' ->
        create_multiplication (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | PartialDerivative (expr', expr'') ->
        create_partialDerivative
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | If (expr', expr'', expr''') ->
        create_if
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
          (rewrite_conditions_in no_event expr''')
    | Equality (expr', expr'') when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument
            (create_equality
              (remove_no_event expr')
              (remove_no_event expr''))]
    | Equality (expr', expr'') ->
        create_equality
          (remove_no_event expr')
          (remove_no_event expr'')
    | Greater (expr', expr'') when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument
            (create_greater
              (remove_no_event expr')
              (remove_no_event expr''))]
    | Greater (expr', expr'') ->
        create_greater
          (remove_no_event expr')
          (remove_no_event expr'')
    | GreaterEqual (expr', expr'') when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument
            (create_greater_equal
              (remove_no_event expr')
              (remove_no_event expr''))]
    | GreaterEqual (expr', expr'') ->
        create_greater_equal
          (remove_no_event expr')
          (remove_no_event expr'')
    | And exprs when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument (create_and (List.map remove_no_event exprs))]
    | And exprs -> create_and (List.map remove_no_event exprs)
    | Or exprs when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument (create_or (List.map remove_no_event exprs))]
    | Or exprs -> create_or (List.map remove_no_event exprs)
    | Not expr' when no_event ->
        create_blackBox
          "noEvent"
          [ScalarArgument (create_not (remove_no_event expr'))]
    | Not expr' -> create_not (remove_no_event expr')
    | BooleanValue _ | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> expr
  and rewrite_conditions_in_argument no_event = function
    | ScalarArgument expr ->
        ScalarArgument (rewrite_conditions_in no_event expr)
    | ArrayArgument (dims, exprs) ->
        ArrayArgument
          (dims, Array.map (rewrite_conditions_in no_event) exprs)

let perform_simplifications max_simplifs model =
  Array.iter
    (fun equation ->
      equation.expression <- rewrite_conditions_in false equation.expression)
    model.equations;
  eliminate_explicit_variables max_simplifs model;
  compute_io_dependency model

let find_submodels model =
  let final_index_of_variables = final_index_of_variables model in
  let size =
    Array.fold_left
      (fun acc i -> if i >= 0 then acc + 1 else acc)
      0
      final_index_of_variables
  in
  let graph = CausalityGraph.create size in
  Array.iteri
    (fun i equation ->
      if not equation.solved then
        List.iter
          (fun expr ->
            match nature expr with
              | Variable j ->
                  if not model.equations.(j).solved then
                    let i' = final_index_of_variables.(i)
                    and j' = final_index_of_variables.(j) in
                    CausalityGraph.connect i' j' graph
              | _ -> assert false)
          equation.inner_variables)
    model.equations;
  CausalityGraph.strongly_connected_components graph
