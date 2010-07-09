(*
 *  Translator from Modelica 2.x to flat Modelica
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

(** The main functions are:
{ul
{- [ generate_code ]: main function
  {ul
  {- [ collect_function_definitions ]: Collect function definitions }
  {- [ generate_component_description ]: Generate component descriptions
    {ul
    {- [ collect_component_elements ]: Returns a [ flat_instance ] containing all variables and equations
      {ul
        {- [ expand_equations ]: Generation of connect equations }
        {- [ introduce_derivative_variables ]: Introduce derivative variables}
      }
    }
    {- [ generate_dynamic_description ]: Generate the dynamic Model description
      {ul
        {- [ generate_flatten_instance ]: Dynamic model description as flat Modelica }
        {- [ generate_flatten_XML ]: if the "-xml" option is activated }
      }
    }
    {- [ generate_function_definitions ]: Generate function definitions in a file named filename_functions.mo }
    {- [ generate_initial_description ]: Generate initialization description in a file named filename_init.xml,
      and abstract relations in a file named filename_relations.xml.
      {ul
        {- [ generate_flatten_XML ]: Generates an XML description of initialization problem }
        {- [ generate_relations ]: Generates an XML description of abstract relations and other information }
      }
    }
    }
  }
  }
}
}*)

open ErrorDico (* To have access to GenericError *)

type flat_instance =
  {
    variables: Instantiation.component_description list;
    dynamic_equations: Instantiation.equation_desc list;
    initial_equations: Instantiation.equation_desc list;
    abstract_relations: abstract_relation list
  }

and function_description =
  {
    inputs: (string * Types.class_specifier) list;
    outputs: (string * Types.class_specifier) list
  }

and abstract_relation =
  | Rel of Instantiation.component_description list

and 'a tree = Leaf of (string * 'a) | Node of string * 'a tree list

and element =
  {
    kind: element_kind;
    id: string;
    comment: string;
    initial_value: Instantiation.expression option;
    nominal_value: Instantiation.expression option; 
    output: bool;
    fixed: bool option
  }

and element_kind =
  | Input
  | Parameter
  | Variable
  | DiscreteVariable

type stats =
  {
    nb_ipars: int;
    nb_rpars: int;
    nb_spars: int;
    nb_dvars: int;
    nb_cvars: int;
    nb_inps: int;
    nb_outps: int
  }


(* Utilities *)

let evaluate t = Lazy.force t

(* Remove enclosing parenthesis *)
let unbraced s =
  let n = String.length s in
  try
    match s.[0], s.[n - 1] with
    | '(', ')' -> String.sub s 1 (n - 2)
    | _ -> s
  with
  | _ -> s

(* function used to hide XML special characters *)
let hide_spc s =
  let encoded_s = ref "" in
  let hide_special_character c = match c with
    | '<' -> encoded_s := !encoded_s ^ "&lt;"
    | '>' -> encoded_s := !encoded_s ^ "&gt;"
    | '&' -> encoded_s := !encoded_s ^ "&amp;"
    | '\'' -> encoded_s := !encoded_s ^ "&apos;"
    | '\"' -> encoded_s := !encoded_s ^ "&quot;"
    | _ -> encoded_s := !encoded_s ^ (String.make 1 c) in
  String.iter hide_special_character s;
  !encoded_s


(* Code generation functions *)

let rec generate_code xml init filename inst_defs =
  let fun_defs =
    List.fold_left collect_function_definitions [] inst_defs in
  let add_instance_element acc (id, elt_desc) =
    match evaluate elt_desc.Instantiation.element_nature with
    | Instantiation.Class _ -> acc
    | Instantiation.Component cpnt_desc -> (id, cpnt_desc) :: acc in
  match List.fold_left add_instance_element [] inst_defs with
  | [] -> ()
  | [ id, cpnt_desc ] ->
      generate_component_description xml init filename fun_defs id cpnt_desc
  | _ -> assert false

and collect_function_definitions fun_defs (id, elt_desc) =
  match evaluate elt_desc.Instantiation.element_nature with
  | Instantiation.Class cl_def ->
      let ctx =
        {
          path = cl_def.Instantiation.class_path;
          location = cl_def.Instantiation.class_location;
          instance_nature = Instantiation.ClassElement
        } in
      let cl_spec = cl_def.Instantiation.class_type in
      fun_defs @
      collect_function_definitions_in_class_specifier ctx cl_spec
  | Instantiation.Component cpnt_desc ->
      let ctx =
        {
          path = cpnt_desc.Instantiation.component_path;
          location = cpnt_desc.Instantiation.component_location;
          instance_nature =
            Instantiation.ComponentElement cpnt_desc.Instantiation.class_name
        } in
      fun_defs @
      collect_function_definitions_in_component ctx cpnt_desc

and collect_function_definitions_in_class_specifier ctx = function
  | Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
    Types.TupleType _ -> []
  | Types.ClassType cl_type ->
      collect_function_definition_in_class_type ctx cl_type

and collect_function_definition_in_class_type ctx cl_type =
  let fun_defs = collect_inner_functions ctx cl_type in
  collect_function ctx fun_defs cl_type

and collect_inner_functions ctx cl_type =
  match evaluate cl_type.Types.kind with
    | Types.Class | Types.Model | Types.Block | Types.Package |
      Types.Function ->
        let named_elts = cl_type.Types.named_elements in
        List.fold_left
          (collect_function_definitions_in_type ctx)
          []
          named_elts
    | Types.Record | Types.ExpandableConnector | Types.Connector -> []

and collect_function_definitions_in_type ctx fun_defs (id, elt_type) =
  let elt_type' = evaluate elt_type in
  match elt_type'.Types.dynamic_scope, elt_type'.Types.element_nature with
    | None, Types.ClassElement cl_spec ->
        let ctx' =
          {
            ctx with
            path = ctx.path @ [Instantiation.Name id]
          } in
        let cl_spec' = evaluate cl_spec in
        let fun_defs' =
          collect_function_definitions_in_class_specifier ctx' cl_spec' in
        fun_defs' @ fun_defs
    | Some _, _ |
      None,
      (Types.ComponentElement _ | Types.ComponentTypeElement _ |
       Types.PredefinedTypeElement _) -> fun_defs

and collect_function ctx fun_defs cl_type =
  match evaluate cl_type.Types.kind with
    | Types.Function ->
        let named_elts = cl_type.Types.named_elements in
        function_description_of_named_elements ctx named_elts :: fun_defs
    | Types.Class | Types.Model | Types.Block | Types.Record |
      Types.ExpandableConnector | Types.Connector| Types.Package -> fun_defs

and function_description_of_named_elements ctx named_elts =
  let collect_input_or_output (id, elt_type) fun_desc =
    let collect_input_or_output' cpnt_type =
      match evaluate cpnt_type.Types.causality with
        | Types.Input ->
            let cl_spec = evaluate cpnt_type.Types.base_class in
            { fun_desc with inputs = (id, cl_spec) :: fun_desc.inputs }
        | Types.Output ->
            let cl_spec = evaluate cpnt_type.Types.base_class in
            { fun_desc with outputs = (id, cl_spec) :: fun_desc.outputs }
        | Types.Acausal -> fun_desc in
    let elt_type' = evaluate elt_type in
    match elt_type'.Types.element_nature with
      | Types.ComponentElement cpnt_type ->
          collect_input_or_output' cpnt_type
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ -> fun_desc in
  let fun_desc = { inputs = []; outputs = [] } in
  ctx,
  List.fold_right collect_input_or_output named_elts fun_desc

and collect_function_definitions_in_component ctx cpnt_desc =
  match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.DynamicArray cpnt_desc ->
        let ctx' =
          { ctx with path = ctx.path @ [Instantiation.Index 0] } in
        collect_function_definitions_in_component ctx' cpnt_desc
    | Instantiation.Instance inst ->
        let elts = evaluate inst.Instantiation.elements in
        let named_elts = elts.Instantiation.named_elements in
        List.fold_left collect_function_definitions [] named_elts
    | Instantiation.PredefinedTypeInstance _ -> []
    | Instantiation.StaticArray [||] -> []
    | Instantiation.StaticArray cpnt_descs ->
        let ctx' =
          { ctx with path = ctx.path @ [Instantiation.Index 0] } in
        collect_function_definitions_in_component ctx' cpnt_descs.(0)

and generate_function_definition oc acc (ctx, fun_desc) =
  let ext_name = Printf.sprintf "%s" (last_id ctx.path)
  and name = string_of_path ctx.path in
  match List.mem ext_name acc with
  | true -> acc
  | false ->
      Printf.fprintf oc "function %s \"%s\"\n" ext_name name;
      List.iter (generate_function_inout ctx oc "input") fun_desc.inputs;
      List.iter (generate_function_inout ctx oc "output") fun_desc.outputs;
      Printf.fprintf oc "external;\nend %s;\n" ext_name;
      ext_name :: acc

and generate_function_inout ctx oc inout (id, cl_spec) =
  let generate_dimensions ndims =
    let rec generate_dimensions' ndims =
      match ndims with
      | 0 -> assert false
      | 1 -> Printf.fprintf oc ":"
      | _ -> Printf.fprintf oc ":, "; generate_dimensions' (ndims - 1) in
    match ndims with
    | 0 -> ()
    | _ ->
      Printf.fprintf oc "[";
      generate_dimensions' ndims;
      Printf.fprintf oc "]" in
  let rec generate_function_inout' cl_spec ndims = match cl_spec with
    | Types.PredefinedType { Types.base_type = Types.RealType } ->
        Printf.fprintf oc "\t%s Real" inout;
        generate_dimensions ndims;
        Printf.fprintf oc " %s;\n" id
    | Types.PredefinedType { Types.base_type = Types.IntegerType }
      when ndims = 0 && inout = "input" ->
        Printf.fprintf oc "\t%s Integer %s;\n" inout id
    | Types.PredefinedType { Types.base_type = Types.StringType }
      when ndims = 0 && inout = "input" ->
        Printf.fprintf oc "\t%s String %s;\n" inout id
    | Types.ArrayType (_, cl_spec) when inout = "input" ->
        generate_function_inout' cl_spec (ndims + 1)
    | _ ->
        raise (GenericError
          { err_msg =
              ["_NotYetImplemented"; "_NonSupportedTypeOfFuncInOut"; id];
            err_info = [];
            err_ctx = ctx }) in
  generate_function_inout' cl_spec 0

and generate_component_description xml init filename fun_defs id cpnt_desc =
  let chop_extension s = try Filename.chop_extension s with _ -> s in
  let filename = match filename with
    | None -> id
    | Some s -> chop_extension s in
  let ctx =
      {
        path = cpnt_desc.Instantiation.component_path;
        location = cpnt_desc.Instantiation.component_location;
        instance_nature =
          Instantiation.ComponentElement cpnt_desc.Instantiation.class_name
      }
  and flat_inst = collect_component_elements cpnt_desc in
  generate_dynamic_description ctx xml filename fun_defs id flat_inst;
  generate_function_definitions ctx filename fun_defs;
  if init then
    generate_initial_description ctx filename fun_defs id flat_inst

and generate_dynamic_description ctx xml filename fun_defs id flat_inst =
  let vars = flat_inst.variables
  and equs = flat_inst.dynamic_equations
  and ext = if xml then ".xml" else ".mo" in
  let oc = open_out (filename ^ ext) in
  try
    if xml then
      generate_flatten_XML ctx fun_defs oc id vars equs
    else
    generate_flatten_instance ctx fun_defs oc id vars equs;
    close_out oc;
  with exn -> close_out oc; raise exn

and generate_initial_description ctx filename fun_defs id flat_inst =
  let oc = open_out (filename ^ "_init.xml") in
  try
    let varss = List.map (function Rel r -> r) flat_inst.abstract_relations in
    let vars =
      List.fold_left add_component flat_inst.variables (List.flatten varss)
    and equs = flat_inst.initial_equations in
    generate_flatten_XML ctx fun_defs oc id vars equs;
    generate_relations filename vars flat_inst.abstract_relations;
    close_out oc;
  with exn -> close_out oc; raise exn

and generate_function_definitions ctx filename fun_defs =
  let generate_function_definitions' oc =
    try
      let _ = List.fold_left (generate_function_definition oc) [] fun_defs in
      close_out oc
    with exn -> close_out oc; raise exn in
  match fun_defs with
  | [] -> ()
  | _ ->
    let oc = open_out (filename ^ "_functions.mo") in
    generate_function_definitions' oc

and collect_component_elements cpnt_desc =
  let ctx =
    {path = cpnt_desc.Instantiation.component_path;
     location = cpnt_desc.Instantiation.component_location;
     instance_nature =
       Instantiation.ComponentElement cpnt_desc.Instantiation.class_name} in
  match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx })
    | Instantiation.Instance inst -> collect_instance_elements ctx inst
    | Instantiation.PredefinedTypeInstance _
      when is_fixed_parameter cpnt_desc ->
        {
          variables = [];
          dynamic_equations = [];
          initial_equations = [];
          abstract_relations = []
        }
    | Instantiation.PredefinedTypeInstance _ ->
			  let decl_equs = collect_declaration_equation cpnt_desc in
        {
          variables = [ cpnt_desc ];
          dynamic_equations = decl_equs;
          initial_equations = decl_equs;
          abstract_relations = []
        }
    | Instantiation.StaticArray cpnt_descs ->
        collect_array_elements cpnt_descs

and collect_instance_elements ctx inst =
  let elts = evaluate inst.Instantiation.elements in
  let named_elts = elts.Instantiation.named_elements
  and unnamed_elts = elts.Instantiation.unnamed_elements in
  let flat_inst = collect_instance_named_elements named_elts
  and dyn_equs, init_equs =
    List.fold_left (collect_equations ctx) ([], []) unnamed_elts in
  let init_equs, rels =
    List.fold_left
      (introduce_derivative_variables ctx)
      ([], [])
      (dyn_equs @ init_equs) in
  { flat_inst with
    dynamic_equations = flat_inst.dynamic_equations @ dyn_equs;
    initial_equations = flat_inst.initial_equations @ init_equs;
    abstract_relations = flat_inst.abstract_relations @ rels
  }

and collect_instance_named_elements named_elts =
  let collect_instance_named_elements' flat_inst (_, elt_desc) =
    let elt_nat = evaluate elt_desc.Instantiation.element_nature in
    match elt_nat with
      | Instantiation.Class _ -> flat_inst
      | Instantiation.Component cpnt_desc ->
          let flat_inst' = collect_component_elements cpnt_desc in
          {
            variables = flat_inst.variables @ flat_inst'.variables;
            dynamic_equations =
              flat_inst.dynamic_equations @ flat_inst'.dynamic_equations;
            initial_equations =
              flat_inst.initial_equations @ flat_inst'.initial_equations;
            abstract_relations =
              flat_inst.abstract_relations @ flat_inst'.abstract_relations
          } in
  let flat_inst =
    {
      variables = [];
      dynamic_equations = [];
      initial_equations = [];
      abstract_relations = []
    } in
  List.fold_left collect_instance_named_elements' flat_inst named_elts

and introduce_derivative_variables ctx (init_equs, rels) equ =
  let rec introduce_derivative_variables' expr = match expr with
    | Instantiation.BinaryOperation (oper_kind, expr1, expr2) ->
        let expr1, cpnt_descs1 = introduce_derivative_variables' expr1
        and expr2, cpnt_descs2 = introduce_derivative_variables' expr2 in
        Instantiation.BinaryOperation (oper_kind, expr1, expr2),
        cpnt_descs1 @ cpnt_descs2
    | Instantiation.FunctionCall
        (Instantiation.PredefinedIdentifier "der",
         [ Instantiation.ComponentReference cpnt_desc ]) ->
        let cpnt_desc' = component_derivative cpnt_desc in
        Instantiation.ComponentReference cpnt_desc',
        [ cpnt_desc' ]
    | Instantiation.FunctionCall
        (Instantiation.PredefinedIdentifier "der", _) ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_UnsupportedDerOperArg"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.If (alts, default) ->
        let f (cond, expr) =
          let cond, cpnt_descs1 = introduce_derivative_variables' cond
          and expr, cpnt_descs2 = introduce_derivative_variables' expr in
          (cond, expr), cpnt_descs1 @ cpnt_descs2 in
        let alts, cpnt_descss = List.split (List.map f alts) in
        let default, cpnt_descs' = introduce_derivative_variables' default in
        Instantiation.If (alts, default),
        (List.flatten cpnt_descss) @ cpnt_descs'
    | Instantiation.IndexedAccess (expr, exprs) ->
        let expr, cpnt_descs = introduce_derivative_variables' expr in
        Instantiation.IndexedAccess (expr, exprs),
        cpnt_descs
    | Instantiation.NoEvent expr ->
        let expr, cpnt_descs = introduce_derivative_variables' expr in
        Instantiation.NoEvent expr,
        cpnt_descs
    | Instantiation.UnaryOperation (oper_kind, expr) ->
        let expr, cpnt_descs = introduce_derivative_variables' expr in
        Instantiation.UnaryOperation (oper_kind, expr),
        cpnt_descs
    | Instantiation.VectorReduction (exprs, expr) ->
        let expr, cpnt_descs = introduce_derivative_variables' expr in
        Instantiation.VectorReduction (exprs, expr),
        cpnt_descs
    | Instantiation.Record record_elts ->
        let f (id, expr) =
          let expr, cpnt_descs = introduce_derivative_variables' expr in
          (id, expr), cpnt_descs in
        let record_elts, cpnt_descs = List.split (List.map f record_elts) in
        Instantiation.Record record_elts,
        List.flatten cpnt_descs
    | Instantiation.Tuple exprs ->
        let exprs' = List.map introduce_derivative_variables' exprs in
        let exprs', cpnt_descs' = List.split exprs' in
        Instantiation.Tuple exprs',
        List.flatten cpnt_descs'
    | Instantiation.Vector exprs ->
        let exprs' = Array.map introduce_derivative_variables' exprs in
        let exprs', cpnt_descs' = List.split (Array.to_list exprs') in
        Instantiation.Vector (Array.of_list exprs'),
        List.flatten cpnt_descs'
    | Instantiation.FunctionCall (expr, exprs) ->
        let exprs' = List.map introduce_derivative_variables' exprs in
        let exprs', cpnt_descs' = List.split exprs' in
        Instantiation.FunctionCall (expr, exprs'),
        List.flatten cpnt_descs'
    | Instantiation.ComponentReference cpnt_desc -> expr, [ cpnt_desc ]
    | _ -> expr, [] in
  match equ with
  | Instantiation.Equal (expr1, expr2) ->
      let expr1, cpnt_descs1 = introduce_derivative_variables' expr1
      and expr2, cpnt_descs2 = introduce_derivative_variables' expr2 in
      let cpnt_descs =
        List.fold_left add_component [] (cpnt_descs1 @ cpnt_descs2) in
      (Instantiation.Equal (expr1, expr2)) :: init_equs,
      (Rel cpnt_descs) :: rels
  | Instantiation.ConnectFlows _ | Instantiation.ConditionalEquationE _ |
    Instantiation.WhenClauseE _ -> init_equs, rels

and collect_equations ctx (dyn_equs, init_equs) unnamed_elt =
  match unnamed_elt with
  | Instantiation.EquationClause (NameResolve.Permanent, equs) ->
      dyn_equs @ (expand_equations ctx (evaluate equs)), init_equs
  | Instantiation.EquationClause (NameResolve.Initial, equs) ->
      dyn_equs, init_equs @ (expand_equations ctx (evaluate equs))
  | Instantiation.AlgorithmClause _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_AlgorithmClause"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and expand_equations ctx equs =
  let expand_equation equ = equ.Instantiation.nature in
  let add_connection (expr, sign) (expr', sign') cnect_sets =
    let contains_at_least_one_node_to_connect cnect_set =
      List.mem_assoc expr cnect_set || List.mem_assoc expr' cnect_set in
    let cnect_sets, cnect_sets' =
      List.partition contains_at_least_one_node_to_connect cnect_sets in
    match cnect_sets with
      | [] -> [(expr, sign); (expr', sign')] :: cnect_sets'
      | [cnect_set; cnect_set'] -> (cnect_set @ cnect_set') :: cnect_sets'
      | [cnect_set] when List.mem_assoc expr cnect_set ->
          ((expr', sign') :: cnect_set) :: cnect_sets'
      | [cnect_set] -> ((expr, sign) :: cnect_set) :: cnect_sets'
      | _ :: _ :: _ :: _ -> assert false in
  let expand_connection cnect_sets = function
    | Instantiation.ConnectFlows (sign, expr, sign', expr') ->
        add_connection (expr, sign) (expr', sign') cnect_sets
    | Instantiation.Equal _ | Instantiation.ConditionalEquationE _ |
      Instantiation.WhenClauseE _ -> cnect_sets in
  let generate_flow_equation cnect_set =
    let to_expression (expr, sign) = match sign with
      | NameResolve.Positive -> expr
      | NameResolve.Negative ->
          Instantiation.UnaryOperation (Instantiation.UnaryMinus, expr) in
    let add_expressions expr expr' =
      Instantiation.BinaryOperation (Instantiation.Plus, expr, expr') in
    let exprs = List.map to_expression cnect_set in
    let lhs = List.fold_left add_expressions (Instantiation.Real 0.) exprs in
    Instantiation.Equal (lhs, Instantiation.Real 0.) in
  let collect_equation equs equ = match equ with
    | Instantiation.ConnectFlows _ -> equs
    | Instantiation.Equal _ | Instantiation.ConditionalEquationE _ |
      Instantiation.WhenClauseE _ -> equ :: equs in
  let equ_descs = List.flatten (List.map expand_equation equs) in
  let cnect_sets = List.fold_left expand_connection [] equ_descs
  and equs = List.fold_left collect_equation [] equ_descs in
  let equs' = List.map generate_flow_equation cnect_sets in
  equs @ equs'

and collect_array_elements cpnt_descs =
  let rec collect_array_elements' flat_inst i =
    if i = Array.length cpnt_descs then flat_inst
    else
      let flat_inst' = collect_component_elements cpnt_descs.(i) in
      let flat_inst =
        {
          variables = flat_inst.variables @ flat_inst'.variables;
          dynamic_equations =
            flat_inst.dynamic_equations @ flat_inst'.dynamic_equations;
          initial_equations =
            flat_inst.initial_equations @ flat_inst'.initial_equations;
          abstract_relations =
            flat_inst.abstract_relations @ flat_inst'.abstract_relations
        } in
      collect_array_elements' flat_inst (i + 1) in
  let flat_inst =
    {
      variables = [];
      dynamic_equations = [];
      initial_equations = [];
      abstract_relations = []
    } in
  collect_array_elements' flat_inst 0

and collect_declaration_equation cpnt_desc =
  let var = cpnt_desc.Instantiation.variability
  and equ = cpnt_desc.Instantiation.declaration_equation in
  match var, equ with
  | (Types.Continuous | Types.Discrete), Some expr ->
      let expr' = Instantiation.ComponentReference cpnt_desc in
      [ Instantiation.Equal (expr', evaluate expr) ]
  | _ -> []

and generate_flatten_instance ctx fun_defs oc id vars equs =
  Printf.fprintf oc "class %s\n" id;
  List.iter (generate_variable_declaration ctx oc) vars;
  Printf.fprintf oc "equation\n";
  generate_equation_descriptions ctx fun_defs oc equs;
  Printf.fprintf oc "end %s;\n" id

and generate_variable_declaration ctx oc cpnt_desc =
  Printf.fprintf oc "\t";
  generate_variable_variability oc cpnt_desc;
  generate_variable_causality oc cpnt_desc;
  generate_variable_type ctx oc cpnt_desc;
  generate_variable_name oc cpnt_desc;
  generate_variable_start_value ctx oc cpnt_desc;
  generate_initialization ctx oc cpnt_desc;
  generate_comment oc cpnt_desc;
  Printf.fprintf oc ";\n"

and generate_variable_variability oc cpnt_desc =
  match cpnt_desc.Instantiation.variability with
    | Types.Constant -> Printf.fprintf oc "constant "
    | Types.Parameter -> Printf.fprintf oc "parameter "
    | Types.Discrete -> Printf.fprintf oc "discrete "
    | Types.Continuous -> ()

and generate_variable_causality oc cpnt_desc =
  let inout = cpnt_desc.Instantiation.causality in
  match inout with
    | Types.Input -> Printf.fprintf oc "input "
    | Types.Output -> Printf.fprintf oc "output "
    | Types.Acausal -> ()

and generate_variable_type ctx oc cpnt_desc =
  let generate_variable_type' predef =
    let var = cpnt_desc.Instantiation.variability in
    match predef.Instantiation.predefined_type, var with
      | Instantiation.IntegerType, Types.Parameter  ->
          Printf.fprintf oc "Integer "
      | Instantiation.IntegerType, _  ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_IntegerType"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Instantiation.RealType, _ -> Printf.fprintf oc "Real "
      | Instantiation.BooleanType, _ ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_BooleanType"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Instantiation.StringType, Types.Parameter ->
          Printf.fprintf oc "String "
      | Instantiation.StringType, _ ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_StringType"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Instantiation.EnumerationType, _ ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_EnumType"];
              err_info = [];
              err_ctx = ctx }) (*error*) in
  let var_type = evaluate cpnt_desc.Instantiation.component_nature in
  match var_type with
    | Instantiation.PredefinedTypeInstance predef ->
        generate_variable_type' predef
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Instance _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_InstanceType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.StaticArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_StaticArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)

and generate_variable_name oc cpnt_desc =
  let name = ident_of_path cpnt_desc.Instantiation.component_path in
  Printf.fprintf oc "%s" name

and generate_variable_start_value ctx oc cpnt_desc =
  let generate_start_value attrs =
    try
      let expr = evaluate (List.assoc "start" attrs) in
      Printf.fprintf oc "(start=%s)"
        (string_of_expression ctx [] expr)
    with Not_found -> () in
  let generate_variable_start_value' predef =
    let attrs = predef.Instantiation.attributes in
    generate_start_value attrs in
  let var_type = evaluate cpnt_desc.Instantiation.component_nature in
  match var_type with
    | Instantiation.PredefinedTypeInstance predef ->
        generate_variable_start_value' predef
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Instance _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_InstanceType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.StaticArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_StaticArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)

and string_of_path = function
  | [] -> assert false
  | [Instantiation.Name id] -> id
  | [Instantiation.Index i] -> Printf.sprintf "[%d]" (i + 1)
  | Instantiation.Name id :: path ->
      Printf.sprintf "%s.%s" id (string_of_path path)
  | Instantiation.Index i :: path ->
      Printf.sprintf "[%d].%s" (i + 1) (string_of_path path)

and ident_of_path path =
  let rec ident_of_path' path =
    match path with
    | [] -> assert false
    | [Instantiation.Name id] -> unquoted id
    | [Instantiation.Index i] -> Printf.sprintf "[%d]" (i + 1)
    | Instantiation.Name id :: path ->
        Printf.sprintf "%s.%s" (unquoted id) (ident_of_path' path)
    | Instantiation.Index i :: path ->
        Printf.sprintf "[%d].%s" (i + 1) (ident_of_path' path) in
  match path with
  | [] -> assert false
  | [Instantiation.Name id] -> assert false
  | [Instantiation.Index i] -> assert false
  | Instantiation.Name id :: path ->
      Printf.sprintf "`%s`" (ident_of_path' path)
  | Instantiation.Index i :: path -> assert false

and unquoted id =
  let n = String.length id in
  try
    match id.[0] with
    | '`' | '\'' -> String.sub id 1 (n - 2)
    | _ -> id
  with
  | _ -> id

and generate_initialization ctx oc cpnt_desc =
  let var = cpnt_desc.Instantiation.variability
  and equ = cpnt_desc.Instantiation.declaration_equation in
  match var, equ with
  | Types.Parameter, Some expr ->
      Printf.fprintf oc " = %s"
        (string_of_expression ctx [] (evaluate expr))
  | _ -> ()

and generate_comment oc cpnt_desc =
  Printf.fprintf oc " \"%s\"" cpnt_desc.Instantiation.comment

and generate_equation_descriptions ctx fun_defs oc equ_descs =
  List.iter (generate_equation_description ctx fun_defs oc) equ_descs

and generate_equation_description ctx fun_defs oc equ_desc =
  match equ_desc with
  | Instantiation.Equal (expr, expr') ->
      Printf.fprintf oc "%s" (string_of_equal ctx fun_defs expr expr')
  | Instantiation.ConditionalEquationE _ -> assert false
  | Instantiation.ConnectFlows _ -> assert false
  | Instantiation.WhenClauseE alts ->
      generate_when_clause ctx fun_defs oc alts

and string_of_equal ctx fun_defs expr expr' = match expr with
  | Instantiation.Tuple [] ->
      Printf.sprintf "\t%s;\n"
        (string_of_expression ctx fun_defs expr')
  | _ ->
      Printf.sprintf "\t%s = %s;\n"
        (string_of_expression ctx fun_defs expr)
        (string_of_expression ctx fun_defs expr')

and generate_when_clause ctx fun_defs oc alts = match alts with
  | [] -> ()
  | [ (expr, equs) ] ->
      Printf.fprintf oc "when %s then\n"
        (string_of_expression ctx fun_defs expr);
      List.iter (generate_when_equation ctx fun_defs oc) equs;
      Printf.fprintf oc "end when;\n"
  | (expr, equs) :: alts ->
      Printf.fprintf oc "when %s then\n"
        (string_of_expression ctx fun_defs expr);
      List.iter (generate_when_equation ctx fun_defs oc) equs;
      Printf.fprintf oc "else";
      generate_when_clause ctx fun_defs oc alts

and generate_when_equation ctx fun_defs oc equ =
  let equ' = equ.Instantiation.nature in
  generate_equation_descriptions ctx fun_defs oc equ'

and string_of_expression ctx fun_defs = function
  | Instantiation.BinaryOperation (bin_op, expr, expr') ->
      string_of_binary_operation ctx fun_defs bin_op expr expr'
  | Instantiation.ClassReference cl_def ->
      string_of_class_reference fun_defs cl_def
  | Instantiation.ComponentReference cpnt_desc ->
      ident_of_path cpnt_desc.Instantiation.component_path
  | Instantiation.EnumerationElement _ ->
      raise (GenericError
        { err_msg = [ "_NotYetImplemented";
                      "_ExprOfType";
                      "enumeration" ];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.False ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_Expr"; "false"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.FieldAccess _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_FieldAccessExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.FunctionCall (expr, exprs) ->
      string_of_function_call ctx fun_defs expr exprs
  | Instantiation.If (alts, expr) ->
      string_of_if ctx fun_defs alts expr
  | Instantiation.IndexedAccess _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_IndexedAccessExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Integer i when Int32.to_int i >= 0 ->
      Printf.sprintf "%ld" i
  | Instantiation.Integer i ->
      let expr = Instantiation.Integer (Int32.neg i)
      and un_op = Instantiation.UnaryMinus in
      string_of_unary_operation ctx fun_defs un_op expr
  | Instantiation.LoopVariable _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_LoopVar"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.NoEvent expr -> string_of_no_event ctx fun_defs expr
  | Instantiation.PredefinedIdentifier id -> Printf.sprintf "%s" id
  | Instantiation.Range _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_RangeExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Real f ->
      Printf.sprintf "%s" (string_of_float f)
  | Instantiation.Record _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_ExprOfType"; "record"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.String s -> Printf.sprintf "\"%s\"" s
  | Instantiation.True ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_Expr"; "true"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Tuple _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_TupleExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.UnaryOperation (un_op, expr) ->
      string_of_unary_operation ctx fun_defs un_op expr
  | Instantiation.Vector exprs ->
      string_of_vector ctx fun_defs exprs
  | Instantiation.VectorReduction _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_VectorReduct"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and string_of_binary_operation ctx fun_defs bin_op expr expr' =
  let string_of_binary_operation_kind = function
    | Instantiation.And -> "and"
    | Instantiation.Divide -> "/"
    | Instantiation.EqualEqual -> "=="
    | Instantiation.GreaterEqual -> ">="
    | Instantiation.Greater -> ">"
    | Instantiation.LessEqual -> "<="
    | Instantiation.Less -> "<"
    | Instantiation.Times -> "*"
    | Instantiation.NotEqual -> "<>"
    | Instantiation.Or -> "or"
    | Instantiation.Plus -> "+"
    | Instantiation.Power -> "^"
    | Instantiation.Minus -> "-" in
  Printf.sprintf "(%s %s %s)"
    (string_of_expression ctx fun_defs expr)
    (string_of_binary_operation_kind bin_op)
    (string_of_expression ctx fun_defs expr')

and string_of_class_reference fun_defs cl_def =
  let rec last = function
    | [] -> assert false
    | [Instantiation.Name id] -> id
    | [Instantiation.Index _] -> assert false
    | _ :: path -> last path in
  let ctx =
    {
      path = cl_def.Instantiation.class_path;
      location = cl_def.Instantiation.class_location;
      instance_nature = Instantiation.ClassElement
    } in
  let string_of_external_call ext_call =
    match ext_call.NameResolve.nature with
    | NameResolve.PrimitiveCall "builtin" |
      NameResolve.PrimitiveCall "C" -> last ctx.path
    | NameResolve.PrimitiveCall lang ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented";
                       "_ExternalCallToLanguage";
                       lang];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | NameResolve.ExternalProcedureCall _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented";
                       "_ExternalProcedureCall"];
            err_info = [];
            err_ctx = ctx }) (*error*) in
  let string_of_long_description long_desc =
    match evaluate long_desc.NameResolve.external_call with
    | None ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented";
                       "_NonExternalCallClassRef"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Some ext_call -> string_of_external_call ext_call in
  match cl_def.Instantiation.description with
  | Instantiation.ClassDescription (_, cl_desc) ->
      string_of_long_description cl_desc.Instantiation.long_description
  | Instantiation.PredefinedType _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented";
                     "_PredefinedTypeClassRef"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and string_of_function_call ctx fun_defs expr exprs =
  Printf.sprintf "%s(%s)"
    (string_of_expression ctx fun_defs expr)
    (string_of_expressions ctx fun_defs exprs)

and string_of_expressions ctx fun_defs exprs =
  String.concat ", " (List.map (string_of_expression ctx fun_defs) exprs)

and string_of_if ctx fun_defs alts expr =
  let rec string_of_alternatives = function
    | [] -> Printf.sprintf " %s" (string_of_expression ctx fun_defs expr)
    | (expr, expr') :: alts ->
        Printf.sprintf "(if %s then %s else%s)"
          (string_of_expression ctx fun_defs expr)
          (string_of_expression ctx fun_defs expr')
          (string_of_alternatives alts) in
  string_of_alternatives alts

and string_of_no_event ctx fun_defs expr =
  Printf.sprintf "noEvent(%s)"
    (string_of_expression ctx fun_defs expr)

and string_of_unary_operation ctx fun_defs un_op expr =
  let string_of_unary_operation_kind = function
    | Instantiation.Not -> "not"
    | Instantiation.UnaryMinus -> "-" in
  Printf.sprintf "(%s %s)"
    (string_of_unary_operation_kind un_op)
    (string_of_expression ctx fun_defs expr)

and string_of_vector ctx fun_defs exprs =
  let exprs' = Array.to_list exprs in
  Printf.sprintf "{ %s }"
    (string_of_expressions ctx fun_defs exprs')

and last_id path =
  let rec last_id' id path = match path with
    | [] -> id
    | (Instantiation.Name id) :: path -> last_id' id path
    | (Instantiation.Index _) :: path -> last_id' id path in
  last_id' "" path

and string_of_float f =
  let add_parenthesis s =
    if String.contains s '-' then Printf.sprintf "(%s)" s else s in
  match Printf.sprintf "%.16g" f with
  | s when (String.contains s '.') || (String.contains s 'e') ->
      add_parenthesis s
  | s -> add_parenthesis (Printf.sprintf "%s." s)

and component_derivative cpnt_desc =
  let derivative_path path =
    let rec derivative_path' path =
      match path with
      | [] -> assert false
      | (Instantiation.Name s) :: path ->
          (Instantiation.Name ("__der_" ^ s)) :: path
      | (Instantiation.Index i) :: path ->
          (Instantiation.Index i) :: (derivative_path' path) in
    List.rev (derivative_path' (List.rev path)) in
  let path = cpnt_desc.Instantiation.component_path in
  let id = unquoted (ident_of_path path) in
  let component_derivative_nature cpnt_desc =
    match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.PredefinedTypeInstance predef_type_inst ->
        let attribs = [ "start", lazy (Instantiation.Real 0.) ] in
        Instantiation.PredefinedTypeInstance
          { predef_type_inst with Instantiation.attributes = attribs }
    | cpnt_nat -> cpnt_nat in
  {
    cpnt_desc with
    Instantiation.component_path = derivative_path path;
    Instantiation.component_nature =
      lazy (component_derivative_nature cpnt_desc);
    Instantiation.declaration_equation = None;
    Instantiation.comment = "Time derivative of " ^ id
  }

and add_component cpnt_descs cpnt_desc =
  let equal_components cpnt_desc cpnt_desc' =
    cpnt_desc.Instantiation.component_path =
      cpnt_desc'.Instantiation.component_path in
  match List.exists (equal_components cpnt_desc) cpnt_descs with
  | false -> cpnt_desc :: cpnt_descs
  | true -> cpnt_descs

and is_fixed_parameter cpnt_desc =
  match cpnt_desc.Instantiation.variability with
  | Types.Constant -> true
  | Types.Parameter -> is_fixed cpnt_desc
  | _ -> false

and is_fixed cpnt_desc =
  let var = cpnt_desc.Instantiation.variability
  and cpnt_nat = evaluate cpnt_desc.Instantiation.component_nature in
  let bool_of_fixed predef =
    match evaluate (List.assoc "fixed" predef.Instantiation.attributes) with
          | Instantiation.False -> false
    | _ -> true in
  match var, cpnt_nat with
  | Types.Constant, _ -> true
  | _, Instantiation.PredefinedTypeInstance predef
    when List.mem_assoc "fixed" predef.Instantiation.attributes ->
      bool_of_fixed predef
  | Types.Parameter, _ -> true
  | _ -> false

and defined_attribute cpnt_desc attrib_name =
  match evaluate cpnt_desc.Instantiation.component_nature with
  | Instantiation.PredefinedTypeInstance predef ->
      List.mem_assoc "fixed" predef.Instantiation.attributes
  | _ -> false

and generate_relations filename vars rels =
  let add_indentifier_stats stats cpnt_desc =
    match
      cpnt_desc.Instantiation.variability,
      cpnt_desc.Instantiation.causality,
      Lazy.force cpnt_desc.Instantiation.component_nature
    with
    | Types.Parameter, _,
      Instantiation.PredefinedTypeInstance
        { Instantiation.predefined_type = Instantiation.IntegerType } ->
        { stats with nb_ipars = stats.nb_ipars + 1 }
    | Types.Parameter, _,
      Instantiation.PredefinedTypeInstance
        { Instantiation.predefined_type = Instantiation.RealType } ->
        { stats with nb_rpars = stats.nb_rpars + 1 }
    | Types.Parameter, _,
      Instantiation.PredefinedTypeInstance
        { Instantiation.predefined_type = Instantiation.StringType } ->
        { stats with nb_spars = stats.nb_spars + 1 }
    | Types.Discrete, _, _ -> { stats with nb_dvars = stats.nb_dvars + 1 }
    | Types.Continuous, Types.Input, _ ->
        { stats with nb_inps = stats.nb_inps + 1 }
    | Types.Continuous, Types.Output, _ ->
        { stats with
          nb_cvars = stats.nb_cvars + 1;
          nb_outps = stats.nb_outps + 1
        }
    | Types.Continuous, Types.Acausal, _ ->
        { stats with nb_cvars = stats.nb_cvars + 1 }
    | _ -> stats in
  let variable_id cpnt_desc =
    let name = ident_of_path cpnt_desc.Instantiation.component_path in
    hide_spc (unquoted name) in
  let generate_identifier oc tabs cpnt_desc =
    let rec generate_tabs tabs =
      if tabs > 0 then begin
        Printf.fprintf oc "\t"; generate_tabs (tabs - 1)
      end in
    match
      cpnt_desc.Instantiation.variability,
      cpnt_desc.Instantiation.causality
    with
    | Types.Parameter, _ ->
        let id = variable_id cpnt_desc in
        generate_tabs tabs;
        Printf.fprintf oc "<parameter>%s</parameter>\n" id
    | Types.Constant, _ -> ()
    | _, (Types.Acausal | Types.Output) ->
        let id = variable_id cpnt_desc in
        generate_tabs tabs;
        Printf.fprintf oc "<implicit_variable>%s</implicit_variable>\n" id
    | _, Types.Input ->
        let id = variable_id cpnt_desc in
        generate_tabs tabs;
        Printf.fprintf oc "<input>%s</input>\n" id in
  let generate_relation oc rel =
    match rel with
    | Rel cpnt_descs ->
        Printf.fprintf oc "\t\t<implicit_relation>\n";
        List.iter (generate_identifier oc 3) cpnt_descs;
        Printf.fprintf oc "\t\t</implicit_relation>\n" in
  let generate_output oc cpnt_desc =
    match
      cpnt_desc.Instantiation.variability,
      cpnt_desc.Instantiation.causality
    with
    | (Types.Parameter | Types.Constant), _ |
      _, (Types.Acausal | Types.Input) -> ()
    | _, Types.Output ->
        let id = variable_id cpnt_desc in
        Printf.fprintf oc
          "\t\t<output>\n\
          \t\t\t<name>%s</name>\n\
          \t\t\t<dependencies>\n\
          \t\t\t\t<variable>%s</variable>\n\
          \t\t\t</dependencies>\n\
          \t\t</output>\n"
          id
          id in
  let oc' = open_out (filename ^ "_relations.xml") in
  Printf.fprintf oc' "<model>\n";
  let stats =
    List.fold_left
      add_indentifier_stats
      {
        nb_ipars = 0;
        nb_rpars = 0;
        nb_spars = 0;
        nb_dvars = 0;
        nb_cvars = 0;
        nb_inps = 0;
        nb_outps = 0
      }
      vars in
  Printf.fprintf oc'
    "\t<model_info>\n\
    \t\t<number_of_integer_parameters>%d</number_of_integer_parameters>\n\
    \t\t<number_of_real_parameters>%d</number_of_real_parameters>\n\
    \t\t<number_of_string_parameters>%d</number_of_string_parameters>\n\
    \t\t<number_of_discrete_variables>%d</number_of_discrete_variables>\n\
    \t\t<number_of_continuous_variables>%d</number_of_continuous_variables>\n\
    \t\t<number_of_continuous_unknowns>%d</number_of_continuous_unknowns>\n\
    \t\t<number_of_inputs>%d</number_of_inputs>\n\
    \t\t<number_of_outputs>%d</number_of_outputs>\n\
    \t</model_info>\n"
    stats.nb_ipars
    stats.nb_rpars
    stats.nb_spars
    stats.nb_dvars
    stats.nb_cvars
    stats.nb_cvars
    stats.nb_inps
    stats.nb_outps;
  Printf.fprintf oc' "\t<identifiers>\n";
  List.iter (generate_identifier oc' 2) vars;
  Printf.fprintf oc' "\t</identifiers>\n";
  Printf.fprintf oc' "\t<implicit_relations>\n";
  List.iter (generate_relation oc') rels;
  Printf.fprintf oc' "\t</implicit_relations>\n";
  Printf.fprintf oc' "\t<outputs>\n";
  List.iter (generate_output oc') vars;
  Printf.fprintf oc' "\t</outputs>\n";
  Printf.fprintf oc' "</model>\n";
  close_out oc'

and generate_flatten_XML ctx fun_defs oc id vars equs =
  let print_when_clause equ =
    let string_of_equation equ =
      let string_of_equation' equ_desc = match equ_desc with
    | Instantiation.Equal (expr, expr') ->
            hide_spc (string_of_equal ctx fun_defs expr expr')
    | _ -> assert false in
      String.concat
        " "
        (List.map string_of_equation' equ.Instantiation.nature) in
    let rec string_of_when_clause alts = match alts with
      | [] -> ""
      | [ (expr, equs) ] ->
          Printf.sprintf "when %s then\n %s end when;\n"
            (string_of_expression ctx fun_defs expr)
            (String.concat " " (List.map string_of_equation equs))
      | (expr, equs) :: alts ->
          Printf.sprintf "when %s then\n %s else %s"
            (string_of_expression ctx fun_defs expr)
            (String.concat " " (List.map string_of_equation equs))
            (string_of_when_clause alts) in
    match equ with
    | Instantiation.WhenClauseE alts ->
    Printf.fprintf oc "<when_clause value=\"%s\"/>\n"
      (string_of_when_clause alts)
    | _ -> ()
  and print_equation equ = match equ with
    | Instantiation.Equal (expr, expr') ->
    Printf.fprintf oc "<equation value=\"%s\"/>\n"
          (hide_spc (string_of_equal ctx fun_defs expr expr'))
    | _ -> () in
  let print_equations equs =
    Printf.fprintf oc "<equations>\n";
    List.iter print_equation equs;
    Printf.fprintf oc "</equations>\n"
  and print_when_clauses equs =
    Printf.fprintf oc "  <when_clauses>\n";
    List.iter print_when_clause equs;
    Printf.fprintf oc "  </when_clauses>\n" in
  Printf.fprintf oc "<model>\n";
  Printf.fprintf oc "<name>%s</name>\n" (hide_spc id);
  print_tree ctx fun_defs oc (build_tree vars);
  print_equations equs;
  print_when_clauses equs;
  Printf.fprintf oc "</model>\n"

and build_tree vars =
  let is_output caus = match caus with
    | Types.Output  -> true
    | _ -> false
  and variable_kind caus var = match caus, var with
    | Types.Input, _ -> Input
    | _, Types.Parameter -> Parameter
    | _, Types.Discrete -> DiscreteVariable
    | _ -> Variable
  and attribute_value name attrs =
      try
        Some (evaluate (List.assoc name attrs))
      with Not_found -> None in
  let variable_initial_value cpnt_desc =
    let cpnt_nat = evaluate cpnt_desc.Instantiation.component_nature
    and var = cpnt_desc.Instantiation.variability
    and equ = cpnt_desc.Instantiation.declaration_equation in
    match cpnt_nat, var, equ with
    | Instantiation.PredefinedTypeInstance _, Types.Parameter, Some expr ->
        Some (evaluate expr)
    | Instantiation.PredefinedTypeInstance predef,
      (Types.Continuous | Types.Discrete),
      _ ->
        let attrs = predef.Instantiation.attributes in
        attribute_value "start" attrs
    | _ -> None
  and variable_nominal_value cpnt_desc =
    match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.PredefinedTypeInstance predef ->
        let attrs = predef.Instantiation.attributes in
        attribute_value "nominal" attrs
    | _ -> None
  and fixed cpnt_desc =
    match defined_attribute cpnt_desc "fixed" with
    | false -> None
    | true -> Some (is_fixed cpnt_desc) in
  let terminal_element cpnt_desc =
    let caus = cpnt_desc.Instantiation.causality
    and var = cpnt_desc.Instantiation.variability in
    let id =
      unquoted (ident_of_path cpnt_desc.Instantiation.component_path) in
    {
      kind = variable_kind caus var;
      id = id;
      comment = cpnt_desc.Instantiation.comment;
      initial_value = variable_initial_value cpnt_desc;
      nominal_value = variable_nominal_value cpnt_desc; 
      output = is_output caus;
      fixed = fixed cpnt_desc
    } in
  let t_elts = List.map terminal_element vars in
  List.fold_left
    (fun ts t_elt -> insert (split t_elt.id) t_elt ts)
    []
    t_elts

and print_tree ctx fun_defs oc ts =
  let string_of_kind = function
    | Input -> "input"
    | Parameter -> "fixed_parameter"
    | Variable -> "variable"
    | DiscreteVariable -> "discrete_variable" in
  let string_of_initial_value elt = match elt.initial_value with
    | None -> ""
    | Some expr -> string_of_expression ctx fun_defs expr in
  let string_of_nominal_value elt = match elt.nominal_value with
    | None -> ""
    | Some expr -> string_of_expression ctx fun_defs expr in 
  let string_of_fixed elt = match elt.fixed with
    | None -> ""
    | Some true -> "true"
    | Some false -> "false" in
  let rec print_tree_element = function
    | Node (s, ts) ->
        Printf.fprintf oc "<struct>\n";
        Printf.fprintf oc "<name>%s</name>\n" (hide_spc s);
        Printf.fprintf oc "<subnodes>\n";
        List.iter print_tree_element ts;
        Printf.fprintf oc "</subnodes>\n";
        Printf.fprintf oc "</struct>\n"
    | Leaf (s, elt) ->
        Printf.fprintf oc "<terminal>\n";
        Printf.fprintf oc "<name>%s</name>\n" (hide_spc s);
        Printf.fprintf oc "<kind>%s</kind>\n" (string_of_kind elt.kind);
        Printf.fprintf oc "<id>%s</id>\n" (hide_spc elt.id);        
        Printf.fprintf oc "<fixed value=\"%s\"/>\n" (string_of_fixed elt);
        Printf.fprintf oc "<initial_value value=\"%s\"/>\n"
          (hide_spc (unbraced (string_of_initial_value elt)));
        Printf.fprintf oc "<nominal_value value=\"%s\"/>\n"
          (hide_spc (unbraced (string_of_nominal_value elt))); 
        Printf.fprintf oc "<comment value=\"%s\"/>\n" (hide_spc elt.comment);
        if elt.output then
          Printf.fprintf oc "<output/>\n";
        (* if elt.kind <> Parameter && elt.initial_value <> None then
          Printf.fprintf oc "<selected value=\"y\" />\n";*)
        Printf.fprintf oc "</terminal>\n"
  in
  Printf.fprintf oc "  <elements>\n";
  List.iter print_tree_element ts;
  Printf.fprintf oc "  </elements>\n"

and insert path x ts =
  let rec insert' s path' = function
    | [] -> [Node (s, insert path' x [])]
    | Node (s', ts'') :: ts' when s = s' -> Node (s', insert path' x ts'') :: ts'
    | t' :: ts' -> t' :: insert' s path' ts' in
  match path with
  | [s] -> ts @ [Leaf (s, x)] (*the order of elements is important in Scicos*)
  | s :: path' -> insert' s path' ts
  | [] -> assert false

and cut_on_dot s =
  let rec cut_on_dot' i =
    if i = String.length s then s, None
      else if s.[i] = '.' then String.sub s 0 i, Some (String.sub s (i + 1) (String.length s - i - 1))
      else cut_on_dot' (i + 1)
  in cut_on_dot' 0

and split name =
  let s, name_opt = cut_on_dot name in
  match name_opt with
    | None -> [s]
    | Some name' -> s :: split name'
