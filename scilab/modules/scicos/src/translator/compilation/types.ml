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



type element_type =
  {
    protected: bool;
    final: bool;
    replaceable: bool;
    dynamic_scope: dynamic_scope option;
    element_nature: element_nature
  }

and class_type =
  {
    partial: bool;
    kind: kind Lazy.t;
    named_elements: (string * element_type Lazy.t) list
  }

and kind =
  | Class
  | Model
  | Block
  | Record
  | ExpandableConnector
  | Connector
  | Package
  | Function

and dynamic_scope =
  | Inner
  | Outer
  | InnerOuter

and element_nature =
  | ComponentElement of component_type
  | ClassElement of class_specifier Lazy.t
  | ComponentTypeElement of component_type
  | PredefinedTypeElement of predefined_type

and component_type =
  {
    flow: bool Lazy.t;
    variability: variability Lazy.t;
    causality: causality Lazy.t;
    base_class: class_specifier Lazy.t;
  }

and variability = Continuous | Discrete | Parameter | Constant

and causality = Acausal | Input | Output

and class_specifier =
  | PredefinedType of predefined_type
  | ClassType of class_type
  | ComponentType of component_type
  | ArrayType of dimension * class_specifier
  | TupleType of class_specifier list

and predefined_type =
  {
    base_type: base_type;
    attributes: (string * bool) list
 }

and base_type =
  | BooleanType
  | IntegerType
  | RealType
  | StringType
  | EnumerationType of string list

and dimension =
  | ConstantDimension of int32
  | ParameterDimension
  | DiscreteDimension

type type_comparison =
  | NotRelated
  | Subtype
  | Supertype
  | SameType


(* Useful functions *)

let evaluate x = Lazy.force x

(* type calculations *)

let min_variability var var' = match var, var' with
  | Constant, _ | _, Constant -> Constant
  | Parameter, _ | _, Parameter -> Parameter
  | Discrete, _ | _, Discrete -> Discrete
  | Continuous, Continuous -> Continuous

and max_variability var var' = match var, var' with
  | Continuous, _ | _, Continuous -> Continuous
  | Discrete, _ | _, Discrete -> Discrete
  | Parameter, _ | _, Parameter -> Parameter
  | Constant, Constant -> Constant

let higher_variability var var' =
  (max_variability var var') = var

and lower_variability var var' =
  (max_variability var var') = var'

let add_dimensions dims cl_spec =
  let add_dimension dim cl_spec = ArrayType (dim, cl_spec) in
  List.fold_right add_dimension dims cl_spec

(* Utilities *)

let empty_tuple_class_type = TupleType []

let boolean_class_type =
  PredefinedType { base_type = BooleanType; attributes = ["start", false] }

and integer_class_type =
  PredefinedType
    { base_type = IntegerType; attributes = ["start", false; "nominal", false] }

and real_class_type =
  PredefinedType
    { base_type = RealType; attributes = ["start", false; "nominal", false] }

and string_class_type =
  PredefinedType { base_type = StringType; attributes = ["start", false] }

and enumeration_class_type enum_lits =
  PredefinedType
    { base_type = EnumerationType enum_lits; attributes = ["start", false] }

let boolean_component_type var =
  {
    flow = lazy false;
    variability = lazy var;
    causality = lazy Acausal;
    base_class = lazy boolean_class_type;
  }

let integer_component_type var =
  { (boolean_component_type var) with
    base_class = lazy integer_class_type
  }

let real_component_type var =
  { (boolean_component_type var) with
    base_class = lazy real_class_type
  }

let string_component_type var =
  { (boolean_component_type var) with
    base_class = lazy string_class_type
  }

let enumeration_component_type var enum_lits =
  { 
    (boolean_component_type var) with
    base_class = lazy (enumeration_class_type enum_lits)
  }

let integer_array_component_type var dims =
  let cl_spec = integer_class_type in
  {
    flow = lazy false;
    variability = lazy var;
    causality = lazy Acausal;
    base_class = lazy (add_dimensions dims cl_spec)
  }

let empty_tuple_type var =
  ComponentElement
    { (boolean_component_type var) with
      base_class = lazy (empty_tuple_class_type)
    }

let boolean_type var = ComponentElement (boolean_component_type var)

let integer_type var = ComponentElement (integer_component_type var)

let integer_array_type var dim =
  let cl_spec =
    ArrayType
      (dim,
       PredefinedType { base_type = IntegerType; attributes = [] }) in
  let cpnt_type =
    {
      flow = lazy false;
      variability = lazy var;
      causality = lazy Acausal;
      base_class = lazy cl_spec
    } in
  ComponentElement cpnt_type

let real_type var = ComponentElement (real_component_type var)

let string_type var =
  ComponentElement (string_component_type var)

let enumeration_type var enum_lits =
  ComponentElement (enumeration_component_type var enum_lits)

let function_type inputs outputs =
  let named_elements inout args =
    let element_type cpnt_type =
      {
        protected = false;
        final = true;
        replaceable = false;
        dynamic_scope = None;
        element_nature =
          ComponentElement { cpnt_type with causality = lazy inout }
      } in
    let named_element (id, cpnt_type) = id, lazy (element_type cpnt_type) in
    List.map named_element args in
  let cl_type =
    {
      partial = false;
      kind = lazy Function;
      named_elements =
        named_elements Input inputs @ named_elements Output outputs
    } in
  ClassElement (lazy (ClassType cl_type))

let reversed_element_dimensions elt_type =
  let rec reversed_dimensions dims = function
    | ArrayType (dim, cl_spec) -> reversed_dimensions (dim :: dims) cl_spec
    | PredefinedType _ | ClassType _ | ComponentType _ | TupleType _ -> dims in
  match elt_type with
    | ComponentElement cpnt_type ->
        let cl_spec = evaluate cpnt_type.base_class in
        reversed_dimensions [] cl_spec
    | ClassElement _ | ComponentTypeElement _ | PredefinedTypeElement _ -> []

let scalar_component_type cpnt_type =
  let rec scalar_class_specifier cl_spec = match cl_spec with
    | ArrayType (_, cl_spec) -> scalar_class_specifier cl_spec
    | _ -> cl_spec in
  {
    cpnt_type with
    base_class = lazy (scalar_class_specifier (evaluate cpnt_type.base_class))
  }


(* General type comparisons *)

let rec compare_class_types ct ct' =
  match Lazy.force ct.kind, Lazy.force ct'.kind with
  | Class, Class -> compare_classes ct ct'
  | Model, Model -> compare_models ct ct'
  | Block, Block -> compare_blocks ct ct'
  | Record, Record -> compare_records ct ct'
  | ExpandableConnector, ExpandableConnector ->
      compare_expandable_connectors ct ct'
  | Connector, Connector -> compare_connectors ct ct'
  | Package, Package -> compare_packages ct ct'
  | Function, Function -> compare_functions ct ct'
  | _ -> NotRelated

and compare_classes ct ct' =
  let rec compare_classes' type_cmp named_elts named_elts' =
    match named_elts' with
    | [] -> type_cmp
    | (s', _) :: _ when not (List.mem_assoc s' named_elts) -> NotRelated
    | (s', elt_type') :: named_elts' ->
        begin
          let type_cmp' =
            compare_elements
              (Lazy.force (List.assoc s' named_elts))
              (Lazy.force elt_type') in
          match type_cmp, type_cmp' with
            | SameType, (SameType | Subtype) ->
                compare_classes' type_cmp' named_elts named_elts'
            | Subtype, (SameType | Subtype) ->
                compare_classes' Subtype named_elts named_elts'
            | _ -> NotRelated
        end in
  let named_elts = ct.named_elements
  and named_elts' = ct'.named_elements in
  let l = List.length named_elts
  and l' = List.length named_elts' in
  if l < l' then invert (compare_classes' Subtype named_elts' named_elts)
  else if l = l' then compare_classes' SameType named_elts named_elts'
  else compare_classes' Subtype named_elts named_elts'

and invert = function
  | NotRelated -> NotRelated
  | Subtype -> Supertype
  | Supertype -> Subtype
  | SameType -> SameType

and compare_models ct ct' = compare_classes ct ct'

and compare_blocks ct ct' = compare_classes ct ct'

and compare_records ct ct' = compare_classes ct ct'

and compare_expandable_connectors ct ct' = compare_classes ct ct'

and compare_connectors ct ct' = compare_classes ct ct'

and compare_packages ct ct' = compare_classes ct ct'

and compare_functions ct ct' = compare_classes ct ct'

and compare_elements elt_type elt_type' =
  if
    elt_type.protected = elt_type'.protected &&
    elt_type.final = elt_type'.final &&
    elt_type.replaceable = elt_type'.replaceable &&
    elt_type.dynamic_scope = elt_type'.dynamic_scope
  then compare_element_natures elt_type.element_nature elt_type'.element_nature
  else NotRelated

and compare_element_natures elt_nature elt_nature' = match elt_nature, elt_nature' with
  | ComponentElement cpntt, ComponentElement cpntt' -> compare_component_types cpntt cpntt'
  | ClassElement cs, ClassElement cs' -> compare_specifiers (Lazy.force cs) (Lazy.force cs')
  | ComponentTypeElement cpntt, ComponentTypeElement cpntt' -> compare_component_types cpntt cpntt'
  | PredefinedTypeElement pt, PredefinedTypeElement pt' -> compare_predefined_types pt pt'
  | (ComponentElement _ | ClassElement _ | ComponentTypeElement _ | PredefinedTypeElement _),
    (ComponentElement _ | ClassElement _ | ComponentTypeElement _ | PredefinedTypeElement _) ->
      NotRelated

and compare_component_types cpntt cpntt' =
  (*if
    Lazy.force cpntt.flow = Lazy.force cpntt'.flow &&
    Lazy.force cpntt.variability = Lazy.force cpntt'.variability &&
    Lazy.force cpntt.causality = Lazy.force cpntt'.causality
  then*)
    compare_specifiers (Lazy.force cpntt.base_class) (Lazy.force cpntt'.base_class)
  (*else NotRelated*)

and compare_specifiers cs cs' = match cs, cs' with
  | PredefinedType pt, PredefinedType pt' -> compare_predefined_types pt pt'
  | ClassType ct, ClassType ct' -> compare_class_types ct ct'
  | ComponentType cpntt, ComponentType cpntt' -> compare_component_types cpntt cpntt'
  | ArrayType (dim, cs), ArrayType (dim', cs')
    when compare_dimensions dim dim' ->
      compare_specifiers cs cs'
  | TupleType css, TupleType css' -> compare_tuple_types css css'
  | (PredefinedType _ | ClassType _ | ComponentType _ | ArrayType _ | TupleType _),
    (PredefinedType _ | ClassType _ | ComponentType _ | ArrayType _ | TupleType _) ->
      NotRelated

and compare_dimensions dim dim' = match dim, dim' with
  | ConstantDimension i, ConstantDimension i' when i <> i' -> false
  | _ -> true

and compare_tuple_types css css' =
  if List.for_all2 (fun cs cs' -> compare_specifiers cs cs' = SameType) css css' then
    SameType
  else NotRelated

and compare_predefined_types pt pt' = match pt.base_type, pt'.base_type with
  | BooleanType, BooleanType -> SameType
  | IntegerType, IntegerType -> SameType
  | RealType, RealType -> SameType
  | RealType, IntegerType -> Supertype
  | IntegerType, RealType -> Subtype
  | StringType, StringType -> SameType
  | EnumerationType enum_elts, EnumerationType enum_elts'
    when enum_elts = enum_elts' -> SameType
  | _ -> NotRelated

(* Printing utilities *)

let fprint_tabs oc offset =
  for i = 1 to offset do Printf.fprintf oc "\t" done

let rec fprint_class_type oc id cl_type =
  if cl_type.partial then Printf.fprintf oc "partial ";
  fprint_kind oc (Lazy.force cl_type.kind);
  Printf.fprintf oc "%s\n" id;
  fprint_named_elements oc 1 cl_type.named_elements;
  Printf.fprintf oc "end %s;\n" id

and fprint_kind oc = function
  | Class -> Printf.fprintf oc "class "
  | Model -> Printf.fprintf oc "model "
  | Block -> Printf.fprintf oc "block "
  | Record -> Printf.fprintf oc "record "
  | ExpandableConnector -> Printf.fprintf oc "expandable connector "
  | Connector -> Printf.fprintf oc "connector "
  | Package -> Printf.fprintf oc "package "
  | Function -> Printf.fprintf oc "function "

and fprint_named_elements oc offset named_elts =
  List.iter
    (function (s, elt_type) -> fprint_named_element oc offset (s, Lazy.force elt_type))
    named_elts

and fprint_named_element oc offset (id, elt_type) =
  fprint_tabs oc offset;
  if elt_type.protected then Printf.fprintf oc "protected ";
  if elt_type.final then Printf.fprintf oc "final ";
  if elt_type.replaceable then Printf.fprintf oc "replaceable ";
  fprint_dynamic_scope oc elt_type.dynamic_scope;
  fprint_element_nature oc offset id elt_type.element_nature

and fprint_dynamic_scope oc = function
  | None -> ()
  | Some Inner -> Printf.fprintf oc "inner "
  | Some Outer -> Printf.fprintf oc "outer "
  | Some InnerOuter -> Printf.fprintf oc "inner outer "

and fprint_element_nature oc offset id = function
  | ComponentElement cpnt_type -> fprint_component_type oc offset id cpnt_type
  | ClassElement cl_spec -> fprint_class_specifier oc offset id (Lazy.force cl_spec)
  | ComponentTypeElement cpnt_type -> fprint_component_type_type oc offset id cpnt_type
  | PredefinedTypeElement predef_type -> fprint_predefined_type_type oc id predef_type

and fprint_class_specifier oc offset id = function
  | PredefinedType _ -> assert false
  | ClassType cl_type -> fprint_class_type_specifier oc offset id cl_type
  | ComponentType _ -> assert false
  | ArrayType (_, cs) -> fprint_class_specifier oc offset id cs
  | TupleType _ -> assert false

and fprint_class_type_specifier oc offset id cl_type =
  if cl_type.partial then Printf.fprintf oc "partial ";
  fprint_kind oc (Lazy.force cl_type.kind);
  Printf.fprintf oc "%s\n" id;
  fprint_named_elements oc (offset + 1) cl_type.named_elements;
  fprint_tabs oc offset;
  Printf.fprintf oc "end %s;\n" id

and fprint_component_type_type oc offset id cpnt_type =
  Printf.fprintf oc "type %s = " id;
  fprint_component_type oc offset "" cpnt_type;
  Printf.fprintf oc ";\n"

and fprint_predefined_type_type oc id predef_type =
  Printf.fprintf oc "type %s = " id;
  fprint_predefined_type oc predef_type;
  Printf.fprintf oc ";\n"

and fprint_component_type oc offset id cpnt_type =
  if Lazy.force cpnt_type.flow then Printf.fprintf oc "flow ";
  fprint_variability oc (Lazy.force cpnt_type.variability);
  fprint_causality oc (Lazy.force cpnt_type.causality);
  fprint_class_specifier_type oc offset (Lazy.force cpnt_type.base_class);
  fprint_dimensions oc (Lazy.force cpnt_type.base_class);
  Printf.fprintf oc " %s;\n" id

and fprint_variability oc = function
  | Continuous -> ()
  | Discrete -> Printf.fprintf oc "discrete "
  | Parameter -> Printf.fprintf oc "parameter "
  | Constant -> Printf.fprintf oc "constant "

and fprint_causality oc = function
  | Acausal -> ()
  | Input -> Printf.fprintf oc "input "
  | Output -> Printf.fprintf oc "output "

and fprint_class_specifier_type oc offset = function
  | PredefinedType predef_type -> fprint_predefined_type oc predef_type
  | ClassType cl_type -> fprint_class_type_specifier_type oc offset cl_type
  | ComponentType cpnt_type -> fprint_component_type_specifier_type oc offset cpnt_type
  | ArrayType (_, cs) -> fprint_class_specifier_type oc offset cs
  | TupleType _ -> assert false

and fprint_predefined_type oc predef_type = match predef_type.base_type with
  | BooleanType -> Printf.fprintf oc "Boolean"
  | IntegerType -> Printf.fprintf oc "Integer"
  | RealType -> Printf.fprintf oc "Real"
  | StringType -> Printf.fprintf oc "String"
  | EnumerationType enum_elts -> fprint_enumeration_type oc enum_elts

and fprint_enumeration_type oc ss =
  let rec fprint_enumeration_type' = function
    | [] -> ()
    | [s] -> Printf.fprintf oc "%s" s
    | s :: ss -> Printf.fprintf oc "%s, " s; fprint_enumeration_type' ss in
  Printf.fprintf oc "enumeration(";
  fprint_enumeration_type' ss;
  Printf.fprintf oc ")"

and fprint_class_type_specifier_type oc offset cl_type =
  if cl_type.partial then Printf.fprintf oc "partial ";
  fprint_kind oc (Lazy.force cl_type.kind);
  Printf.fprintf oc "_\n";
  fprint_named_elements oc (offset + 1) cl_type.named_elements;
  fprint_tabs oc offset;
  Printf.fprintf oc "end _"

and fprint_component_type_specifier_type oc offset cpnt_type =
  Printf.fprintf oc "(";
  if Lazy.force cpnt_type.flow then Printf.fprintf oc "flow ";
  fprint_variability oc (Lazy.force cpnt_type.variability);
  fprint_causality oc (Lazy.force cpnt_type.causality);
  fprint_class_specifier_type oc offset (Lazy.force cpnt_type.base_class);
  fprint_dimensions oc (Lazy.force cpnt_type.base_class);
  Printf.fprintf oc ")"

and fprint_dimensions oc cs =
  let fprint_dimension = function
    | ConstantDimension d -> Printf.fprintf oc "%ld" d
    | ParameterDimension -> Printf.fprintf oc "p"
    | DiscreteDimension -> Printf.fprintf oc ":" in
  let rec fprint_dimensions' dim = function
    | PredefinedType _ | ClassType _ | ComponentType _ | TupleType _ ->
        fprint_dimension dim
    | ArrayType (dim', cs') ->
        fprint_dimension dim;
        Printf.fprintf oc ", ";
        fprint_dimensions' dim' cs' in
  match cs with
    | PredefinedType _ | ClassType _ | ComponentType _ | TupleType _ -> ()
    | ArrayType (dim, cs) ->
        Printf.fprintf oc "["; fprint_dimensions' dim cs; Printf.fprintf oc "]"

(* String conversion utilities *)

let rec string_of_kind kind = match kind with
  | Class -> "class "
  | Model -> "model "
  | Block -> "block "
  | Record -> "record "
  | ExpandableConnector -> "expandable connector "
  | Connector -> "connector "
  | Package -> "package "
  | Function -> "function "

and string_of_dynamic_scope dyn_scope = match dyn_scope with
  | None -> ""
  | Some Inner -> "inner "
  | Some Outer -> "outer "
  | Some InnerOuter -> "inner outer "

and string_of_class_specifier cl_spec =
  let string_of_dimension dim = match dim with
    | ConstantDimension d -> Int32.to_string d
    | ParameterDimension -> "p"
    | DiscreteDimension -> ":" in
  let string_of_dimensions dims =
    let rec string_of_dimensions' dims = match dims with
      | [] -> ""
      | [dim] -> string_of_dimension dim
      | dim :: dims ->
          (string_of_dimension dim) ^ ", " ^ (string_of_dimensions' dims) in
    match dims with
      | [] -> ""
      | _ -> "[" ^ (string_of_dimensions' dims) ^ "]" in
  let rec string_of_class_specifier' dims cl_spec = match cl_spec with
    | PredefinedType predef_type ->
        (string_of_predefined_type predef_type) ^
        (string_of_dimensions dims)
    | ClassType cl_type ->
        (string_of_class_type cl_type) ^
        (string_of_dimensions dims) 
    | ComponentType cpnt_type ->
        (string_of_component_type cpnt_type) ^
        (string_of_dimensions dims) 
    | ArrayType (dim, cs) ->
        string_of_class_specifier' (dim :: dims) cs
    | TupleType cl_specs ->
        "(" ^ (string_of_tuple_type cl_specs) ^ ")" ^
        (string_of_dimensions dims) in
  string_of_class_specifier' [] cl_spec

and string_of_tuple_type cl_specs = match cl_specs with
  | [] -> ""
  | [cl_spec] -> string_of_class_specifier cl_spec
  | cl_spec :: cl_specs ->
      (string_of_class_specifier cl_spec) ^ ", " ^
      (string_of_tuple_type cl_specs)

and string_of_class_type cl_type =
  string_of_kind (Lazy.force cl_type.kind)

and string_of_component_type cpnt_type =
  string_of_class_specifier (Lazy.force cpnt_type.base_class)

and string_of_variability var = match var with
  | Continuous -> "continuous"
  | Discrete -> "discrete"
  | Parameter -> "parameter"
  | Constant -> "constant"

and string_of_causality c = match c with
  | Acausal -> ""
  | Input -> "input"
  | Output -> "output"

and string_of_predefined_type predef_type =
  string_of_base_type predef_type.base_type

and string_of_base_type base_type = match base_type with
  | BooleanType -> "Boolean"
  | IntegerType -> "Integer"
  | RealType -> "Real"
  | StringType -> "String"
  | EnumerationType enum_elts -> string_of_enumeration_type enum_elts

and string_of_enumeration_type ss =
  let rec string_of_enumeration_type' ss = match ss with
    | [] -> ""
    | [s] -> s
    | s :: ss -> s ^ ", " ^ (string_of_enumeration_type' ss) in
  "enumeration(" ^ (string_of_enumeration_type' ss) ^ ")"

and string_of_element_nature = function
  | ComponentElement _ -> "_ComponentElement"
  | ClassElement _ -> "_ClassElement"
  | ComponentTypeElement _ -> "_ComponentTypeElement"
  | PredefinedTypeElement _ -> "_PredefinedTypeElement"
