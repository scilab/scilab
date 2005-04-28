
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
    parameter_id: int;
    parameter_comment: string;
    parameter_start: t Lazy.t
  }

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
    variable_start: t Lazy.t
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
    mutable external_functions: string list list
  }

and parameter_description =
  {
    mutable main: bool;
    mutable p_name: string;
    mutable p_comment: string;
    mutable value: t
  }

and discrete_variable_description =
  {
    mutable d_output: int option;
    mutable d_v_name: string;
    mutable d_v_comment: string;
    mutable d_start_value: t
  }

and variable_description =
  {
    mutable output: int option;
    mutable state: bool;
    mutable v_name: string;
    mutable v_comment: string;
    mutable start_value: t
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
  Lazy.force variable.variable_start

let rec symbolic_expression_of_expression maps iexpr =
  let rec symbolic_expression_of_expression' iexpr =
    match iexpr.Instantiation.tex_expression with
      | Instantiation.Abs iexpr ->
          let expr = symbolic_expression_of_expression' iexpr in
          symbolic_if
            (symbolic_gt expr zero)
            expr
            (symbolic_minus expr)
      | Instantiation.Addition (iexpr, iexpr') ->
          symbolic_add
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.And (iexpr, iexpr') ->
          symbolic_and
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Boolean false -> false_value
      | Instantiation.Boolean true -> true_value
      | Instantiation.Cardinality _ ->
          invalid_arg "symbolic_expression_of_expression'"
      | Instantiation.CompoundElement _ ->
          invalid_arg "symbolic_expression_of_expression'"
      | Instantiation.Cos iexpr ->
          symbolic_cos (symbolic_expression_of_expression' iexpr)
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
      | Instantiation.ExternalFunctionCall (path, iexprs) ->
          symbolic_blackBox
            (function_name_of path)
            (List.map symbolic_expression_of_expression' iexprs)
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
      | Instantiation.Integer i ->
          let i1 = Int32.to_int (Int32.shift_right i 16)
          and i2 = Int32.to_int (Int32.logand i (Int32.of_int 0xffff)) in
          let num =
            Num.add_num
              (Num.mult_num (Num.num_of_int i1) (Num.num_of_int 65536))
              (Num.num_of_int i2)
          in create_number num
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
          create_blackBox "noEvent" [symbolic_expression_of_expression' iexpr]
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
      | Instantiation.ParameterValue (_, iref) ->
          let id = string_of_reference iref in
          if is_main_parameter id (Lazy.force maps.parameters_map) then
            create_parameter
              (StringMap.find id (Lazy.force maps.parameters_map)).parameter_id
          else
            get_parameter_start id (Lazy.force maps.parameters_map)
      | Instantiation.Power (iexpr, iexpr') ->
          symbolic_power
            (symbolic_expression_of_expression' iexpr)
            (symbolic_expression_of_expression' iexpr')
      | Instantiation.Real f -> create_number (num_of_float f)
      | Instantiation.Sin iexpr ->
          symbolic_sin (symbolic_expression_of_expression' iexpr)
      | Instantiation.Sqrt iexpr ->
          symbolic_sqrt (symbolic_expression_of_expression' iexpr)
      | Instantiation.String s -> create_constant s
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
  let rec add_if_not_in name = function
    | [] -> [name]
    | (name' :: _) as names when name = name' -> names
    | name' :: names -> name' :: add_if_not_in name names
  in
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
      | Instantiation.ExternalFunctionCall (name, iexprs) ->
          let funcalls = add_if_not_in name funcalls in
          List.fold_left collect_in_expressions funcalls iexprs
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
        Instantiation.Cos iexpr | Instantiation.Exp iexpr |
        Instantiation.Floor iexpr | Instantiation.Log iexpr |
        Instantiation.Sin iexpr | Instantiation.Sqrt iexpr |
        Instantiation.Tan iexpr | Instantiation.Tanh iexpr ->
          collect_in_expressions funcalls iexpr
      | Instantiation.ParameterValue _ | Instantiation.Real _ |
        Instantiation.String _ | Instantiation.Time |
        Instantiation.VariableStart _ | Instantiation.VariableValue _ |
        Instantiation.Boolean _ | Instantiation.Der _ | Instantiation.Integer _ ->
          funcalls
      | Instantiation.Cardinality _ | Instantiation.CompoundElement _ |
        Instantiation.Vector _ -> assert false
  in collect_in_equations [] iequs

let separate_parameters_from_variables icpnts =
  let is_parameter = function
    | _, Instantiation.InstantiatedParameter _ -> true
    | _, Instantiation.InstantiatedVariable _ -> false
  in List.partition is_parameter icpnts

let separate_inputs_from_others icpnts =
  let is_input = function
    | Instantiation.InstantiatedDiscreteVariable (_, Compilation.Input, _) |
      Instantiation.InstantiatedRealVariable (_, Compilation.Input, _, _) ->
        true
    | Instantiation.InstantiatedDiscreteVariable _ |
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
    | Instantiation.InstantiatedRealVariable _ |
      Instantiation.InstantiatedCompoundVariable _ -> false
  in
  let filter_variable = function
    | _, Instantiation.InstantiatedParameter _ -> false
    | _, Instantiation.InstantiatedVariable ivar -> is_discrete ivar
  in List.partition filter_variable icpnts

let separate_outputs_from_others icpnts =
  let is_output = function
    | Instantiation.InstantiatedDiscreteVariable (_, Compilation.Output, _) |
      Instantiation.InstantiatedRealVariable (_, Compilation.Output, _, _) ->
        true
    | Instantiation.InstantiatedDiscreteVariable _ |
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

let symbolic_equation maps = function
  | Instantiation.Equation (iexpr, iexpr') ->
      let expr =
        symbolic_sub
          (symbolic_expression_of_expression maps iexpr)
          (symbolic_expression_of_expression maps iexpr')
      in
      {
        solved = false;
        inner_variables = variables_of expr;
        inner_derivatives = derivatives_of expr;
        assignable_variables = assignable_variables_of expr;
        expression = expr
      }
  | _ -> assert false

let symbolic_surfaces maps when_clauses =
  List.map
    (fun (iexpr, surfaces) ->
      symbolic_expression_of_expression maps iexpr,
      List.map
        (function
          | Instantiation.Reinit (iexpr, iexpr') ->
            let var = symbolic_expression_of_expression maps iexpr in
            begin match nature var with
              | Variable i ->
                  Reinit (var, symbolic_expression_of_expression maps iexpr')
              | _ -> assert false
            end
          | Instantiation.Assign (iexpr, iexpr') ->
            let var = symbolic_expression_of_expression maps iexpr in
            begin match nature var with
              | DiscreteVariable i ->
                  Assign (var, symbolic_expression_of_expression maps iexpr')
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
    | HyperbolicCosine expr' ->
        create_hyperbolicCosine (propagate_noEvent' no_event expr')
    | HyperbolicSine expr' ->
        create_hyperbolicSine (propagate_noEvent' no_event expr')
    | HyperbolicTangent expr' ->
        create_hyperbolicTangent (propagate_noEvent' no_event expr')
    | Logarithm expr' -> create_logarithm (propagate_noEvent' no_event expr')
    | RationalPower (expr', num) ->
        create_rationalPower (propagate_noEvent' no_event expr') num
    | Sign expr' -> create_sign (propagate_noEvent' no_event expr')
    | Sine expr' -> create_sine (propagate_noEvent' no_event expr')
    | Tangent expr' -> create_tangent (propagate_noEvent' no_event expr')
    | Addition exprs' ->
        create_addition (sort (List.map (propagate_noEvent' no_event) exprs'))
    | BlackBox ("noEvent", [expr']) -> propagate_noEvent' true expr'
    | BlackBox (name, exprs') ->
        create_blackBox name (List.map (propagate_noEvent' no_event) exprs')
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
      Parameter _ | TimeVariable | Variable _ -> expr
  and propagate_noEvent_into_if no_event expr expr' expr'' =
    let cond =
      if no_event then create_blackBox "noEvent" [propagate_noEvent' false expr]
      else begin match nature expr with
        | BlackBox ("noEvent", [expr']) ->
            create_blackBox "noEvent" [propagate_noEvent' false expr]
        | _ -> propagate_noEvent' false expr
      end
    in
    create_if
      cond
      (propagate_noEvent' no_event expr')
      (propagate_noEvent' no_event expr'')
  in propagate_noEvent' false expr

let create_model iexpr =
  let get_parameter_info maps s i = function
    | Instantiation.InstantiatedParameter (
        Instantiation.InstantiatedIntegerParameter (s', kind, iexpr)) |
      Instantiation.InstantiatedParameter (
        Instantiation.InstantiatedRealParameter (s', kind, iexpr)) ->
        {
          parameter_kind = kind;
          parameter_id = i;
          parameter_comment = s';
          parameter_start = lazy (symbolic_expression_of_expression maps iexpr)
        }
    | _ -> assert false
  and get_input_info maps s i = function
    | Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedDiscreteVariable (s', _, _)) |
      Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedRealVariable (s', _, _, _)) ->
        {
          input_id = i;
          input_name = s;
          input_comment = s'
        }
    | _ -> assert false
  and get_variable_info maps s i = function
    | Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedDiscreteVariable (s', _, iexpr)) |
      Instantiation.InstantiatedVariable (
        Instantiation.InstantiatedRealVariable (s', _, _, iexpr)) ->
        {
          variable_id = i;
          variable_comment = s';
          variable_start = lazy (symbolic_expression_of_expression maps iexpr)
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
  let icpnts, iinit_equs, iequs = Instantiation.expand_class iexpr in
  let parameters, variables = separate_parameters_from_variables icpnts in
  let inputs, non_inputs = separate_inputs_from_others variables in
  let discrete_variables, others =
    separate_discrete_variables_from_others non_inputs
  in
  let outputs, _ = separate_outputs_from_others non_inputs in
  let function_names =
    collect_external_function_names iequs @
    collect_external_function_names iinit_equs
  in
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
            d_start_value = zero
          })
    and variables_array =
      Array.init
        nb_vars
        (fun _ ->
          {
            output = None;
            state = true;
            v_name = "";
            v_comment = "";
            start_value = zero
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
    let _ =
      List.fold_left
        (fun i equ ->
          assert (i < Array.length equations_array);
          equations_array.(i) <- symbolic_equation maps equ; i + 1)
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
        variable.d_start_value <- Lazy.force dvar.variable_start)
      (Lazy.force maps.discrete_variables_map);
    StringMap.iter
      (fun s var ->
        assert (var.variable_id < Array.length variables_array);
        let variable = variables_array.(var.variable_id) in
        variable.output <- output_index s outputs;
        variable.state <-
          List.memq (create_variable var.variable_id) derived_variables;
        variable.v_name <- s;
        variable.v_comment <- var.variable_comment;
        variable.start_value <- Lazy.force var.variable_start)
      (Lazy.force maps.variables_map);
    let when_clauses_list = symbolic_surfaces maps when_clauses in
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
      external_functions = function_names
    }

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
    | None -> ()
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
    if not (List.memq var model.equations.(n).assignable_variables) then
      IntegerElement.Infinity
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
    and stj = model.variables.(j).state in
    match sti, stj with
      | true, false -> i
      | false, true -> j
      | _ ->
          let svi = model.variables.(i).start_value
          and svj = model.variables.(j).start_value in
          begin match nature svi, nature svj with
            | _, Number (Num.Int 0) -> i
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
    let sti = model.variables.(i).state
    and stj = model.variables.(j).state in
    let state = sti || stj in
    model.variables.(i).state <- state;
    model.variables.(j).state <- state;
    match sti, stj with
      | true, false -> model.variables.(j).start_value <- svi
      | false, true -> model.variables.(i).start_value <- svj
      | _ ->
          begin match nature svi, nature svj with
            | _, Number (Num.Int 0) -> model.variables.(j).start_value <- svi
            | _ -> model.variables.(i).start_value <- svj
          end
  in
  let simplify_trivial_relation n =
    match nature model.equations.(n).expression with
      | Addition [node; node'] when !max_simplifs_ref >= 0 ->
          begin match nature node, nature node' with
            | Variable i, Number _ | Number _, Variable i ->
                permute_equations i n;
                perform_then_propagate_inversion model i;
                decr max_simplifs_ref
            | Variable i, Variable j ->
                let k = choose_variable i j in
                update_variable_attributes i j;
                permute_equations k n;
                perform_then_propagate_inversion model k;
                decr max_simplifs_ref
            | Variable i, Multiplication [node; node'] |
              Multiplication [node; node'], Variable i ->
                begin match nature node, nature node' with
                  | Number _, Variable j | Variable j, Number _ ->
                      let k = choose_variable i j in
                      update_variable_attributes i j;
                      permute_equations k n;
                      perform_then_propagate_inversion model k;
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
  let rec eliminate_explicit_variables' simplifs =
    let assocs = perform_hungarian_method model in
    permute_equations model assocs;
    let bad_variable_choice, success, simplifs =
      List.fold_left
        (fun (bad_variable_choice, success, simplifs) assoc ->
          match assoc with
            | (_, None) -> assert false
            | i, Some j when simplifs >= 0 ->
                begin try
                  if not model.variables.(i).state then
                    perform_then_propagate_inversion model i;
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

let rec is_greater_equal expr = match nature expr with
  | BlackBox ("noEvent", [expr']) -> is_greater_equal expr'
  | Or [expr1; expr2] ->
      begin match nature expr1, nature expr2 with
        | Equality (expr11, expr12), Greater (expr21, expr22)
          when expr11 == expr21 && expr12 == expr22 ||
          expr11 == expr22 && expr12 == expr21 -> true
        | Greater (expr11, expr12), Equality (expr21, expr22)
          when expr11 == expr21 && expr12 == expr22 ||
          expr11 == expr22 && expr12 == expr21 -> true
        | _ -> false
      end
  | _ -> false

let rec rewrite_conditions_in no_event expr =
  let rec rewrite_if no_event expr expr' expr'' =
    let no_event_if_necessary expr =
      if no_event then create_blackBox "noEvent" [expr] else expr
    in match nature expr with
    | BlackBox ("noEvent", [expr1]) when nature expr1 = BooleanValue true ->
        rewrite_conditions_in no_event expr'
    | BlackBox ("noEvent", [expr1]) when nature expr1 = BooleanValue false ->
        rewrite_conditions_in no_event expr''
    | BlackBox ("noEvent", [expr1]) ->
        create_if
          (create_blackBox "noEvent" [rewrite_conditions_in true expr1])
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | Equality (expr1, expr2) ->
        create_if
          (no_event_if_necessary
            (create_equality
              (rewrite_conditions_in no_event expr1)
              (rewrite_conditions_in no_event expr2)))
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | Greater (expr1, expr2) ->
        create_if
          (no_event_if_necessary
            (create_greater
              (rewrite_conditions_in no_event expr1)
              (rewrite_conditions_in no_event expr2)))
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | And [] -> rewrite_conditions_in no_event expr'
    | And [expr] ->
        create_if
          (no_event_if_necessary expr)
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | And (expr :: exprs) ->
        rewrite_if no_event expr (create_if (create_and exprs) expr' expr'') expr''
    | Or [] -> rewrite_conditions_in no_event expr''
    | Or [expr] ->
        create_if
          (no_event_if_necessary expr)
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | Or [expr1; expr2] when is_greater_equal expr ->
        begin match nature expr1, nature expr2 with
          | Greater (expr1, expr2), _ | _, Greater (expr1, expr2) ->
              let expr1' = rewrite_conditions_in no_event expr1
              and expr2' = rewrite_conditions_in no_event expr2 in
              create_if
                (no_event_if_necessary
                  (create_or
                    [create_greater expr1' expr2'; create_equality expr1' expr2']))
                (rewrite_conditions_in no_event expr')
                (rewrite_conditions_in no_event expr'')
          | _ -> assert false
        end
    | Or (expr :: exprs) ->
        rewrite_if no_event expr expr' (create_if (create_or exprs) expr' expr'')
    | Not expr ->
        create_if
          (no_event_if_necessary expr)
          (rewrite_conditions_in no_event expr'')
          (rewrite_conditions_in no_event expr')
    | _ -> assert false
  in match nature expr with
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
    | Not expr' -> create_not (rewrite_conditions_in no_event expr')    
    | RationalPower (expr', num) ->
        create_rationalPower (rewrite_conditions_in no_event expr') num
    | Sign expr' -> create_sign (rewrite_conditions_in no_event expr')
    | Sine expr' -> create_sine (rewrite_conditions_in no_event expr')
    | Tangent expr' -> create_tangent (rewrite_conditions_in no_event expr')
    | Equality (expr1, expr2) ->
        create_equality
          (rewrite_conditions_in no_event expr1)
          (rewrite_conditions_in no_event expr2)
    | Greater (expr1, expr2) ->
        create_greater
          (rewrite_conditions_in no_event expr1)
          (rewrite_conditions_in no_event expr2)
    | Addition exprs' ->
        create_addition (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | And exprs' ->
        create_and (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | BlackBox ("noEvent", [expr']) -> rewrite_conditions_in true expr'
    | BlackBox (name, exprs') ->
        create_blackBox name (List.map (rewrite_conditions_in no_event) exprs')
    | Multiplication exprs' ->
        create_multiplication (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | Or exprs' ->
        create_or (sort (List.map (rewrite_conditions_in no_event) exprs'))
    | PartialDerivative (expr', expr'') ->
        create_partialDerivative
          (rewrite_conditions_in no_event expr')
          (rewrite_conditions_in no_event expr'')
    | If (expr', expr'', expr''') -> rewrite_if no_event expr' expr'' expr'''
    | Constant _ | DiscreteVariable _ | Number _ | Parameter _ | TimeVariable |
      Variable _ -> expr
    | _ -> assert false

let perform_simplifications max_simplifs model =
  Array.iter
    (fun equation ->
      equation.expression <- rewrite_conditions_in false equation.expression)
    model.equations;
  eliminate_explicit_variables max_simplifs model;
  compute_io_dependency model

let compute_structural_index model =
  failwith "compute_structural_index: not yet implemented"

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
