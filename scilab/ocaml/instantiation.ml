

(* Datatypes *)

type instantiated_class =
  {
    icl_components: (string * instantiated_component Lazy.t) list Lazy.t;
    icl_init_equs: equation list Lazy.t;
    icl_equs: equation list Lazy.t
  }

and instantiated_component =
  | InstantiatedParameter of instantiated_parameter
  | InstantiatedVariable of instantiated_variable

and instantiated_parameter =
  | InstantiatedIntegerParameter of string * parameter_kind * typed_expression
  | InstantiatedRealParameter of string * parameter_kind * typed_expression

and parameter_kind =
  | Main
  | Sub

and instantiated_variable =
  | InstantiatedDiscreteVariable of string * Compilation.inout *
    typed_expression
  | InstantiatedRealVariable of string * Compilation.inout *
    Compilation.nature * typed_expression
  | InstantiatedCompoundVariable of string * typed_expression

and equation =
  | Equation of typed_expression * typed_expression
  | ConditionalEquation of (typed_expression * equation list) list *
    equation list
  | FlowConnection of typed_expression * typed_expression
  | When of when_clause_type

and when_clause_type =
  (typed_expression * typed_when_expression list) list

and typed_when_expression =
  | Reinit of typed_expression * typed_expression
  | Assign of typed_expression * typed_expression

and typed_expression =
  {
    tex_type: expression_type;
    tex_expression: expression
  }

and expression_type =
  | BooleanType of int array
  | CartesianProduct of expression_type list
  | CompoundType of int array
  | IntegerType of int array
  | RealType of int array
  | StringType of int array

and expression =
  | Abs of typed_expression
  | Addition of typed_expression * typed_expression
  | And of typed_expression * typed_expression
  | Boolean of bool
  | Cardinality of typed_expression
  | CompoundElement of instantiated_class
  | Cos of typed_expression
  | Der of typed_expression
  | Division of typed_expression * typed_expression
  | Equals of typed_expression * typed_expression
  | Exp of typed_expression
  | ExternalFunctionCall of string list * typed_expression list
  | Floor of typed_expression
  | GreaterEqualThan of typed_expression * typed_expression
  | GreaterThan of typed_expression * typed_expression
  | If of (typed_expression * typed_expression) list * typed_expression
  | Integer of int32
  | Log of typed_expression
  | Max of typed_expression * typed_expression
  | Min of typed_expression * typed_expression
  | Mod of typed_expression * typed_expression
  | Minus of typed_expression
  | Multiplication of typed_expression * typed_expression
  | NoEvent of typed_expression
  | Not of typed_expression
  | NotEquals of typed_expression * typed_expression
  | Or of typed_expression * typed_expression
  | ParameterValue of int * reference
  | Power of typed_expression * typed_expression
  | Real of float
  | Sin of typed_expression
  | Sqrt of typed_expression
  | String of string
  | Subtraction of typed_expression * typed_expression
  | Tan of typed_expression
  | Tanh of typed_expression
  | Time
  | VariableStart of int * reference
  | VariableValue of int * reference
  | Vector of typed_expression array

and reference = (string * int array) list

and instantiation_context =
  | ToplevelContext
  | ClassContext of instantiation_context * instantiated_class Lazy.t
  | ForContext of instantiation_context * int

and modification =
  | Modification of (string * int array) * modification list * typed_expression option


(* Misc *)

let string_of_expression  iexpr =
  let rec string_of_expression' iexpr = match iexpr.tex_expression with
    | Abs iexpr -> "(abs " ^ string_of_expression' iexpr ^ ")"
    | Addition (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " + " ^
        string_of_expression' iexpr' ^
        ")"
    | And (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " and " ^
        string_of_expression' iexpr' ^
        ")"
    | Boolean false -> "false"
    | Boolean true -> "true"
    | Cos iexpr -> "(cos " ^ string_of_expression' iexpr ^ ")"
    | Der _ -> "(der <var>)"
    | Division (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " / " ^
        string_of_expression' iexpr' ^
        ")"
    | Equals (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " == " ^
        string_of_expression' iexpr' ^
        ")"
    | Exp iexpr -> "(exp " ^ string_of_expression' iexpr ^ ")"
    | ExternalFunctionCall (name, iexprs) -> "<funcall>"
    | GreaterEqualThan (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " >= " ^
        string_of_expression' iexpr' ^
        ")"
    | GreaterThan (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " > " ^
        string_of_expression' iexpr' ^
        ")"
    | If (iif_exprs, iexpr) -> string_of_if iif_exprs iexpr
    | Integer i -> Int32.to_string i
    | Log iexpr -> "(log " ^ string_of_expression' iexpr ^ ")"
    | Max (iexpr, iexpr') ->
        "(max " ^
        string_of_expression' iexpr ^
        ", " ^
        string_of_expression' iexpr' ^
        ")"
    | Min (iexpr, iexpr') ->
        "(min " ^
        string_of_expression' iexpr ^
        ", " ^
        string_of_expression' iexpr' ^
        ")"
    | Minus iexpr -> "(- " ^ string_of_expression' iexpr ^ ")"
    | Multiplication (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " * " ^
        string_of_expression' iexpr' ^
        ")"
    | NoEvent iexpr -> "(noEvent " ^ string_of_expression' iexpr ^ ")"
    | Not iexpr -> "(not " ^ string_of_expression' iexpr ^ ")"
    | NotEquals (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " <> " ^
        string_of_expression' iexpr' ^
        ")"
    | Or (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " or " ^
        string_of_expression' iexpr' ^
        ")"
    | ParameterValue (_, iref) -> string_of_reference iref
    | Power (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " ^ " ^
        string_of_expression' iexpr' ^
        ")"
    | Real f -> string_of_float f
    | Sin iexpr -> "(sin " ^ string_of_expression' iexpr ^ ")"
    | Sqrt iexpr -> "(sqrt " ^ string_of_expression' iexpr ^ ")"
    | String s -> s
    | Subtraction (iexpr, iexpr') ->
        "(" ^
        string_of_expression' iexpr ^
        " - " ^
        string_of_expression' iexpr' ^
        ")"
    | Tan iexpr -> "(tan " ^ string_of_expression' iexpr ^ ")"
    | Tanh iexpr -> "(tanh " ^ string_of_expression' iexpr ^ ")"
    | Time -> "time"
    | VariableStart (_, iref) -> string_of_reference iref
    | VariableValue (_, iref) -> string_of_reference iref
    | Vector iexprs ->
        Array.fold_left
          (fun acc iexpr -> acc ^ string_of_expression' iexpr ^ ", ")
          "{"
          iexprs ^ "}"
    | _ -> failwith "string_of_expression: bad expression"
  and string_of_if iif_exprs iexpr = match iif_exprs with
    | [] -> string_of_expression' iexpr
    | (iexpr', iexpr'') :: iif_exprs' ->
        "(if " ^
        string_of_expression' iexpr' ^
        " then " ^
        string_of_expression' iexpr'' ^
        " else " ^
        string_of_if iif_exprs' iexpr ^
        ")"
  and string_of_reference = function
    | [] -> ""
    | (id, _) :: iref -> id ^ "[]" ^ string_of_reference iref
  in string_of_expression' iexpr

let separate_parameters_from_variables cpnts =
  let rec partition pars vars = function
    | [] -> List.rev pars, List.rev vars
    | Compilation.Parameter par :: cpnts ->
        partition (par :: pars) vars cpnts
    | Compilation.Variable var :: cpnts ->
        partition pars (var :: vars) cpnts
  in partition [] [] cpnts

let separate_inputs_from_others vars =
  let rec partition inputs others = function
    | [] -> List.rev inputs, List.rev others
    | ((Compilation.DiscreteVariable attrs | Compilation.RealVariable attrs)
      as var) :: vars when attrs.Compilation.vat_inout = Compilation.Input ->
        partition (var :: inputs) others vars
    | var :: vars -> partition inputs (var :: others) vars
  in partition [] [] vars

let separate_outputs_from_others vars =
  let rec partition outputs others = function
    | [] -> List.rev outputs, List.rev others
    | ((Compilation.DiscreteVariable attrs | Compilation.RealVariable attrs)
      as var) :: vars when attrs.Compilation.vat_inout = Compilation.Output ->
        partition (var :: outputs) others vars
    | var :: vars -> partition outputs (var :: others) vars
  in partition [] [] vars


(* Instantiation *)

let rec instantiate_main_class ctx modifs ccl =
  let rec ctx' = ClassContext (ctx, licl)
  and licl = lazy(
    {
      icl_components = lazy(
        instantiate_main_components
          ctx'
          modifs
          ccl.Compilation.ccl_public_cpnts);
      icl_init_equs = lazy(
        instantiate_equations ctx' ccl.Compilation.ccl_initial_equs);
      icl_equs = lazy(
        instantiate_equations ctx' ccl.Compilation.ccl_equs)
    })
  in
    {
      tex_type = CompoundType [||];
      tex_expression = CompoundElement (Lazy.force licl)
    }

and instantiate_main_components ctx modifs lccpnts =
  let rec instantiate_main_component modifs' ccpnt = match ccpnt with
      | Compilation.Parameter param ->
          InstantiatedParameter (instantiate_main_parameter ctx modifs' param)
      | Compilation.Variable var ->
          InstantiatedVariable (instantiate_variable ctx modifs' var)
  in
  List.map
    (fun (s, lccpnt) ->
      let modifs' = List.fold_left (filter_modification_of s) [] modifs in
      s, lazy (instantiate_main_component modifs' (Lazy.force lccpnt)))
    (Lazy.force lccpnts)

and instantiate_main_parameter ctx modifs = function
  | Compilation.IntegerParameter attrs ->
      let default =
        {
          tex_type = IntegerType [||];
          tex_expression = Integer Int32.zero
        }
      and make comment ivalue =
        InstantiatedIntegerParameter (comment, Main, ivalue)
      in initialize_parameter ctx default modifs attrs make
  | Compilation.RealParameter attrs ->
      let default =
        {
          tex_type = RealType [||];
          tex_expression = Real 0.0
        }
      and make comment ivalue =
        InstantiatedRealParameter (comment, Main, ivalue)
      in initialize_parameter ctx default modifs attrs make

and instantiate_class ctx modifs ccl =
  let rec ctx' = ClassContext (ctx, licl)
  and licl = lazy(
    {
      icl_components = lazy(
        instantiate_components ctx' modifs ccl.Compilation.ccl_public_cpnts);
      icl_init_equs = lazy(
        instantiate_equations ctx' ccl.Compilation.ccl_initial_equs);
      icl_equs = lazy(
        instantiate_equations ctx' ccl.Compilation.ccl_equs)
    })
  in
    {
      tex_type = CompoundType [||];
      tex_expression = CompoundElement (Lazy.force licl)
    }

and instantiate_components ctx modifs lccpnts =
  let rec instantiate_component modifs' ccpnt = match ccpnt with
      | Compilation.Parameter (param) ->
          InstantiatedParameter (instantiate_parameter ctx modifs' param)
      | Compilation.Variable (var) ->
          InstantiatedVariable (instantiate_variable ctx modifs' var)
  in
  List.map
    (fun (s, lccpnt) ->
      let modifs' = List.fold_left (filter_modification_of s) [] modifs in
      s, lazy (instantiate_component modifs' (Lazy.force lccpnt)))
    (Lazy.force lccpnts)

and filter_modification_of s modifs = function
  | Compilation.CompiledModification ((s', [||]), modifs', None) when s = s' ->
      modifs' @ modifs
  | Compilation.CompiledModification ((s', [||]), modifs', Some cexpr)
    when s = s' ->
      Compilation.CompiledModification (("start", [||]), [], Some cexpr) ::
      (modifs' @ modifs)
  | _ -> modifs

and instantiate_parameter ctx modifs = function
  | Compilation.IntegerParameter attrs ->
      let default =
        {
          tex_type = IntegerType [||];
          tex_expression = Integer Int32.zero
        }
      and make comment ivalue =
        InstantiatedIntegerParameter (comment, Sub, ivalue)
      in initialize_parameter ctx default modifs attrs make
  | Compilation.RealParameter attrs ->
      let default =
        {
          tex_type = RealType [||];
          tex_expression = Real 0.0
        }
      and make comment ivalue =
        InstantiatedRealParameter (comment, Sub, ivalue)
      in initialize_parameter ctx default modifs attrs make

and initialize_parameter ctx default modifs attrs make =
  let comment = attrs.Compilation.pat_comment
  and cdims = attrs.Compilation.pat_dimensions
  and value_opt = attrs.Compilation.pat_value in
  let dims = Array.map (compute_subscript ctx) cdims in
  let ivalue = calculate_initial_value ctx dims default modifs value_opt in
  make comment ivalue


and calculate_initial_value ctx dims default modifs value_opt =
  match modifs, value_opt with
    | [], None -> create_array dims default
    | [], Some cexpr ->
        let iexpr = instantiate_expression ctx cexpr in
        begin match iexpr.tex_type, default.tex_type with
          | (IntegerType dims', IntegerType _ |
            (RealType dims' | IntegerType dims'), RealType _)
            when dims' = dims -> iexpr
          | _ ->
              failwith
                ("calculate_initial_value: type error: " ^
                string_of_expression iexpr)
        end
    | [Compilation.CompiledModification (("start", [||]), [], Some cexpr)], _ ->
        let iexpr = instantiate_expression ctx cexpr in
        begin match iexpr.tex_type, default.tex_type with
          | (IntegerType dims', IntegerType _ |
            (RealType dims' | IntegerType dims'), RealType _)
            when dims' = dims -> iexpr
          | _ ->
              failwith
                ("calculate_initial_value: type error: " ^
                string_of_expression iexpr)
        end
    | _ -> failwith "calculate_initial_value: invalid modification"

and instantiate_variable ctx modifs = function
    | Compilation.DiscreteVariable attrs ->
      let default =
        {
          tex_type = RealType [||];
          tex_expression = Real 0.0
        }
      and make comment inout flow ivalue =
        InstantiatedDiscreteVariable (comment, inout, ivalue)
      in initialize_base_type_variable ctx default modifs attrs make
    | Compilation.RealVariable attrs ->
      let default =
        {
          tex_type = RealType [||];
          tex_expression = Real 0.0
        }
      and make comment inout flow ivalue =
        InstantiatedRealVariable (comment, inout, flow, ivalue)
      in initialize_base_type_variable ctx default modifs attrs make
    | Compilation.CompoundVariable (lccl, attrs) ->
        initialize_compound_variable ctx modifs lccl attrs

and initialize_compound_variable ctx modifs lccl attrs =
  let comment = attrs.Compilation.vat_comment
  and cdims = attrs.Compilation.vat_dimensions
  and modifs' = attrs.Compilation.vat_modifications in
  let dims = Array.map (compute_subscript ctx) cdims in
  let ivalue = init_array dims ctx modifs modifs' (Lazy.force lccl) in
  InstantiatedCompoundVariable (comment, ivalue)

and init_array dims ctx modifs modifs' ccl =
  let preprocess_modification = function
    | Compilation.CompiledModification ((_, cs), modifs, None) ->
        Array.to_list (Array.map (compute_subscript ctx) cs), modifs
    | _ -> failwith "init_array: invalid modification" in
  let rec filter_pmodifs i = function
    | (i' :: _, _) as pmodif :: pmodifs' when i = i' ->
        pmodif :: filter_pmodifs i pmodifs'
    | (_ :: _, _) :: pmodifs' -> filter_pmodifs i pmodifs'
    | _ -> failwith "init_array: type error"
  and create_array i pmodifs =
    if i = Array.length dims then
      begin match pmodifs with
        | [] -> instantiate_class ctx modifs' ccl
        | [([], modifs)] ->
            instantiate_class ctx (merge_modifications modifs modifs') ccl
        | _ -> failwith "init_array: invalid modification"
      end
    else
      let iexpr_array = Array.init dims.(i) (init_array' pmodifs i) in
        {
          tex_type = compute_array_type iexpr_array;
          tex_expression = Vector iexpr_array
        }
  and init_array' pmodifs n i =
    let pmodifs' = filter_pmodifs i pmodifs in
    create_array (n + 1) pmodifs'
  in create_array 0 (List.map preprocess_modification modifs)

and merge_modifications modifs modifs' =
  let rec add_modif_to modifs modif = match modifs, modif with
    | Compilation.CompiledModification (field, inners, cexpr_opt) as modif' ::
      modifs',
      Compilation.CompiledModification (field', inners', cexpr_opt')
      when field = field' ->
        let inners'' = merge_modifications inners inners'
        and cexpr_opt'' = merge_optional_values cexpr_opt cexpr_opt' in
        Compilation.CompiledModification (field, inners'', cexpr_opt'') ::
        modifs'
    | modif' :: modifs', _ -> modif' :: add_modif_to modifs' modif
    | [], _ -> [modif]
  and merge_optional_values cexpr_opt cexpr_opt' =
    match cexpr_opt, cexpr_opt' with
      | None, None -> None
      | Some cexpr, (Some _ | None) -> cexpr_opt
      | None, Some cexpr' -> cexpr_opt
  in List.fold_left add_modif_to modifs modifs'

and initialize_base_type_variable ctx default modifs attrs make =
  let comment = attrs.Compilation.vat_comment
  and inout = attrs.Compilation.vat_inout
  and flow = attrs.Compilation.vat_nature
  and cdims = attrs.Compilation.vat_dimensions
  and value_opt = match attrs.Compilation.vat_modifications with
    | [Compilation.CompiledModification (("value", [||]), [], cexpr_opt)] |
      [Compilation.CompiledModification (("start", [||]), [], cexpr_opt)] ->
        cexpr_opt
    | [] -> None
    | _ -> failwith "initialize_variable: invalid modification"
  in
  let dims = Array.map (compute_subscript ctx) cdims in
  let ivalue = calculate_initial_value ctx dims default modifs value_opt in
  make comment inout flow ivalue

and create_array dims default =
  let rec create_array' i value =
    if i < 0 then value
    else
      let iexpr_array = Array.make dims.(i) value in
      let value' =
        {
          tex_type = compute_array_type iexpr_array;
          tex_expression = Vector iexpr_array
        }
      in create_array' (i - 1) value'
  in create_array' (Array.length dims - 1) default

and instantiate_equations ctx lequs =
  List.fold_left (instantiate_equation ctx) [] (Lazy.force lequs)

and instantiate_equation ctx equs = function
  | Compilation.CompiledEquality (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType dims, BooleanType dims' |
          IntegerType dims, IntegerType dims' |
          (RealType dims | IntegerType dims), RealType dims' |
          RealType dims, IntegerType dims' |
          StringType dims, StringType dims'
          when dims = dims' -> Equation (iexpr, iexpr') :: equs
        | _ ->
          let s = string_of_expression iexpr
          and s' = string_of_expression iexpr' in
            failwith
              ("instanciate_equation: type error in equality between " ^
              s ^ " and " ^ s')
      end
  | Compilation.CompiledFlowConnection (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | RealType dims, RealType dims' when dims = dims' ->
            FlowConnection (iexpr, iexpr') :: equs
        | _ -> failwith "instanciate_equation: type error in connection"
      end
  | Compilation.CompiledIf (cif_clauses, cequs) ->
      let iif_clauses =
        List.map
          (fun (cexpr, cequs) ->
            let iexpr = instantiate_expression ctx cexpr
            and iequs = List.fold_left (instantiate_equation ctx) [] cequs in
            match iexpr.tex_type with
              | BooleanType [||] -> iexpr, iequs
              | _ -> failwith "instanciate_equation: type error in if")
          cif_clauses
      and iequs = List.fold_left (instantiate_equation ctx) [] cequs in
      ConditionalEquation (iif_clauses, iequs) :: equs
  | Compilation.CompiledFor (cexpr, cexpr', cexpr'', cequs) ->
      begin match
        evaluate_integer_expression ctx cexpr,
        evaluate_integer_expression ctx cexpr',
        evaluate_integer_expression ctx cexpr''
      with
        | Integer i, Integer i', Integer i'' ->
            let start = Int32.to_int i
            and stop = Int32.to_int i'
            and step = Int32.to_int i''
            in expand_for_equation ctx start stop step cequs @ equs
        | _ -> failwith "instanciate_equation: type error in for"
      end
  | Compilation.CompiledWhen cwhen_clauses ->
      let iwhen_clauses =
        List.map
          (fun (cexpr, cequs) ->
            let iexpr = instantiate_expression ctx cexpr
            and iequs = instantiate_when_equations ctx cequs in
            match iexpr.tex_type with
              | BooleanType [||] -> iexpr, iequs
              | _ -> failwith "instanciate_equation: type error in when")
          cwhen_clauses
      in When iwhen_clauses :: equs

and evaluate_integer_expression ctx = function
  | Compilation.Addition (cexpr, cexpr') ->
      begin match
        evaluate_integer_expression ctx cexpr,
        evaluate_integer_expression ctx cexpr'
      with
        | Integer i, Integer i' -> Integer (Int32.add i i')
        | _ -> failwith "evaluate_integer_expression: not an integer expression"
      end
  | Compilation.Division (cexpr, cexpr') ->
      begin match
        evaluate_integer_expression ctx cexpr,
        evaluate_integer_expression ctx cexpr'
      with
        | Integer i, Integer i' -> Integer (Int32.div i i')
        | _ -> failwith "evaluate_integer_expression: not an integer expression"
      end
  | Compilation.Integer i -> Integer i
  | Compilation.Minus cexpr ->
      begin match evaluate_integer_expression ctx cexpr with
        | Integer i -> Integer (Int32.sub Int32.zero i)
        | _ -> failwith "evaluate_integer_expression: not an integer expression"
      end
  | Compilation.Multiplication (cexpr, cexpr') ->
      begin match
        evaluate_integer_expression ctx cexpr,
        evaluate_integer_expression ctx cexpr'
      with
        | Integer i, Integer i' -> Integer (Int32.mul i i')
        | _ -> failwith "evaluate_integer_expression: not an integer expression"
      end
  | Compilation.Reference (Compilation.LoopVariableReference level) ->
      get_loop_variable_value ctx level
  | Compilation.Subtraction (cexpr, cexpr') ->
      begin match
        evaluate_integer_expression ctx cexpr,
        evaluate_integer_expression ctx cexpr'
      with
        | Integer i, Integer i' -> Integer (Int32.sub i i')
        | _ -> failwith "evaluate_integer_expression: not an integer expression"
      end
  | _ -> failwith "evaluate_integer_expression: unallowed expression"

and instantiate_when_equations ctx cequs =
  let instantiate_when_equation = function
    | Compilation.Reinit (Compilation.VariableReference (level, path), cexpr) ->
      let iexpr = get_component_reference ctx level path
      and iexpr' = instantiate_expression ctx cexpr in
      begin match iexpr, iexpr'.tex_type with
        | { tex_type = RealType dims; tex_expression = VariableValue _ },
          (RealType dims' | IntegerType dims') when dims = dims' ->
            Reinit (iexpr, iexpr')
        | _ -> failwith "instantiate_when_equations: type error in reinit"
      end
    | Compilation.Assign (Compilation.VariableReference (level, path), cexpr) ->
      let iexpr = get_component_reference ctx level path
      and iexpr' = instantiate_expression ctx cexpr in
      begin match iexpr, iexpr'.tex_type with
        | { tex_type = RealType dims; tex_expression = VariableValue _ },
          (RealType dims' | IntegerType dims') when dims = dims' ->
            Assign (iexpr, iexpr')
        | _ -> failwith "instantiate_when_equations: type error in assignment"
      end
    | _ -> failwith "instantiate_when_equations: type error"
  in List.rev_map instantiate_when_equation cequs

and instantiate_expression ctx = function
  | Compilation.Abs cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Abs iexpr
            }
        | _ -> failwith "instantiate_expression: type error on abs"
      end
  | Compilation.Addition (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType dims, IntegerType dims' when dims = dims' ->
            {
              tex_type = IntegerType dims;
              tex_expression = Addition (iexpr, iexpr')
            }
        | (IntegerType dims | RealType dims),
          (IntegerType dims' | RealType dims') when dims = dims' ->
            {
              tex_type = RealType dims;
              tex_expression = Addition (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on +"
      end
  | Compilation.And (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType dims, BooleanType dims' when dims = dims' ->
            {
              tex_type = BooleanType dims;
              tex_expression = And (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on and"
      end
  | Compilation.Boolean b ->
      {
        tex_type = BooleanType [||];
        tex_expression = Boolean b
      }
  | Compilation.Cardinality cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | CompoundType dims ->
            {
              tex_type = IntegerType dims;
              tex_expression = Cardinality iexpr
            }
        | _ -> failwith "instantiate_expression: type error on cardinality"
      end
  | Compilation.Cos cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Cos iexpr
            }
        | _ -> failwith "instantiate_expression: type error on cos"
      end
  | Compilation.Der cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Der iexpr
            }
        | _ -> failwith "instantiate_expression: type error on der"
      end
  | Compilation.Division (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType dims, IntegerType [||] ->
            {
              tex_type = IntegerType dims;
              tex_expression = Division (iexpr, iexpr')
            }
        | (IntegerType dims | RealType dims),
          (IntegerType [||] | RealType [||]) ->
            {
              tex_type = RealType dims;
              tex_expression = Division (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on division"
      end
  | Compilation.Equals (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType [||], BooleanType [||] |
          IntegerType [||], IntegerType [||] |
          StringType [||], StringType [||] ->
            {
              tex_type = BooleanType [||];
              tex_expression = Equals (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on equals"
      end
  | Compilation.ExternalFunctionCall (name, lccl, cexprs) ->
      let iexprs = List.map (instantiate_expression ctx) cexprs in
      let tex_type = check_function_type ctx lccl iexprs in
      {
        tex_type = tex_type;
        tex_expression = ExternalFunctionCall (name, iexprs)
      }
  | Compilation.Exp cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Exp iexpr
            }
        | _ -> failwith "instantiate_expression: type error on exp"
      end
  | Compilation.Floor cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | IntegerType dims ->
            {
              tex_type = IntegerType dims;
              tex_expression = Floor iexpr
            }
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Floor iexpr
            }
        | _ -> failwith "instantiate_expression: type error on floor"
      end
  | Compilation.GreaterEqualThan (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType [||], BooleanType [||] |
          IntegerType [||], IntegerType [||] |
          (IntegerType [||] | RealType [||]), RealType [||] |
          RealType [||], IntegerType [||] |
          StringType [||], StringType [||] ->
            {
              tex_type = BooleanType [||];
              tex_expression = GreaterEqualThan (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on >="
      end
  | Compilation.GreaterThan (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType [||], BooleanType [||] |
          IntegerType [||], IntegerType [||] |
          (IntegerType [||] | RealType [||]), RealType [||] |
          RealType [||], IntegerType [||] |
          StringType [||], StringType [||] ->
            {
              tex_type = BooleanType [||];
              tex_expression = GreaterThan (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on >"
      end
  | Compilation.If (alts, default) ->
      let idefault = instantiate_expression ctx default in
      let tex_type, ialts =
        instantiate_if_alternatives ctx idefault.tex_type alts
      in
        {
          tex_type = tex_type;
          tex_expression = If (ialts, idefault)
        }
  | Compilation.Integer i ->
      {
        tex_type = IntegerType [||];
        tex_expression = Integer i
      }
  | Compilation.Log cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Log iexpr
            }
        | _ -> failwith "instantiate_expression: type error on log"
      end
  | Compilation.Max (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType [||], IntegerType [||] ->
            {
              tex_type = IntegerType [||];
              tex_expression = Max (iexpr, iexpr')
            }
        | (IntegerType [||] | RealType [||]), RealType [||] |
          RealType [||], IntegerType [||] ->
            {
              tex_type = RealType [||];
              tex_expression = Max (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on max"
      end
  | Compilation.Min (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType [||], IntegerType [||] ->
            {
              tex_type = IntegerType [||];
              tex_expression = Min (iexpr, iexpr')
            }
        | (IntegerType [||] | RealType [||]), RealType [||] |
          RealType [||], IntegerType [||] ->
            {
              tex_type = RealType [||];
              tex_expression = Min (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on min"
      end
  | Compilation.Mod (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType [||], IntegerType [||] ->
            {
              tex_type = IntegerType [||];
              tex_expression = Mod (iexpr, iexpr')
            }
        | (IntegerType [||] | RealType [||]), RealType [||] |
          RealType [||], IntegerType [||] ->
            {
              tex_type = RealType [||];
              tex_expression = Mod (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on mod"
      end
  | Compilation.Minus cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | IntegerType dims ->
            {
              tex_type = IntegerType dims;
              tex_expression = Minus iexpr
            }
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Minus iexpr
            }
        | _ -> failwith "instantiate_expression: type error on unary -"
      end
  | Compilation.Multiplication (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType [||], IntegerType [||] ->
            {
              tex_type = IntegerType [||];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | (IntegerType [||] | RealType [||]),
          (IntegerType [||] | RealType [||]) ->
            {
              tex_type = RealType [||];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | IntegerType [|n|], IntegerType [|n'|] when n = n' ->
            {
              tex_type = IntegerType [||];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | (IntegerType [|n|] | RealType [|n|]),
          (IntegerType [|n'|] | RealType [|n'|]) when n = n' ->
            {
              tex_type = RealType [||];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | IntegerType [|n|], IntegerType [|n'; m|] when n = n' ->
            {
              tex_type = IntegerType [|m|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | (IntegerType [|n|] | RealType [|n|]),
          (IntegerType [|n'; m|] | RealType [|n'; m|]) when n = n' ->
            {
              tex_type = RealType [|m|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | IntegerType [|n; m|], IntegerType [|m'|] when m = m' ->
            {
              tex_type = IntegerType [|n|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | (IntegerType [|n; m|] | RealType [|n; m|]),
          (IntegerType [|m'|] | RealType [|m'|]) when m = m' ->
            {
              tex_type = RealType [|n|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | IntegerType [|n; m|], IntegerType [|m'; p|] when m = m' ->
            {
              tex_type = IntegerType [|n; p|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | (IntegerType [|n; m|] | RealType [|n; m|]),
          (IntegerType [|m'; p|] | RealType [|m'; p|]) when m = m' ->
            {
              tex_type = RealType [|n; p|];
              tex_expression = Multiplication (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on *"
      end
  | Compilation.NoEvent cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      {
        tex_type = iexpr.tex_type;
        tex_expression = NoEvent iexpr
      }
  | Compilation.Not cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | BooleanType dims ->
            {
              tex_type = BooleanType dims;
              tex_expression = Not iexpr
            }
        | _ -> failwith "instantiate_expression: type error on not"
      end
  | Compilation.NotEquals (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType [||], BooleanType [||] |
          IntegerType [||], IntegerType [||] |
          StringType [||], StringType [||] ->
            {
              tex_type = BooleanType [||];
              tex_expression = NotEquals (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on <>"
      end
  | Compilation.Or (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | BooleanType dims, BooleanType dims' when dims = dims' ->
            {
              tex_type = BooleanType dims;
              tex_expression = Or (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on or"
      end
  | Compilation.Power (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType [|m; n|], IntegerType [||] when m = n ->
            {
              tex_type = IntegerType [|m; m|];
              tex_expression = Power (iexpr, iexpr')
            }
        | RealType [|m; n|], IntegerType [||] when m = n ->
            {
              tex_type = RealType [|m; m|];
              tex_expression = Power (iexpr, iexpr')
            }
        | IntegerType [||], IntegerType [||] ->
            {
              tex_type = IntegerType [||];
              tex_expression = Power (iexpr, iexpr')
            }
        | IntegerType [||], RealType [||] ->
            {
              tex_type = RealType [||];
              tex_expression = Power (iexpr, iexpr')
            }
        | RealType [||], (IntegerType [||] | RealType [||]) ->
            {
              tex_type = RealType [||];
              tex_expression = Power (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on ^"
      end
  | Compilation.Real f ->
      {
        tex_type = RealType [||];
        tex_expression = Real f
      }
  | Compilation.Reference (Compilation.ParameterReference (level, path)) |
    Compilation.Reference (Compilation.VariableReference (level, path)) ->
      get_component_reference ctx level path
  | Compilation.Reference (Compilation.LoopVariableReference level) ->
      {
        tex_type = IntegerType [||];
        tex_expression = get_loop_variable_value ctx level
      }
  | Compilation.Reference (Compilation.ClassReference (level, strings)) ->
      failwith "instantiate_expression: class references not allowed"
  | Compilation.Sin cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Sin iexpr
            }
        | _ -> failwith "instantiate_expression: type error on sin"
      end
  | Compilation.Sqrt cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Sqrt iexpr
            }
        | _ -> failwith "instantiate_expression: type error on sqrt"
      end
  | Compilation.String s ->
      {
        tex_type = StringType [||];
        tex_expression = String s
      }
  | Compilation.Subtraction (cexpr, cexpr') ->
      let iexpr = instantiate_expression ctx cexpr
      and iexpr' = instantiate_expression ctx cexpr' in
      begin match iexpr.tex_type, iexpr'.tex_type with
        | IntegerType dims, IntegerType dims' when dims = dims' ->
            {
              tex_type = IntegerType dims;
              tex_expression = Subtraction (iexpr, iexpr')
            }
        | (IntegerType dims | RealType dims),
          (IntegerType dims' | RealType dims') when dims = dims' ->
            {
              tex_type = RealType dims;
              tex_expression = Subtraction (iexpr, iexpr')
            }
        | _ -> failwith "instantiate_expression: type error on -"
      end
  | Compilation.Tan cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Tan iexpr
            }
        | _ -> failwith "instantiate_expression: type error on tan"
      end
  | Compilation.Tanh cexpr ->
      let iexpr = instantiate_expression ctx cexpr in
      begin match iexpr.tex_type with
        | RealType dims ->
            {
              tex_type = RealType dims;
              tex_expression = Tanh iexpr
            }
        | _ -> failwith "instantiate_expression: type error on tanh"
      end
  | Compilation.Time ->
      {
        tex_type = RealType [||];
        tex_expression = Time
      }
  | Compilation.Vector cexprs ->
      let iexprs = Array.map (instantiate_expression ctx) cexprs in
      {
        tex_type = compute_array_type iexprs;
        tex_expression = Vector iexprs
      }

and check_function_type ctx lccl iexprs =
  let compare_input_types iexpr tex_type =
    iexpr.tex_type = tex_type ||
    match iexpr.tex_type, tex_type with
      | IntegerType dims, RealType dims' when dims = dims' -> true
      | _ -> false
  and extract_type = function
    | Compilation.DiscreteVariable { Compilation.vat_dimensions = cdims } ->
        let dims = Array.map (compute_subscript ctx) cdims in
        RealType dims
    | Compilation.RealVariable { Compilation.vat_dimensions = cdims } ->
        let dims = Array.map (compute_subscript ctx) cdims in
        RealType dims
    | _ -> assert false
  in
  let ccpnts = Lazy.force ((Lazy.force lccl).Compilation.ccl_public_cpnts) in
  let cpnts = List.map (fun (_, lcpnt) -> Lazy.force lcpnt) ccpnts in
  let pars, vars = separate_parameters_from_variables cpnts in
  let inputs, others = separate_inputs_from_others vars in
  let outputs, others = separate_outputs_from_others others in
  match pars, others with
    | [], [] ->
        let input_types = List.map extract_type inputs in
        begin try
          if List.for_all2 compare_input_types iexprs input_types then
            begin match List.map extract_type outputs with
              | [] -> failwith "check_function_type: no return value"
              | [t] -> t
              | ts -> CartesianProduct ts
            end
          else failwith "check_function_type: type error"
        with _ -> failwith "check_function_type: type error"
        end
    | _ -> failwith "check_function_type: invalid function declaration"

and instantiate_if_alternatives ctx tex_type alts =
  let rec instantiate_if_alternative (cexpr, cexpr') (tex_type, ialts) =
    let iexpr = instantiate_expression ctx cexpr
    and iexpr' = instantiate_expression ctx cexpr' in
    begin match iexpr.tex_type, iexpr'.tex_type, tex_type with
      | BooleanType [||], IntegerType dims, IntegerType dims'
        when dims = dims' ->
          tex_type, (iexpr, iexpr') :: ialts
      | BooleanType [||], (IntegerType dims | RealType dims), RealType dims'
        when dims = dims' ->
          RealType dims, (iexpr, iexpr') :: ialts
      | BooleanType [||], RealType dims, IntegerType dims'
        when dims = dims' ->
          RealType dims, (iexpr, iexpr') :: ialts
      | BooleanType [||], BooleanType dims, BooleanType dims'
        when dims = dims' ->
          tex_type, (iexpr, iexpr') :: ialts
      | BooleanType [||], StringType dims, StringType dims'
        when dims = dims' ->
          tex_type, (iexpr, iexpr') :: ialts
      | _ -> failwith "instantiate_if_alternative: type error"
    end
  in List.fold_right instantiate_if_alternative alts (tex_type, [])

and compute_array_type iexpr_array =
  let rec resize_dims dims n =
    Array.init
      (Array.length dims + 1)
      (fun i -> if i = 0 then n else dims.(i - 1))
  and type_checks tex_type i =
    i = Array.length iexpr_array ||
    iexpr_array.(i).tex_type = tex_type &&
    type_checks tex_type (i + 1)
  in
    let n = Array.length iexpr_array in
    if n = 0 then
      failwith "compute_array_type: empty literal array"
    else if type_checks iexpr_array.(0).tex_type 1 then
      begin match iexpr_array.(0).tex_type with
        | BooleanType dims -> BooleanType (resize_dims dims n)
        | CompoundType dims -> CompoundType (resize_dims dims n)
        | IntegerType dims -> IntegerType (resize_dims dims n)
        | RealType dims -> RealType (resize_dims dims n)
        | StringType dims -> StringType (resize_dims dims n)
        | CartesianProduct _ -> assert false
      end
    else failwith "compute_array_type: type error"

and expand_for_equation ctx start stop step cequs =
  let rec expand_for_equation' i iequs =
    if i > stop then iequs
    else
      let iequs' = List.fold_left (expand_equation i) iequs cequs in
      expand_for_equation' (i + step) iequs'
  and expand_equation i equ =
    let ctx' = ForContext (ctx, i) in
    instantiate_equation ctx' equ
  in
    if start >= 0 && stop >= 0 && start <= stop && step > 0 then
      expand_for_equation' start []
    else
      failwith "expand_for_equation: bad indexes"

and get_component_reference ctx level path =
  let icl = find_component_level ctx level in
  find_instantiated_component ctx icl level path

and get_loop_variable_value ctx level = match ctx, level with
  | ForContext (ctx', i), 0 -> Integer (Int32.of_int i)
  | ForContext (ctx', i), n -> get_loop_variable_value ctx' (level - 1)
  | _ -> failwith "get_loop_variable_value: compilation error"

and find_component_level ctx level = match ctx, level with
  | ToplevelContext, _ -> failwith "find_component_level: compilation error"
  | ClassContext (ctx', licl), 0 -> Lazy.force licl
  | ClassContext (ctx', _), n -> find_component_level ctx' (level - 1)
  | ForContext (ctx', _), _ -> find_component_level ctx' level

and find_instantiated_component ctx icl level = function
  | (s, cs) :: path when List.mem_assoc s (Lazy.force icl.icl_components) ->
      begin match Lazy.force (List.assoc s (Lazy.force icl.icl_components)) with
        | InstantiatedParameter ipar ->
            search_into_parameter ctx s cs level path ipar
        | InstantiatedVariable ivar ->
            search_into_variable ctx s cs level path ivar
      end
  | (s, cs) :: _ ->
      failwith ("find_instantiated_component: identifier not found: " ^ s)
  | _ -> assert false

and search_into_parameter ctx s cs level path ipar =
  let ics = Array.map (compute_subscript ctx) cs in
  match path, ipar with
  | ([] | [("value", [||])] | [("start", [||])]),
    InstantiatedIntegerParameter (_, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = IntegerType dims;
        tex_expression = ParameterValue (level, [(s, ics)])
      }
  | ([] | [("value", [||])] | [("start", [||])]),
    InstantiatedRealParameter (_, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = RealType dims;
        tex_expression = ParameterValue (level, [(s, ics)])
      }
  | (s, cs) :: _, _ -> failwith ("find_parameter: parameter not found: " ^ s)

and search_into_variable ctx s cs level path ivar =
  let ics = Array.map (compute_subscript ctx) cs in
  match path, ivar with
  | ([] | [("value", [||])]), InstantiatedDiscreteVariable (_, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = RealType dims;
        tex_expression = VariableValue (level, [(s, ics)])
      }
  | ([] | [("value", [||])]), InstantiatedRealVariable (_, _, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = RealType dims;
        tex_expression = VariableValue (level, [(s, ics)])
      }
  | [("start", [||])], InstantiatedDiscreteVariable (_, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = RealType dims;
        tex_expression = VariableStart (level, [(s, ics)])
      }
  | [("start", [||])], InstantiatedRealVariable (_, _, _, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = RealType dims;
        tex_expression = VariableStart (level, [(s, ics)])
      }
  | _ :: _ as path', InstantiatedCompoundVariable (_, iexpr) ->
      search_into_compound_variable ctx s ics level path' iexpr
  | [], InstantiatedCompoundVariable (_, iexpr) ->
      let dims = find_subvector_dims ics iexpr in
      {
        tex_type = CompoundType dims;
        tex_expression = VariableValue (level, [(s, ics)])
      }
  | (s, _) :: _, _ -> failwith ("find_variable: variable not found: " ^ s)

and search_into_compound_variable ctx s ics level path iexpr =
  let iexpr' = get_compound_subvector ics iexpr in
  match iexpr'.tex_type, iexpr'.tex_expression with
    | CompoundType [||], CompoundElement icl ->
        begin match find_instantiated_component ctx icl level path with
          | { tex_type = tex_type;
              tex_expression = VariableStart (level, iref) } ->
              {
                tex_type = tex_type;
                tex_expression = VariableStart (level, ((s, ics) :: iref))
              }
          | { tex_type = tex_type;
              tex_expression = VariableValue (level, iref) } ->
              {
                tex_type = tex_type;
                tex_expression = VariableValue (level, ((s, ics) :: iref))
              }
          | { tex_type = tex_type;
              tex_expression = ParameterValue (level, iref) } ->
              {
                tex_type = tex_type;
                tex_expression = ParameterValue (level, ((s, ics) :: iref))
              }
          | _ -> failwith "search_into_compound_variable: compilation error"
        end
    | CompoundType dims, Vector iexprs when Array.length dims > 0 ->
        (* FIXME: Not yet implemented *)
        failwith "search_into_compound_variable: compilation error"
    | _ -> failwith "search_into_compound_variable: compilation error"

and compute_subscript ctx = function
  | Compilation.Indefinite ->
      failwith "compute_subscript: invalid subscript"
  | Compilation.Definite cexpr ->
      begin match evaluate_integer_expression ctx cexpr with
        | Integer i when Int32.to_int i >= 0 -> Int32.to_int i
        | _ -> failwith "compute_subscript: type error"
      end

and find_subvector_dims ics = function
  | { tex_type = RealType dims } | { tex_type = IntegerType dims } |
    { tex_type = CompoundType dims } | { tex_type = StringType dims } |
    { tex_type = BooleanType dims }
    when no_out_of_bounds_access ics dims ->
      Array.sub dims (Array.length ics) (Array.length dims - Array.length ics)
  | _ -> failwith "find_subvector_dims: type error"

and get_compound_subvector ics iexpr =
  let rec get_compound_subvector' i iexpr =
    if i = Array.length ics then iexpr
    else begin match iexpr with
      | { tex_expression = Vector iexprs }
        when Array.length iexprs > ics.(i) ->
          get_compound_subvector' (i + 1) iexprs.(i)
      | _ -> failwith "get_compound_subvector: type error"
    end
  in get_compound_subvector' 0 iexpr

and no_out_of_bounds_access ics dims =
  let rec no_out_of_bounds_access' i =
    i < 0 || ics.(i) <= dims.(i) && no_out_of_bounds_access' (i - 1)
  in
    Array.length ics <= Array.length dims &&
    no_out_of_bounds_access' (Array.length ics - 1)


(* Flattening *)

let map2 f a a' =
  if Array.length a <> Array.length a' then invalid_arg "map2"
  else Array.mapi (fun i x -> f x a'.(i)) a

let rec array_map op iexpr = match iexpr with
  | { tex_expression = Vector iexprs } ->
      let tex_expr = Vector (Array.map (array_map op) iexprs) in
      { iexpr with tex_expression = tex_expr }
  | _ -> { iexpr with tex_expression = op iexpr }

let rec array_map2 op iexpr iexpr' = match iexpr, iexpr' with
  | { tex_expression = Vector iexprs }, { tex_expression = Vector iexprs' } ->
      let tex_expr = Vector (map2 (array_map2 op) iexprs iexprs') in
      { iexpr with tex_expression = tex_expr }
  | _ -> { iexpr with tex_expression = op iexpr iexpr' }

let rec list_of_array = function
  | { tex_expression = Vector iexprs } ->
      List.flatten (Array.to_list (Array.map list_of_array iexprs))
  | iexpr -> [iexpr]

let rec expand_class = function
  | { tex_type = CompoundType [||]; tex_expression = CompoundElement icl } ->
      let icpnts, iinit_equs, iequs = flatten_instantiated_class "main" icl in
      let flows = collect_flows icpnts
      and ss = collect_connected_components iequs in
      icpnts,
      iinit_equs,
      evaluate_cardinalities ss (perform_connections flows iequs)
  | _ -> assert false

and flatten_instantiated_class path icl =
  let icpnts, iinit_equs, iequs =
    flatten_components path icl.icl_components
  in
  let iinit_equs =
    flatten_equations
      (update_equations path (Lazy.force icl.icl_init_equs) @ iinit_equs)
  and iequs =
    flatten_equations
      (update_equations path (Lazy.force icl.icl_equs) @ iequs)
  in icpnts, iinit_equs, iequs

and collect_flows icpnts =
  let collect_flow flows = function
    | s, InstantiatedVariable
      (InstantiatedRealVariable (_, _, Compilation.Flow, _)) ->
        [{
          tex_type = RealType [||];
          tex_expression = VariableValue (0, [(s, [||])])
        }] :: flows
    | _ -> flows
  in List.fold_left collect_flow [] icpnts

and flatten_components path icpnts =
  let rec flatten_component icpnts iinit_equs iequs s = function
    | InstantiatedParameter ipar ->
        flatten_parameter icpnts s ipar, iinit_equs, iequs
    | InstantiatedVariable ivar ->
        flatten_variable icpnts iinit_equs iequs s ivar
  and flatten_parameter icpnts s = function
    | InstantiatedIntegerParameter (cmt, kind,
      ({ tex_type = IntegerType dims } as iexpr)) ->
        let make iexpr =
          InstantiatedParameter
            (InstantiatedIntegerParameter (cmt, kind, iexpr))
        in
        let ipars = flatten_component_tree make path s iexpr in
        icpnts @ ipars
    | InstantiatedRealParameter (cmt, kind,
      ({ tex_type = (IntegerType dims | RealType dims) } as iexpr)) ->
        let make iexpr =
          InstantiatedParameter (InstantiatedRealParameter (cmt, kind, iexpr))
        in
        let ipars = flatten_component_tree make path s iexpr in
        icpnts @ ipars
    | _ -> failwith "flatten_parameter: type error"
  and flatten_variable icpnts iinit_equs iequs s = function
    | InstantiatedDiscreteVariable (cmt, inout,
      ({ tex_type = (IntegerType dims | RealType dims) } as iexpr)) ->
        let make iexpr =
          InstantiatedVariable (
            InstantiatedDiscreteVariable (cmt, inout, iexpr))
        in
        let ivars = flatten_component_tree make path s iexpr in
        icpnts @ ivars, iinit_equs, iequs
    | InstantiatedRealVariable (cmt, inout, flow,
      ({ tex_type = (IntegerType dims | RealType dims) } as iexpr)) ->
        let make iexpr =
          InstantiatedVariable
            (InstantiatedRealVariable (cmt, inout, flow, iexpr))
        in
        let ivars = flatten_component_tree make path s iexpr in
        icpnts @ ivars, iinit_equs, iequs
    | InstantiatedCompoundVariable (cmt,
      ({ tex_type = CompoundType dims } as iexpr)) ->
        let make iexpr =
          InstantiatedVariable (InstantiatedCompoundVariable (cmt, iexpr)) in
        let ivars = flatten_component_tree make path s iexpr in
        let icpnts', iinit_equs', iequs' = explode_compound_components ivars in
        icpnts @ icpnts', iinit_equs @ iinit_equs', iequs @ iequs'
    | _ -> failwith "flatten_variable: type error"
  and flatten_components' icpnts iinit_equs iequs = function
    | [] -> icpnts, iinit_equs, iequs
    | (s, icpnt) :: icpnts'' ->
        let icpnts', iinit_equs', iequs' =
          flatten_component icpnts iinit_equs iequs s (Lazy.force icpnt)
        in flatten_components' icpnts' iinit_equs' iequs' icpnts''
  in flatten_components' [] [] [] (Lazy.force icpnts)

and flatten_component_tree make path s iexpr =
  let rec flatten_component_tree' path = function
    | { tex_expression = Vector iexprs } ->
        flatten_subcomponents path iexprs 1
    | iexpr ->
        let iexpr' = update_typed_expression path iexpr in
        [(path ^ "]", make iexpr')]
  and flatten_subcomponents path iexprs i =
    if i > Array.length iexprs then []
    else
      let path' = match iexprs.(i - 1) with
        | { tex_expression = Vector iexprs } ->
          path ^ string_of_int i ^ "]["
        | _ -> path ^ string_of_int i
      in
        (flatten_component_tree' path' iexprs.(i - 1)) @
        flatten_subcomponents path iexprs (i + 1)
  in match iexpr with
    | { tex_expression = Vector _ } ->
        flatten_component_tree' (path ^ "." ^ s ^ "[") iexpr
    | _ ->
        let iexpr' = update_typed_expression path iexpr in
        [(path ^ "." ^ s, make iexpr')]

and explode_compound_components ivars =
  let rec explode_compound_component icpnts iinit_equs iequs = function
    | path, InstantiatedVariable (InstantiatedCompoundVariable (cmt,
      { tex_expression = CompoundElement icl })) ->
        let icpnts', iinit_equs', iequs' =
          flatten_instantiated_class path icl
        in icpnts @ icpnts', iinit_equs @ iinit_equs', iequs @ iequs'
    | _ -> failwith "explode_compound_component: Not a compound component"
  and explode_compound_components' icpnts iinit_equs iequs = function
    | [] -> icpnts, iinit_equs, iequs
    | ivar :: ivars ->
        let icpnts', iinit_equs', iequs' =
          explode_compound_component icpnts iinit_equs iequs ivar
        in explode_compound_components' icpnts' iinit_equs' iequs' ivars
  in explode_compound_components' [] [] [] ivars

and update_equations path iequs =
  List.rev_map (update_equation path) iequs

and update_equation path = function
  | Equation (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Equation (iexpr, iexpr')
  | ConditionalEquation (iif_clauses, iequs) ->
      let iif_clauses = update_if_clauses path iif_clauses
      and iequs = update_equations path iequs in
       ConditionalEquation (iif_clauses, iequs)
  | FlowConnection (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      FlowConnection (iexpr, iexpr')
  | When iwhen_clauses ->
      let iwhen_clauses = update_when_clauses path iwhen_clauses in
      When iwhen_clauses

and update_if_clauses path iif_clauses =
  let rec update_if_clause (iexpr, iequs) =
    let iexpr = update_typed_expression path iexpr
    and iequs = update_equations path iequs in
    iexpr, iequs
  in List.map update_if_clause iif_clauses

and update_when_clauses path iwhen_clauses =
  let rec update_when_clause (iexpr, iwhen_equs) =
    let iexpr' = update_typed_expression path iexpr
    and iwhen_equs' =
      List.map
        (function
          | Reinit (iexpr, iexpr') ->
              let iexpr = update_typed_expression path iexpr
              and iexpr' = update_typed_expression path iexpr' in
              Reinit (iexpr, iexpr')
          | Assign (iexpr, iexpr') ->
              let iexpr = update_typed_expression path iexpr
              and iexpr' = update_typed_expression path iexpr' in
              Assign (iexpr, iexpr'))
        iwhen_equs in
    (iexpr', iwhen_equs')
  in List.map update_when_clause iwhen_clauses

and update_typed_expression path iexpr =
  { iexpr with tex_expression =
      update_expression path iexpr.tex_expression
  }

and update_expression path = function
  | Abs iexpr -> Abs (update_typed_expression path iexpr)
  | Addition (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Addition (iexpr, iexpr')
  | And (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      And (iexpr, iexpr')
  | Boolean _ as iexpr -> iexpr
  | Cardinality iexpr -> Cardinality (update_typed_expression path iexpr)
  | CompoundElement _ as iexpr -> iexpr
  | Cos iexpr -> Cos (update_typed_expression path iexpr)
  | Der iexpr -> Der (update_typed_expression path iexpr)
  | Division (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Division (iexpr, iexpr')
  | Equals (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Equals (iexpr, iexpr')
  | Exp iexpr -> Exp (update_typed_expression path iexpr)
  | ExternalFunctionCall (name, iexprs) ->
      let iexprs = List.map (update_typed_expression path) iexprs in
      ExternalFunctionCall (name, iexprs)
  | Floor iexpr -> Floor (update_typed_expression path iexpr)
  | GreaterEqualThan (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      GreaterEqualThan (iexpr, iexpr')
  | GreaterThan (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      GreaterThan (iexpr, iexpr')
  | If (iif_exprs, iexpr) ->
      If (
        List.map
          (fun (iexpr, iexpr') ->
            let iexpr = update_typed_expression path iexpr
            and iexpr' = update_typed_expression path iexpr' in
            iexpr, iexpr')
          iif_exprs,
        update_typed_expression path iexpr)
  | Integer _ as iexpr -> iexpr
  | Log iexpr -> Log (update_typed_expression path iexpr)
  | Max (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Max (iexpr, iexpr')
  | Min (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Min (iexpr, iexpr')
  | Mod (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Mod (iexpr, iexpr')
  | Minus iexpr -> Minus (update_typed_expression path iexpr)
  | Multiplication (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Multiplication (iexpr, iexpr')
  | NoEvent iexpr -> NoEvent (update_typed_expression path iexpr)
  | Not iexpr -> Not (update_typed_expression path iexpr)
  | NotEquals (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      NotEquals (iexpr, iexpr')
  | Or (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Or (iexpr, iexpr')
  | ParameterValue (level, iref) ->
      ParameterValue (level, update_reference level path iref)
  | Power (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Power (iexpr, iexpr')
  | Real _ as iexpr -> iexpr
  | Sin iexpr -> Sin (update_typed_expression path iexpr)
  | Sqrt iexpr -> Sqrt (update_typed_expression path iexpr)
  | String _ as iexpr -> iexpr
  | Subtraction (iexpr, iexpr') ->
      let iexpr = update_typed_expression path iexpr
      and iexpr' = update_typed_expression path iexpr' in
      Subtraction (iexpr, iexpr')
  | Tan iexpr -> Tan (update_typed_expression path iexpr)
  | Tanh iexpr -> Tanh (update_typed_expression path iexpr)
  | Time -> Time
  | VariableStart (level, iref) ->
      VariableStart (level, update_reference level path iref)
  | VariableValue (level, iref) ->
      VariableValue (level, update_reference level path iref)
  | Vector iexprs ->
      Vector (Array.map (update_typed_expression path) iexprs)

and update_reference level path =
  let rec to_string = function
    | [] -> ""
    | [i] -> string_of_int i
    | i :: is -> string_of_int i ^ "][" ^ to_string is
  in function
    | [(s, [||])] ->
        let path' = update_path level path in
        [(path' ^ "." ^ s, [||])]
    | [(s, ics)] ->
        let path' = update_path level path in
        [(path' ^ "." ^ s ^ "[" ^ to_string (Array.to_list ics) ^ "]", [||])]
    | (s, [||]) :: iref ->
        let path' = update_path level path in
        update_reference level (path' ^ "." ^ s) iref
    | (s, ics) :: iref ->
        let path' = update_path level path in
        update_reference
          level
          (path' ^ "." ^ s ^ "[" ^ to_string (Array.to_list ics) ^ "]")
          iref
    | [] -> assert false

and update_path level path = match level with
  | 0 -> path
  | n ->
      let path' = String.sub path 0 (String.rindex path '.') in
      update_path (n - 1) path'

and flatten_equations iequs =
  List.flatten (List.rev_map flatten_equation iequs)

and flatten_equation = function
  | Equation (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      List.map2
        (fun iexpr iexpr' -> Equation (iexpr, iexpr'))
        (list_of_array iexpr)
        (list_of_array iexpr')
  | ConditionalEquation (iif_clauses, iequs) ->
      let iif_clauses = flatten_if_clauses iif_clauses
      and iequs = flatten_equations iequs in
      [ConditionalEquation (iif_clauses, iequs)]
  | FlowConnection (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      List.map2
        (fun iexpr iexpr' -> FlowConnection (iexpr, iexpr'))
        (list_of_array iexpr)
        (list_of_array iexpr')
  | When iwhen_clauses ->
      let iwhen_clauses = flatten_when_clauses iwhen_clauses in
      [When iwhen_clauses]

and flatten_if_clauses iif_clauses =
  let rec flatten_if_clause (iexpr, iequs) =
    let iexpr = flatten_typed_expression iexpr
    and iequs = flatten_equations iequs in
    iexpr, iequs
  in List.map flatten_if_clause iif_clauses

and flatten_when_clauses iwhen_clauses =
  let rec flatten_when_clause (iexpr, iwhen_equs) =
    let iexpr' = flatten_typed_expression iexpr
    and iwhen_equs' =
      List.map
        (function
          | Reinit (iexpr, iexpr') ->
              let iexpr = flatten_typed_expression iexpr
              and iexpr' = flatten_typed_expression iexpr' in
              Reinit (iexpr, iexpr')
          | Assign (iexpr, iexpr') ->
              let iexpr = flatten_typed_expression iexpr
              and iexpr' = flatten_typed_expression iexpr' in
              Assign (iexpr, iexpr'))
        iwhen_equs in
    (iexpr', iwhen_equs')
  in List.map flatten_when_clause iwhen_clauses

and flatten_typed_expression iexpr'' = match iexpr''.tex_expression with
  | Abs iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Abs iexpr) iexpr
  | Addition (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      array_map2 (fun iexpr iexpr' -> Addition (iexpr, iexpr')) iexpr iexpr'
  | And (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      array_map2 (fun iexpr iexpr' -> And (iexpr, iexpr')) iexpr iexpr'
  | Boolean _ -> iexpr''
  | Cardinality iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Cardinality iexpr) iexpr
  | CompoundElement _ -> iexpr''
  | Cos iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Cos iexpr) iexpr
  | Der iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Der iexpr) iexpr
  | Division (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      let iexpr = array_map (fun iexpr -> Division (iexpr, iexpr')) iexpr in
      { iexpr with tex_type = iexpr''.tex_type }
  | Equals (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = Equals (iexpr, iexpr') }
  | Exp iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Exp iexpr) iexpr
  | ExternalFunctionCall (name, [iexpr]) ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> ExternalFunctionCall (name, [iexpr])) iexpr
  | ExternalFunctionCall (name, iexprs) ->
      let iexprs = List.map flatten_typed_expression iexprs in
      { iexpr'' with tex_expression = ExternalFunctionCall (name, iexprs) }
  | Floor iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Floor iexpr) iexpr
  | GreaterEqualThan (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = GreaterEqualThan (iexpr, iexpr') }
  | GreaterThan (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = GreaterThan (iexpr, iexpr') }
  | If (iif_exprs, iexpr) -> (*FIXME: Matrix case*)
      { iexpr'' with tex_expression = If (
        List.map
          (fun (iexpr, iexpr') ->
            let iexpr = flatten_typed_expression iexpr
            and iexpr' = flatten_typed_expression iexpr' in
            iexpr, iexpr')
          iif_exprs,
        flatten_typed_expression iexpr) }
  | Integer _ -> iexpr''
  | Log iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Log iexpr) iexpr
  | Max (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = Max (iexpr, iexpr') }
  | Min (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = Min (iexpr, iexpr') }
  | Mod (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = Mod (iexpr, iexpr') }
  | Minus iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Minus iexpr) iexpr
  | Multiplication (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      flatten_multiplication iexpr iexpr' iexpr''
  | NoEvent iexpr ->
      { iexpr'' with tex_expression = NoEvent (flatten_typed_expression iexpr) }
  | Not iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Not iexpr) iexpr
  | NotEquals (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = NotEquals (iexpr, iexpr') }
  | Or (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      array_map2 (fun iexpr iexpr' -> Or (iexpr, iexpr')) iexpr iexpr'
  | ParameterValue (level, iref) ->
      let make iref = ParameterValue (level, iref) in
      expand_identifier iexpr''.tex_type make level iref
  | Power (iexpr, iexpr') -> (*FIXME: Matrix case*)
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      { iexpr'' with tex_expression = Power (iexpr, iexpr') }
  | Real _ -> iexpr''
  | Sin iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Sin iexpr) iexpr
  | Sqrt iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Sqrt iexpr) iexpr
  | String _ -> iexpr''
  | Subtraction (iexpr, iexpr') ->
      let iexpr = flatten_typed_expression iexpr
      and iexpr' = flatten_typed_expression iexpr' in
      array_map2 (fun iexpr iexpr' -> Subtraction (iexpr, iexpr')) iexpr iexpr'
  | Tan iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Tan iexpr) iexpr
  | Tanh iexpr ->
      let iexpr = flatten_typed_expression iexpr in
      array_map (fun iexpr -> Tanh iexpr) iexpr
  | Time -> iexpr''
  | VariableStart (level, iref) ->
      let make iref = VariableStart (level, iref) in
      expand_identifier iexpr''.tex_type make level iref
  | VariableValue (level, iref) ->
      let make iref = VariableValue (level, iref) in
      expand_identifier iexpr''.tex_type make level iref
  | Vector _ -> iexpr''

and flatten_multiplication iexpr iexpr' iexpr'' =
  let extract_subvector = function
    | { tex_expression = Vector iexprs } -> iexprs
    | _ -> assert false
  and transpose_matrix = function
    | {
        tex_type = (RealType [|m; n|] | IntegerType [|m; n|]);
        tex_expression = Vector iexprs
      } ->
        {
          tex_type = RealType [||]; (* Doesn't matter *)
          tex_expression = Vector(
            Array.init n (fun i ->
              {
                tex_type = RealType [||]; (* Doesn't matter *)
                tex_expression = Vector (
                  Array.init m (fun j ->
                    match iexprs.(j) with
                      | { tex_expression = Vector iexprs' } ->
                          iexprs'.(i)
                      | _ -> assert false))
              }))
        }
    | _ -> assert false
  in
  let rec flatten_vector_by_vector_product iexprs iexprs' =
    let rec flatten_vector_by_vector_product' acc i =
      if i = Array.length iexprs then acc
      else
        let iexpr =
          {
            tex_type = RealType [||]; (* Doesn't matter *)
            tex_expression = Multiplication (iexprs.(i), iexprs'.(i))
          }
        in
        let acc' =
          {
            tex_type = RealType [||]; (* Doesn't matter *)
            tex_expression = Addition (acc, iexpr)
          }
        in flatten_vector_by_vector_product' acc' (i + 1)
    in
    let acc =
      {
        tex_type = RealType [||]; (* Doesn't matter *)
        tex_expression = Multiplication (iexprs.(0), iexprs'.(0))
      }
    in flatten_vector_by_vector_product' acc 1 (* FIXME: empty vectors *)
  and flatten_vector_by_matrix_product iexpr iexpr' =
    let iexpr' = transpose_matrix iexpr' in
    match iexpr.tex_expression, iexpr'.tex_expression with
      | Vector iexprs, Vector iexprs' ->
          {
            tex_type = RealType [||]; (* Doesn't matter *)
            tex_expression =
              Vector (Array.map
                (fun iexpr' ->
                  let iexprs' = extract_subvector iexpr' in
                  flatten_vector_by_vector_product iexprs iexprs')
                iexprs')
          }
      | _ -> assert false
  and flatten_matrix_by_vector_product iexpr iexpr' =
    match iexpr.tex_expression, iexpr'.tex_expression with
      | Vector iexprs, Vector iexprs' ->
          {
            tex_type = RealType [||]; (* Doesn't matter *)
            tex_expression =
              Vector (Array.map
                (fun iexpr ->
                  let iexprs = extract_subvector iexpr in
                  flatten_vector_by_vector_product iexprs iexprs')
                iexprs)
          }
      | _ -> assert false
  and flatten_matrix_by_matrix_product iexpr iexpr' =
    match iexpr'.tex_expression with
      | Vector iexprs' ->
          let iexpr = transpose_matrix iexpr in
          {
            tex_type = RealType [||]; (* Doesn't matter *)
            tex_expression =
              Vector (Array.map
                (fun iexpr' -> flatten_matrix_by_vector_product iexpr iexpr')
                iexprs')
          }
      | _ -> assert false
  in match iexpr.tex_type, iexpr'.tex_type with
  | (IntegerType [||] | RealType [||]), (IntegerType [||] | RealType [||]) ->
      { iexpr'' with tex_expression = Multiplication (iexpr, iexpr') }
  | (IntegerType [||] | RealType [||]), _ ->
      let iexpr =
        array_map (fun iexpr' -> Multiplication (iexpr, iexpr')) iexpr'
      in { iexpr with tex_type = iexpr''.tex_type }
  | (IntegerType [|n|] | RealType [|n|]),
    (IntegerType [|n'|] | RealType [|n'|]) ->
      begin
        match iexpr.tex_expression, iexpr'.tex_expression with
        | Vector iexprs, Vector iexprs' ->
            flatten_vector_by_vector_product iexprs iexprs'
        | _ -> assert false
      end
  | (IntegerType [|n|] | RealType [|n|]),
    (IntegerType [|n'; m'|] | RealType [|n'; m'|]) ->
      flatten_vector_by_matrix_product iexpr iexpr'
  | (IntegerType [|n; m|] | RealType [|n; m|]),
    (IntegerType [|m'|] | RealType [|m'|]) ->
      flatten_matrix_by_vector_product iexpr iexpr'
  | (IntegerType [|n; m|] | RealType [|n; m|]),
    (IntegerType [|m'; p'|] | RealType [|m'; p'|]) ->
      flatten_matrix_by_matrix_product iexpr iexpr'
  | _ -> assert false

and expand_identifier tex_type make level iref =
  let get_dims = function
    | BooleanType dims | CompoundType dims | IntegerType dims | RealType dims |
      StringType dims -> dims
    | CartesianProduct _ -> assert false
  and base_type tex_type dims = match tex_type with
    | BooleanType _ -> BooleanType dims
    | CompoundType _ -> CompoundType dims
    | IntegerType _ -> IntegerType dims
    | RealType _ -> RealType dims
    | StringType _ -> StringType dims
    | CartesianProduct _ -> assert false
  in
  let dims = get_dims tex_type in
  let rec expand_identifier' name n =
    if n = Array.length dims then
      {
        tex_type = base_type tex_type [||];
        tex_expression = make [(name, [||])]
      }
    else
      let iexprs = Array.init dims.(n) (create_subcomponent name n) in
      {
        tex_type =
          base_type tex_type (Array.sub dims n (Array.length dims - n));
        tex_expression = Vector iexprs
      }
  and create_subcomponent name n i =
    let name' = name ^ "[" ^ string_of_int (i + 1) ^ "]" in
    expand_identifier' name' (n + 1)
  in match iref with
    | [(name, [||])] -> expand_identifier' name 0
    | _ -> failwith "expand_identifier: bad reference"

and perform_connections flows iequs =
  let rec remove_set_containing iexpr = function
    | [] -> failwith "remove_set_containing"
    | iexprs :: iexprss when List.mem iexpr iexprs ->
        iexprs, iexprss
    | iexprs :: iexprss ->
        let iexprs', iexprss' = remove_set_containing iexpr iexprss in
        iexprs', iexprs :: iexprss'
  and merge_connections flows others = function
    | [] -> others @ (List.rev_map sum_to_zero flows)
    | FlowConnection (iexpr, iexpr') :: iequs' ->
        let iexprs, flows = remove_set_containing iexpr flows in
        let iexprs', flows = remove_set_containing iexpr' flows in
        let flows' = (iexprs @ iexprs') :: flows in
        merge_connections flows' others iequs'
    | iequ :: iequs' -> merge_connections flows (iequ :: others) iequs'
  and sum_to_zero = function
    | iexpr :: iexprs ->
        let sum =
          List.fold_left
            (fun acc iexpr ->
              {
                tex_type = iexpr.tex_type;
                tex_expression = Addition (acc, iexpr)
              })
            iexpr
            iexprs
        and zero = { tex_type = RealType [||]; tex_expression = Real 0.0 }
        in Equation (sum, zero)
    | [] -> failwith "perform_connections: invalid connection"
  in merge_connections flows [] iequs

and collect_connected_components =
  let truncate_identifier = function
    | VariableValue (_, [(s, [||])]) ->
        let i = String.rindex s '.' in
        String.sub s 0 i
    | _ -> failwith "truncate_identifier: flattened reference expected"
  in function
    | [] -> []
    | FlowConnection ({ tex_expression = iexpr }, { tex_expression = iexpr' })
      :: iequs ->
        let s = truncate_identifier iexpr
        and s' = truncate_identifier iexpr' in
        s :: s' :: collect_connected_components iequs
    | _ :: iequs -> collect_connected_components iequs

and evaluate_cardinalities ss iequs =
  let rec evaluate_cardinalities_in_equation = function
    | Equation (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        Equation (iexpr, iexpr')
    | ConditionalEquation (iif_clauses, iequs) ->
        failwith
          "evaluate_cardinalities_in_equation: conditional equations not\
          allowed."
    | iequ -> iequ
  and evaluate_cardinalities_in_expression iexpr'' =
    match iexpr''.tex_expression with
    | Addition (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Addition (iexpr, iexpr')
        }
    | And (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = And (iexpr, iexpr')
        }
    | Division (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Division (iexpr, iexpr')
        }
    | Equals (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Equals (iexpr, iexpr')
        }
    | GreaterEqualThan (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = GreaterEqualThan (iexpr, iexpr')
        }
    | GreaterThan (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = GreaterThan (iexpr, iexpr')
        }
    | Max (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Max (iexpr, iexpr')
        }
    | Min (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Min (iexpr, iexpr')
        }
    | Mod (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Mod (iexpr, iexpr')
        }
    | Multiplication (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Multiplication (iexpr, iexpr')
        }
    | NotEquals (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = NotEquals (iexpr, iexpr')
        }
    | Or (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Or (iexpr, iexpr')
        }
    | Power (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Power (iexpr, iexpr')
        }
    | Subtraction (iexpr, iexpr') ->
        let iexpr = evaluate_cardinalities_in_expression iexpr
        and iexpr' = evaluate_cardinalities_in_expression iexpr' in
        {
          tex_type = RealType [||];
          tex_expression = Subtraction (iexpr, iexpr')
        }
    | Boolean _ | CompoundElement _ | Integer _ | Real _ | String _ |
      Time | Vector _ | ParameterValue _ | VariableStart _ |
      VariableValue _-> iexpr''
    | Cardinality { tex_expression = VariableValue (_, [(s, [||])]) } ->
        {
          tex_type = IntegerType [||];
          tex_expression =
            Integer (
              List.fold_left
                (fun acc s' -> if s = s' then Int32.add acc Int32.one else acc)
                Int32.zero
                ss)
        }
    | Cardinality _ ->
        failwith "evaluate_cardinalities_in_expression: wrong call to cardinality ()."
    | Abs iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Abs iexpr
        }
    | Cos iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Cos iexpr
        }
    | Der iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Der iexpr
        }
    | Exp iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Exp iexpr
        }
    | Floor iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Floor iexpr
        }
    | Log iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Log iexpr
        }
    | Minus iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Minus iexpr
        }
    | NoEvent iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = NoEvent iexpr
        }
    | Not iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Not iexpr
        }
    | Sin iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Sin iexpr
        }
    | Sqrt iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Sqrt iexpr
        }
    | Tan iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Tan iexpr
        }
    | Tanh iexpr ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        {
          tex_type = RealType [||];
          tex_expression = Tanh iexpr
        }
    | ExternalFunctionCall (name, [iexpr]) ->
        let iexpr = evaluate_cardinalities_in_expression iexpr in
        { iexpr'' with tex_expression = ExternalFunctionCall (name, [iexpr]) }
    | ExternalFunctionCall (name, iexprs) ->
        let iexprs = List.map evaluate_cardinalities_in_expression iexprs in
        { iexpr'' with tex_expression = ExternalFunctionCall (name, iexprs) }
    | If (iif_exprs, iexpr) ->
        { iexpr'' with tex_expression = If (
          List.map
            (fun (iexpr, iexpr') ->
              let iexpr = evaluate_cardinalities_in_expression iexpr
              and iexpr' = evaluate_cardinalities_in_expression iexpr' in
              iexpr, iexpr')
            iif_exprs,
          evaluate_cardinalities_in_expression iexpr) }
  in List.rev_map evaluate_cardinalities_in_equation iequs
