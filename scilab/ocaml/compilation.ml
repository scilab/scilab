
open Precompilation


(* Misc *)

let opt_map f = function
  | Some x -> Some (f x)
  | None -> None


(* Exceptions *)

exception InvalidModification
exception TypeError


(* Datatypes *)

type compilation_context =
  | ClassContext of compilation_context * compiled_class Lazy.t
  | ForContext of compilation_context * string
  | ModificationContext of compilation_context * compiled_class Lazy.t
  | TopLevelContext

and compiled_unit =
  | CompiledClass of compiled_class Lazy.t
  | CompiledFunction of compiled_class Lazy.t

and compiled_class =
  {
    ccl_public_cpnts: (string * compiled_component Lazy.t) list Lazy.t;
    ccl_initial_equs: compiled_equation list Lazy.t;
    ccl_equs: compiled_equation list Lazy.t;
  }

and compiled_modification =
  | CompiledModification of field * compiled_modification list *
    compiled_expression option

and field = string * compiled_subscript array

and compiled_reference =
  | ParameterReference of level * path
  | VariableReference of level * path
  | LoopVariableReference of level
  | ClassReference of level * string list

and path = field list

and level = int

and compiled_subscript =
  | Indefinite
  | Definite of compiled_expression

and parameter =
  | IntegerParameter of parameter_attributes
  | RealParameter of parameter_attributes

and parameter_attributes =
  {
    pat_dimensions: compiled_subscript array;
    pat_comment: string;
    pat_value: compiled_expression option
  }

and variable =
  | DiscreteVariable of variable_attributes
  | RealVariable of variable_attributes
  | CompoundVariable of compiled_class Lazy.t * variable_attributes

and variable_attributes =
  {
    vat_dimensions: compiled_subscript array;
    vat_nature: nature;
    vat_inout: inout;
    vat_comment: string;
    vat_modifications: compiled_modification list
  }

and compiled_component =
  | Parameter of parameter
  | Variable of variable

and nature =
  | Flow
  | Potential

and inout =
  | Input
  | Output
  | Both

and compiled_equation =
  | CompiledEquality of compiled_expression * compiled_expression
  | CompiledFlowConnection of compiled_expression * compiled_expression
  | CompiledIf of (compiled_expression * compiled_equation list) list *
    compiled_equation list
  | CompiledFor of compiled_expression * compiled_expression *
    compiled_expression * compiled_equation list
  | CompiledWhen of (compiled_expression * compiled_when_expression list) list

and compiled_when_expression =
  | Reinit of compiled_reference * compiled_expression
  | Assign of compiled_reference * compiled_expression

and compiled_expression =
  | Abs of compiled_expression
  | Addition of compiled_expression * compiled_expression
  | And of compiled_expression * compiled_expression
  | Boolean of bool
  | Cardinality of compiled_expression
  | Cos of compiled_expression
  | Der of compiled_expression
  | Division of compiled_expression * compiled_expression
  | Equals of compiled_expression * compiled_expression
  | Exp of compiled_expression
  | ExternalFunctionCall of string list * compiled_class Lazy.t *
    compiled_expression list
  | Floor of compiled_expression
  | GreaterEqualThan of compiled_expression * compiled_expression
  | GreaterThan of compiled_expression * compiled_expression
  | If of (compiled_expression * compiled_expression) list * compiled_expression
  | Integer of int32
  | Log of compiled_expression
  | Max of compiled_expression * compiled_expression
  | Min of compiled_expression * compiled_expression
  | Minus of compiled_expression
  | Mod of compiled_expression * compiled_expression
  | Multiplication of compiled_expression * compiled_expression
  | NoEvent of compiled_expression
  | Not of compiled_expression
  | NotEquals of compiled_expression * compiled_expression
  | Or of compiled_expression * compiled_expression
  | Power of compiled_expression * compiled_expression
  | Real of float
  | Reference of compiled_reference
  | Sin of compiled_expression
  | Sqrt of compiled_expression
  | String of string
  | Subtraction of compiled_expression * compiled_expression
  | Tan of compiled_expression
  | Tanh of compiled_expression
  | Time
  | Vector of compiled_expression array


(* Marshaling *)

let paths = ref [""]
(*current searching path (for marshaling) *)

let read_class_file f =
  let rec read_class_file' = function
    | [] ->
        let s = Filename.chop_extension f in
        failwith ("read_class_file: Class " ^ s ^ " not found")
    | s :: ss ->
        try
          let ic = open_in_bin (Filename.concat s f) in
          let paths', cu =
            (Marshal.from_channel ic : string list ref * compiled_unit)
          in paths' := !paths;
          (* **side effect**: Updating new path before evaluating lazy values *)
          cu
        with
          | Sys_error _ -> read_class_file' ss
  in read_class_file' !paths

let write_class_file f cu =
  let oc = open_out_bin f
  and flags = [Marshal.Closures] in
  Marshal.to_channel oc (paths, cu : string list ref * compiled_unit) flags

let create_filename name ext =
  let rec create_name prefix = function
    | [] -> failwith "create_name: Empty name"
    | [s] -> prefix, s
    | s :: ss -> create_name (prefix @ [s]) ss
  in
  let prefix, base = create_name [] name in
  List.fold_right Filename.concat prefix (base ^ ext)


(* Compilation *)

let rec compile_main_class pcl = match pcl.public_classes with
  | [] -> failwith "compile_main_class: No main class declared"
  | [(_, pcl)] ->
      begin match pcl.class_kind with
        | ParseTree.Function ->
            CompiledFunction (compile_compound_class TopLevelContext pcl)
        | ParseTree.Class ->
            CompiledClass (compile_compound_class TopLevelContext pcl)
        | _ ->
            failwith
              "compile_main_class: Only external functions and classes allowed"
      end
  | _ -> failwith "compile_main_class: More than one class declared at toplevel"

and compile_compound_class ctx pcl =
  let rec ctx' = ClassContext (ctx, lccl)
  and defined_equs = lazy (compile_equation_clauses ctx' pcl.equs)
  and lccl = lazy
    {
      ccl_public_cpnts = lazy (
        List.map
          (fun (id, cpnt) -> id, lazy (compile_component ctx' cpnt))
          pcl.public_cpnts);
      ccl_initial_equs = lazy (fst (Lazy.force defined_equs));
      ccl_equs = lazy (snd (Lazy.force defined_equs));
    }
  in lccl

and compile_equation_clauses ctx equ_clauses =
  let rec compile_equation_clauses' init_equs equs = function
    | [] -> init_equs, equs
    | ParseTree.EquationClause (ParseTree.Initial, pt_equs) :: equ_clauses ->
        let init_equs' =
          List.fold_left
            (fun init_equs pt_equ ->
              (compile_equation_or_annotation ctx pt_equ) @ init_equs)
            init_equs
            pt_equs
        in compile_equation_clauses' init_equs' equs equ_clauses
    | ParseTree.EquationClause (ParseTree.NotInitial, pt_equs) :: equ_clauses ->
        let equs' =
          List.fold_left
            (fun equs pt_equ ->
              (compile_equation_or_annotation ctx pt_equ) @ equs)
            equs
            pt_equs
        in compile_equation_clauses' init_equs equs' equ_clauses
  in compile_equation_clauses' [] [] equ_clauses

and compile_equation_or_annotation ctx = function
  | ParseTree.Equation (equ, _) -> compile_equation ctx equ
  | ParseTree.EquationAnnotation _ ->
      failwith "compile_equation: Annotations not allowed"

and compile_equation ctx = function
  | ParseTree.Equality (expr, expr') ->
      let cexpr = compile_expression ctx expr
      and cexpr' = compile_expression ctx expr' in
      [CompiledEquality (cexpr, cexpr')]
  | ParseTree.ConnectClause (conn_ref, conn_ref') ->
      let var = find_connector ctx conn_ref
      and var' = find_connector ctx conn_ref' in
      begin match var, var' with
        | CompoundVariable (lccl, attrs), CompoundVariable (lccl', attrs') ->
            compile_connection ctx conn_ref conn_ref' lccl attrs lccl' attrs'
        | _ -> failwith "compile_equation: Invalid connection"
      end
  | ParseTree.WhenClauseE when_clauses ->
      [CompiledWhen (List.map (compile_when_clause ctx) when_clauses)]
  | ParseTree.ConditionalEquationE (if_clauses, equs) ->
      let cif_clauses = List.map (compile_if_clause ctx) if_clauses
      and cequs =
        List.fold_left
          (fun equs equ -> compile_equation ctx equ @ equs)
          []
          equs
      in [CompiledIf (cif_clauses, cequs)]
  | ParseTree.ForClauseE (for_indices, equs) ->
      compile_for_clause ctx for_indices equs
  | ParseTree.FunctionCallE _ ->
      failwith "compile_equation: Unsupported equation type"

and find_connector ctx conn_ref = match ctx, conn_ref with
  | ClassContext (_, lccl), [(s, array_subscr)]
    when List.mem_assoc s (Lazy.force (Lazy.force lccl).ccl_public_cpnts) ->
      let ccl = Lazy.force lccl in
      begin match
        Lazy.force (List.assoc s (Lazy.force ccl.ccl_public_cpnts))
      with
        | Variable var -> var
        | _ -> failwith "find_connector: Bad reference"
      end
  | ClassContext _, [(s, _)] ->
      failwith ("find_connector: unknown identifier: " ^ s)
  | ClassContext (_, lccl),
    (s, array_subscr) :: ([(s', array_subscr')] as conn_ref')
    when List.mem_assoc s (Lazy.force (Lazy.force lccl).ccl_public_cpnts) ->
      let ccl = Lazy.force lccl in
      begin match
        Lazy.force (List.assoc s (Lazy.force ccl.ccl_public_cpnts))
      with
        | Variable (CompoundVariable (lccl', _)) ->
            let ctx' = ClassContext (ctx, lccl') in
            find_connector ctx' conn_ref'
        | _ -> failwith "find_connector: Bad reference"
      end
  | ClassContext _, (s, _) :: _ ->
      failwith ("find_connector: unknown identifier: " ^ s)
  | ClassContext (_, lccl), [] -> failwith "find_connector: empty reference"
  | ForContext (_, id), _ ->
      failwith
        ("find_connector: connector defined in a for loop on variable " ^ id)
  | _ -> assert false

and compile_connection ctx conn_ref conn_ref' lccl attrs lccl' attrs' =
  let create_connection (s, lcpnt) = match Lazy.force lcpnt with
    | Variable (RealVariable { vat_nature = Flow }) ->
        let cref = compile_component_reference ctx (conn_ref @ [(s, [||])])
        and cref' = compile_component_reference ctx (conn_ref' @ [(s, [||])])
        in CompiledFlowConnection (Reference cref, Reference cref')
    | Variable (RealVariable { vat_nature = Potential }) ->
        let cref = compile_component_reference ctx (conn_ref @ [(s, [||])])
        and cref' = compile_component_reference ctx (conn_ref' @ [(s, [||])])
        in CompiledEquality (Reference cref, Reference cref')
    | _ -> failwith "compile_connection: Invalid connector"
  in List.map create_connection (Lazy.force (Lazy.force lccl).ccl_public_cpnts)
  (*FIXME: check class equality *)

and compile_if_clause ctx = function
  | expr, equs ->
      compile_expression ctx expr,
      List.fold_left
        (fun equs equ -> compile_equation ctx equ @ equs)
        []
        equs

and compile_when_clause ctx = function
  | expr, when_equs ->
      compile_expression ctx expr,
      List.map (compile_when_equation ctx) when_equs

and compile_when_equation ctx = function
  | ParseTree.FunctionCallE
    ([("reinit", [||])],
    Some (ParseTree.ArgList ([ParseTree.Reference cpnt_ref; expr] , None))) ->
      Reinit (
        compile_component_reference ctx cpnt_ref,
        compile_expression ctx expr)
  | ParseTree.Equality (ParseTree.Reference cpnt_ref, expr) ->
      Assign (
        compile_component_reference ctx cpnt_ref,
        compile_expression ctx expr)
  | _ -> failwith "compile_when_equation: Invalid equation"

and compile_for_clause ctx for_indices equs = match for_indices with
  | [] ->
      List.fold_left
        (fun equs equ -> compile_equation ctx equ @ equs)
        []
        equs
  | ( s, Some (ParseTree.Range (expr, expr', None))) :: for_indices' ->
      let cexpr = compile_expression ctx expr
      and cexpr' = compile_expression ctx expr'
      and ctx' = ForContext (ctx, s) in
      let cfor = compile_for_clause ctx' for_indices' equs
      in [CompiledFor (cexpr, cexpr', Integer (Int32.one), cfor)]
  | ( s, Some (ParseTree.Range (expr, expr', Some expr''))) :: for_indices' ->
      let cexpr = compile_expression ctx expr
      and cexpr' = compile_expression ctx expr'
      and cexpr'' = compile_expression ctx expr''
      and ctx' = ForContext (ctx, s) in
      let cfor = compile_for_clause ctx' for_indices' equs
      in [CompiledFor (cexpr, cexpr', cexpr'', cfor)]
  | _ -> failwith "compile_for_clause: Invalid range"

and compile_expression ctx expr =
  let rec compile_expression' = function
    | ParseTree.Addition (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Addition (cexpr, cexpr')
    | ParseTree.And (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        And (cexpr, cexpr')
    | ParseTree.Division (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Division (cexpr, cexpr')
    | ParseTree.End ->
        failwith "compile_expression: end keyword not allowed"
    | ParseTree.Equals (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Equals (cexpr, cexpr')
    | ParseTree.ExpressionList [|expr|] ->
        compile_expression' expr
    | ParseTree.ExpressionList exprs ->
        failwith "compile_expression: expression lists not allowed"
    | ParseTree.False -> Boolean false
    | ParseTree.FunctionCall
      ([("noEvent", [||])], Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        NoEvent cexpr
    | ParseTree.FunctionCall
      ([("der", [||])], Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Der cexpr
    | ParseTree.FunctionCall
      ([("floor", [||])], Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Floor cexpr
    | ParseTree.FunctionCall
      ([("max", [||])],
      Some (ParseTree.ArgList ([expr; expr'] , None))) ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Max (cexpr, cexpr')
    | ParseTree.FunctionCall
      ([("min", [||])],
      Some (ParseTree.ArgList ([expr; expr'] , None))) ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Min (cexpr, cexpr')
    | ParseTree.FunctionCall
      ([("mod", [||])],
      Some (ParseTree.ArgList ([expr; expr'] , None))) ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Mod (cexpr, cexpr')
    | ParseTree.FunctionCall
      ([("cardinality", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Cardinality cexpr
    | ParseTree.FunctionCall
      ([("abs", [||])], Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Abs cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("cos", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Cos cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("exp", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Exp cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("log", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Log cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("sin", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Sin cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("sqrt", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Sqrt cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("tan", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Tan cexpr
    | ParseTree.FunctionCall
      ([("Modelica", [||]); ("Math", [||]); ("tanh", [||])],
      Some (ParseTree.ArgList ([expr] , None))) ->
        let cexpr = compile_expression' expr in
        Tanh cexpr
    | ParseTree.FunctionCall (path, Some (ParseTree.ArgList (exprs , None))) ->
        let cexprs = List.map compile_expression' exprs
        and name, lccl = get_function_from path
        in ExternalFunctionCall (name, lccl, cexprs)
    | ParseTree.FunctionCall _ ->
        failwith "compile_expression: invalid function call"
    | ParseTree.GreaterEqualThan (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        GreaterEqualThan (cexpr, cexpr')
    | ParseTree.GreaterThan (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        GreaterThan (cexpr, cexpr')
    | ParseTree.If (assocs, expr) ->
        compile_if assocs expr
    | ParseTree.Integer s ->Integer (Int32.of_string s)
    | ParseTree.LessEqualThan (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        GreaterEqualThan (cexpr', cexpr)
    | ParseTree.LessThan (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        GreaterThan (cexpr', cexpr)
    | ParseTree.ArrayConcatenation exprss ->
        failwith "compile_expression: array concatenation not allowed"
    | ParseTree.Minus expr -> Minus (compile_expression' expr)
    | ParseTree.Multiplication (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Multiplication (cexpr, cexpr')
    | ParseTree.Not expr -> Not (compile_expression' expr)
    | ParseTree.NotEquals (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        NotEquals (cexpr, cexpr')
    | ParseTree.Or (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Or (cexpr, cexpr')
    | ParseTree.Plus expr -> compile_expression' expr
    | ParseTree.Power (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Power (cexpr, cexpr')
    | ParseTree.Range (expr, expr', expr_opt) ->
        failwith "compile_expression: ranges not allowed"
    | ParseTree.Real s -> Real (float_of_string s)
    | ParseTree.Reference [("time", [||])] -> Time
    | ParseTree.Reference cpnt_ref ->
        Reference (compile_component_reference ctx cpnt_ref)
    | ParseTree.String s -> String s
    | ParseTree.Subtraction (expr, expr') ->
        let cexpr = compile_expression' expr
        and cexpr' = compile_expression' expr' in
        Subtraction (cexpr, cexpr')
    | ParseTree.True -> Boolean true
    | ParseTree.VectorOrRecord (ParseTree.ArgList (exprs, None)) ->
        Vector (Array.of_list (List.map compile_expression' exprs))
    | ParseTree.VectorOrRecord (ParseTree.ArgList (_, Some _)) ->
        failwith "compile_expression: for expressions not allowed"
    | ParseTree.VectorOrRecord (ParseTree.NamedArgList _) ->
        failwith "compile_expression: records not allowed"
  and get_function_from path =
    let name =
      List.map
        (function
          | (s, [||]) -> s
          | _ -> failwith "compile_expression: invalid function reference")
        path
    in
    let f = create_filename name ".moc" in
    let cu = read_class_file f in
    match cu with
      | CompiledFunction lccl -> name, lccl
      | CompiledClass _ ->
          failwith "compile_expression: invalid function reference"
  and compile_if couples expr =
    let cexpr = compile_expression' expr in
    let ccouples =
      List.fold_right
        (fun couple ccouples ->
          let cexpr, cexpr' = compile_couple couple in
          (cexpr, cexpr') :: ccouples)
        couples
        []
    in
    If (ccouples, cexpr)
  and compile_couple (expr, expr') =
    let cexpr = compile_expression' expr
    and cexpr' = compile_expression' expr' in
    cexpr, cexpr'
  in compile_expression' expr

and compile_component_reference ctx cpnt_ref =
  let rec compile_reference' ctx' level level' cpnt_ref = match ctx' with
    | TopLevelContext -> raise Not_found
    | ClassContext (ctx'', lccl) ->
        compile_component_path ctx ctx' level cpnt_ref
    | ModificationContext (ctx'', lccl) ->
        begin try
          compile_reference' ctx'' (level + 1) level' cpnt_ref
        with
          | Not_found ->
              compile_component_path ctx ctx' level cpnt_ref
        end
    | ForContext (ctx'', id) ->
        compile_loop_variable ctx'' level level' id cpnt_ref
  and compile_loop_variable ctx'' level level' id = function
    | [(id', [||])] when id' = id -> LoopVariableReference level'
    | _ -> compile_reference' ctx'' level (level' + 1) cpnt_ref
  in compile_reference' ctx 0 0 cpnt_ref

and compile_component_path ctx ctx' level cpnt_ref =
  let rec compile_component_path_in lccl = function
    | [] -> failwith "compile_component_path: Invalid path"
    | (id, array_subscrs) :: cpnt_ref ->
        let cpnt = find_local_component_in lccl id in
        let cs = compile_subscripts ctx array_subscrs in
        (*FIXME: range case *)
        let path = (id, cs) :: compile_path_rest cpnt_ref in
        begin match cpnt with
          | Parameter _ -> ParameterReference (level, path)
          | Variable _ -> VariableReference (level, path)
        end
  and compile_path_rest = function
    | [] -> []
    | (id, array_subscrs) :: cpnt_ref ->
        let cs = compile_subscripts ctx array_subscrs in
        (*FIXME: range case *)
        (id, cs) :: compile_path_rest cpnt_ref
  in match ctx' with
    | ClassContext (_, lccl) | ModificationContext (_, lccl) ->
        compile_component_path_in lccl cpnt_ref
    | TopLevelContext | ForContext _ ->
        assert false (* Never applied to this kind of context *)

and compile_subscripts ctx subscripts =
  let rec compile_subscript = function
    | ParseTree.All ->
        failwith "compile_subscripts: ranges not allowed"
    | ParseTree.Subscript expr ->
        Definite (compile_expression ctx expr)
  in Array.map compile_subscript subscripts

and compile_modifications ctx modifs =
  List.rev_map (compile_modification ctx) modifs

and compile_modification ctx modif =
  try compile_component_modification ctx modif with
    | Not_found ->
        let s = match modif with
          | Modification (cpnt_ref, _, _) ->
              List.fold_left (fun acc (id, _) -> acc ^ "." ^ id) "" cpnt_ref
        in failwith ("compile_modification: component not found: " ^ s)

and find_local_component_in lccl id =
  let public_cpnts = Lazy.force (Lazy.force lccl).ccl_public_cpnts in
  try Lazy.force (List.assoc id public_cpnts) with
    | Not_found -> failwith ("find_local_component_in: " ^ id ^ " not found")

and find_local_component_class_in lccl id =
  match find_local_component_in lccl id with
    | Variable (CompoundVariable (lccl, _)) -> lccl
    | _ -> failwith "find_local_component_class_in: base type"

and is_base_type_parameter_of lccl id =
  match find_local_component_in lccl id with
    | Parameter (IntegerParameter _ | RealParameter _) -> true
    | _ -> false

and is_base_type_variable_of lccl id =
  match find_local_component_in lccl id with
    | Variable (DiscreteVariable _ | RealVariable _) -> true
    | _ -> false

and compile_component_modification ctx modif = match ctx, modif with
  | ModificationContext (_, lccl),
    Modification ([(id, subscrs)], [], Some expr)
    when is_base_type_parameter_of lccl id ->
      let cs = compile_subscripts ctx subscrs
      and cexpr = compile_expression ctx expr in
      CompiledModification ((id, cs), [], Some cexpr)
  | ModificationContext (_, lccl),
    Modification ((id, _) :: _, _, _)
    when is_base_type_parameter_of lccl id ->
      failwith "compile_component_modification: invalid parameter modification"
  | ModificationContext (_, lccl),
    Modification ([(id, subscrs)], modifs, None)
    when is_base_type_variable_of lccl id ->
      let cs = compile_subscripts ctx subscrs
      and cmodifs = compile_base_type_variable_modifications ctx modifs in
      CompiledModification ((id, cs), cmodifs, None)
  | ModificationContext (_, lccl),
    Modification ([(id, _)], _, _)
    when is_base_type_variable_of lccl id ->
      failwith "compile_component_modification: invalid variable modification"
  | ModificationContext (_, lccl),
    Modification ([_], _, _) ->
      failwith "compile_component_modification: invalid component modification"
  | ModificationContext (_, lccl),
    Modification ((id, subscrs) :: cpnt_ref, modifs, expr_opt) ->
      let cs = compile_subscripts ctx subscrs
      and cexpr_opt = opt_map (compile_expression ctx) expr_opt
      and ctx' =
        ModificationContext (ctx, find_local_component_class_in lccl id)
      and modif = Modification (cpnt_ref, modifs, None) in
      let cmodif = compile_modification ctx' modif in
      CompiledModification ((id, cs), [cmodif], cexpr_opt)
  | (TopLevelContext | ClassContext _), _ ->
      assert false (* Never applied to this kind of context *)
  | _ -> raise InvalidModification

and compile_base_type_variable_modifications ctx = function
  | [] -> []
  | Modification ([("start", [||])], [], Some expr) :: modifs ->
      let cexpr = compile_expression ctx expr in
      CompiledModification (("start", [||]), [], Some cexpr) ::
      compile_base_type_variable_modifications ctx modifs
  | _ ->
      failwith "compile_base_type_variable_modifications: invalid modification"

and compile_component ctx pcpnt = match pcpnt.variability with
  | Some ParseTree.Parameter -> Parameter (compile_parameter ctx pcpnt)
  | Some ParseTree.Discrete | None -> Variable (compile_variable ctx pcpnt)
  | _ -> failwith "compile_component: only variables and parameters allowed"

and compile_parameter ctx pcpnt =
  let attrs =
    {
      pat_dimensions = compile_subscripts ctx pcpnt.subscripts;
      pat_comment = compile_comment pcpnt.comment;
      pat_value =
        opt_map (compile_expression ctx) (snd pcpnt.modification)
    }
  in match pcpnt.class_name with
    | ["Integer"] -> IntegerParameter attrs
    | ["Real"] -> RealParameter attrs
    | _ -> failwith "compile_parameter: only base types allowed"

and compile_variable ctx pcpnt =
  let cmodifs = match pcpnt.class_name, snd pcpnt.modification with
    | ["Integer"], None | ["Real"], None ->
        compile_base_type_variable_modifications ctx (fst pcpnt.modification)
    | name, None ->
        let f = create_filename name ".moc" in
        let cu = read_class_file f in
        begin match cu with
          | CompiledClass lccl ->
              let ctx' = ModificationContext (ctx, lccl) in
              compile_modifications ctx' (fst pcpnt.modification)
          | CompiledFunction _ ->
              failwith "compile_variable: functions not allowed as variables"
        end
    | _, Some _ ->
        failwith
          "compile_variable: equations not allowed in variable declarations"
  in
  let attrs =
    {
      vat_dimensions = compile_subscripts ctx pcpnt.subscripts;
      vat_nature = compile_nature pcpnt.flow;
      vat_inout = compile_inout pcpnt.inout;
      vat_comment = compile_comment pcpnt.comment;
      vat_modifications = cmodifs
    }
  in match pcpnt.class_name, pcpnt.variability with
    | ["Real"], Some ParseTree.Discrete -> DiscreteVariable attrs
    | ["Real"], None -> RealVariable attrs
    | name, None ->
        let f = create_filename name ".moc" in
        let cu = read_class_file f in
        begin match cu with
          | CompiledClass lccl -> CompoundVariable (lccl, attrs)
          | CompiledFunction _ ->
              failwith "compile_variable: functions not allowed as variables"
        end
    | _ -> failwith "compile_variable: invalid variable declaration"

and compile_nature = function
  | Some ParseTree.Flow -> Flow
  | None -> Potential

and compile_inout = function
  | Some ParseTree.Input -> Input
  | Some ParseTree.Output -> Output
  | None -> Both

and compile_comment = function
  | ParseTree.Comment (ParseTree.StringComment ss, None) ->
      List.fold_left ( ^ ) "" ss
  | ParseTree.Comment (_, Some _) ->
      failwith "compile_comment: Annotations not allowed"
