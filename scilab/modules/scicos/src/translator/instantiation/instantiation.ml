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

type ('a, 'b) node =
  {
    nature: 'a;
    info: 'b
  }

type instance =
  {
    enclosing_instance: instance option;
    kind: Types.kind;
    elements: instance_elements Lazy.t
  }

and instance_elements =
  {
    named_elements: (string * element_description) list;
    unnamed_elements: equation_or_algorithm_clause list
  }

and element_description =
  {
    redeclare: bool;
    element_nature: element_nature Lazy.t
  }

and element_nature =
  | Class of class_definition
  | Component of component_description

and class_definition =
  {
    class_type: Types.class_specifier;
    class_path: path;
    class_flow: bool option;
    class_variability: Types.variability option;
    class_causality: Types.causality option;
    description: description;
    modification: modification_argument list;
    class_location: Parser.location
  }

and path = path_element list

and path_element =
  | Name of string
  | Index of int

and description =
  | ClassDescription of context * class_description
  | PredefinedType of predefined_type

and class_description =
  {
    class_kind: Types.kind;
    class_annotations: (annotation list) Lazy.t;
    long_description: NameResolve.long_description
  }

and annotation =
  | InverseFunction of inverse_function Lazy.t
  | UnknownAnnotation of Parser.location Syntax.class_modification Lazy.t

and inverse_function =
  {
    function_class: class_definition;
    arguments: (string * string) list
  }

and class_modification = (string * modification_argument) list

and modification_argument =
  {
    each: bool;
    action: modification_action
  }

and modification_action =
  | ElementModification of modification
  | ElementRedeclaration of element_description

and modification =
  | Modification of class_modification * expression Lazy.t option
  | Assignment of expression Lazy.t
  | Equality of expression Lazy.t

and component_description =
  {
    component_path: path;
    flow: bool;
    variability: Types.variability;
    causality: Types.causality;
    component_nature: component_nature Lazy.t;
    declaration_equation: expression Lazy.t option;
    comment: string;
    component_location: Parser.location;
    class_name: string
  }

and component_nature =
  | DynamicArray of component_description
      (* one representative member of the collection *)
  | Instance of instance
  | PredefinedTypeInstance of predefined_type_instance
  | StaticArray of component_description array

and predefined_type_instance =
  {
    predefined_type: predefined_type;
    attributes: (string * expression Lazy.t) list
  }

and predefined_type =
  | BooleanType
  | IntegerType
  | RealType
  | StringType
  | EnumerationType

and equation_or_algorithm_clause =
  | EquationClause of NameResolve.validity * equation list Lazy.t
  | AlgorithmClause of NameResolve.validity * algorithm list Lazy.t

and validity = Initial | Permanent

and equation = (equation_desc list, NameResolve.equation) node

and equation_desc =
  | Equal of expression * expression
  | ConditionalEquationE of (expression * equation list) list *
      equation list
  | ConnectFlows of NameResolve.sign * expression *
      NameResolve.sign * expression
  | WhenClauseE of (expression * equation list) list

and algorithm = (algorithm_desc list, NameResolve.algorithm) node

and algorithm_desc =
  | Assign of expression * expression
  | FunctionCallA of expression * expression list
  | MultipleAssign of expression list * expression * expression list
  | Break
  | Return
  | ConditionalEquationA of (expression * algorithm list) list *
      algorithm list
  | ForClauseA of expression (* range *) * algorithm list
  | WhileClause of expression * algorithm list
  | WhenClauseA of (expression * algorithm list) list

and expression =
  | BinaryOperation of binary_operator_kind * expression * expression
  | ClassReference of class_definition
  | ComponentReference of component_description
  | EnumerationElement of string
  | False
  | FieldAccess of expression * string
  | FunctionCall of expression * expression list
  | If of (expression (* condition *) * expression) list *
      expression (* default *)
  | IndexedAccess of expression * expression list (* subscripts *)
  | Integer of int32
  | LoopVariable of int (* number of nested for loops to skip *)
  | NoEvent of expression
  | PredefinedIdentifier of string
  | Range of expression * expression * expression
  | Real of float
  | Record of (string * expression) list
  | String of string
  | True
  | Tuple of expression list
  | UnaryOperation of unary_operator_kind * expression
  | Vector of expression array
  | VectorReduction of expression list (* ranges *) * expression

and unary_operator_kind =
  | Not
  | UnaryMinus

and binary_operator_kind =
  | And
  | Divide
  | EqualEqual
  | GreaterEqual
  | Greater
  | LessEqual
  | Less
  | Times
  | NotEqual
  | Or
  | Plus
  | Power
  | Minus

and context =
  {
    toplevel: (string * element_description) list Lazy.t;
    path: path;
    context_flow: bool option;
    context_variability: Types.variability option;
    context_causality: Types.causality option;
    parent_context: context option; (* for normal parent scope lookup *)
    class_context: context_nature; (* for normal (class-based) lookup *)
    instance_context: instance option; (* for dynamically scoped identifiers *)
    location: Parser.location;
    instance_nature: instance_nature
  }

and context_nature =
  | ToplevelContext
  | InstanceContext of instance
  | ForContext of context *
      expression option (* current value of the loop variable, if available *)
  | FunctionEvaluationContext of context * expression * expression list

(* Error description *)
and error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: context
  }

and instance_nature =
  | ClassElement
  | ComponentElement of string

exception InstantError of error_description


(* Utilities *)

let levels = ref 0

let spaces () = for i = 1 to !levels do Printf.printf "  " done

let nest i =
  spaces (); Printf.printf "ForContext %ld\n" i;
  incr levels

let nest2 i =
  spaces (); Printf.printf "ReductionContext %ld\n" i;
  incr levels

let unnest () =
  decr levels;
  spaces (); Printf.printf "Leaving ForContext\n"

let evaluate x = Lazy.force x

module ArrayExt =
  struct
    let map2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.map2"
      else begin
        let create_array i = f a.(i) a'.(i) in
        Array.init l create_array
      end
    let for_all2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.for_all2"
      else begin
        let rec for_all2' i =
          i = l || (f a.(i) a'.(i) && for_all2' (i + 1)) in
        for_all2' 0
      end
    let exists2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.exists2"
      else begin
        let rec exists2' i =
          i < l && (f a.(i) a'.(i) || exists2' (i + 1)) in
        exists2' 0
      end
  end


(* Instantiation functions *)

let rec evaluate_toplevel_definitions dic defs =
  let rec ctx =
    {
      toplevel = lazy (dic @ evaluate defs');
      path = [];
      context_flow = None;
      context_variability = None;
      context_causality = None;
      parent_context = None;
      class_context = ToplevelContext;
      instance_context = None;
      location = {Parser.start = 0; Parser.enddd = 0; filename = Parser.CommandLine};
      instance_nature = ClassElement
    }
  and defs' = lazy (List.map (evaluate_toplevel_definition ctx) defs) in
  evaluate defs'

and evaluate_toplevel_definition ctx (id, elt_desc) =
  let elt_loc = [Name id] in
  let ctx = {ctx with
               path = elt_loc;
               location = elt_desc.NameResolve.element_location;
               instance_nature = instance_nature_of_element elt_desc} in
  let elt_nat = elt_desc.NameResolve.element_nature in
  let elt_desc' =
    {
      redeclare = false;
      element_nature = lazy (evaluate_toplevel_element ctx elt_loc elt_nat)
    } in
  id, elt_desc'

and evaluate_toplevel_element ctx elt_loc = function
  | NameResolve.Component cpnt_desc ->
      let cpnt_desc' =
        instantiate_component_description ctx [] None elt_loc cpnt_desc in
      Component cpnt_desc'
  | NameResolve.Class cl_def ->
      let cl_def' = evaluate_class_definition ctx [] elt_loc cl_def in
      Class cl_def'
  | NameResolve.ComponentType _ ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_ComponentTypeElemInstant"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | NameResolve.PredefinedType _ ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_PredefinedTypeElemInstant"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and instantiate_class_description ctx modifs rhs elt_loc cl_desc =
  let elements inst =
    let ctx' =
      { ctx with
        toplevel = lazy (evaluate ctx.toplevel);
        path = elt_loc;
        parent_context = Some ctx;
        class_context = InstanceContext inst;
        instance_context = None
      } in
    instantiate_class_elements ctx' modifs rhs cl_desc.long_description in
  let rec inst =
    {
      enclosing_instance = enclosing_instance ctx;
      kind = cl_desc.class_kind;
      elements = lazy (elements inst)
    } in
  inst

and enclosing_instance ctx = match ctx.class_context with
  | ToplevelContext -> None
  | InstanceContext inst -> Some inst
  | ForContext (ctx', _) | FunctionEvaluationContext (ctx', _, _) ->
      enclosing_instance ctx'

and instantiate_class_elements ctx modifs rhs long_desc =
  let rec merge_elements named_elts unnamed_elts = function
    | [] ->
        {
          named_elements = named_elts;
          unnamed_elements = unnamed_elts
        }
    | inherited_elts :: inherited_eltss ->
        let named_elts' = named_elts @ inherited_elts.named_elements
        and unnamed_elts' = unnamed_elts @ inherited_elts.unnamed_elements in
        merge_elements named_elts' unnamed_elts' inherited_eltss in
  let named_elts = long_desc.NameResolve.named_elements
  and unnamed_elts = long_desc.NameResolve.unnamed_elements
  and exts = long_desc.NameResolve.extensions in
  let named_elts' = instantiate_local_named_elements ctx modifs rhs named_elts
  and unnamed_elts' = instantiate_local_unnamed_elements ctx unnamed_elts
  and inherited_eltss = instantiate_inherited_elements ctx modifs rhs exts in
  merge_elements named_elts' unnamed_elts' inherited_eltss

and instantiate_local_named_elements ctx modifs rhs named_elts =
  List.fold_right (instantiate_local_named_element ctx modifs rhs) named_elts []

and instantiate_local_named_element ctx modifs rhs (id, elt_desc) named_elts =
  let rec filter_current_element_modifications = function
    | [] -> []
    | (id', arg) :: modifs when id' = id ->
        arg :: filter_current_element_modifications modifs
    | _ :: modifs -> filter_current_element_modifications modifs
  and select_current_element_value = function
    | None -> None
    | Some expr -> Some (lazy (field_access ctx (evaluate expr) id)) in
  let modifs' = filter_current_element_modifications modifs
  and rhs' = select_current_element_value rhs
  and elt_loc = ctx.path @ [Name id] in
  let ctx = {ctx with
               path = elt_loc;
               location = elt_desc.NameResolve.element_location;
               instance_nature = instance_nature_of_element elt_desc} in
  let elt_nat =
    lazy (instantiate_local_element_nature ctx modifs' rhs' elt_loc elt_desc) in
  let named_elt =
    id,
    {
      redeclare = elt_desc.NameResolve.redeclare;
      element_nature = elt_nat
    } in
  named_elt :: named_elts

and instantiate_local_element_nature ctx modifs rhs elt_loc elt_desc =
  match elt_desc.NameResolve.element_nature with
    | NameResolve.Component cpnt_desc ->
        let cpnt_desc' =
          instantiate_component_description ctx modifs rhs elt_loc cpnt_desc in
       Component cpnt_desc'
    | NameResolve.Class cl_def ->
        let cl_def' = evaluate_class_definition ctx modifs elt_loc cl_def in
        Class cl_def'
    | NameResolve.ComponentType _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_ComponentTypeElemInstant"];
            err_info = [];
            err_ctx = ctx })
    | NameResolve.PredefinedType _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_PredefinedTypeElemInstant"];
            err_info = [];
            err_ctx = ctx })

and instantiate_component_description ctx modifs rhs elt_loc cpnt_desc =
  let cpnt_type = evaluate cpnt_desc.NameResolve.component_type in
  let flow = evaluate cpnt_type.Types.flow
  and var = evaluate cpnt_type.Types.variability
  and inout = evaluate cpnt_type.Types.causality
  and type_spec = evaluate cpnt_desc.NameResolve.type_specifier
  and dims = evaluate cpnt_desc.NameResolve.dimensions
  and modifs' = match evaluate cpnt_desc.NameResolve.modification with
    | None -> modifs
    | Some modif ->
        let modif' = evaluate_modification ctx modif in
        modifs @ [{ each = false; action = ElementModification modif' }]
  and cmt = cpnt_desc.NameResolve.comment in
  component_nature ctx modifs' rhs elt_loc flow var inout type_spec dims cmt

and component_nature ctx modifs rhs elt_loc flow var inout type_spec dims cmt =
  let type_spec' = evaluate_expression ctx type_spec in
  let ctx = {ctx with location = expression_location ctx type_spec} in
  expand_array ctx modifs rhs elt_loc flow var inout type_spec' dims cmt

and expand_array ctx modifs rhs elt_loc flow var inout type_spec dims cmt =
  let rec expand_along_dimension dim dims = match dim with
    | NameResolve.Colon -> expand_dynamic_array dims
    | NameResolve.Expression expr ->
        let expr' = evaluate_expression ctx expr in
        expand_static_array dims expr' expr
  and expand_dynamic_array dims =
    (* No need to select modifications since all of them have 'each' set *)
    let elt_loc' = elt_loc @ [Index 0] in
    let ctx = { ctx with path = elt_loc' } in
    let expr =
      expand_array ctx modifs rhs elt_loc' flow var inout type_spec dims cmt in
    DynamicArray expr
  and expand_static_array dims expr' expr =
    let ctx = {ctx with location = expression_location ctx expr} in
    let expand_element i =
      let rec select_subargument arg = match arg.each with
        | true -> arg
        | false -> { arg with action = select_subarray arg.action }
      and select_subarray arg = match arg with
	| ElementModification modif ->
            ElementModification (select_submodification modif)
        | ElementRedeclaration _ -> arg
      and select_sub_class_modification_element (id, arg) =
        id, select_subargument arg
      and select_submodification = function
        | Modification (modifs, rhs) ->
            let modifs' = List.map select_sub_class_modification_element modifs
            and rhs' = select_rhs_subarray rhs in
            Modification (modifs', rhs')
        | Assignment expr ->
            let expr' = lazy (select_row i (evaluate expr)) in
            Assignment expr'
        | Equality expr ->
            let expr' = lazy (select_row i (evaluate expr)) in
            Equality expr'
      and select_rhs_subarray = function
        | None -> None
        | Some expr -> Some (lazy (select_row i (evaluate expr)))
      and select_row i = function
	      | Vector exprs ->
            begin
              try
                exprs.(i)
              with
              | _ -> raise (InstantError
                  { err_msg = ["_IndexOutOfBound"];
                    err_info = [];
                    err_ctx = ctx}) (*error*)
            end
        | expr ->
            let subs = [Integer (Int32.succ (Int32.of_int i))] in
            evaluate_indexed_access ctx expr subs in
      let modifs = List.map select_subargument modifs
      and rhs = select_rhs_subarray rhs
      and elt_loc = elt_loc @ [Index i] in
      expand_array ctx modifs rhs elt_loc flow var inout type_spec dims cmt in
    match expr' with
      | Integer i ->
          let a = Array.init (Int32.to_int i) expand_element in
          StaticArray a
      | _ ->
          raise (InstantError
          { err_msg = ["_NonIntegerArrayDim"];
            err_info = [];
            err_ctx = ctx }) (*error*) in
  match dims with
    | [] ->
        let cl_def = class_definition_of_type_specification ctx type_spec in
        create_instance ctx modifs rhs elt_loc flow var inout cl_def cmt
    | dim :: dims ->
        {
          component_path = elt_loc;
          flow = flow;
          variability = var;
          causality = inout;
          component_nature = lazy (expand_along_dimension dim dims);
          declaration_equation = rhs;
          comment = cmt;
          component_location = ctx.location;
          class_name = instance_class_name ctx.instance_nature
        }

and create_instance ctx modifs rhs elt_loc flow var inout cl_def cmt =
  let merge_class_modifications arg modifs = match arg.action with
    | ElementModification (Modification (modifs', _)) -> modifs' @ modifs
    | ElementModification (Assignment _ | Equality _) -> modifs
    | ElementRedeclaration _ -> modifs in
  let rec declaration_equation modifs rhs =
    let rec declaration_equation' = function
      | [] -> None
      | {
          action =
            ElementModification (
              Modification (_, Some expr) | Assignment expr | Equality expr)
        } :: _ -> Some expr
      | _ :: args -> declaration_equation' args in
    match rhs with
      | None -> declaration_equation' modifs
      | Some _ -> rhs in
  let flow' = match cl_def.class_flow, ctx.context_flow with
    | None, None -> flow
    | Some flow', None | None, Some flow' -> flow || flow'
    | Some flow', Some flow'' -> flow || flow' || flow''
  and var' = match cl_def.class_variability, ctx.context_variability with
    | None, None -> var
    | Some var', None | None, Some var' -> Types.min_variability var var'
    | Some var', Some var'' ->
        Types.min_variability var (Types.min_variability var' var'')
  and inout' = match inout, cl_def.class_causality with
    | Types.Input, _ | _, Some Types.Input -> Types.Input
    | Types.Output, _ | _, Some Types.Output -> Types.Output
    | _ -> Types.Acausal in
  let modifs' =
    List.fold_right
      merge_class_modifications
      (modifs @ cl_def.modification)
      []
  and rhs' = declaration_equation modifs rhs in
  match cl_def.description with
    | ClassDescription (ctx', cl_desc) ->
        let class_name = instance_class_name ctx.instance_nature in
        let ctx' =
          { ctx' with
            context_flow = Some flow';
            context_variability = Some var';
            context_causality = Some inout';
            instance_context = enclosing_instance ctx;
            instance_nature = ComponentElement class_name
          } in
        {
          component_path = elt_loc;
          flow = flow';
          variability = var';
          causality = inout';
          component_nature =
            lazy (create_class_instance ctx' modifs' rhs' elt_loc cl_desc);
          declaration_equation = rhs';
          comment = cmt;
          component_location = ctx'.location;
          class_name = class_name
        }
    | PredefinedType predef ->
        let class_name = instance_class_name ctx.instance_nature in
        let ctx' =
          { ctx with
            context_flow = Some flow';
            context_variability = Some var';
            context_causality = Some inout';
            instance_nature = ComponentElement class_name
          } in
        {
          component_path = elt_loc;
          flow = flow';
          variability = var';
          causality = inout';
          component_nature =
            lazy (create_predefined_type_instance ctx' modifs' predef);
          declaration_equation = rhs';
          comment = cmt;
          component_location = ctx'.location;
          class_name = class_name
        }

and create_temporary_instance ctx cl_def =
  match cl_def.description with
    | ClassDescription (ctx', cl_desc) ->
        {
          component_path = [];
          flow = false;
          variability = Types.Continuous;
          causality = Types.Acausal;
          component_nature =
            lazy (create_class_instance ctx' [] None [] cl_desc);
          declaration_equation = None;
          comment = "";
          component_location = ctx'.location;
          class_name = instance_class_name ctx.instance_nature
        }
    | PredefinedType predef -> assert false (*error*)

and class_definition_of_type_specification ctx type_spec =
  let predefined_class_specifier = function
    | "Boolean" -> Types.boolean_class_type
    | "Integer" -> Types.integer_class_type
    | "Real" -> Types.real_class_type
    | "String" -> Types.string_class_type
    | s ->
        raise (InstantError
          { err_msg = ["_UnknownIdentifier"; s];
            err_info = [];
            err_ctx = ctx }) (*error*)
  and predefined_class_description = function
    | "Boolean" -> PredefinedType BooleanType
    | "Integer" -> PredefinedType IntegerType
    | "Real" -> PredefinedType RealType
    | "String" -> PredefinedType StringType
    | s ->
        raise (InstantError
          { err_msg = ["_UnknownIdentifier"; s];
            err_info = [];
            err_ctx = ctx }) (*error*) in
  match type_spec with
    | ClassReference cl_def -> cl_def
    | PredefinedIdentifier id ->
        {
          class_type = predefined_class_specifier id;
          class_path = [Name id];
          class_flow = None;
          class_variability = None;
          class_causality = None;
          description = predefined_class_description id;
          modification = [];
          class_location = ctx.location
        }
    | _ -> assert false (*error*)

and create_class_instance ctx modifs rhs elt_loc cl_desc =
  let inst = instantiate_class_description ctx modifs rhs elt_loc cl_desc in
  Instance inst

and create_predefined_type_instance ctx modifs predef =
  let inst =
    {
      predefined_type = predef;
      attributes = predefined_type_attributes ctx modifs
    } in
  PredefinedTypeInstance inst

and predefined_type_attributes ctx modifs =
  let rec predefined_type_attributes attrs = function
    | [] -> attrs
    | (id, { action = ElementModification (Equality expr) }) :: modifs
      when not (List.mem_assoc id attrs) ->
        let attrs' = (id, expr) :: attrs in
        predefined_type_attributes attrs' modifs
    | _ :: modifs -> predefined_type_attributes attrs modifs in
  predefined_type_attributes [] modifs

and instantiate_inherited_elements ctx modifs rhs exts =
  List.fold_right (instantiate_inherited_element ctx modifs rhs) exts []

and instantiate_inherited_element ctx modifs rhs (_, modif_cl) inherited_elts =
  let instantiate_inherited_element' modifs cl_def =
    match cl_def.description with
      | ClassDescription (ctx', cl_desc) ->
          let ctx' = { ctx with parent_context = Some ctx' } in
          let long_desc = cl_desc.long_description in
          instantiate_class_elements ctx' modifs rhs long_desc
      | PredefinedType _ -> assert false (*error*) in
  let type_spec = evaluate modif_cl.NameResolve.base_class
  and modifs' = evaluate modif_cl.NameResolve.class_modification in
  let type_spec' = evaluate_expression ctx type_spec
  and ctx = {ctx with location = expression_location ctx type_spec} in
  let modifs = modifs @ evaluate_class_modification ctx modifs' in
  match type_spec' with
    | ClassReference cl_def ->
        instantiate_inherited_element' modifs cl_def :: inherited_elts
    | _ -> assert false (*error*)
 
and evaluate_class_definition ctx modifs elt_loc cl_def =
  match evaluate cl_def.NameResolve.description with
    | NameResolve.LongDescription long_desc ->
        let cl_anns = long_desc.NameResolve.class_annotations in
        let cl_def' =
          {
            class_kind = Types.Class;
            class_annotations = lazy (evaluate_class_annotations ctx cl_anns);
            long_description = long_desc
          } in
        {
          class_type = evaluate cl_def.NameResolve.class_type;
          class_path = elt_loc;
          class_flow = None;
          class_variability = None;
          class_causality = None;
          description = ClassDescription (ctx, cl_def');
          modification = modifs;
          class_location = ctx.location
        }
    | NameResolve.ShortDescription short_desc ->
        raise (InstantError
          {err_msg = ["_NotYetImplemented"; "_ShortClassDef"];
           err_info = [];
           err_ctx = {ctx with path = elt_loc;
                      instance_nature = ClassElement}})

and evaluate_class_annotations ctx cl_anns =
  let evaluate_inverse_function inv_func =
    let inv_func = evaluate inv_func in
    let expr =
      evaluate_expression ctx inv_func.NameResolve.function_class in
    match expr with
    | ClassReference cl_def ->
        {
          function_class = cl_def;
          arguments = inv_func.NameResolve.arguments
        }
    | _ -> assert false (*error*) in
  let evaluate_class_annotation cl_ann = match cl_ann with
    | NameResolve.InverseFunction inv_func ->
        InverseFunction (lazy (evaluate_inverse_function inv_func))
    | NameResolve.UnknownAnnotation cl_ann ->
        UnknownAnnotation cl_ann in
  List.map evaluate_class_annotation (evaluate cl_anns)

and evaluate_class_modification ctx cl_modif =
  let add_modification_argument arg cl_modif' =
    match arg.NameResolve.action with
      | None -> cl_modif'
      | Some modif ->
          let arg' =
            arg.NameResolve.target,
            {
              each = arg.NameResolve.each;
              action = evaluate_modification_action ctx modif
            } in
          arg' :: cl_modif' in
  List.fold_right add_modification_argument cl_modif []

and evaluate_modification_action ctx = function
  | NameResolve.ElementModification modif ->
      let modif' = evaluate_modification ctx modif in
      ElementModification modif'
  | NameResolve.ElementRedeclaration elt_desc ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_ElementRedeclaration"];
          err_info = [];
          err_ctx = ctx })

and evaluate_modification ctx = function
  | NameResolve.Modification (modifs, rhs) ->
      let modifs' = evaluate_class_modification ctx modifs
      and rhs' = evaluate_modification_expression ctx rhs in
      Modification (modifs', rhs')
  | NameResolve.Assignment expr ->
      let expr = evaluate expr in
      let ctx = {ctx with location = expression_location ctx expr} in
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_AssignExprInElemModif"];
          err_info = [];
          err_ctx = ctx })
  | NameResolve.Equality expr ->
      let expr' = lazy (evaluate_expression ctx (evaluate expr)) in
      Equality expr'

and evaluate_modification_expression ctx = function
  | None -> None
  | Some expr ->
      let expr' = lazy (evaluate_expression ctx (evaluate expr)) in
      Some expr'

and instantiate_local_unnamed_elements ctx unnamed_elts =
  List.map (instantiate_local_unnamed_element ctx) (evaluate unnamed_elts)

and instantiate_local_unnamed_element ctx unnamed_elt =
  match unnamed_elt with
    | NameResolve.EquationClause (validity, equs) ->
        EquationClause (validity, lazy (instantiate_equations ctx equs))
    | NameResolve.AlgorithmClause (validity, algs) ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_AlgoClause"];
            err_info = [];
            err_ctx = ctx })

and instantiate_equations ctx equs =
  let instantiate_equations' equ equs =
    let equs' =  instantiate_equation ctx equ in
    { nature = equs'; info = equ } :: equs in
  List.fold_right instantiate_equations' equs []

and instantiate_equation ctx equ = match equ.NameResolve.nature with
  | NameResolve.Equal (expr, expr') -> instantiate_equal ctx expr expr'
  | NameResolve.ConditionalEquationE (alts, default) ->
      instantiate_conditional_equation ctx alts default
  | NameResolve.ForClauseE (ranges, equs) ->
      instantiate_for_clause_e ctx ranges equs
  | NameResolve.ConnectFlows (sign, expr, sign', expr') ->
      instantiate_connection ctx sign expr sign' expr'
  | NameResolve.WhenClauseE alts ->
      instantiate_when_clause_e ctx alts

and instantiate_equal ctx expr expr' =
  let rec equal_expr expr expr' =
    match expr, expr' with
    | BinaryOperation (bin_oper_kind, expr1, expr2),
      BinaryOperation (bin_oper_kind', expr1', expr2') ->
        (bin_oper_kind = bin_oper_kind') &&
        (equal_expr expr1 expr1') &&
        (equal_expr expr2 expr2')
    | ClassReference cl_def, ClassReference cl_def' ->
        cl_def.class_path = cl_def'.class_path
    | ComponentReference cpnt_desc, ComponentReference cpnt_desc' ->
        cpnt_desc.component_path = cpnt_desc'.component_path
    | EnumerationElement s, EnumerationElement s' -> s = s'
    | False, False -> true
    | FieldAccess (expr, s), FieldAccess (expr', s') ->
        (equal_expr expr expr') && (s = s')
    | FunctionCall (expr, exprs), FunctionCall (expr', exprs') ->
        (equal_expr expr expr') &&
        (List.length exprs = List.length exprs') &&
        (List.for_all2 (=) exprs exprs')
    | If (alts, default), If (alts', default') ->
        let f (cond, expr) (cond', expr') =
          (equal_expr cond cond') && (equal_expr expr expr') in
        (List.length alts = List.length alts') &&
        (List.for_all2 f alts alts') &&
        (equal_expr default default')
    | IndexedAccess (expr, exprs), IndexedAccess (expr', exprs') ->
        (equal_expr expr expr') &&
        (List.length exprs = List.length exprs') &&
        (List.for_all2 (=) exprs exprs')
    | Integer i, Integer i' -> Int32.compare i i' = 0
    | LoopVariable i, LoopVariable i' -> i = i'
    | NoEvent expr, NoEvent expr' -> equal_expr expr expr'
    | PredefinedIdentifier s, PredefinedIdentifier s' -> s = s'
    | Range (start, step, stop), Range (start', step', stop') ->
        (equal_expr start start') &&
        (equal_expr step step') &&
        (equal_expr stop stop')
    | Real f, Real f' -> f = f'
    | Record elts, Record elts' ->
        let f (s, expr) (s', expr') =
          (s = s') && (equal_expr expr expr') in
        (List.length elts = List.length elts') &&
        (List.for_all2 f elts elts')
    | String s, String s' -> s = s'
    | True, True -> true
    | Tuple exprs, Tuple exprs' ->
        (List.length exprs = List.length exprs') &&
        (List.for_all2 equal_expr exprs exprs')
    | UnaryOperation (un_oper_kind, expr),
      UnaryOperation (un_oper_kind', expr') ->
        (un_oper_kind = un_oper_kind') &&
        (equal_expr expr expr')
    | Vector exprs, Vector exprs' ->
        (Array.length exprs = Array.length exprs') &&
        (ArrayExt.for_all2 equal_expr exprs exprs')
    | VectorReduction (exprs, expr), VectorReduction (exprs', expr') ->
        (List.length exprs = List.length exprs') &&
        (List.for_all2 equal_expr exprs exprs') &&
        (equal_expr expr expr')
    | _ -> false in
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  match equal_expr expr expr' with
  | true -> []
  | false -> [ Equal (expr, expr') ]

and instantiate_conditional_equation ctx alts default =
  let rec instantiate_alternatives acc = function
    | [] -> instantiate_default acc default
    | (cond, equs) :: alts -> instantiate_alternative acc cond equs alts
  and instantiate_alternative acc cond equs alts =
    let cond' = evaluate_expression ctx cond in
    match cond' with
      | False -> instantiate_alternatives acc alts
      | True -> instantiate_default acc equs
      | _ ->
          let equs' = instantiate_equations ctx equs in
          instantiate_alternatives ((cond', equs') :: acc) alts
  and instantiate_default acc equs =
    let equs' = instantiate_equations ctx equs in
    [ConditionalEquationE (List.rev acc, equs')] in
  let alts' = instantiate_alternatives [] alts in
  List.flatten (List.map (expand_equation ctx) alts')

and expand_equation ctx equ =
  let rec expand_equation' equ =
    let expand_conditional_equation alts default =
      let add_alternative (b, equs) altss =
        let g equ = List.flatten (List.map expand_equation' equ.nature) in
        let equs' = List.flatten (List.map g equs) in
        let f (expr1, expr2) (expr1', expr2') = match expr1, expr2 with
          | If (alts1, default1), If (alts2, default2) ->
              If ((b, expr1') :: alts1, default1),
              If ((b, expr2') :: alts2, default2)
          | _ -> assert false in
        try
          List.map2 f altss equs'
        with
        | _ ->
            raise (InstantError
              {err_msg = ["_InvalidCondEquation"];
               err_info = [];
               err_ctx = ctx}) in
      let g equ = List.flatten (List.map expand_equation' equ.nature) in
      let default' = List.flatten (List.map g default) in
      let f = function (expr1, expr2) -> If ([], expr1), If ([], expr2) in
      List.fold_right add_alternative alts (List.map f default') in
    match equ with
    | ConditionalEquationE (alts, default) ->
        expand_conditional_equation alts default
    | Equal (expr, expr') -> [ expr, expr' ]
    | _ ->
        raise (InstantError
          {err_msg = ["_InvalidCondEquation"];
           err_info = [];
           err_ctx = ctx}) in
  let f (expr, expr') = Equal (expr, expr') in
  List.map f (expand_equation' equ)

and instantiate_when_clause_e ctx alts =
  let instantiate_alternative (cond, equs) =
    let cond' = evaluate_expression ctx cond in
    let equs' = instantiate_equations ctx equs in
    cond', equs' in
  [WhenClauseE (List.map instantiate_alternative alts)]

and instantiate_connection ctx sign expr sign' expr' =
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  [ConnectFlows (sign, expr, sign', expr')]

and instantiate_for_clause_e ctx ranges equs =
  let rec instantiate_for_clause_e' ctx = function
    | [] -> List.flatten (List.map (instantiate_equation ctx) equs)
    | ranges -> equations_of_reduction ctx ranges
  and equations_of_reduction ctx ranges = match ranges with
    | (Vector exprs) :: ranges ->
        let f expr =
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some expr)
            } in
          instantiate_for_clause_e' ctx' ranges in
        List.flatten (List.map f (Array.to_list exprs))
    | _ ->
        raise (InstantError
          {err_msg = ["_InvalidForClauseRange"];
           err_info = [];
           err_ctx = ctx}) in
  let ranges = List.map (evaluate_expression ctx) ranges in
  instantiate_for_clause_e' ctx ranges

and evaluate_expression ctx expr =
  let ctx = {ctx with location = expression_location ctx expr} in
  match expr.NameResolve.nature with
    | NameResolve.BinaryOperation (binop, expr, expr') ->
        evaluate_binary_operation ctx binop expr expr'
    | NameResolve.DynamicIdentifier (level, id) ->
        evaluate_dynamic_identifier ctx level id
    | NameResolve.False -> False
    | NameResolve.FieldAccess (expr, id) ->
        evaluate_field_access ctx expr id
    | NameResolve.FunctionArgument pos -> evaluate_function_argument ctx pos
    | NameResolve.FunctionCall (expr, exprs, expr') ->
        evaluate_function_call ctx expr exprs expr'
    | NameResolve.FunctionInvocation exprs ->
        evaluate_function_invocation ctx exprs
    | NameResolve.If (alts, default) -> evaluate_if ctx alts default
    | NameResolve.IndexedAccess (expr, exprs) ->
        let expr = evaluate_expression ctx expr
        and exprs = List.map (evaluate_expression ctx) exprs in
        evaluate_indexed_access ctx expr exprs
    | NameResolve.Integer i -> Integer i
    | NameResolve.LocalIdentifier (level, id) ->
        evaluate_local_identifier ctx level id
    | NameResolve.LoopVariable level -> evaluate_loop_variable ctx level
    | NameResolve.NoEvent expr -> evaluate_no_event ctx expr
    | NameResolve.PredefinedIdentifier id -> PredefinedIdentifier id
    | NameResolve.Range (start, step, stop) ->
        evaluate_range ctx start step stop
    | NameResolve.Real f -> Real f
    | NameResolve.String s -> String s
    | NameResolve.ToplevelIdentifier id ->
        evaluate_toplevel_identifier ctx id
    | NameResolve.True -> True
    | NameResolve.Tuple exprs -> evaluate_tuple ctx exprs
    | NameResolve.UnaryOperation (unop, expr) ->
        evaluate_unary_operation ctx unop expr
    | NameResolve.VectorReduction (ranges, expr) ->
        evaluate_vector_reduction ctx ranges expr
    | NameResolve.Vector exprs -> evaluate_vector ctx exprs
    | NameResolve.Coercion (coer, expr) ->
        evaluate_coercion ctx coer expr

and evaluate_binary_operation ctx binop expr expr' =
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  let expr = flatten_expression expr
  and expr' = flatten_expression expr' in
  match binop with
    | NameResolve.And -> evaluate_and expr expr'
    | NameResolve.Divide -> evaluate_divide ctx expr expr'
    | NameResolve.EqualEqual -> evaluate_equalequal expr expr'
    | NameResolve.GreaterEqual -> evaluate_greater_equal expr expr'
    | NameResolve.Greater -> evaluate_greater expr expr'
    | NameResolve.LessEqual -> evaluate_less_equal expr expr'
    | NameResolve.Less -> evaluate_less expr expr'
    | NameResolve.Times -> evaluate_times expr expr'
    | NameResolve.NotEqual -> evaluate_not_equal expr expr'
    | NameResolve.Or -> evaluate_or expr expr'
    | NameResolve.Plus -> evaluate_plus expr expr'
    | NameResolve.Power -> evaluate_power ctx expr expr'
    | NameResolve.Minus -> evaluate_minus expr expr'

and evaluate_dynamic_identifier ctx level id =
  let rec evaluate_dynamic_identifier' inst level =
    match level, inst.enclosing_instance with
    | 0, _ -> instance_field_access ctx inst id
    | _, Some inst -> evaluate_dynamic_identifier' inst (level - 1)
    | _, None -> assert false (*error*) in
  match ctx.instance_context with
    | Some inst -> evaluate_dynamic_identifier' inst level
    | None -> assert false (*error*)

and evaluate_field_access ctx expr id =
  let expr = evaluate_expression ctx expr in
  field_access ctx expr id

and evaluate_function_argument ctx pos = match ctx.class_context with
  | FunctionEvaluationContext (_, expr, _) when pos = 0 -> expr
  | FunctionEvaluationContext (_, _, exprs) -> List.nth exprs (pos - 1)
  | ForContext (ctx', _) -> evaluate_function_argument ctx' pos
  | InstanceContext _ | ToplevelContext -> assert false (*error*)

and evaluate_function_call ctx expr exprs expr' =
  let expr = evaluate_expression ctx expr
  and exprs = List.map (evaluate_expression ctx) exprs in
  let exprs = List.map flatten_expression exprs in
  let ctx' =
    { ctx with
      class_context = FunctionEvaluationContext (ctx, expr, exprs)
    } in
  evaluate_expression ctx' expr'

and evaluate_function_invocation ctx exprs =
  let exprs = List.map (evaluate_expression ctx) exprs in
  let exprs = List.map flatten_expression exprs in
  let evaluate_function_with_arguments = function
    | ClassReference cl_def ->
        evaluate_class_function_invocation cl_def exprs
    | PredefinedIdentifier s ->
        evaluate_predefined_function_invocation ctx s exprs
    | ComponentReference _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_ComponentFuncInvocation"];
            err_info = [];
            err_ctx = ctx })
    | _ -> assert false (*error*) in
  let rec evaluate_function_invocation' ctx = match ctx.class_context with
    | FunctionEvaluationContext (_, expr, _) ->
        evaluate_function_with_arguments expr
    | ForContext (ctx', _) -> evaluate_function_invocation' ctx'
    | InstanceContext _ | ToplevelContext -> assert false (*error*) in
  evaluate_function_invocation' ctx

and evaluate_if ctx alts default =
  let create_if alts default = match alts with
    | [] -> default
    | _ :: _ -> If (alts, default) in
  let rec evaluate_alternatives alts' alts = match alts with
    | [] ->
        let default = evaluate_expression ctx default in
        create_if (List.rev alts') default
    | (expr, expr') :: alts ->
        let expr = evaluate_expression ctx expr in
        evaluate_alternative expr expr' alts' alts
  and evaluate_alternative expr expr' alts' alts = match expr with
    | True ->
        let default = evaluate_expression ctx expr' in
        create_if (List.rev alts') default
    | False -> evaluate_alternatives alts' alts
    | _ ->
        let expr' = evaluate_expression ctx expr' in
        evaluate_alternatives ((expr, expr') :: alts') alts in
  evaluate_alternatives [] alts

and evaluate_indexed_access ctx expr exprs =
  let rec vector_indexed_access exprs' exprs = match exprs with
    | [] -> expr
    | Integer i :: exprs ->
        let expr' =
          try
            exprs'.(Int32.to_int i - 1)
          with _ ->
              raise (InstantError
                { err_msg = ["_IndexOutOfBound"];
                  err_info = [];
                  err_ctx = ctx}) (*error*) in
        evaluate_indexed_access ctx expr' exprs
    | (Vector subs) :: exprs ->
        let f sub = vector_indexed_access exprs' (sub :: exprs) in
        Vector (Array.map f subs)
    | _ -> IndexedAccess (expr, exprs)
  and component_indexed_access cpnt_desc exprs =
    let rec static_array_indexed_access cpnt_descs exprs = match exprs with
      | [] -> expr
      | Integer i :: exprs ->
          let i' = Int32.to_int i in
          if Array.length cpnt_descs >= i' then
            let cpnt_desc = cpnt_descs.(i' - 1) in
            let expr' = ComponentReference cpnt_desc in
            evaluate_indexed_access ctx expr' exprs
          else
            raise (InstantError
              { err_msg = ["_IndexOutOfBound"];
                err_info = [];
                err_ctx = ctx}) (*error*)
      | (Vector subs) :: exprs ->
          let f sub = static_array_indexed_access cpnt_descs (sub :: exprs) in
          Vector (Array.map f subs)
      | exprs -> IndexedAccess (expr, exprs) in
    match evaluate cpnt_desc.component_nature with
    | DynamicArray _ -> IndexedAccess (ComponentReference cpnt_desc, exprs)
    | StaticArray cpnt_descs ->
        static_array_indexed_access cpnt_descs exprs
    | Instance _ | PredefinedTypeInstance _ -> expr in
  match expr, exprs with
  | _, [] -> expr
  | ComponentReference cpnt_desc, _ ->
      component_indexed_access cpnt_desc exprs
  | Vector exprs', _ ->
      vector_indexed_access exprs' exprs
  | If (alts, default), _ ->
      let f (cond, expr) = (cond, evaluate_indexed_access ctx expr exprs) in
      If (List.map f alts, evaluate_indexed_access ctx default exprs)
  | _ -> IndexedAccess (expr, exprs)

and evaluate_local_identifier ctx level id =
  let rec evaluate_local_identifier' ctx inst level =
    match level, ctx.parent_context with
      | 0, _ -> instance_field_access ctx inst id
      | _, Some ctx -> evaluate_local_identifier ctx (level - 1) id
      | _, None -> assert false (*error*) in
  match ctx.class_context with
    | ForContext (ctx, _) | FunctionEvaluationContext (ctx, _, _) ->
        evaluate_local_identifier ctx level id
    | InstanceContext inst -> evaluate_local_identifier' ctx inst level
    | ToplevelContext -> assert false (*error*)

and evaluate_loop_variable ctx level =
  let rec evaluate_loop_variable' ctx level' =
    match level', ctx.class_context with
      | 0, ForContext (_, None) -> assert false (*LoopVariable level'*)
      | 0, ForContext (_, Some expr) -> expr
      | _, ForContext (ctx, _) -> evaluate_loop_variable ctx (level' - 1)
      | _, FunctionEvaluationContext (ctx, _, _) ->
          evaluate_loop_variable' ctx level'
      | _, (InstanceContext _ | ToplevelContext) -> assert false (*error*) in
  evaluate_loop_variable' ctx level

and evaluate_no_event ctx expr =
  let expr = evaluate_expression ctx expr in
  match expr with
    | True | False | Integer _ | Real _ | String _ | EnumerationElement _ ->
        expr
    | _ -> NoEvent expr

and evaluate_range ctx start step stop =
  let start = evaluate_expression ctx start
  and step = evaluate_expression ctx step
  and stop = evaluate_expression ctx stop in
  let real_of_expression expr = match expr with
    | Real r -> r
    | Integer i -> Int32.to_float i
    | _ -> assert false in
  let integer_interval istart istep istop = match istart, istep, istop with
    | _
      when (Int32.compare istop istart) *
        (Int32.compare istep Int32.zero) < 0 ->
        Vector (Array.make 0 (Integer istart))
    | _ ->
        let n =
          Int32.div (Int32.sub istop istart) istep in
        let n' = Int32.to_int (Int32.succ n) in
        let f i =
          let i' = Int32.of_int i in
          let j =
            Int32.add istart (Int32.mul i' istep) in
          Integer j in
        Vector (Array.init n' f)
  and real_interval rstart rstep rstop = match rstart, rstep, rstop with
    | _ when (rstop -. rstart) /. rstep < 0. ->
        Vector (Array.make 0 (Real rstart))
    | _ ->
        let n = truncate ((rstop -. rstart) /. rstep) + 1
        and f i = Real (rstart +. float_of_int i *. rstep) in
        Vector (Array.init n f) in
  match start, step, stop with
  | _, Integer istep, _
    when Int32.compare istep Int32.zero = 0 ->
      raise (InstantError
        {err_msg = ["_RangeStepValueCannotBeNull"];
         err_info = [];
         err_ctx = ctx})
  | _, Real rstep, _ when rstep = 0. ->
      raise (InstantError
        {err_msg = ["_RangeStepValueCannotBeNull"];
         err_info = [];
         err_ctx = ctx})
  | Integer istart, Integer istep, Integer istop ->
      integer_interval istart istep istop
  | (Integer _ | Real _), (Integer _ | Real _), (Integer _ | Real _) ->
      let rstart = real_of_expression start
      and rstep = real_of_expression step
      and rstop = real_of_expression stop in
      real_interval rstart rstep rstop
  | _, _, _ -> Range (start, step, stop)

and evaluate_coercion ctx coer expr =
  let rec evaluate_real_of_integer expr' = match expr' with
    | Integer i -> Real (Int32.to_float i)
    | Vector exprs ->
        Vector (Array.map evaluate_real_of_integer exprs)
    | _ -> expr' in
  let expr' = evaluate_expression ctx expr in
  match coer with
  | NameResolve.RealOfInteger -> evaluate_real_of_integer expr'

and evaluate_toplevel_identifier ctx id =
  let elt_desc = List.assoc id (evaluate ctx.toplevel) in
  match evaluate elt_desc.element_nature with
    | Class cl_def -> ClassReference cl_def
    | Component cpnt_desc -> ComponentReference cpnt_desc

and evaluate_tuple ctx exprs =
  Tuple (List.map (evaluate_expression ctx) exprs)

and evaluate_unary_operation ctx unop expr =
  let expr = evaluate_expression ctx expr in
  let expr = flatten_expression expr in
  match unop with
    | NameResolve.Not -> evaluate_not expr
    | NameResolve.UnaryMinus -> evaluate_unary_minus expr
    | NameResolve.UnaryPlus -> expr

(*and evaluate_vector_reduction ctx ranges expr =
  let rec evaluate_vector_reduction' ctx = function
    | [] -> evaluate_expression ctx expr
    | ranges -> vector_of_reduction ctx ranges
  and vector_of_reduction ctx = function
    | Range (Integer start, Integer step, Integer stop) :: ranges ->
        vector_of_range ctx start step stop ranges
    | ranges ->
        let ctx' =
          { ctx with
            class_context = ForContext (ctx, None)
          } in
        VectorReduction (ranges, evaluate_expression ctx' expr)
  and vector_of_range ctx start step stop ranges =
    let rec expression_list pred start = match pred start with
      | true -> []
      | false ->
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some (Integer start))
            } in
          let expr = evaluate_vector_reduction' ctx' ranges in
          expr :: expression_list pred (Int32.add start step) in
    let cmp = Int32.compare step 0l in
    match cmp with
      | 0 when Int32.compare start stop <> 0 -> assert false (*error*)
      | 0 -> Vector [||]
      | _ when cmp < 0 ->
          let pred = function i -> Int32.compare i stop < 0 in
          let exprs = expression_list pred start in
          Vector (Array.of_list exprs)
      | _ ->
          let pred = function i -> Int32.compare i stop > 0 in
          let exprs = expression_list pred start in
          Vector (Array.of_list exprs) in
  let ranges = List.map (evaluate_expression ctx) ranges in
  evaluate_vector_reduction' ctx ranges*)

and evaluate_vector_reduction ctx ranges expr =
  let rec evaluate_vector_reduction' ctx = function
    | [] -> evaluate_expression ctx expr
    | ranges -> vector_of_reduction ctx ranges
  and vector_of_reduction ctx = function
    | Range (Integer u, Integer p, Integer v) :: ranges ->
        vector_of_integer_range ctx u p v ranges
    | Range (Real u, Real p, Real v) :: ranges ->
        vector_of_real_range ctx u p v ranges
    | Vector exprs :: ranges ->
        let f i =
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some exprs.(i))
            } in
          evaluate_vector_reduction' ctx' ranges in
        Vector (Array.init (Array.length exprs) f)
    | _ -> assert false
  and vector_of_integer_range ctx start step stop ranges =
    let rec expression_list pred start = match pred start with
      | true -> []
      | false ->
          let expr = Integer start in
          let ctx' =
            { ctx with
              class_context =
                ForContext (ctx, Some expr)
            } in
          let expr = evaluate_vector_reduction' ctx' ranges in
          let next = Int32.add start step in
          expr :: expression_list pred next in
    match step with
    | _ when Int32.compare step Int32.zero = 0 ->
        raise (InstantError
          {err_msg = ["_RangeStepValueCannotBeNull"];
           err_info = [];
           err_ctx = ctx})
    | _ when Int32.compare step Int32.zero < 0 ->
        let pred = function i -> (Int32.compare i stop < 0) in
        Vector (Array.of_list (expression_list pred start))
    | _ ->
        let pred = function i -> (Int32.compare i stop > 0) in
        Vector (Array.of_list (expression_list pred start))
  and vector_of_real_range ctx start step stop ranges =
    let rec expression_list pred start = match pred start with
      | true -> []
      | false ->
          let expr = Real start in
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some expr)
            } in
          let expr = evaluate_vector_reduction' ctx' ranges in
          expr :: expression_list pred (start +. step) in
    match step with
    | 0. ->
        raise (InstantError
          {err_msg = ["_RangeStepValueCannotBeNull"];
           err_info = [];
           err_ctx = ctx})
    | _ when step < 0. ->
        let pred = function f -> f < stop in
        Vector (Array.of_list (expression_list pred start))
    | _ ->
        let pred = function f -> f > stop in
        Vector (Array.of_list (expression_list pred start)) in
  let ranges = List.map (evaluate_expression ctx) ranges in
  evaluate_vector_reduction' ctx ranges

and evaluate_vector ctx exprs =
  let exprs = List.map (evaluate_expression ctx) exprs in
  Vector (Array.of_list exprs)

and evaluate_and expr expr' = match expr, expr' with
  | False, (False | True) | True, False -> False
  | True, True -> True
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_and exprs exprs')
  | _ -> BinaryOperation (And, expr, expr')

and evaluate_divide ctx expr expr' = match expr, expr' with
  | _, Integer 0l ->
      raise (InstantError
        { err_msg = ["_DivisionByZero"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer 0l, _ -> Integer 0l
  | Integer i, Integer i' ->
      Real ((Int32.to_float i) /. (Int32.to_float i'))
  | _, Real 0. ->
      raise (InstantError
        { err_msg = ["_DivisionByZero"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer i, Real f -> Real (Int32.to_float i /. f)
  | Real f, Integer i -> Real (f /. Int32.to_float i)
  | Real f, Real f' -> Real (f /. f')
  | Vector exprs, _ ->
      let divide_element expr = evaluate_divide ctx expr expr' in
      Vector (Array.map divide_element exprs)
  | _ -> BinaryOperation (Divide, expr, expr')

and evaluate_equalequal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i = i' -> True
  | Integer i, Real f | Real f, Integer i when f = Int32.to_float i -> True
  | Real f, Real f' when f = f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | Vector exprs, Vector exprs'
    when
      ArrayExt.for_all2
        (fun expr expr' -> evaluate_equalequal expr expr' = True)
        exprs
        exprs' -> True
  | Vector _, Vector _ -> False
  | _ -> BinaryOperation (EqualEqual, expr, expr')

and evaluate_greater_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i >= i' -> True
  | Integer i, Real f when Int32.to_float i >= f -> True
  | Real f, Integer i when f >= Int32.to_float i -> True
  | Real f, Real f' when f >= f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (GreaterEqual, expr, expr')

and evaluate_greater expr expr' = match expr, expr' with
  | Integer i, Integer i' when i > i' -> True
  | Integer i, Real f when Int32.to_float i > f -> True
  | Real f, Integer i when f > Int32.to_float i -> True
  | Real f, Real f' when f > f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (Greater, expr, expr')

and evaluate_less_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i <= i' -> True
  | Integer i, Real f when Int32.to_float i <= f -> True
  | Real f, Integer i when f <= Int32.to_float i -> True
  | Real f, Real f' when f <= f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (LessEqual, expr, expr')

and evaluate_less expr expr' = match expr, expr' with
  | Integer i, Integer i' when i < i' -> True
  | Integer i, Real f when Int32.to_float i < f -> True
  | Real f, Integer i when f < Int32.to_float i -> True
  | Real f, Real f' when f < f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (Less, expr, expr')

and evaluate_times expr expr' =
  let rec line exprs i = match exprs.(i) with
    | Vector exprs -> exprs
    | _ -> assert false
  and column exprs j =
    let f i = match exprs.(i) with
      | Vector exprs -> exprs.(j)
      | _ -> assert false in
    Array.init (Array.length exprs) f
  and ndims expr = match expr with
    | Vector exprs when Array.length exprs = 0 -> assert false
    | Vector exprs -> 1 + ndims exprs.(0)
    | _ -> 0
  and size expr i = match expr, i with
    | _, 0 -> assert false
    | Vector exprs, 1 -> Array.length exprs
    | _, 1 -> 0
    | Vector exprs, _ when i > 1 -> size exprs.(0) (i - 1)
    | _, _ -> assert false
  and vector_mult exprs exprs' =
    let exprs = ArrayExt.map2 evaluate_times exprs exprs' in
    match Array.length exprs with
    | 0 -> assert false
    | 1 -> exprs.(0)
    | n ->
        let exprs' = Array.sub exprs 1 (n - 1) in
        Array.fold_left evaluate_plus exprs.(0) exprs' in
  match expr, expr' with
  | Integer 0l, _ | _, Integer 0l -> Integer 0l
  | Integer 1l, _ -> expr'
  | _, Integer 1l -> expr
  | Integer i, Integer i' -> Integer (Int32.mul i i')
  | Integer i, Real f | Real f, Integer i -> Real (f *. Int32.to_float i)
  | Real f, Real f' -> Real (f *. f')
  | _, Vector exprs' when (ndims expr = 0) ->
      Vector (Array.map (evaluate_times expr) exprs')
  | Vector exprs, _ when (ndims expr' = 0) ->
      Vector (Array.map (evaluate_times expr') exprs)
  | Vector exprs, Vector exprs' when (ndims expr = 1) && (ndims expr' = 1) ->
      vector_mult exprs exprs'
  | Vector exprs, Vector exprs' when (ndims expr = 1) && (ndims expr' = 2) ->
      let f j = vector_mult exprs (column exprs' j) in
      Vector (Array.init (size expr' 2) f)
  | Vector exprs, Vector exprs' when (ndims expr = 2) && (ndims expr' = 1) ->
      let f i = vector_mult (line exprs i) exprs' in
      Vector (Array.init (size expr 1) f)
  | Vector exprs, Vector exprs' when (ndims expr = 2) && (ndims expr' = 2) ->
      let f i j = vector_mult (line exprs i) (column exprs' j) in
      let g i = Vector (Array.init (size expr' 2) (f i)) in
      Vector (Array.init (size expr 1) g)
  | _ -> BinaryOperation (Times, expr, expr')

and evaluate_not_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i <> i' -> True
  | Integer i, Real f | Real f, Integer i when f <> Int32.to_float i -> True
  | Real f, Real f' when f <> f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | Vector exprs, Vector exprs'
    when
      ArrayExt.exists2
        (fun expr expr' -> evaluate_equalequal expr expr' = False)
        exprs
        exprs' -> True
  | Vector _, Vector _ -> False
  | _ -> BinaryOperation (NotEqual, expr, expr')

and evaluate_or expr expr' = match expr, expr' with
  | True, (False | True) | False, True -> True
  | False, False -> False
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_or exprs exprs')
  | _ -> BinaryOperation (Or, expr, expr')

and evaluate_plus expr expr' = match expr, expr' with
  | Integer 0l, _ -> expr'
  | _, Integer 0l -> expr
  | Integer i, Integer i' -> Integer (Int32.add i i')
  | Integer i, Real f | Real f, Integer i -> Real (f +. Int32.to_float i)
  | Real f, Real f' -> Real (f +. f')
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_plus exprs exprs')
  | _ -> BinaryOperation (Plus, expr, expr')

and evaluate_power ctx expr expr' =
  match expr, expr' with
  | (Integer 0l | Real 0.), (Integer 0l | Real 0.) ->
      raise (InstantError
        { err_msg = ["_ZeroRaisedToTheZeroPower"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | (Integer 0l | Real 0.), Integer i'
    when Int32.compare i' 0l < 0 ->
      raise (InstantError
        { err_msg = ["_ZeroRaisedToNegativePower"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | (Integer 0l | Real 0.), Real f' when f' < 0. ->
      raise (InstantError
        { err_msg = ["_ZeroRaisedToNegativePower"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer 0l, Integer _ ->
      (* We know the answer for sure since second argument is constant *)
      Real 0.
  | (Integer 0l | Real 0.), (Integer _ | Real _) -> Real 0.
  | Integer i, Real _ when Int32.compare i 0l < 0 ->
      raise (InstantError
        { err_msg = ["_RealExponentOfNegativeNumber"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Real f, Real _ when f < 0. ->
      raise (InstantError
        { err_msg = ["_RealExponentOfNegativeNumber"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer i, Integer i' ->
      Real ((Int32.to_float i) ** (Int32.to_float i'))
  | Integer i, Real f -> Real ((Int32.to_float i) ** f)
  | Real f, Integer i' -> Real (f ** (Int32.to_float i'))
  | Real f, Real f' -> Real (f ** f')
  | Vector exprs, Integer i ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented";
                     "_VectorRaisedToIntegerPower"];
          err_info = [];
          err_ctx = ctx })
  | _ -> BinaryOperation (Power, expr, expr')

and evaluate_minus expr expr' = match expr, expr' with
  | Integer 0l, _ -> evaluate_unary_minus expr'
  | _, Integer 0l -> expr
  | Integer i, Integer i' -> Integer (Int32.sub i i')
  | Integer i, Real f -> Real (Int32.to_float i -. f)
  | Real f, Integer i -> Real (f -. Int32.to_float i)
  | Real f, Real f' -> Real (f -. f')
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_minus exprs exprs')
  | _ -> BinaryOperation (Minus, expr, expr')

and evaluate_class_function_invocation cl_def exprs =
  FunctionCall (ClassReference cl_def, exprs)

and evaluate_predefined_function_invocation ctx s exprs =
  match s, exprs with
  | "size", _ -> evaluate_size exprs
  | "reinit", [expr; expr'] -> evaluate_reinit expr expr'
  | "der", [expr] -> evaluate_der expr
  | "pre", [expr] -> evaluate_pre expr
  | ("edge" | "change" | "initial" | "terminal" | "sample" |
    "delay" | "assert" | "terminate"), _ ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_PredefinedOperator"; s];
          err_info = [];
          err_ctx = ctx}) (*error*)
  | "abs", [expr] -> evaluate_abs expr
  | "sign", [expr] -> evaluate_sign expr
  | "cos", [expr] -> evaluate_cos expr
  | "sin", [expr] -> evaluate_sin expr
  | "tan", [expr] -> evaluate_tan expr
  | "exp", [expr] -> evaluate_exp expr
  | "log", [expr] -> evaluate_log expr
  | "sqrt", [expr] -> evaluate_sqrt expr
  | "asin", [expr] -> evaluate_asin expr
  | "acos", [expr] -> evaluate_acos expr
  | "atan", [expr] -> evaluate_atan expr
  | "sinh", [expr] -> evaluate_sinh expr
  | "cosh", [expr] -> evaluate_cosh expr
  | "tanh", [expr] -> evaluate_tanh expr
  | "asinh", [expr] -> evaluate_asinh expr
  | "acosh", [expr] -> evaluate_acosh expr
  | "atanh", [expr] -> evaluate_atanh expr
  | "log10", [expr] -> evaluate_log10 expr
  | "max", [expr; expr'] -> evaluate_max expr expr'
  | "min", [expr; expr'] -> evaluate_min expr expr'
  | "div", [expr; expr'] -> evaluate_div ctx expr expr'
  | "mod", [expr; expr'] -> evaluate_mod expr expr'
  | "rem", [expr; expr'] -> evaluate_rem expr expr'
  | "ceil", [expr] -> evaluate_ceil expr
  | "floor", [expr] -> evaluate_floor expr
  | "max", [expr] -> evaluate_max_array expr
  | "min", [expr] -> evaluate_min_array expr
  | "sum", [expr] -> evaluate_sum expr
  | "product", [expr] -> evaluate_product expr
  | "scalar", [expr] -> evaluate_scalar ctx expr
  | "ones", exprs -> evaluate_ones ctx exprs
  | "zeros", exprs -> evaluate_zeros ctx exprs
  | "fill", expr :: exprs -> evaluate_fill ctx expr exprs
  | "identity", [expr] -> evaluate_identity ctx expr
  | "diagonal", [expr] -> evaluate_diagonal ctx expr
  | "vector", [ expr ] -> evaluate_vector_operator ctx expr
  | "matrix", [ expr ] -> evaluate_matrix_operator ctx expr
  | "transpose", [ expr ] -> evaluate_transpose expr
  | "symmetric", [ expr ] -> evaluate_symmetric ctx expr
  | _ ->
      raise (InstantError
        { err_msg = ["_UnknownFunction"; s];
          err_info = [];
          err_ctx = ctx}) (*error*)

and evaluate_symmetric ctx expr = match expr with
  | Vector [||] -> assert false
  | Vector exprs when size exprs.(0) 0 <> Array.length exprs ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "symmetric"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Vector exprs ->
      let f i j =
        if i > j then element i (element j expr)
        else element j (element i expr) in
      let n = Array.length exprs in
      let g i = Vector (Array.init n (f i)) in
      Vector (Array.init n g)
   | _ -> assert false

and evaluate_transpose expr =
  match expr with
  | Vector exprs  ->
      let f i = Vector (Array.map (element i) exprs) in
      Vector (Array.init (size expr 1) f)
  | _ -> assert false

and evaluate_matrix_operator ctx expr =
  let rec scalar expr = match expr with
    | Vector [| expr |] -> scalar expr
    | Vector _ ->
        raise (InstantError
          { err_msg = ["_InvalidArgOfOper"; "matrix"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | _ -> expr in
  match expr with
  | _ when ndims expr < 2 ->
      evaluate_promote ctx 2 expr
  | _ when ndims expr = 2 -> expr
  | Vector exprs ->
      let f expr = Vector (Array.map scalar (array_elements expr)) in
      Vector (Array.map f exprs)
  | _ -> assert false

and evaluate_promote ctx n expr =
  let rec evaluate_promote' i expr =
    match expr with
    | _ when i = 0 -> expr
    | Vector exprs when i > 0 ->
        Vector (Array.map (evaluate_promote' i) exprs)
    | _ when i > 0 ->
        Vector [| evaluate_promote' (i - 1) expr |]
    | _ -> assert false in
  match ndims expr with
  | n' when n' < n ->
      evaluate_promote' (n - n') expr
  | _ -> expr

and evaluate_vector_operator ctx expr =
  let rec evaluate_scalar expr = match expr with
    | Vector [| expr |] -> evaluate_scalar expr
    | Vector _ ->
        raise (InstantError
          { err_msg = ["_InvalidArgOfOper"; "vector"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | _ -> expr
  and evaluate_vector_operator' expr = match expr with
    | Vector [| expr |] -> evaluate_vector_operator' expr
    | Vector exprs ->
        Array.map evaluate_scalar exprs
    | _ -> [| expr |] in
  Vector (evaluate_vector_operator' expr)

and evaluate_max_array expr =
  let rec evaluate_max_list exprs = match exprs with
    | [] -> assert false
    | [ expr ] -> expr
    | expr :: exprs ->
        evaluate_max expr (evaluate_max_list exprs) in
  evaluate_max_list (scalar_elements expr)

and evaluate_min_array expr =
  let rec evaluate_min_list exprs = match exprs with
    | [] -> assert false
    | [ expr ] -> expr
    | expr :: exprs ->
        evaluate_min expr (evaluate_min_list exprs) in
  evaluate_min_list (scalar_elements expr)

and evaluate_sum expr =
  let rec evaluate_sum_list exprs = match exprs with
    | [] -> Integer Int32.zero
    | [ expr ] -> expr
    | expr :: exprs ->
        evaluate_plus expr (evaluate_sum_list exprs) in
  match expr with
  | Vector exprs ->
      evaluate_sum_list (scalar_elements expr)
  | _ -> assert false

and evaluate_product expr =
  let rec evaluate_product_list exprs = match exprs with
    | [] -> Integer Int32.one
    | [ expr ] -> expr
    | expr :: exprs ->
        evaluate_times expr (evaluate_product_list exprs) in
  match expr with
  | Vector exprs ->
      evaluate_product_list (scalar_elements expr)
  | _ -> assert false

and evaluate_fill ctx expr exprs =
  let rec evaluate_fill' dims = match dims with
    | [] -> expr
    | Integer i :: dims when Int32.compare i Int32.zero > 0 ->
      let i = Int32.to_int i in
        Vector (Array.make i (evaluate_fill' dims))
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "fill"];
          err_info = [];
            err_ctx = ctx }) (*error*) in
  evaluate_fill' exprs

and evaluate_zeros ctx exprs =
  let rec evaluate_zeros' dims = match dims with
    | [] -> Integer Int32.zero
    | Integer i :: dims when Int32.compare i Int32.zero > 0 ->
      let i = Int32.to_int i in
        Vector (Array.make i (evaluate_zeros' dims))
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "zeros"];
          err_info = [];
            err_ctx = ctx }) (*error*) in
  evaluate_zeros' exprs

and evaluate_ones ctx exprs =
  let rec evaluate_ones' dims = match dims with
    | [] -> Integer Int32.one
    | Integer i :: dims when Int32.compare i Int32.zero > 0 ->
      let i = Int32.to_int i in
        Vector (Array.make i (evaluate_ones' dims))
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "ones"];
          err_info = [];
            err_ctx = ctx }) (*error*) in
  evaluate_ones' exprs

and evaluate_identity ctx expr =
  let n = match expr with
    | Integer i when Int32.compare i Int32.zero > 0 ->
        Int32.to_int i
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "identity"];
          err_info = [];
            err_ctx = ctx }) (*error*) in
  let f i j =
    Integer (if j = i then Int32.one else Int32.zero) in
  let g i = Vector (Array.init n (f i)) in
  Vector (Array.init n g)

and evaluate_diagonal ctx expr =
  let exprs = match expr with
    | Vector [||] ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "diagonal"];
          err_info = [];
          err_ctx = ctx }) (*error*)
    | Vector exprs -> exprs
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "diagonal"];
          err_info = [];
            err_ctx = ctx }) (*error*) in
  let n = Array.length exprs in
  let f i j =
    if j = i then exprs.(i) else Integer Int32.zero in
  let g i = Vector (Array.init n (f i)) in
  Vector (Array.init n g)

and evaluate_scalar ctx expr =
  let rec evaluate_scalar' expr = match expr with
    | Vector [| expr |] -> evaluate_scalar' expr
    | Vector _ ->
        raise (InstantError
          { err_msg = ["_InvalidArgOfOper"; "scalar"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | _ -> expr in
  match expr with
  | Vector [| expr |] -> evaluate_scalar' expr
  | _ ->
      raise (InstantError
        { err_msg = ["_InvalidArgOfOper"; "scalar"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and evaluate_reinit expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_reinit exprs exprs')
  | _, _ ->
      FunctionCall (PredefinedIdentifier "reinit", [ expr; expr' ])

and evaluate_der expr = match expr with
  | Integer _ | String _ | Real _ -> Real 0.
  | Vector exprs -> Vector (Array.map evaluate_der exprs)
  | BinaryOperation (Plus, expr, expr') ->
      let expr = evaluate_der expr
      and expr' = evaluate_der expr' in
      BinaryOperation (Plus, expr, expr')
  | BinaryOperation (Minus, expr, expr') ->
      let expr = evaluate_der expr
      and expr' = evaluate_der expr' in
      BinaryOperation (Minus, expr, expr')
  | BinaryOperation (Times, expr1, expr2) ->
      let expr1' = evaluate_der expr1
      and expr2' = evaluate_der expr2 in
      let expr1 = BinaryOperation (Times, expr1', expr2)
      and expr2 = BinaryOperation (Times, expr1, expr2') in
      BinaryOperation (Plus, expr1, expr2)
  | BinaryOperation (Divide, expr1, expr2) ->
      let expr1' = evaluate_der expr1
      and expr2' = evaluate_der expr2 in
      let expr1' = BinaryOperation (Times, expr1', expr2)
      and expr2' = BinaryOperation (Times, expr1, expr2') in
      let expr1 = BinaryOperation (Minus, expr1', expr2')
      and expr2 = BinaryOperation (Times, expr2, expr2) in
      BinaryOperation (Divide, expr1, expr2)
  | BinaryOperation (Power, expr, Integer i) ->
      let expr' = evaluate_der expr
      and j = Int32.sub i Int32.one in
      let expr' = BinaryOperation (Times, Integer i, expr')
      and expr = BinaryOperation (Power, expr, Integer j) in
      BinaryOperation (Times, expr', expr)
  | BinaryOperation (Power, expr, Real f) ->
      let expr' = evaluate_der expr
      and f' = f -. 1. in
      let expr' = BinaryOperation (Times, Real f, expr')
      and expr = BinaryOperation (Power, expr, Real f') in
      BinaryOperation (Times, expr', expr)
  | FunctionCall (PredefinedIdentifier "cos", [ expr ]) ->
      let expr' = evaluate_der expr
      and expr = FunctionCall (PredefinedIdentifier "sin", [ expr ]) in
      let expr = UnaryOperation (UnaryMinus, expr) in
      BinaryOperation (Times, expr', expr)
  | FunctionCall (PredefinedIdentifier "sin", [ expr ]) ->
      let expr' = evaluate_der expr
      and expr = FunctionCall (PredefinedIdentifier "cos", [ expr ]) in
      BinaryOperation (Times, expr', expr)
  | FunctionCall (PredefinedIdentifier "tan", [ expr1 ]) ->
      let expr1' = evaluate_der expr1
      and expr = BinaryOperation (Times, expr, expr) in
      let expr = BinaryOperation (Plus, Real 1., expr) in
      BinaryOperation (Times, expr1', expr)
  | FunctionCall (PredefinedIdentifier "exp", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      BinaryOperation (Times, expr1', expr)
  | FunctionCall (PredefinedIdentifier "log", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      BinaryOperation (Divide, expr1', expr)
  | FunctionCall (PredefinedIdentifier "sqrt", [ expr1 ]) ->
      evaluate_der (BinaryOperation (Power, expr1, Real 0.5))
  | FunctionCall (PredefinedIdentifier "asin", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Minus, Real 1., expr1) in
      let expr1 = FunctionCall (PredefinedIdentifier "sqrt", [ expr1 ]) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "acos", [ expr1 ]) ->
      let expr1' = UnaryOperation (UnaryMinus, evaluate_der expr1) in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Minus, Real 1., expr1) in
      let expr1 = FunctionCall (PredefinedIdentifier "sqrt", [ expr1 ]) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "atan", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Plus, Real 1., expr1) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "sinh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = FunctionCall (PredefinedIdentifier "cosh", [ expr1 ]) in
      BinaryOperation (Times, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "cosh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = FunctionCall (PredefinedIdentifier "sinh", [ expr1 ]) in
      BinaryOperation (Times, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "tanh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr, expr) in
      let expr1 = BinaryOperation (Minus, Real 1., expr1) in
      BinaryOperation (Times, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "asinh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Plus, Real 1., expr1) in
      let expr1 = FunctionCall (PredefinedIdentifier "sqrt", [ expr1 ]) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "acosh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Minus, expr1, Real 1.) in
      let expr1 = FunctionCall (PredefinedIdentifier "sqrt", [ expr1 ]) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "atanh", [ expr1 ]) ->
      let expr1' = evaluate_der expr1 in
      let expr1 = BinaryOperation (Times, expr1, expr1) in
      let expr1 = BinaryOperation (Minus, expr1, Real 1.) in
      BinaryOperation (Divide, expr1', expr1)
  | FunctionCall (PredefinedIdentifier "log10", [ expr1 ]) ->
      let expr1 = FunctionCall (PredefinedIdentifier "log", [ expr1 ]) in
      BinaryOperation (Divide, evaluate_der expr1, Real (log 10.))
  | FunctionCall
      (PredefinedIdentifier ("div" | "mod" | "rem" | "ceil" | "floor"), _) ->
      Real 0.
  | If (alts, default) ->
      let alts' =
        List.map (function (cond, expr) -> (cond, evaluate_der expr)) alts in
      If (alts', evaluate_der default)
  | NoEvent expr -> NoEvent (evaluate_der expr)
  | UnaryOperation (UnaryMinus, expr) ->
      UnaryOperation (UnaryMinus, evaluate_der expr)
  | VectorReduction (exprs, expr) ->
      VectorReduction (exprs, evaluate_der expr)
  | _ -> FunctionCall (PredefinedIdentifier "der", [ expr ])

and evaluate_pre expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_pre exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "pre", [ expr ])

and evaluate_cos expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_cos exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "cos", [ expr ])

and evaluate_sin expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_sin exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "sin", [ expr ])

and evaluate_tan expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_tan exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "tan", [ expr ])

and evaluate_exp expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_exp exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "exp", [ expr ])

and evaluate_log expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_log exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "log", [ expr ])

and evaluate_sqrt expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_sqrt exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "sqrt", [ expr ])

and evaluate_asin expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_asin exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "asin", [ expr ])

and evaluate_acos expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_acos exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "acos", [ expr ])

and evaluate_atan expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_atan exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "atan", [ expr ])

and evaluate_sinh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_sinh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "sinh", [ expr ])

and evaluate_cosh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_cosh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "cosh", [ expr ])

and evaluate_tanh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_tanh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "tanh", [ expr ])

and evaluate_asinh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_asinh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "asinh", [ expr ])

and evaluate_acosh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_acosh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "acosh", [ expr ])

and evaluate_atanh expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_atanh exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "atanh", [ expr ])

and evaluate_log10 expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_log10 exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "log10", [ expr ])

and evaluate_max expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_max exprs exprs')
  | Real f, Real f' -> Real (max f f')
  | _, _ ->
      let b = BinaryOperation (GreaterEqual, expr, expr') in
      If ([b, expr], expr')

and evaluate_min expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_min exprs exprs')
  | Real f, Real f' -> Real (min f f')
  | _, _ ->
      let b = BinaryOperation (GreaterEqual, expr', expr) in
      If ([b, expr], expr')

and evaluate_abs expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_abs exprs)
  | Real f -> Real (abs_float f)
  | Integer i -> Integer (Int32.abs i)
  | _ ->
      let b = BinaryOperation (GreaterEqual, expr, Real 0.)
      and default = UnaryOperation (UnaryMinus, expr) in
      If ([b, expr], default)

and evaluate_sign expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_sign exprs)
  | Real f when f > 0. -> Real 1.
  | Real f when f < 0. -> Real (-. 1.)
  | Real _ -> Real 0.
  | Integer i when Int32.compare i Int32.zero > 0 ->
      Integer Int32.one
  | Integer i when Int32.compare i Int32.zero < 0 ->
      Integer Int32.minus_one
  | Integer _ -> Integer Int32.zero
  | _ ->
      let b = BinaryOperation (Greater, expr, Real 0.)
      and b' = BinaryOperation (Greater, Real 0., expr) in
      If ([(b, Integer Int32.one); (b', Integer Int32.minus_one)],
          Integer Int32.zero)

and evaluate_div ctx expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 (evaluate_div ctx) exprs exprs')
  | _, Real 0. ->
      raise (InstantError
        { err_msg = ["_DivisionByZero"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | _, Integer i when i = Int32.zero ->
      raise (InstantError
        { err_msg = ["_DivisionByZero"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer i, Integer i' -> Integer (Int32.div i i')
  | Real f, Integer i' ->
      let f' = Int32.to_float i' in
      Real (float_of_int (truncate (f /. f')))
  | Integer i, Real f' ->
      let f = Int32.to_float i in
      Real (float_of_int (truncate (f /. f')))
  | Real f, Real f' ->
      Real (float_of_int (truncate (f /. f')))
  | _, _ ->
      FunctionCall (PredefinedIdentifier "div", [ expr; expr' ])

and evaluate_mod expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_mod exprs exprs')
  | _, _ ->
      FunctionCall (PredefinedIdentifier "mod", [ expr; expr' ])

and evaluate_rem expr expr' = match expr, expr' with
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_rem exprs exprs')
  | _, _ ->
      FunctionCall (PredefinedIdentifier "rem", [ expr; expr' ])

and evaluate_ceil expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_ceil exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "ceil", [ expr ])

and evaluate_floor expr = match expr with
  | Vector exprs ->
      Vector (Array.map evaluate_floor exprs)
  | _ ->
      FunctionCall (PredefinedIdentifier "floor", [ expr ])

and evaluate_size exprs =
  let rec evaluate_size' expr i = match expr, i with
    | ComponentReference cpnt_desc, _ ->
        evaluate_component_size cpnt_desc i
    | Vector exprs, 1 -> Integer (Int32.of_int (Array.length exprs))
    | Vector exprs, _ -> evaluate_size' exprs.(0) (i - 1)
    | _ -> assert false (*error*)
  and evaluate_component_size cpnt_desc i =
    match evaluate cpnt_desc.component_nature, i with
      | DynamicArray _, _ -> FunctionCall (PredefinedIdentifier "size", exprs)
      | StaticArray cpnt_descs, 1 ->
          Integer (Int32.of_int (Array.length cpnt_descs))
      | StaticArray cpnt_descs, _ ->
          evaluate_component_size cpnt_descs.(i) (i - 1)
      | _ -> assert false (*error*)
  and evaluate_size_list = function
    | ComponentReference cpnt_desc -> assert false
    | Vector exprs ->
        let size = Integer (Int32.of_int (Array.length exprs)) in
        size :: evaluate_size_list exprs.(0)
    | _ -> [] in
  match exprs with
    | [expr] -> Vector (Array.of_list (evaluate_size_list expr))
    | [expr; Integer i] -> evaluate_size' expr (Int32.to_int i)
    | [expr; _] -> FunctionCall (PredefinedIdentifier "size", exprs)
    | _ -> assert false (*error*)

and evaluate_not expr = match expr with
  | True -> False
  | False -> True
  | Vector exprs -> Vector (Array.map evaluate_not exprs)
  | _ -> UnaryOperation (Not, expr)

and evaluate_unary_minus expr = match expr with
  | Integer i -> Integer (Int32.neg i)
  | Real f -> Real (~-. f)
  | Vector exprs -> Vector (Array.map evaluate_unary_minus exprs)
  | _ -> UnaryOperation (UnaryMinus, expr)

and field_access ctx expr id =
  let rec field_access' = function
    | ClassReference cl_def ->
        let cpnt_desc = create_temporary_instance ctx cl_def in
        component_field_access cpnt_desc
    | ComponentReference cpnt_desc -> component_field_access cpnt_desc
    | Record fields -> List.assoc id fields
    | Vector exprs -> Vector (Array.map field_access' exprs)
    | _ -> FieldAccess (expr, id)
  and component_field_access cpnt_desc =
    match evaluate cpnt_desc.component_nature with
      | DynamicArray _ -> FieldAccess (expr, id)
      | Instance inst -> instance_field_access ctx inst id
      | PredefinedTypeInstance _ ->
          raise (InstantError
            { err_msg = ["_CannotAccessToPredefTypeAttrib"; id];
              err_info = [];
              err_ctx = ctx}) (*error*)
      | StaticArray cpnt_descs ->
          Vector (Array.map component_field_access cpnt_descs) in
  field_access' expr

and instance_field_access ctx inst id =
  let evaluate_component cpnt_desc =
    let evaluate_declaration_equation = function
      | Some expr -> evaluate expr
      | None ->
          raise (InstantError
            { err_msg = ["_MissingDeclEquForFixedId"; id];
              err_info = [];
              err_ctx = ctx}) (*error*) in
    let rec evaluate_parameter cpnt_desc =
      let evaluate_predefined_type_instance predef =
        match evaluate (List.assoc "fixed" predef.attributes) with
        | True -> evaluate_declaration_equation cpnt_desc.declaration_equation
        | False -> ComponentReference cpnt_desc
        | _ -> assert false (*error*) in
      match evaluate cpnt_desc.component_nature with
      | PredefinedTypeInstance predef
        when List.mem_assoc "fixed" predef.attributes ->
          evaluate_predefined_type_instance predef
      | DynamicArray cpnt_desc -> assert false
      | Instance _ -> ComponentReference cpnt_desc
      | PredefinedTypeInstance _ ->
          evaluate_declaration_equation cpnt_desc.declaration_equation
      | StaticArray cpnt_descs ->
          Vector (Array.map evaluate_parameter cpnt_descs)
          (*let f i =
            let decl_equ = cpnt_descs.(i).declaration_equation in
            evaluate_declaration_equation decl_equ in
          Vector (Array.init (Array.length cpnt_descs) f)*) in
    match cpnt_desc.variability with
      | Types.Constant ->
          evaluate_declaration_equation cpnt_desc.declaration_equation
      | Types.Parameter -> evaluate_parameter cpnt_desc
      | _ -> ComponentReference cpnt_desc in
  let elts = evaluate inst.elements in
  let elt_desc = List.assoc id elts.named_elements in
  match evaluate elt_desc.element_nature with
  | Class cl_def -> ClassReference cl_def
  | Component cpnt_desc -> evaluate_component cpnt_desc

and expression_location ctx expr =
  match expr.NameResolve.info.NameResolve.syntax with
    | None -> ctx.location
    | Some expr -> expr.Syntax.info

and class_name_of_component cpnt_desc =
  let type_spec = Lazy.force cpnt_desc.NameResolve.type_specifier in
  let expr_info = type_spec.NameResolve.info in
  match expr_info.NameResolve.syntax with
  | None -> ""
  | Some expr -> Syntax.string_of_expression expr

and instance_nature_of_element elt_desc =
  match elt_desc.NameResolve.element_nature with
    | NameResolve.Component cpnt_desc ->
        ComponentElement (class_name_of_component cpnt_desc)
    | _ -> ClassElement

and instance_class_name instance_nature =
  match instance_nature with
    | ComponentElement s -> s
    | ClassElement -> ""

and flatten_expression expr =
  let rec flatten_component cpnt_desc =
    match evaluate cpnt_desc.component_nature with
    | StaticArray cpnt_descs ->
        Vector (Array.map flatten_component cpnt_descs)
    | _ -> ComponentReference cpnt_desc in
  match expr with
  | ComponentReference cpnt_desc ->
      flatten_component cpnt_desc
  | _ -> expr

and size expr i = match expr, i with
  | Vector [||], _ -> 0
  | Vector exprs, 0 -> Array.length exprs
  | Vector exprs, _ when i > 0 -> size exprs.(0) (i - 1)
  | _ -> invalid_arg "_IndexOutOfBound"

and sizes expr =
  Array.init (ndims expr) (size expr)

and ndims expr =
  let rec ndims' i expr = match expr with
    | Vector [||] -> i + 1
    | Vector exprs -> ndims' (i + 1) exprs.(0)
    | _ -> i in
  ndims' 0 expr

and element i expr = match expr with
  | Vector exprs -> exprs.(i)
  | _ -> assert false

and array_elements expr = match expr with
  | Vector exprs -> exprs
  | _ -> assert false

and scalar_elements expr = match expr with
  | Vector exprs ->
      let exprss =
        Array.to_list (Array.map scalar_elements exprs) in
      List.flatten exprss
  | _ -> [ expr ]

(* for debug*)

and generate_expression oc = function
  | BinaryOperation (bin_op, expr, expr') ->
      generate_binary_operation oc bin_op expr expr'
  | ClassReference cl_def ->
      generate_class_reference oc cl_def
  | ComponentReference cpnt_desc ->
      generate_component_reference oc cpnt_desc
  | EnumerationElement _ -> assert false
  | False -> assert false
  | FieldAccess _ -> assert false
  | FunctionCall (expr, exprs) ->
      generate_function_call oc expr exprs
  | If (alts, expr) -> generate_if oc alts expr
  | IndexedAccess _ -> assert false
  | Integer i when Int32.to_int i >= 0 ->
      Printf.fprintf oc "%ld" i
  | Integer i ->
      let expr = Integer (Int32.neg i)
      and un_op = UnaryMinus in
      generate_unary_operation oc un_op expr
  | LoopVariable _ -> Printf.fprintf oc "LoopVariable"
  | NoEvent expr -> generate_no_event oc expr
  | PredefinedIdentifier id -> Printf.fprintf oc "%s" id
  | Range _ -> Printf.fprintf oc "Range"
  | Real f ->
      Printf.fprintf oc "%s" (string_of_float f)
  | Record _ -> Printf.fprintf oc "Record"
  | String _ -> Printf.fprintf oc "String"
  | True -> Printf.fprintf oc "True"
  | Tuple _ -> Printf.fprintf oc "Tuple"
  | UnaryOperation (un_op, expr) ->
      generate_unary_operation oc un_op expr
  | Vector exprs ->
      generate_vector oc exprs
  | VectorReduction _ -> Printf.fprintf oc "VectorReduction"

and generate_binary_operation oc bin_op expr expr' =
  let string_of_binary_operation_kind = function
    | And -> "and"
    | Divide -> "/"
    | EqualEqual -> "=="
    | GreaterEqual -> ">="
    | Greater -> ">"
    | LessEqual -> "<="
    | Less -> "<"
    | Times -> "*"
    | NotEqual -> "<>"
    | Or -> "or"
    | Plus -> "+"
    | Power -> "^"
    | Minus -> "-" in
  Printf.fprintf oc "(";
  generate_expression oc expr;
  Printf.fprintf oc " %s " (string_of_binary_operation_kind bin_op);
  generate_expression oc expr';
  Printf.fprintf oc ")"

and generate_class_reference oc cl_def =
  let rec last = function
    | [] -> assert false
    | [Name id] -> id
    | [Index _] -> assert false
    | _ :: path -> last path in
  let generate_external_call ext_call =
    match ext_call.NameResolve.nature with
      | NameResolve.PrimitiveCall "builtin" ->
          Printf.fprintf oc "builtin"
      | NameResolve.PrimitiveCall "C" ->
          Printf.fprintf oc "PrimitiveCall"
      | NameResolve.PrimitiveCall lang -> assert false
      | NameResolve.ExternalProcedureCall _ -> assert false in
  let generate_long_dscription long_desc =
    match evaluate long_desc.NameResolve.external_call with
      | None -> assert false
      | Some ext_call -> generate_external_call ext_call in
  match cl_def.description with
    | ClassDescription (_, cl_desc) ->
        generate_long_dscription cl_desc.long_description
    | PredefinedType _ -> assert false

and generate_component_reference oc cpnt_desc =
  let name = ident_of_path cpnt_desc.component_path in
  Printf.fprintf oc "%s" name

and generate_function_call oc expr exprs =
  generate_expression oc expr;  
  Printf.fprintf oc "(";
  generate_expressions oc exprs;
  Printf.fprintf oc ")"

and generate_expressions oc = function
  | [] -> ()
  | [expr] -> generate_expression oc expr;
  | expr :: exprs ->
      generate_expression oc expr;
      Printf.fprintf oc ", ";
      generate_expressions oc exprs

and generate_if oc alts expr =
  let rec generate_alternatives = function
    | [] -> Printf.fprintf oc " "; generate_expression oc expr
    | (expr, expr') :: alts ->
        Printf.fprintf oc "(if ";
        generate_expression oc expr;
        Printf.fprintf oc " then ";
        generate_expression oc expr';
        Printf.fprintf oc " else";
        generate_alternatives alts;
        Printf.fprintf oc ")" in
  generate_alternatives alts

and generate_no_event oc expr =
  Printf.fprintf oc "noEvent(";
  generate_expression oc expr;
  Printf.fprintf oc ")"

and generate_unary_operation oc un_op expr =
  let string_of_unary_operation_kind = function
    | Not -> "not"
    | UnaryMinus -> "-" in
  Printf.fprintf oc "(%s " (string_of_unary_operation_kind un_op);
  generate_expression oc expr;
  Printf.fprintf oc ")"

and generate_vector oc exprs =
  let exprs' = Array.to_list exprs in
  Printf.fprintf oc "{ ";
  generate_expressions oc exprs';
  Printf.fprintf oc " }"

and last_id path =
  let rec last_id' id path = match path with
    | [] -> id
    | (Name id) :: path -> last_id' id path
    | (Index _) :: path -> last_id' id path in
  last_id' "" path

and string_of_float f =
  let add_parenthesis s =
    if String.contains s '-' then Printf.sprintf "(%s)" s else s in
  match Printf.sprintf "%.16g" f with
  | s when (String.contains s '.') || (String.contains s 'e') ->
      add_parenthesis s
  | s -> add_parenthesis (Printf.sprintf "%s." s)

and ident_of_path path =
  let rec ident_of_path' path =
    match path with
    | [] -> assert false
    | [Name id] -> id
    | [Index i] -> Printf.sprintf "[%d]" (i + 1)
    | Name id :: path ->
        Printf.sprintf "%s.%s" id (ident_of_path' path)
    | Index i :: path ->
        Printf.sprintf "[%d].%s" (i + 1) (ident_of_path' path) in
  match path with
  | [] -> assert false
  | [Name id] -> assert false
  | [Index i] -> assert false
  | Name id :: path ->
      Printf.sprintf "`%s`" (ident_of_path' path)
  | Index i :: path -> assert false

