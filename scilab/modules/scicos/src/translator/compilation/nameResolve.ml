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

(** Resolution of types for Modelica elements from the abstract syntax tree.
The main functions are:
{ul
{- [ resolve_toplevel ]: Main function. Resolve a list of top level syntax element}
{- [ resolve_variable_definition ]: Resolution of a variable definition}
{- [ resolve_class_definition ]: Resolution of a class definition}
{- [ resolve_modification ]: Resolution of modifications}
{- [ resolve_expression ]: Resolution of syntax expressions
  {ul
  {- [ resolve_binary_operation ]: Resolve binary operation expression }
  {- [ resolve_unuary_operation ]: Resolve unary operation }
  {- [ resolve_identifier ]: scoping algorithm and resolution of identifiers}
  {- [ apply_binary_coercions, apply_rhs_coercions ]: Adding of Real to Integer implicit coercions}
  {- [ resolve_function_call ]: Resolution of a function call expression }
  {- [ resolve_field_access ]: Resolve field access }
  {- [ resolve_if ]: Resolve [ if ] expression }
  {- [ resolve_indexed_access ]: Resolve indexed access }
  {- [ resolve_vector ]: Resolve vector expression }
  {- [ resolve_range ]: resolve range expression }
  }
}
{- [ resolve_equation ]: Resolution of an equation
  {ul
  {- [ resolve_equal ]: Resolution of a simple equation of the form [ expr1 = expr2 ] }
  {- [ resolve_conditional_equation_e ]: Resolution of conditional equations }
  {- [ resolve_for_clause_e ]: Resolution of for equations }
  {- [ resolve_connect_clause ]: resolution of connect equations }
  {- [ resolve_when_clause_e ]: resolution of when equations}
  {- [ equations ]: resolution of array, record and for equations
  }
}
}
*)

(* The type [ node ] is used to attach syntax information to resolved elements *)
type ('a, 'b) node =
  {
    nature: 'a;
    info: 'b
  }

(* Type of resolved elements *)

and element_description =
  {
    element_type: Types.element_type Lazy.t;
    redeclare: bool;
    element_nature: element_nature;
    element_location: Parser.location
  }

and element_nature =
  | Component of component_description
  | Class of class_definition
  | ComponentType of component_type_description
  | PredefinedType of Types.predefined_type

and component_description =
  {
    component_type: Types.component_type Lazy.t;
    type_specifier: expression Lazy.t;
    dimensions: dimension list Lazy.t;
    modification: modification option Lazy.t;
    comment: string
  }

and dimension =
  | Colon
  | Expression of expression

and class_definition =
  {
    class_type: Types.class_specifier Lazy.t;
    enclosing_class: class_definition option;
    encapsulated: bool;
    description: class_description Lazy.t;
  }

and class_description =
  | LongDescription of long_description
  | ShortDescription of modified_class

and long_description =
  {
    class_annotations: (annotation list) Lazy.t;
    imports: import_description list;
    extensions: (visibility * modified_class) list;
    named_elements: named_element list;
    unnamed_elements: equation_or_algorithm_clause list Lazy.t;
    external_call: external_call option Lazy.t
  }

and annotation =
  | InverseFunction of inverse_function Lazy.t
  | UnknownAnnotation of (Parser.location Syntax.class_modification) Lazy.t

and inverse_function =
  {
    function_class: expression;
    arguments: (string * string) list
  }

and import_description = unit

and visibility = Public | Protected

and named_element = string * element_description

and modified_class =
  {
    modified_class_type: Types.class_type Lazy.t;
    base_class: expression Lazy.t;
    class_modification: class_modification Lazy.t
  }

and component_type_description =
  {
    described_type: Types.component_type Lazy.t;
    base_type: expression Lazy.t;
    type_dimensions: dimension list Lazy.t;
    type_modification: class_modification Lazy.t
  }

and external_call = (external_call_desc, Parser.location Syntax.externalll) node

and external_call_desc =
  | PrimitiveCall of string
  | ExternalProcedureCall of language *
      expression option (* rhs *) * string * expression list

and language = C | FORTRAN

and modification =
  | Modification of class_modification * expression Lazy.t option
  | Assignment of expression Lazy.t
  | Equality of expression Lazy.t

and class_modification = modification_argument list

and modification_argument =
  {
    each: bool;
    final: bool;
    target: string;
    action: modification_action option
  }

and modification_action =
  | ElementModification of modification
  | ElementRedeclaration of element_description

(* Type of equations and algorithms *)

and equation_or_algorithm_clause =
  | EquationClause of validity * equation list
  | AlgorithmClause of validity * algorithm list

and validity = Initial | Permanent

and equation = (equation_desc, Parser.location Syntax.equation option) node

and equation_desc =
  | Equal of expression * expression
  | ConditionalEquationE of (expression * equation list) list * equation list
  | ForClauseE of expression list (* ranges *) * equation list
  | ConnectFlows of sign * expression * sign * expression
  | WhenClauseE of (expression * equation list) list

and sign = Positive | Negative

and algorithm = (algorithm_desc, Parser.location Syntax.algorithm option) node

and algorithm_desc =
  | Assign of expression * expression
  | FunctionCallA of expression * expression list
  | MultipleAssign of expression list * expression * expression list
  | Break
  | Return
  | ConditionalEquationA of (expression * algorithm list) list *
      algorithm list
  | ForClauseA of expression list (* ranges *) * algorithm list
  | WhileClause of expression * algorithm list
  | WhenClauseA of (expression * algorithm list) list

(* Type of expressions *)

and expression = (expression_desc, expression_information) node

(* Type of a resolved expression:
- [ syntax ]: expression syntax (this information is optional, some expressions
  are dynamicaly created during typing analysis)
- [ type_description ]: expression type *)
and expression_information =
  {
    syntax: Parser.location Syntax.expression option;
    type_description: Types.element_nature
  }

and expression_desc =
  | BinaryOperation of binary_operator_kind * expression * expression
  | DynamicIdentifier of int (** number of nested classes to skip *) *
      string (** name to be searched for at instanciation time *)
  | False
  | FieldAccess of expression * string
  | FunctionArgument of int (** the position of the argument in the call *)
  | FunctionCall of expression (** function *) *
      expression list (** arguments *) *
      expression (** the expression involving the function call *)
      (** creation of a dynamic function context *)
  | FunctionInvocation of expression list
      (** invocation of the current function in context *)
  | If of (expression (** condition *) * expression) list *
      expression (** default *)
  | IndexedAccess of expression * expression list (* subscripts *)
  | Integer of int32
  | LocalIdentifier of int (** number of nested classes to skip *) *
      string (** key in the dictionary of the defining class *)
  | LoopVariable of int (** number of nested for loops to skip *)
  | NoEvent of expression
  | PredefinedIdentifier of string (** predefined identifier *)
  | Range of expression * expression * expression
  | Real of float
  | String of string
  | ToplevelIdentifier of string (** key in the toplevel dictionary *)
  | True
  | Tuple of expression list
  | UnaryOperation of unary_operator_kind * expression
  | Vector of expression list
  | VectorReduction of expression list (** nested ranges *) * expression
  | Coercion of coercion_kind * expression

and coercion_kind =
  | RealOfInteger (** Implicit conversion of Integer to Real *)

and unary_operator_kind =
  | Not
  | UnaryMinus
  | UnaryPlus

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

(* Context types. Contexts are used to resolve identifiers in expressions *)

type context =
  {
    toplevel: (string * element_description) list Lazy.t;
    context_nature: context_nature;
    location: Parser.location
  }

and context_nature =
  | ToplevelContext
  | ClassContext of class_definition
  | SubscriptContext of
      context * expression (* evaluating to an array *) *
      int32 (* dimension index *) * Types.dimension
  | ForContext of context * string * Types.element_nature

(* Type Errors detected during compilation *)

type error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: context
  }

exception CompilError of error_description

(* Utilities *)

let evaluate x = Lazy.force x

let resolve_elements add_element elts other_elts =
  let resolve_other_elements other_elt acc = match other_elt.Syntax.nature with
    | Syntax.Public elts -> List.fold_right (add_element Public) elts acc
    | Syntax.Protected elts -> List.fold_right (add_element Protected) elts acc
    | Syntax.EquationClause _ | Syntax.AlgorithmClause _ -> acc in
  List.fold_right
    (add_element Public)
    elts
    (List.fold_right resolve_other_elements other_elts [])

let resolved_expression syntax nat elt_nat =
  {
    nature = nat;
    info = { syntax = syntax; type_description = elt_nat }
  }

let one =
  let nat = Integer 1l
  and elt_nat = Types.integer_type Types.Constant in
  resolved_expression None nat elt_nat


(* Name resolution functions *)

let rec resolve_toplevel dic nodes =
  let add_element ctx acc (id, elt_desc) =
    match List.mem_assoc id acc with
    | true ->
        let ctx = { ctx with location = elt_desc.element_location } in
        raise (CompilError
          {err_msg = ["_DuplicateDeclarationOfElement"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | false -> acc @ [ (id, elt_desc) ] in
  let rec ctx =
    {
      toplevel =
        lazy (List.fold_left (add_element ctx) dic (evaluate elt_descs));
      context_nature = ToplevelContext;
      location =
        {
          Parser.start = 0;
                  Parser.enddd = 0;
          Parser.filename = Parser.CommandLine
        }
    }
  and elt_descs = lazy (resolve_toplevel_nodes ctx nodes) in
  evaluate ctx.toplevel

and resolve_toplevel_nodes ctx nodes =
  let rec resolve_toplevel_nodes' nodes' =
    match nodes' with
      | [] -> []
      | node :: nodes' ->
          (resolve_toplevel_statements ctx node) @
          (resolve_toplevel_nodes' nodes') in
  let collect_toplevel_defs (cl_defs, nodes) node =
    match node.Syntax.nature with
      | Syntax.ClassDefinitions cl_defs' -> cl_defs' @ cl_defs, nodes
      | _ -> cl_defs, [node] @ nodes in
  let cl_defs, nodes = List.fold_left collect_toplevel_defs ([], []) nodes in
  let node = {Syntax.nature = Syntax.ClassDefinitions cl_defs;
              Syntax.info = ctx.location} in
  (resolve_toplevel_statements ctx node) @
  resolve_toplevel_nodes' nodes

and resolve_toplevel_statements ctx node = match node.Syntax.nature with
  | Syntax.ClassDefinitions cl_defs -> resolve_class_definitions ctx cl_defs
  | Syntax.Expression expr -> raise (CompilError
      {err_msg = ["_NotYetImplemented"; "_TopLevelExpr"];
       err_info = [];
       err_ctx = {ctx with location = expr.Syntax.info}})
  | Syntax.VariablesDefinitions (expr, subs, cpnt_decls) ->
      resole_variables_definitions ctx expr subs cpnt_decls
  | Syntax.Command algo -> raise (CompilError
      {err_msg = ["_NotYetImplemented"; "_TopLevelAlgorithm"];
       err_info = [];
       err_ctx = {ctx with location = algo.Syntax.info}})
  | Syntax.Within path -> raise (CompilError
      {err_msg = ["_NotYetImplemented"; "_WithinClause"];
       err_info = [("_Expr", Syntax.string_of_toplevel_element node)];
       err_ctx = {ctx with location = node.Syntax.info}})
  | Syntax.Import imprt -> raise (CompilError
      {err_msg = ["_NotYetImplemented"; "_ImportClause"];
       err_info = [("_Expr", Syntax.string_of_toplevel_element node)];
       err_ctx = {ctx with location = imprt.Syntax.info}})

and resole_variables_definitions ctx expr subs cpnt_decls =
  let type_spec = lazy (resolve_expression ctx expr)
  and dims = lazy (resolve_dimensions ctx subs) in
  List.map (resolve_variable_definition ctx type_spec dims expr) cpnt_decls

and resolve_variable_definition ctx type_spec dims expr cpnt_decl =
  let type_pref = false, None, Types.Acausal in
  let id, elt_nat, elt_loc =
    resolve_component_declaration ctx type_pref type_spec dims expr cpnt_decl in
  let rec elt_desc =
    {
      element_type =
        lazy (element_type ctx false None None None elt_desc);
      redeclare = false;
      element_nature = elt_nat;
      element_location = elt_loc
    } in
  id, elt_desc

and resolve_class_definitions ctx cl_defs =
  List.map (resolve_class_definition ctx) cl_defs

and resolve_class_definition ctx cl_def = match cl_def.Syntax.nature with
  | Syntax.ClassDefinition (final, def) ->
      let loc = (match def.Syntax.nature with
        | Syntax.Definition (_, _, _, cl_spec) -> cl_spec.Syntax.info) in
      let rec elt_desc =
        {
          element_type = lazy (element_type ctx false final None None elt_desc);
          redeclare = false;
          element_nature = resolve_definition ctx def;
          element_location = loc
        } in
      let s = class_definition_name def in
      s, elt_desc

and class_definition_name def = match def.Syntax.nature with
  | Syntax.Definition (_, _, _, cl_spec) -> class_specifier_name cl_spec

and class_specifier_name cl_spec = match cl_spec.Syntax.nature with
  | Syntax.LongSpecifier (id, _, _) |
    Syntax.ShortSpecifier (id, _, _, _, _, _) |
    Syntax.EnumerationSpecifier (id, _, _) |
    Syntax.ExtensionSpecifier (id, _, _, _) -> id

and resolve_definition ctx def =
  let ctx = {ctx with location = def.Syntax.info} in 
  match def.Syntax.nature with
    | Syntax.Definition (encap, part, kind, cl_spec) ->
        resolve_specification ctx encap part kind cl_spec

and resolve_specification ctx encap part kind cl_spec =
  let encap' = bool_of_encapsulated encap in
  match kind with
    | Syntax.Class ->
        resolve_class_specification ctx encap' part Types.Class cl_spec
    | Syntax.Model ->
        resolve_class_specification ctx encap' part Types.Model cl_spec
    | Syntax.Block ->
        resolve_class_specification ctx encap' part Types.Block cl_spec
    | Syntax.Record ->
        resolve_class_specification ctx encap' part Types.Record cl_spec
    | Syntax.ExpandableConnector ->
	resolve_class_specification
          ctx
          encap'
          part
          Types.ExpandableConnector
          cl_spec
    | Syntax.Connector ->
        resolve_class_specification ctx encap' part Types.Connector cl_spec
    | Syntax.Type when encap' ->
        raise (CompilError
          {err_msg = ["_EncapsulatedCannotBeAppliedTo"; "_TypeDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Syntax.Type -> resolve_type_specification ctx cl_spec
    | Syntax.Package ->
        resolve_class_specification ctx encap' part Types.Package cl_spec
    | Syntax.Function ->
        resolve_class_specification ctx encap' part Types.Function cl_spec

and resolve_type_specification ctx cl_spec =
  let ctx = {ctx with location = cl_spec.Syntax.info} in
  match cl_spec.Syntax.nature with
    | Syntax.LongSpecifier _ ->
        raise (CompilError
          {err_msg = ["_InvalidTypeDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Syntax.ExtensionSpecifier _ ->
        raise (CompilError
          {err_msg = ["_InvalidTypeDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Syntax.ShortSpecifier (_, base_pref, cl_spec, subs, cl_modif, _) ->
        let cpnt_type =
          resolve_type_composition ctx base_pref cl_spec subs cl_modif in
        ComponentType cpnt_type
    | Syntax.EnumerationSpecifier (idt, enum_comp, _) ->
        let enum_type = resolve_enumeration_composition ctx enum_comp in
        PredefinedType enum_type

and resolve_type_composition ctx base_pref cl_spec subs cl_modif =
  let base_pref' = type_prefix base_pref
  and base_type = lazy (resolve_expression ctx cl_spec)
  and dims = lazy (resolve_dimensions ctx subs) in
  let cpnt_type = lazy (component_type ctx base_pref' base_type dims) in
  let cl_modif' = lazy (resolve_type_modification ctx cpnt_type cl_modif) in
  {
    described_type = lazy (modified_described_type ctx cpnt_type cl_modif');
    base_type = base_type;
    type_dimensions = dims;
    type_modification = cl_modif'
  }

and resolve_enumeration_composition ctx enum_comp =
  let resolve_enumeration_literal enum_lit ids =
    match enum_lit.Syntax.nature with
      | Syntax.EnumerationLiteral (id, _) when List.mem id ids ->
          raise (CompilError
            {err_msg = ["_EnumTypeDefWithDuplicLit"; id];
             err_info = [];
             err_ctx = {ctx with location = enum_lit.Syntax.info}}) (*error*)
      | Syntax.EnumerationLiteral (id, _) -> id :: ids in
  match enum_comp.Syntax.nature with
    | Syntax.EnumList (Some enum_lits) ->
        let elts = List.fold_right resolve_enumeration_literal enum_lits [] in
        {
          Types.base_type = Types.EnumerationType elts;
                attributes = ["start", false]
        }
    | Syntax.EnumList None ->
        raise (CompilError
          {err_msg = ["_UnspecifiedEnumLits"];
           err_info = [];
           err_ctx = {ctx with location = enum_comp.Syntax.info}}) (*error*)
    | Syntax.EnumColon ->
        raise (CompilError
          {err_msg = ["_UnspecifiedEnumLits"];
           err_info = [];
           err_ctx = {ctx with location = enum_comp.Syntax.info}}) (*error*)

and resolve_class_specification ctx encap part kind cl_spec =
  let ctx = {ctx with location = cl_spec.Syntax.info} in
  let resolve_specifier encap' cl_def =
    let ctx' = {ctx with context_nature = ClassContext cl_def} in
    resolve_class_specifier ctx ctx' encap cl_spec in
  let rec cl_def =
    {
      class_type = lazy (class_specifier_type ctx part kind cl_def cl_spec);
      enclosing_class = enclosing_class ctx;
      encapsulated = encap;
      description = lazy (resolve_specifier encap cl_def)
    } in
  Class cl_def

and enclosing_class ctx = match ctx.context_nature with
  | ToplevelContext -> None
  | ClassContext cl_def -> Some cl_def
  | SubscriptContext (ctx, _, _, _) |
    ForContext (ctx, _, _) -> enclosing_class ctx

and bool_of_encapsulated = function
  | None -> false
  | Some Syntax.Encapsulated -> true

and resolve_class_specifier ctx ctx' encap cl_spec =
  let ctx = {ctx with location = cl_spec.Syntax.info}
  and ctx' = {ctx' with location = cl_spec.Syntax.info} in
  match cl_spec.Syntax.nature with
    | Syntax.LongSpecifier (_, _, comp) ->
        LongDescription (resolve_composition ctx ctx' comp)
    | Syntax.ShortSpecifier _ when encap ->
        raise (CompilError
          {err_msg = ["_EncapsulatedCannotBeAppliedTo"; "_ShortClassDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Syntax.ShortSpecifier (_, base_pref, cl_spec, subs, cl_modif, _) ->
        let short_desc =
          resolve_short_specifier ctx base_pref cl_spec subs cl_modif in
        ShortDescription short_desc
    | Syntax.ExtensionSpecifier _ when encap ->
        raise (CompilError
          {err_msg = ["_EncapsulatedCannotBeAppliedTo"; "_ClassDefByExtension"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Syntax.ExtensionSpecifier (id , cl_modif, _, comp) ->
        let long_desc =
          resolve_extension_composition ctx ctx' id cl_modif comp in
        LongDescription long_desc
    | Syntax.EnumerationSpecifier _ ->
        raise (CompilError
          {err_msg = ["_InvalidUseOfEnumKeyword"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_short_specifier ctx base_pref cl_spec subs cl_modif =
  let ctx = {ctx with location = cl_spec.Syntax.info} in
  match base_pref.Syntax.nature, subs with
    | Syntax.TypePrefix (None, None, None), None ->
        resolve_modified_class ctx ctx cl_spec cl_modif
    | (Syntax.TypePrefix (Some _, _, _) | Syntax.TypePrefix (_, Some _, _) |
       Syntax.TypePrefix (_, _, Some _)), _ ->
        raise (CompilError
          {err_msg = ["_UseOfTypePrefixInShortClassDef"];
           err_info =
             [("_TypePrefix", Syntax.string_of_base_prefix base_pref)];
           err_ctx = {ctx with location = base_pref.Syntax.info}}) (*error*)
    | Syntax.TypePrefix (None, None, None), Some subs ->
        raise (CompilError
          {err_msg = ["_UseOfSubsInShortClassDef"];
           err_info = [];
           err_ctx = {ctx with location = subs.Syntax.info}}) (*error*)

and resolve_extension_composition ctx ctx' id cl_modif comp =
  raise (CompilError
    {err_msg = ["_NotYetImplemented"; "_ClassExtendsDef"];
     err_info = [];
     err_ctx = ctx})

and resolve_composition ctx ctx' comp = match comp.Syntax.nature with
  | Syntax.Composition (elts, other_elts, extern) ->
      {
        class_annotations = lazy (resolve_class_annotations ctx' elts other_elts);
        imports = resolve_imports ctx' elts other_elts;
        extensions = resolve_extensions ctx ctx' elts other_elts;
        named_elements = resolve_named_elements ctx' elts other_elts;
        unnamed_elements = lazy (resolve_unnamed_elements ctx' other_elts);
        external_call = lazy (resolve_external_call ctx' extern)
      }

and resolve_external_call ctx extern =
  let resolve_external_call' extern' = match extern'.Syntax.nature with
    | Syntax.External (Some id, None, _, _) ->
        { nature = PrimitiveCall id; info = extern' }
    | Syntax.External (Some lang, Some extern_call, _, _) ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_ExternalProcedureCall"];
           err_info = [];
           err_ctx =
              {ctx with location = extern'.Syntax.info}}) (*error*)
    | Syntax.External (None, _, _, _) ->
        { nature = PrimitiveCall "C"; info = extern' } in
        (*raise (CompilError
          {err_msg = ["_UnspecifiedExtCallLang"];
           err_info = [];
           err_ctx =
              {ctx with location = extern'.Syntax.info}}) (*error*) in*)
  match extern with
    | None -> None
    | Some extern' -> Some (resolve_external_call' extern')

and resolve_class_annotations ctx elts other_elts =
  let add_class_annotation vis elt anns = match vis, elt.Syntax.nature with
    | _, Syntax.ClassAnnotation ann ->
        begin match resolve_class_annotation ctx ann with
          | [] -> anns
          | anns' -> anns' @ anns
        end
    | _, (Syntax.ImportClause _ | Syntax.ExtendsClause _ |
      Syntax.ElementDefinition _) -> anns in
  resolve_elements add_class_annotation elts other_elts

and resolve_imports ctx elts other_elts =
  let add_import vis elt imps = match vis, elt.Syntax.nature with
    | _, Syntax.ImportClause (imp_clause, _) ->
        resolve_import_clause ctx imp_clause :: imps
    | _, (Syntax.ClassAnnotation _ | Syntax.ExtendsClause _ |
      Syntax.ElementDefinition _) -> imps in
  resolve_elements add_import elts other_elts

and resolve_extensions ctx ctx' elts other_elts =
  let add_extension vis elt exts = match vis, elt.Syntax.nature with
    | Public, Syntax.ExtendsClause (ext_clause, _) ->
        (Public, resolve_extends_clause ctx ctx' ext_clause) :: exts
    | Protected, Syntax.ExtendsClause (ext_clause, _) ->
        (Protected, resolve_extends_clause ctx ctx' ext_clause) :: exts
    | _, (Syntax.ClassAnnotation _ | Syntax.ImportClause _ |
      Syntax.ElementDefinition _) -> exts in
  resolve_elements add_extension elts other_elts

and resolve_named_elements ctx elts other_elts =
  let add_named_element (id, elt_desc) elts =
    match List.mem_assoc id elts with
    | true ->
        raise (CompilError
          {err_msg = ["_DuplicateDeclarationOfElement"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | false -> (id, elt_desc) :: elts in
  let add_named_elements vis elt elts = match vis, elt.Syntax.nature with
    | Public,
      Syntax.ElementDefinition (redecl, final, dyn_scope, elt_def, _) ->
        let elts' =
          resolve_element_definition ctx false redecl final dyn_scope elt_def in
        List.fold_right add_named_element elts' elts
    | Protected,
      Syntax.ElementDefinition (redecl, final, dyn_scope, elt_def, _) ->
        let elts' =
          resolve_element_definition ctx true redecl final dyn_scope elt_def in
        List.fold_right add_named_element elts' elts
    | _, (Syntax.ClassAnnotation _ | Syntax.ImportClause _ |
      Syntax.ExtendsClause _) -> elts in
  resolve_elements add_named_elements elts other_elts

and resolve_class_annotation ctx ann =
  let rec resolve_class_annotation' cl_modif =
    let add_annotation_information arg acc = match arg.Syntax.nature with
      | Syntax.ElementModification (
          None,
          None,
          { Syntax.nature = Syntax.Identifier "Imagine" },
          Some
            {
              Syntax.nature =
                Syntax.Modification (
                  {
                    Syntax.nature =
                      Syntax.ClassModification
                        [
                          {
                            Syntax.nature =
                              Syntax.ElementModification (
                                None,
                                None,
                                {
                                  Syntax.nature = Syntax.Identifier "AMESim"
                                },
                                Some
                                  {
                                    Syntax.nature =
                                      Syntax.Modification (cl_modif, None)
                                  },
                                [])
                          }
                        ]
                  },
                  None)
            },
          []) -> add_amesim_annotations ctx cl_modif acc
      | Syntax.ElementModification _ | Syntax.ElementRedeclaration _ ->
          (UnknownAnnotation (lazy cl_modif)) :: acc in
    match cl_modif.Syntax.nature with
    | Syntax.ClassModification args ->
        List.fold_right add_annotation_information args [] in
  match ann.Syntax.nature with
  | Syntax.Annotation cl_modif -> resolve_class_annotation' cl_modif

and add_amesim_annotations ctx cl_modif acc =
  let add_inverse_declarations cl_modif =
    let add_inverse_declaration arg acc =
      let add_inverse_declaration' expr modif =
        match expr.Syntax.nature, modif.Syntax.nature with
        | Syntax.IndexedAccess (
            { Syntax.nature = Syntax.Identifier "inverse" }, _),
          Syntax.Eq
            {
              Syntax.nature =
                Syntax.FunctionCall (expr, Some fun_args)
            } -> (resolve_inverse_declaration ctx expr fun_args) :: acc
        | _ ->
            raise (CompilError
              {err_msg = ["_InvalidAnnOfInvFunc"; "_InvalidModifExpr"];
               err_info = [];
               err_ctx =
                 {ctx with location = expr.Syntax.info}}) (*error*) in
      match arg.Syntax.nature with
      | Syntax.ElementModification (Some _, _, _, _, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UseOfEachKeywordNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, Some _, _, _, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UseOfFinalKeywordNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, None, _, None, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UnspecifiedModification"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, None, expr, Some modif, _) ->
          add_inverse_declaration' expr modif
      | Syntax.ElementRedeclaration _ ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_RedeclarationNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*) in
    match cl_modif.Syntax.nature with
    | Syntax.ClassModification args ->
        List.fold_right add_inverse_declaration args acc in
  match cl_modif.Syntax.nature with
  | Syntax.ClassModification
      [
        {
          Syntax.nature =
            Syntax.ElementModification (
              None,
              None,
              {
                Syntax.nature = Syntax.Identifier "InverseFunctions"
              },
              Some
                {
                  Syntax.nature =
                    Syntax.Modification (cl_modif, None)
                },
                [])
        }
      ] -> add_inverse_declarations cl_modif
  | Syntax.ClassModification _ -> acc

and resolve_inverse_declaration ctx expr fun_args =
  let inverse_function_arguments expr' fun_args =
    let map_function_arguments named_args =
      let map_function_argument arg =
        match arg.Syntax.nature with
        | Syntax.NamedArgument (id, expr)
          when List.mem_assoc id named_args ->
            let expr' = resolve_expression ctx expr in
            begin match expr'.nature with
              | LocalIdentifier (0, id') -> id, id'
              | _ ->
                  raise (CompilError
                    {err_msg = ["_InvalidAnnOfInvFunc";
                                "_InvalidFuncArgModif"];
                     err_info = [];
                     err_ctx =
                        {ctx with
                          location = expr.Syntax.info}}) (*error*)
            end
        | Syntax.NamedArgument (id, expr) ->
            raise (CompilError
              {err_msg = ["_InvalidAnnOfInvFunc";
                          "_UnknownArgName"; id];
               err_info = [];
               err_ctx =
                 {ctx with location = arg.Syntax.info}}) (*error*)
        | Syntax.Argument _ ->
            raise (CompilError
              {err_msg = ["_InvalidAnnOfInvFunc";
                          "_CannotUseUnnamedFuncArg"];
               err_info = [];
               err_ctx =
                 {ctx with location = arg.Syntax.info}}) (*error*) in
      match fun_args.Syntax.nature with
      | Syntax.ArgumentList args -> List.map map_function_argument args
      | Syntax.Reduction _ ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc";
                        "_FuncArgReductionNotAllowed"];
             err_info = [];
             err_ctx =
               {ctx with location = fun_args.Syntax.info}}) (*error*) in
    let inverse_function_arguments' cl_type =
      match cl_type.Types.partial, evaluate cl_type.Types.kind with
      | true, _ ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc";
                        "_UseOfPartialClassElement"];
             err_info = [("_ElementFound",
                          Syntax.string_of_expression expr)];
             err_ctx =
               {ctx with location = expr.Syntax.info}}) (*error*)
      | false, Types.Function ->
          map_function_arguments cl_type.Types.named_elements
      | _, kind ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc";
                        "_InvalidTypeOfFuncCallExpr"];
             err_info = [("_ExpectedType", "_Function");
                         ("_TypeFound", Types.string_of_kind kind)];
             err_ctx =
               {ctx with location = expr.Syntax.info}}) (*error*) in
    let elt_nat = expr'.info.type_description in
    match elt_nat with
    | Types.ClassElement cl_spec ->
        let cl_spec = evaluate cl_spec in
        begin match cl_spec with
          | Types.ClassType cl_type ->
              inverse_function_arguments' cl_type
          | _ ->
              raise (CompilError
                {err_msg = ["_InvalidAnnOfInvFunc";
                            "_InvalidTypeOfFuncCallExpr"];
                 err_info =
                   [("_ExpectedType", "_ClassType");
                    ("_TypeFound",
                     Types.string_of_class_specifier cl_spec)];
                 err_ctx =
                   {ctx with location = expr.Syntax.info}}) (*error*)
        end
    | Types.ComponentTypeElement _ ->
        raise (CompilError
          {err_msg = ["_InvalidAnnOfInvFunc";
                      "_InvalidTypeOfFuncCallExpr"];
           err_info = [("_ExpectedType", "_ClassElement");
                       ("_TypeFound", "_ComponentTypeElement")];
           err_ctx =
             {ctx with location = expr.Syntax.info}}) (*error*)
    | Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_InvalidAnnOfInvFunc";
                      "_InvalidTypeOfFuncCallExpr"];
           err_info = [("_ExpectedType", "_ClassElement");
                       ("_TypeFound", "_PredefinedTypeElement")];
           err_ctx =
             {ctx with location = expr.Syntax.info}}) (*error*)
    | Types.ComponentElement _ ->
        raise (CompilError
          {err_msg = ["_InvalidAnnOfInvFunc";
                      "_InvalidTypeOfFuncCallExpr"];
           err_info = [("_ExpectedType", "_ClassElement");
                       ("_TypeFound", "_ComponentElement")];
           err_ctx =
             {ctx with location = expr.Syntax.info}}) (*error*) in
  let expr' = resolve_expression ctx expr in
  match expr'.nature with
  | ToplevelIdentifier _ | LocalIdentifier _ ->
      InverseFunction
        (lazy
          {
            function_class = expr';
            arguments = inverse_function_arguments expr' fun_args
          })
  | _ ->
      raise (CompilError
        {err_msg = ["_InvalidAnnOfInvFunc"; "_InvalidFuncCallExpr"];
         err_info = [];
         err_ctx =
           {ctx with location = expr.Syntax.info}}) (*error*)

(*and resolve_inverse_function_annotation ctx ann =
  let rec resolve_class_annotation' cl_modif =
    let resolve_inverse_declaration expr fun_args =
      let inverse_function_arguments expr' fun_args =
        let map_function_arguments named_args =
          let map_function_argument arg =
            match arg.Syntax.nature with
              | Syntax.NamedArgument (id, expr)
                when List.mem_assoc id named_args ->
                  let expr' = resolve_expression ctx expr in
                  begin match expr'.nature with
                    | LocalIdentifier (0, id') -> id, id'
                    | _ ->
                        raise (CompilError
                          {err_msg = ["_InvalidAnnOfInvFunc";
                                      "_InvalidFuncArgModif"];
                           err_info = [];
                           err_ctx =
                              {ctx with
                                location = expr.Syntax.info}}) (*error*)
                  end
              | Syntax.NamedArgument (id, expr) ->
                  raise (CompilError
                    {err_msg = ["_InvalidAnnOfInvFunc";
                                "_UnknownArgName"; id];
                     err_info = [];
                     err_ctx =
                       {ctx with location = arg.Syntax.info}}) (*error*)
              | Syntax.Argument _ ->
                  raise (CompilError
                    {err_msg = ["_InvalidAnnOfInvFunc";
                                "_CannotUseUnnamedFuncArg"];
                     err_info = [];
                     err_ctx =
                        {ctx with location = arg.Syntax.info}}) (*error*) in
          match fun_args.Syntax.nature with
            | Syntax.ArgumentList args -> List.map map_function_argument args
            | Syntax.Reduction _ ->
                raise (CompilError
                  {err_msg = ["_InvalidAnnOfInvFunc";
                              "_FuncArgReductionNotAllowed"];
                   err_info = [];
                   err_ctx =
                      {ctx with location = fun_args.Syntax.info}}) (*error*) in
        let inverse_function_arguments' cl_type =
          match cl_type.Types.partial, evaluate cl_type.Types.kind with
            | true, _ ->
                raise (CompilError
                  {err_msg = ["_InvalidAnnOfInvFunc";
                              "_UseOfPartialClassElement"];
                   err_info = [("_ElementFound",
                                Syntax.string_of_expression expr)];
                   err_ctx =
                      {ctx with location = expr.Syntax.info}}) (*error*)
            | false, Types.Function ->
                map_function_arguments cl_type.Types.named_elements
            | _, kind ->
                raise (CompilError
                  {err_msg = ["_InvalidAnnOfInvFunc";
                              "_InvalidTypeOfFuncCallExpr"];
                   err_info = [("_ExpectedType", "_Function");
                               ("_TypeFound", Types.string_of_kind kind)];
                   err_ctx =
                     {ctx with location = expr.Syntax.info}}) (*error*) in
        let elt_nat = expr'.info.type_description in
        match elt_nat with
          | Types.ClassElement cl_spec ->
              let cl_spec = evaluate cl_spec in
              begin match cl_spec with
                | Types.ClassType cl_type ->
                    inverse_function_arguments' cl_type
                | _ ->
                    raise (CompilError
                      {err_msg = ["_InvalidAnnOfInvFunc";
                                  "_InvalidTypeOfFuncCallExpr"];
                       err_info =
                         [("_ExpectedType", "_ClassType");
                          ("_TypeFound",
                           Types.string_of_class_specifier cl_spec)];
                       err_ctx =
                         {ctx with location = expr.Syntax.info}}) (*error*)
              end
          | Types.ComponentTypeElement _ ->
              raise (CompilError
                {err_msg = ["_InvalidAnnOfInvFunc";
                            "_InvalidTypeOfFuncCallExpr"];
                 err_info = [("_ExpectedType", "_ClassElement");
                             ("_TypeFound", "_ComponentTypeElement")];
                 err_ctx =
                   {ctx with location = expr.Syntax.info}}) (*error*)
          | Types.PredefinedTypeElement _ ->
              raise (CompilError
                {err_msg = ["_InvalidAnnOfInvFunc";
                            "_InvalidTypeOfFuncCallExpr"];
                 err_info = [("_ExpectedType", "_ClassElement");
                             ("_TypeFound", "_PredefinedTypeElement")];
                 err_ctx =
                   {ctx with location = expr.Syntax.info}}) (*error*)
          | Types.ComponentElement _ ->
              raise (CompilError
                {err_msg = ["_InvalidAnnOfInvFunc";
                            "_InvalidTypeOfFuncCallExpr"];
                 err_info = [("_ExpectedType", "_ClassElement");
                             ("_TypeFound", "_ComponentElement")];
                 err_ctx =
                   {ctx with location = expr.Syntax.info}}) (*error*) in
      let expr' = resolve_expression ctx expr in
      match expr'.nature with
        | ToplevelIdentifier _ | LocalIdentifier _ ->
            {
              function_class = expr';
              arguments =
                inverse_function_arguments expr' fun_args
            }
        | _ ->
            raise (CompilError
              {err_msg = ["_InvalidAnnOfInvFunc"; "_InvalidFuncCallExpr"];
               err_info = [];
               err_ctx =
                 {ctx with location = expr.Syntax.info}}) (*error*) in
    let add_inverse_declaration arg acc =
      let add_inverse_declaration' expr modif =
        match expr.Syntax.nature, modif.Syntax.nature with
        | Syntax.IndexedAccess (
            { Syntax.nature = Syntax.Identifier "inverse" }, _),
          Syntax.Eq
            {
              Syntax.nature =
                Syntax.FunctionCall (expr, Some fun_args)
            } -> lazy (resolve_inverse_declaration expr fun_args) :: acc
        | _ ->
            raise (CompilError
              {err_msg = ["_InvalidAnnOfInvFunc"; "_InvalidModifExpr"];
               err_info = [];
               err_ctx =
                 {ctx with location = expr.Syntax.info}}) (*error*) in
      match arg.Syntax.nature with
      | Syntax.ElementModification (Some _, _, _, _, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UseOfEachKeywordNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, Some _, _, _, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UseOfFinalKeywordNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, None, _, None, _) ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_UnspecifiedModification"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
      | Syntax.ElementModification (None, None, expr, Some modif, _) ->
          add_inverse_declaration' expr modif
      | Syntax.ElementRedeclaration _ ->
          raise (CompilError
            {err_msg = ["_InvalidAnnOfInvFunc"; "_RedeclarationNotAllowed"];
             err_info = [];
             err_ctx = {ctx with location = arg.Syntax.info}}) (*error*) in
    let add_inverse_declarations cl_modif acc =
      let add_inverse_declarations' cl_modif = 
        match cl_modif.Syntax.nature with
        | Syntax.ClassModification args ->
            List.fold_right add_inverse_declaration args acc in
      match cl_modif.Syntax.nature with
      | Syntax.ClassModification
          [
            {
              Syntax.nature =
                Syntax.ElementModification (
                  None,
                  None,
                  {
                    Syntax.nature = Syntax.Identifier "InverseFunctions"
                  },
                  Some
                    {
                      Syntax.nature =
                        Syntax.Modification (cl_modif, None)
                    },
                    [])
            }
          ] -> add_inverse_declarations' cl_modif
      | Syntax.ClassModification _ -> acc in
    let add_annotation_information arg acc = match arg.Syntax.nature with
      | Syntax.ElementModification (
          None,
          None,
          { Syntax.nature = Syntax.Identifier "Imagine" },
          Some
            {
              Syntax.nature =
                Syntax.Modification (
                  {
                    Syntax.nature =
                      Syntax.ClassModification
                        [
                          {
                            Syntax.nature =
                              Syntax.ElementModification (
                                None,
                                None,
                                {
                                  Syntax.nature = Syntax.Identifier "AMESim"
                                },
                                Some
                                  {
                                    Syntax.nature =
                                      Syntax.Modification (cl_modif, None)
                                  },
                                [])
                          }
                        ]
                  },
                  None)
            },
          []) -> add_inverse_declarations cl_modif acc
      | Syntax.ElementModification _ | Syntax.ElementRedeclaration _ -> acc in
    match cl_modif.Syntax.nature with
    | Syntax.ClassModification args ->
        List.fold_right add_annotation_information args [] in
  match ann.Syntax.nature with
  | Syntax.Annotation cl_modif -> resolve_class_annotation' cl_modif*)

and resolve_import_clause ctx imp_clause =
  let ctx = {ctx with location = imp_clause.Syntax.info} in
  raise (CompilError
    {err_msg = ["_NotYetImplemented"; "_ImportClause"];
     err_info = [("_Expr", Syntax.string_of_import imp_clause)];
     err_ctx = ctx})

and resolve_extends_clause ctx ctx' ext_clause =
  match ext_clause.Syntax.nature with
    | Syntax.Extends (cl_spec, cl_modif, _) ->
        resolve_extension ctx ctx' cl_spec cl_modif

and resolve_extension ctx ctx' cl_spec cl_modif =
  let ctx' = {ctx' with location = cl_spec.Syntax.info} in
  let base_class = lazy (resolve_extension_expression ctx cl_spec) in
  let cl_spec' = lazy (base_class_type ctx' cl_spec base_class) in
  let cl_modif' =
    lazy (resolve_class_modification_option ctx' cl_spec' cl_modif) in
  {
    modified_class_type = lazy (modified_class_type ctx' cl_spec' cl_modif');
    base_class = base_class;
    class_modification = cl_modif'
  }

and resolve_extension_expression ctx cl_spec =
  let rec modify_resolved_expression expr = match expr.nature with
    | LocalIdentifier (level, id) ->
        { expr with nature = LocalIdentifier (level + 1, id) }
    | FieldAccess (expr', id) ->
        { expr with
          nature = FieldAccess (modify_resolved_expression expr', id)
        }
    | IndexedAccess (expr', exprs') ->
        let exprs' = List.map modify_resolved_expression exprs' in
        { expr with
          nature = IndexedAccess (modify_resolved_expression expr', exprs')
        }
    | ToplevelIdentifier _ -> expr
    | _ ->
        raise (CompilError
          {err_msg = ["_InvalidExtensionDef"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
  match ctx.context_nature with
    | ToplevelContext | ClassContext _ ->
        let base_class = resolve_expression ctx cl_spec in
        modify_resolved_expression base_class
    | SubscriptContext _ | ForContext _ ->
        raise (CompilError
          {err_msg = ["_InvalidExtensionDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_modified_class ctx ctx' cl_spec cl_modif =
  let ctx' = {ctx' with location = cl_spec.Syntax.info} in
  let base_class = lazy (resolve_expression ctx cl_spec) in
  let cl_spec' = lazy (base_class_type ctx' cl_spec base_class) in
  let cl_modif' =
    lazy (resolve_class_modification_option ctx' cl_spec' cl_modif) in
  {
    modified_class_type = lazy (modified_class_type ctx' cl_spec' cl_modif');
    base_class = base_class;
    class_modification = cl_modif'
  }

and resolve_element_definition ctx protect redecl final dyn_scope elt_def =
  let repl = replaceable_attribute elt_def in
  let element_description (id, elt_nat, elt_loc) =
    let rec elt_desc =
      {
        element_type =
          lazy (element_type ctx protect final repl dyn_scope elt_desc);
        redeclare = bool_of_redeclare redecl;
        element_nature = elt_nat;
        element_location = elt_loc
      } in
    id, elt_desc in
  List.map element_description (declared_elements ctx elt_def)

and replaceable_attribute elt_def = match elt_def.Syntax.nature with
  | Syntax.ClassDefinitionElement (repl, _, _) |
    Syntax.ComponentClauseElement (repl, _, _) -> repl

and bool_of_redeclare = function
  | None -> false
  | Some Syntax.Redeclare -> true

and resolve_type_constraint ctx elt_def = match elt_def.Syntax.nature with
  | Syntax.ClassDefinitionElement (_, _, []) |
    Syntax.ComponentClauseElement (_, _, []) -> None
  | Syntax.ClassDefinitionElement (_, _, _ :: _) |
    Syntax.ComponentClauseElement (_, _, _ :: _) -> assert false

and declared_elements ctx elt_def = match elt_def.Syntax.nature with
  | Syntax.ClassDefinitionElement (_, def, _) ->
      let s = class_definition_name def
      and elt_nat = resolve_definition ctx def
      and loc = match def.Syntax.nature with
        | Syntax.Definition (encap, part, kind, cl_spec) ->
            cl_spec.Syntax.info in
      [s, elt_nat, loc]
  | Syntax.ComponentClauseElement (_, cpnt_cl, _) ->
      resolve_component_clause ctx cpnt_cl

and resolve_component_clause ctx cpnt_cl = match cpnt_cl.Syntax.nature with
  | Syntax.ComponentClause (type_pref, type_spec, subs, cpnt_decls) ->
      let type_pref' = type_prefix type_pref
      and type_spec' = lazy (resolve_expression ctx type_spec)
      and dims = lazy (resolve_dimensions ctx subs) in
      List.map
        (resolve_component_declaration ctx type_pref' type_spec' dims type_spec)
        cpnt_decls

and type_prefix type_pref =
  let bool_of_flow = function
    | None -> false
    | Some Syntax.Flow -> true
  and variability_of_variability = function
    | None -> None
    | Some Syntax.Constant -> Some Types.Constant
    | Some Syntax.Parameter -> Some Types.Parameter
    | Some Syntax.Discrete -> Some Types.Discrete
  and causality_of_inout = function
    | None -> Types.Acausal
    | Some Syntax.Input -> Types.Input
    | Some Syntax.Output -> Types.Output in
  match type_pref.Syntax.nature with
    | Syntax.TypePrefix (flow, var, inout) ->
        bool_of_flow flow,
        variability_of_variability var,
        causality_of_inout inout

and resolve_component_declaration
  ctx type_pref type_spec' dims type_spec cpnt_decl =
    let build_comment_string cmt = match cmt.Syntax.nature with
      | Syntax.Comment (ss, _) -> List.fold_right ( ^ ) ss "" in
    match cpnt_decl.Syntax.nature with
      | Syntax.ComponentDeclaration (decl, cmt) ->
          let cmt' = build_comment_string cmt in
          resolve_declaration ctx type_pref type_spec' dims decl cmt' type_spec

and resolve_declaration ctx type_pref type_spec' dims decl cmt type_spec =
  let ctx = {ctx with location = decl.Syntax.info} in
  match decl.Syntax.nature with
    | Syntax.Declaration (id, subs, modif) ->
        let dims = lazy ((resolve_dimensions ctx subs) @ (evaluate dims)) in
        let cpnt_type = lazy (component_type ctx type_pref type_spec' dims) in
        let modif' =
          lazy (resolve_component_modification ctx cpnt_type modif) in
        let cpnt_desc =
          {
            component_type =
              lazy (modified_component_type ctx (evaluate cpnt_type) modif');
            type_specifier = type_spec';
            dimensions = dims;
            modification = modif';
            comment = cmt;
          } in
        (id, Component cpnt_desc, decl.Syntax.info)

and resolve_dimensions ctx subs =
  let resolve_dimension sub = match sub.Syntax.nature with
    | Syntax.Colon -> Colon
    | Syntax.Subscript expr ->
        Expression (resolve_subscript_expression ctx expr) in
  let resolve_dimensions' = function
    | None -> []
    | Some { Syntax.nature = Syntax.Subscripts subs_elts } ->
        List.map resolve_dimension subs_elts in
  resolve_dimensions' subs

and base_class_type ctx cl_spec base_class =
  match (evaluate base_class).info.type_description with
    | Types.ClassElement cl_spec -> evaluate cl_spec
    | Types.ComponentTypeElement _ ->
        raise (CompilError
          {err_msg = ["_CannotInheritFrom"; "_ComponentTypeElement"];
           err_info =
             [("_ElemFound", Syntax.string_of_expression cl_spec)];
           err_ctx = {ctx with location = cl_spec.Syntax.info}}) (*error*)
    | Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_CannotInheritFrom"; "_PredefinedTypeElement"];
           err_info =
             [("_ElemFound", Syntax.string_of_expression cl_spec)];
           err_ctx = {ctx with location = cl_spec.Syntax.info}}) (*error*)
    | Types.ComponentElement _ ->
        raise (CompilError
          {err_msg = ["_CannotInheritFrom"; "_ComponentElement"];
           err_info = [];
           err_ctx = {ctx with location = cl_spec.Syntax.info}}) (*error*)

and component_type ctx (flow, var, inout) base_type dims =
  let base_type = evaluate base_type in
  let lower_variability var var' = match var, var' with
    | Some Types.Constant,
      (Types.Constant | Types.Parameter | Types.Discrete | Types.Continuous) ->
        Types.Constant
    | Some Types.Parameter,
      (Types.Parameter | Types.Discrete | Types.Continuous) -> Types.Parameter
    | Some Types.Discrete, (Types.Discrete | Types.Continuous) -> Types.Discrete
    | Some Types.Continuous, Types.Continuous -> Types.Continuous
    | None, _ -> var'
    | Some var, (Types.Constant | Types.Parameter | Types.Discrete) ->
        raise (CompilError
          {err_msg = ["_VariablityConflictsInCompDef"];
           err_info =
             [("_TypePrefix", Types.string_of_variability var);
              ("_TypeSpecifierVariability", Types.string_of_variability var')];
           err_ctx = ctx}) (*error*)
  and propagate_causality inout inout' = match inout, inout' with
    | Types.Acausal, (Types.Acausal | Types.Input | Types.Output) -> inout'
    | (Types.Input | Types.Output), Types.Acausal -> inout
    | Types.Input, Types.Input | Types.Output, Types.Output -> inout
    | Types.Input, Types.Output | Types.Output, Types.Input ->
        raise (CompilError
          {err_msg = ["_CausalityConflictsInCompDef"];
           err_info = [("_TypePrefix", Types.string_of_causality inout);
                       ("_TypeSpecifierCausality",
                        Types.string_of_causality inout')];
           err_ctx = ctx}) (*error*) in
  let predefined_type_variability predef = match predef with
    | { Types.base_type = Types.RealType } -> Types.Continuous
    | _ -> Types.Discrete in
  let rec class_specifier_variability cl_spec = match cl_spec with
    | Types.PredefinedType predef -> predefined_type_variability predef
    | Types.ClassType cl_type -> Types.Continuous
    | Types.ComponentType cpnt_type -> evaluate cpnt_type.Types.variability
    | Types.ArrayType (dim, cl_spec) -> class_specifier_variability cl_spec
    | Types.TupleType cl_specs -> assert false in
  match base_type.info.type_description with
    | Types.ComponentElement _ ->
        raise (CompilError
          {err_msg = ["class"; "_ElemExpected"];
           err_info = [("TypeFound", "_ComponentElement")];
           err_ctx = ctx}) (*error*)
    | Types.ClassElement cl_spec ->
        let cl_spec = evaluate cl_spec in
        let var' = class_specifier_variability cl_spec in
        let var' = lazy (lower_variability var var')
        and base_class = lazy (add_dimensions dims cl_spec) in
        component_element (lazy flow) var' (lazy inout) base_class
    | Types.ComponentTypeElement cpnt_type ->
        let flow' = lazy (flow || evaluate cpnt_type.Types.flow)
        and var' =
          lazy (lower_variability var (evaluate cpnt_type.Types.variability))
        and inout' =
          lazy (propagate_causality inout (evaluate cpnt_type.Types.causality))
        and base_class =
          lazy (add_dimensions dims (Types.ComponentType cpnt_type)) in
        component_element flow' var' inout' base_class
    | Types.PredefinedTypeElement predef ->
        let var' = predefined_type_variability predef in
        let var' = lazy (lower_variability var var')
        and base_class =
          lazy (add_dimensions dims (Types.PredefinedType predef)) in
        component_element (lazy flow) var' (lazy inout) base_class

and add_dimensions dims cl_spec =
  let add_dimension dim cl_spec = match dim with
    | Expression { nature = Integer i } ->
        Types.ArrayType (Types.ConstantDimension i, cl_spec)
    | Expression _ -> Types.ArrayType (Types.ParameterDimension, cl_spec)
    | Colon -> Types.ArrayType (Types.DiscreteDimension, cl_spec) in
  List.fold_right add_dimension (evaluate dims) cl_spec

and modified_described_type ctx cpnt_type cl_modif =
  let cpnt_type' = evaluate cpnt_type in
  let cl_spec = cpnt_type'.Types.base_class in
  { cpnt_type' with
    Types.base_class =
      lazy (modify_class_specifier ctx (evaluate cl_modif) cl_spec)
  }

and modified_class_type ctx cl_spec cl_modif =
  let cl_spec' = modify_class_specifier ctx (evaluate cl_modif) cl_spec in
  match cl_spec' with
    | Types.ClassType cl_type -> cl_type
    | Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
      Types.TupleType _ ->
        raise (CompilError
          {err_msg = ["class"; "_ElemExpected"];
           err_info = [("TypeFound",
                        Types.string_of_class_specifier cl_spec')];
           err_ctx = ctx}) (*error*)

and modified_component_type ctx cpnt_type modif =
  let modified_component_type' = function
    | Modification (cl_modif, _) -> modify_component_type ctx cl_modif cpnt_type
    | Assignment _ | Equality _ -> cpnt_type in
  match evaluate modif with
    | None -> cpnt_type
    | Some modif' -> modified_component_type' modif'

(* We can abstract dimensions away since they have been already checked at *)
(* modification resolution time.                                           *)
and modify_class_specifier ctx cl_modif cl_spec =
  let rec modify_class_specifier' cl_spec' = match cl_spec' with
    | Types.PredefinedType predef ->
        Types.PredefinedType (modify_predefined_type ctx cl_modif predef)
    | Types.ClassType cl_type ->
        Types.ClassType (modify_class_type ctx cl_modif cl_type)
    | Types.ComponentType cpnt_type ->
        Types.ComponentType (modify_component_type ctx cl_modif cpnt_type)
    | Types.ArrayType (dim, cl_spec) ->
        Types.ArrayType (dim, modify_class_specifier' cl_spec)
    | Types.TupleType _ ->
        raise (CompilError
          {err_msg = ["_InvalidTypeOfClassSpec"];
           err_info = [("_TypeFound",
                        Types.string_of_class_specifier cl_spec')];
           err_ctx = ctx}) (*error*) in
  modify_class_specifier' (evaluate cl_spec)

and modify_predefined_type ctx cl_modif predef =
  { predef with
    Types.attributes =
      modify_predefined_attributes ctx cl_modif predef.Types.attributes
  }

and modify_predefined_attributes ctx cl_modif attrs =
  let apply_modifications ((id, final) as attr) = function
    | [] -> attr
    | [_] when final -> assert false (*error*)
    | [final', (Assignment _ | Equality _)] -> id, final'
    | _ :: _ -> assert false (*error*) in
  let modify_attribute ((id, _) as attr) =
    let modifs, elt_descs = partition_modifications cl_modif id in
    match modifs, elt_descs with
      | [], [] -> attr
      | _ :: _, [] -> apply_modifications attr modifs
      | [], _ :: _
      | _ :: _, _ :: _ ->
          raise (CompilError
            {err_msg = ["_RedeclarePredefTypeAttrib"; id];
             err_info = [];
             err_ctx = ctx}) (*error*) in
  List.map modify_attribute attrs

and modify_class_type ctx cl_modif cl_type =
  let modify_named_element (id, elt_type) =
    id, lazy (modify_element ctx cl_modif id (evaluate elt_type)) in
  { cl_type with
    Types.named_elements =
      List.map modify_named_element cl_type.Types.named_elements
  }

and modify_element ctx cl_modif id elt_type =
  let modifs, elt_descs = partition_modifications cl_modif id in
  match modifs, elt_descs with
    | [], [] -> elt_type
    | _ :: _, [] -> apply_element_modifications ctx modifs elt_type id
    | [], [elt_desc] -> apply_element_redeclaration ctx elt_desc elt_type
    | [], _ :: _ :: _ ->
        raise (CompilError
          {err_msg = ["_InvalidElemModifDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | _ :: _, _ :: _ ->
        raise (CompilError
          {err_msg = ["_InvalidElemModifDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and partition_modifications cl_modif id =
  let add_element_modification modif_arg modifs = match modif_arg.action with
    | Some (ElementModification modif) -> (modif_arg.final, modif) :: modifs
    | None | Some (ElementRedeclaration _) -> modifs
  and add_element_redeclaration modif_arg elt_descs =
    match modif_arg.action with
      | None | Some (ElementModification _) -> elt_descs
      | Some (ElementRedeclaration elt_desc) ->
          (modif_arg.final, elt_desc) :: elt_descs in
  let is_current_element_modification modif_arg = modif_arg.target = id in
  let cl_modif' = List.filter is_current_element_modification cl_modif in
  let modifs = List.fold_right add_element_modification cl_modif' []
  and elt_descs = List.fold_right add_element_redeclaration cl_modif' [] in
  modifs, elt_descs

and apply_element_redeclaration ctx elt_desc elt_type =
  raise (CompilError
    {err_msg = ["_NotYetImplemented"; "_ElementRedeclaration"];
     err_info = [];
     err_ctx = ctx})

and apply_element_modifications ctx modifs elt_type id =
  let add_modification_arguments (final, modif) cl_modifs = match modif with
    | Modification (cl_modif, _) -> (final, cl_modif) :: cl_modifs
    | Assignment _ | Equality _ -> cl_modifs
  and add_value_modification (final, modif) val_modifs = match modif with
    | Modification (_, Some _) | Assignment _ | Equality _ ->
        final :: val_modifs
    | Modification (_, None) -> val_modifs in
  let cl_modifs = List.fold_right add_modification_arguments modifs []
  and val_modifs = List.fold_right add_value_modification modifs [] in
  let elt_type' = modify_element_type ctx cl_modifs elt_type id in
  modify_element_value ctx val_modifs elt_type' id

and modify_element_type ctx cl_modifs elt_type id =
  let propagate_final_attribute final modif_arg cl_modif =
    { modif_arg with final = final } :: cl_modif in
  let merge_modifications (final, cl_modif) cl_modif' =
    List.fold_right (propagate_final_attribute final) cl_modif cl_modif' in
  let cl_modif = List.fold_right merge_modifications cl_modifs [] in
  { elt_type with
    Types.element_nature = modify_element_nature ctx cl_modif elt_type id
  }

and modify_element_nature ctx cl_modif elt_type id =
  match elt_type.Types.element_nature with
    | _ when elt_type.Types.final ->
        raise (CompilError
          {err_msg = ["_FinalElemModifNotAllowed"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Types.ComponentElement cpnt_type ->
        Types.ComponentElement (modify_component_type ctx cl_modif cpnt_type)
    | Types.ClassElement cl_spec ->
        let cl_spec' = lazy (modify_class_specifier ctx cl_modif cl_spec) in
        Types.ClassElement cl_spec'
    | Types.ComponentTypeElement cpnt_type ->
        let cpnt_type' = modify_component_type ctx cl_modif cpnt_type in
        Types.ComponentTypeElement cpnt_type'
    | Types.PredefinedTypeElement predef ->
        Types.PredefinedTypeElement (modify_predefined_type ctx cl_modif predef)

and modify_element_value ctx val_modifs elt_type id =
  match val_modifs with
    | [] -> elt_type
    | [_] when elt_type.Types.final ->
        raise (CompilError
          {err_msg = ["_FinalElemModifNotAllowed"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | [final] -> { elt_type with Types.final = final }
    | _ :: _ :: _ ->
        raise (CompilError
          {err_msg = ["_DuplicatedModifOfElem"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)

and modify_component_type ctx cl_modif cpnt_type =
  { cpnt_type with
    Types.base_class =
      lazy (modify_class_specifier ctx cl_modif cpnt_type.Types.base_class)
  }

and resolve_type_modification ctx cpnt_type cl_modif =
  let cl_spec = (evaluate cpnt_type).Types.base_class in
  resolve_class_modification_option ctx cl_spec cl_modif

and resolve_component_modification ctx cpnt_type = function
  | None -> None
  | Some modif' ->
      let elt_nat = Types.ComponentElement (evaluate cpnt_type) in
      Some (resolve_modification ctx elt_nat modif')

and resolve_class_modification_option ctx cl_spec = function
  | None -> []
  | Some cl_modif -> resolve_class_modification ctx cl_spec cl_modif

and resolve_modification ctx elt_nat modif =
  let ctx = {ctx with location = modif.Syntax.info} in
  match elt_nat, modif.Syntax.nature with
    | Types.ComponentElement cpnt_type, Syntax.Modification (cl_modif, expr) |
      Types.ComponentTypeElement cpnt_type,
      Syntax.Modification (cl_modif, (None as expr)) ->
        resolve_component_type_modification ctx cpnt_type cl_modif expr
    | Types.ComponentTypeElement _, Syntax.Modification (_, Some _) ->
        raise (CompilError
          {err_msg = ["_InvalidClassElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Types.ClassElement cl_spec, Syntax.Modification (cl_modif, None) ->
        let cl_modif' = resolve_class_modification ctx cl_spec cl_modif in
        Modification (cl_modif', None)
    | Types.ClassElement _, Syntax.Modification (_, Some _) ->
        raise (CompilError
          {err_msg = ["_InvalidClassElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | (Types.PredefinedTypeElement _),
      (Syntax.Modification _ | Syntax.Eq _ | Syntax.ColEq _) ->
        raise (CompilError
          {err_msg = ["_InvalidClassElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | Types.ComponentElement cpnt_type, Syntax.Eq expr ->
        let expr' = lazy (resolve_modification_equation ctx cpnt_type expr) in
        Equality expr'
    | Types.ComponentElement cpnt_type, Syntax.ColEq expr ->
        let expr' = lazy (resolve_modification_algorithm ctx cpnt_type expr) in
        Assignment expr'
    | (Types.ClassElement _ | Types.ComponentTypeElement _),
      (Syntax.Eq _ | Syntax.ColEq _) ->
        raise (CompilError
          {err_msg = ["_InvalidClassElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_component_type_modification ctx cpnt_type cl_modif expr =
  let ctx = {ctx with location = cl_modif.Syntax.info} in
  let cl_spec = cpnt_type.Types.base_class in
  let cl_modif' = resolve_class_modification ctx cl_spec cl_modif in
  let cpnt_type' = modify_component_type ctx cl_modif' cpnt_type in
  let expr' = resolve_value_modification_option ctx cpnt_type' expr in
  Modification (cl_modif', expr')

and resolve_value_modification_option ctx cpnt_type = function
  | None -> None
  | Some expr -> Some (lazy (resolve_modification_equation ctx cpnt_type expr))

and resolve_modification_equation ctx cpnt_type expr =
  let ctx = {ctx with location = expr.Syntax.info} in
  let resolve_modification_equation' cpnt_type' expr' =
    let var = evaluate cpnt_type.Types.variability
    and var' = evaluate cpnt_type'.Types.variability in
    match Types.compare_component_types cpnt_type cpnt_type' with
    | Types.SameType
      when Types.higher_variability var var' -> expr'
    | Types.SameType ->
        let var = Types.string_of_variability var
        and var' = Types.string_of_variability var' in
        raise (CompilError
          {err_msg = ["_VariabilityConflicts"];
           err_info = [("_ExprKind", "A = B");
                       ("_VariabilityOfA", var);
                       ("_VariabilityOfB", var')];
           err_ctx = ctx}) (*error*)
    | _ ->
              let type_A = Types.string_of_component_type cpnt_type
              and type_B = Types.string_of_component_type cpnt_type' in
              raise (CompilError
                {err_msg = [ "_EquTermsNotOfTheSameType"];
                 err_info = [("_ExprKind", "A = B");
                             ("_TypeOfA", type_A);
                             ("_TypeOfB", type_B)];
           err_ctx = ctx}) (*error*) in
  let expr' = resolve_expression ctx expr in
  let expr' = apply_rhs_coercions cpnt_type expr' in
  match expr'.info.type_description with
  | Types.ComponentElement cpnt_type' ->
      resolve_modification_equation' cpnt_type' expr'
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_modification_algorithm ctx cpnt_type expr =
  let ctx = {ctx with location = expr.Syntax.info} in
  let resolve_modification_algorithm' cpnt_type' expr' =
          let var = evaluate cpnt_type.Types.variability
          and var' = evaluate cpnt_type'.Types.variability in
          match Types.compare_component_types cpnt_type cpnt_type' with
    | Types.SameType
              when Types.higher_variability var var' -> expr'
    | Types.SameType ->
              let var = Types.string_of_variability var
              and var' = Types.string_of_variability var' in
              raise (CompilError
          {err_msg = ["_VariabilityConflicts"];
                 err_info = [("_ExprKind", "A := B");
                             ("_VariabilityOfA", var);
                             ("_VariabilityOfB", var')];
                 err_ctx = ctx}) (*error*)
    | _ ->
              let type_A = Types.string_of_component_type cpnt_type
              and type_B = Types.string_of_component_type cpnt_type' in
              raise (CompilError
          {err_msg = [ "_TypeConflictsInAssign"];
                 err_info = [("_ExprKind", "A := B");
                             ("_TypeOfA", type_A);
                             ("_TypeOfB", type_B)];
           err_ctx = ctx}) (*error*) in
  let expr' = resolve_expression ctx expr in
  let expr' = apply_rhs_coercions cpnt_type expr' in
  match expr'.info.type_description with
  | Types.ComponentElement cpnt_type' ->
      resolve_modification_algorithm' cpnt_type' expr'
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_class_modification ctx cl_spec cl_modif =
  match cl_modif.Syntax.nature with
    | Syntax.ClassModification args ->
        List.map (resolve_modification_argument ctx cl_spec) args

and resolve_modification_argument ctx cl_spec arg =
  let ctx = {ctx with location = arg.Syntax.info} in
  let apply_each each =
    let rec drop_dimensions cl_spec = match cl_spec with
      | Types.ArrayType (_, cl_spec') -> drop_dimensions cl_spec'
      | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
        Types.TupleType _ -> cl_spec in
    let cl_spec' = evaluate cl_spec in
    match cl_spec' with
      | Types.ArrayType _ when each -> drop_dimensions cl_spec'
      | Types.PredefinedType _
      | Types.ClassType _
      | Types.ComponentType _
      | Types.TupleType _ when each ->
          raise (CompilError
            {err_msg = ["_EachAppliedToNonArrayElem"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | Types.ArrayType _ | Types.PredefinedType _ | Types.ClassType _ |
        Types.ComponentType _ | Types.TupleType _ -> cl_spec' in
  match arg.Syntax.nature with
    | Syntax.ElementModification (each, final, expr, modif, _) ->
        let each' = bool_of_each each
        and final' = bool_of_final final in
        let cl_spec' = apply_each each' in
        resolve_element_modification ctx cl_spec' each' final' expr modif
    | Syntax.ElementRedeclaration (each, final, elt_def) ->
        let each' = bool_of_each each
        and final' = bool_of_final final in
        let cl_spec' = apply_each each' in
        resolve_element_redeclaration ctx cl_spec' each' final' elt_def

and bool_of_each = function
  | None -> false
  | Some Syntax.Each -> true

and bool_of_final = function
  | None -> false
  | Some Syntax.Final -> true

and resolve_element_modification ctx cl_spec each final expr modif =
  let ctx = {ctx with location = expr.Syntax.info} in
  let rec path_of_expression path expr = match expr.Syntax.nature with
    | Syntax.Identifier id ->
        modification_arguments_of_path cl_spec each final id (List.rev path)
    | Syntax.FieldAccess (expr, id) -> path_of_expression (id :: path) expr
    | _ ->
        raise (CompilError
          {err_msg = ["_InvalidExprInElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*)
  and modification_arguments_of_path cl_spec each final id path =
    let flow = false
    and var = Types.Continuous
    and inout = Types.Acausal in
    let elt_nat = element_field_type_nature ctx flow var inout cl_spec id in
    {
      each = each;
      final = final;
      target = id;
      action = resolve_modification_action ctx modif elt_nat path
    }
  and resolve_modification_action ctx modif elt_nat = function
    | [] -> resolve_modification_option ctx elt_nat modif
    | id :: path ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_FieldAccessInElemModifExpr"];
           err_info = [];
           err_ctx = ctx})
  and resolve_modification_option ctx elt_nat = function
    | None -> None
    | Some modif ->
        Some (ElementModification (resolve_modification ctx elt_nat modif)) in
  path_of_expression [] expr

and resolve_element_redeclaration ctx cl_spec each final elt_def =
  let ctx = {ctx with location = elt_def.Syntax.info} in
  raise (CompilError
    {err_msg = ["_NotYetImplemented"; "_ElementRedeclaration"];
     err_info = [];
     err_ctx = ctx})

and resolve_unnamed_elements ctx other_elts =
  let class_kind =
    let class_context' cl_spec = match cl_spec with
      | Types.ClassType cl_type ->
          Some (evaluate cl_type.Types.kind)
      | _ -> None in
    match ctx.context_nature with
    | ClassContext cl_def ->
        class_context' (evaluate cl_def.class_type)
    | _ -> None in
  let add_equation_or_algorithm_clause other_elt acc =
    match other_elt.Syntax.nature, class_kind with
      | (Syntax.EquationClause _), Some kind
        when List.mem kind [Types.Function; Types.Record; Types.Connector] ->
          raise (CompilError
            {err_msg = ["_EquNotAllowedInTheDefOf"; Types.string_of_kind kind];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | Syntax.EquationClause (init, equ_defs), _ ->
          let init' = bool_of_initial init
          and equ_defs' = resolve_equation_definitions ctx equ_defs in
          EquationClause (init', equ_defs') :: acc
      | Syntax.AlgorithmClause (init, algo_defs), _ ->
          let init' = bool_of_initial init
          and algo_defs' = resolve_algorithm_definitions ctx algo_defs in
          AlgorithmClause (init', algo_defs') :: acc
      | (Syntax.Public _ | Syntax.Protected _), _ -> acc in
  List.fold_right add_equation_or_algorithm_clause other_elts []

and bool_of_initial = function
  | None -> Permanent
  | Some Syntax.Initial -> Initial

and resolve_equation_definitions ctx equ_defs =
  let resolve_equation_definition equ_def = match equ_def.Syntax.nature with
    | Syntax.Equation (equ, _, _) -> resolve_equation ctx equ in
  List.flatten (List.map resolve_equation_definition equ_defs)

and resolve_algorithm_definitions ctx algo_defs =
  let resolve_algorithm_definition algo_def = match algo_def.Syntax.nature with
    | Syntax.Algorithm (algo, _, _) -> resolve_algorithm ctx algo in
  List.map resolve_algorithm_definition algo_defs

and resolve_equation ctx equ =
  let ctx = {ctx with location = equ.Syntax.info} in
  match equ.Syntax.nature with
    | Syntax.Equal (expr, expr') -> resolve_equal ctx equ expr expr'
    | Syntax.ConditionalEquationE (alts, default) ->
        resolve_conditional_equation_e ctx equ alts default
    | Syntax.ForClauseE (for_inds, equs) ->
        resolve_for_clause_e ctx equ for_inds equs
    | Syntax.ConnectClause (expr, expr') ->
        resolve_connect_clause ctx equ expr expr'
    | Syntax.WhenClauseE alts ->
        resolve_when_clause_e ctx equ alts
    | Syntax.FunctionCallE (expr, fun_args) ->
        resolve_functional_call_e ctx equ expr fun_args

and resolve_equal ctx equ expres expres' =
  let resolve_equal' cpnt_type expr cpnt_type' expr' =
    let resolved_equation syn expr expr' =
      {
        nature = Equal (expr, expr');
        info = syn
      } in
    let var = evaluate cpnt_type.Types.variability
    and var' = evaluate cpnt_type'.Types.variability in
    match var, var' with
    | Types.Continuous, _ | _, Types.Continuous ->
        equations ctx equ resolved_equation cpnt_type expr cpnt_type' expr'
    | Types.Discrete, _ | _, Types.Discrete
      when expression_of_variable expres ->
        equations ctx equ resolved_equation cpnt_type expr cpnt_type' expr'
    | Types.Discrete, _ | _, Types.Discrete ->
        raise (CompilError
          {err_msg = ["_LHSOfDiscreteEquMustBeAVar"];
           err_info = [];
           err_ctx = {ctx with location = expres.Syntax.info}}) (*error*)
    | _ ->
        equations ctx equ resolved_equation cpnt_type expr cpnt_type' expr' in
  let expr = resolve_expression ctx expres
  and expr' = resolve_expression ctx expres' in
  let exprs = apply_binary_coercions [ expr; expr' ] in
  let expr = List.nth exprs 0
  and expr' = List.nth exprs 1 in
  let elt_nat = expr.info.type_description
  and elt_nat' = expr'.info.type_description in
  match elt_nat, elt_nat' with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_equal' cpnt_type expr cpnt_type' expr'
    | (Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _), _ ->
        let ctx = {ctx with location = expres.Syntax.info} in
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | _, (Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _) ->
        let ctx = {ctx with location = expres'.Syntax.info} in
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_conditional_equation_e ctx equ alts default =
  let resolve_alternative (expr, equs) =
    let ctx = {ctx with location = expr.Syntax.info} in
    let expr' = resolve_expression ctx expr in
    let resolve_alternative' cpnt_type =
      let cl_spec = evaluate cpnt_type.Types.base_class in
      match cl_spec with
        | Types.PredefinedType { Types.base_type = Types.BooleanType } ->
            let equs' = List.flatten (List.map (resolve_equation ctx) equs) in
            expr', equs'
        | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
          Types.ArrayType _ | Types.TupleType _ ->
            raise (CompilError
              {err_msg = ["_NonBooleanIfCondExpr"];
               err_info =
                 [("_ExprKind", "...if A then...");
                  ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
               err_ctx = ctx}) (*error*) in
    match expr'.info.type_description with
      | Types.ComponentElement cpnt_type -> resolve_alternative' cpnt_type
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*) in
  let alts' = List.map resolve_alternative alts in
  let default' = match default with
    | None -> []
    | Some equs -> List.flatten (List.map (resolve_equation ctx) equs) in
  [{
    nature = ConditionalEquationE (alts', default');
    info = Some equ
  }]

and resolve_for_clause_e ctx equ for_inds equs =
  let range_element_type expr range =
    let ctx = {ctx with location = expr.Syntax.info} in
    let sub_dimension cl_spec = match cl_spec with
      | Types.ArrayType (dim, cl_spec) -> cl_spec
      | Types.PredefinedType _ | Types.ClassType _ |
        Types.ComponentType _ | Types.TupleType _ ->
          raise (CompilError
            {err_msg = ["_InvalidTypeInRangeExpr"];
             err_info =
               [("_ExpectedType", "Integer");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    match range.info.type_description with
      | Types.ComponentElement cpnt_type ->
          let cl_spec = evaluate cpnt_type.Types.base_class in
          let cpnt_type' =
            { cpnt_type with
              Types.base_class = lazy (sub_dimension cl_spec)
            } in
          Types.ComponentElement cpnt_type'
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*) in
  let rec resolve_for_clause_e' acc ctx = function
    | [] ->
        let equs' = List.flatten (List.map (resolve_equation ctx) equs) in
        [{
          nature = ForClauseE (List.rev acc, equs');
          info = Some equ
        }]
    | (_, None) :: _ ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_ImplicitIterRange"];
           err_info = [];
           err_ctx = ctx})
    | (id, Some expr) :: for_inds ->
        let range = resolve_expression ctx expr in
        let elt_nat = range_element_type expr range in
        let ctx' =
          { ctx with
            context_nature = ForContext (ctx, id, elt_nat)
          } in
        resolve_for_clause_e' (range :: acc) ctx' for_inds in
  resolve_for_clause_e' [] ctx for_inds

and resolve_connect_clause ctx equ expres expres' =
  let expr = resolve_expression ctx expres
  and expr' = resolve_expression ctx expres' in
  let resolve_connect_clause' cpnt_typ cpnt_typ' =
    let rec class_type_of_class_specifier cl_spec = match cl_spec with
      | Types.ClassType cl_type -> cl_type
      | Types.ComponentType cpnt_type ->
          let cl_spec = evaluate cpnt_type.Types.base_class in
          class_type_of_class_specifier cl_spec
      | Types.ArrayType (_, cl_spec) -> class_type_of_class_specifier cl_spec
      | Types.PredefinedType _ | Types.TupleType _ ->
          raise (CompilError
              {err_msg = ["_InvalidTypeOfArgInConnectStat"];
               err_info =
                 [("_ExprKind", "connect(A, B)");
                  ("_TypeOfA", Types.string_of_component_type cpnt_typ);
                  ("_TypeOfB", Types.string_of_component_type cpnt_typ')];
               err_ctx = ctx}) (*error*) in
    let connector_sign expr =
      let is_connector_type expr =
        let is_connector_type' cpnt_type =
          let cl_spec = evaluate cpnt_type.Types.base_class in
          let cl_type = class_type_of_class_specifier cl_spec in
          match evaluate cl_type.Types.kind with
            | Types.Connector | Types.ExpandableConnector -> true
            | Types.Class | Types.Model | Types.Block -> false
            | Types.Record ->
                raise (CompilError
                  {err_msg =
                     ["record"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | Types.Package ->
                raise (CompilError
                  {err_msg =
                     ["package"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | Types.Function ->
                raise (CompilError
                  {err_msg =
                     ["function"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*) in
        match expr.info.type_description with
          | Types.ComponentElement cpnt_type ->
              is_connector_type' cpnt_type
          | _ ->
              raise (CompilError
                {err_msg = ["_ClassElemFoundInExpr"];
                 err_info = [];
                 err_ctx = ctx}) (*error*) in
      let is_connectable expr =
        let is_connectable' cpnt_type =
          let cl_spec = evaluate cpnt_type.Types.base_class in
          let cl_type = class_type_of_class_specifier cl_spec in
          match evaluate cl_type.Types.kind with
            | Types.Class | Types.Model | Types.Block -> true
            | Types.Connector | Types.ExpandableConnector -> false
            | Types.Record ->
                raise (CompilError
                  {err_msg =
                     ["record"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | Types.Package ->
                raise (CompilError
                  {err_msg =
                     ["package"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | Types.Function ->
                raise (CompilError
                  {err_msg =
                     ["function"; "_InstanceUsedInConnection"];
                   err_info = [];
                   err_ctx = ctx}) (*error*) in
        match expr.info.type_description with
          | Types.ComponentElement cpnt_type ->
              is_connectable' cpnt_type
          | _ ->
              raise (CompilError
                {err_msg = ["_ClassElemFoundInExpr"];
                 err_info = [];
                 err_ctx = ctx}) (*error*) in
      let rec connector_sign' expr = match expr.nature with
        | LocalIdentifier (0, _) when is_connector_type expr -> Some Negative
        | LocalIdentifier (0, _) when is_connectable expr -> Some Positive
        | (FieldAccess (expr', _) | IndexedAccess (expr', _))
          when is_connector_type expr -> connector_sign' expr'
        | (FieldAccess (expr', _) | IndexedAccess (expr', _))
          when is_connectable expr' -> connector_sign' expr'
        | _ ->
            raise (CompilError
              {err_msg = ["_InvalidTypeOfArgInConnectStat"];
               err_info =
                 [("_ExprKind", "connect(A, B)");
                  ("_TypeOfA", Types.string_of_component_type cpnt_typ);
                  ("_TypeOfB", Types.string_of_component_type cpnt_typ')];
               err_ctx = ctx}) (*error*) in
      match expr.nature with
        | _ when not (is_connector_type expr) ->
            raise (CompilError
              {err_msg = ["_InvalidTypeOfArgInConnectStat"];
               err_info =
                 [("_ExprKind", "connect(A, B)");
                  ("_TypeOfA", Types.string_of_component_type cpnt_typ);
                  ("_TypeOfB", Types.string_of_component_type cpnt_typ')];
               err_ctx = ctx}) (*error*)
        | LocalIdentifier (0, _) -> Some Negative
        | _ -> connector_sign' expr in
    let connect sign cpnt_type sign' cpnt_type' =
      let resolved_equation syn expr expr' =
        let elt_nat = expr.info.type_description
        and elt_nat' = expr'.info.type_description in
        let flow, _, _ = type_prefixes_of_element_nature elt_nat
        and flow', _, _ = type_prefixes_of_element_nature elt_nat' in
        match flow, flow' with
          | false, false ->
              {
                nature = Equal (expr, expr');
                info = syn
              }
          | true, true ->
              {
                nature = ConnectFlows (sign, expr, sign', expr');
                info = syn
              }
          | false, true ->
              raise (CompilError
                {err_msg = ["_CannotConnectFlowAndNonFlowComp"];
                 err_info =
                   [("_ExprKind", "connect(A, B)");
                    ("_TypeOfA", "non-flow connector");
                    ("_TypeOfB", "flow connector")];
                 err_ctx = ctx}) (*error*)
          | true, false ->
              raise (CompilError
                {err_msg = ["_CannotConnectFlowAndNonFlowComp"];
                 err_info =
                   [("_ExprKind", "connect(A, B)");
                    ("_TypeOfA", "flow connector");
                    ("_TypeOfB", "non-flow connector")];
                 err_ctx = ctx}) (*error*) in
      equations ctx equ resolved_equation cpnt_type expr cpnt_type' expr' in
    match connector_sign expr, connector_sign expr' with
      | Some sign, Some sign' -> connect sign cpnt_typ sign' cpnt_typ'
      | None, Some _ -> assert false
      | Some _, None -> assert false
      | None, None -> assert false in
  let elt_nat = expr.info.type_description
  and elt_nat' = expr'.info.type_description in
  match elt_nat, elt_nat' with
    | Types.ComponentElement cpnt_typ, Types.ComponentElement cpnt_typ' ->
        resolve_connect_clause' cpnt_typ cpnt_typ'
    | _, _ ->
        raise (CompilError
            {err_msg = ["_InvalidTypeOfArgInConnectStat"];
             err_info =
               [("_ExprKind", "connect(A, B)");
                ("_TypeOfA", Types.string_of_element_nature elt_nat);
                ("_TypeOfB", Types.string_of_element_nature elt_nat')];
             err_ctx = ctx}) (*error*)

and resolve_when_clause_e ctx equ alts =
  let resolve_alternative (expr, equs) =
    let expr' = resolve_expression ctx expr in
    let rec check_equation equ =
      let check_equal expr expr' =
        match expr.Syntax.nature, expr'.Syntax.nature with
        | _, _ when expression_of_variable expr -> true
        | Syntax.Tuple exprs, Syntax.FunctionCall _
            when List.for_all expression_of_variable exprs -> true
        | _, _ -> raise (CompilError
            {err_msg = ["_InvalidWhenEquation"];
             err_info = [];
             err_ctx = {ctx with location = expr.Syntax.info}}) (*error*) in
      let check_alternative (expr, equs) =
        List.for_all check_equation equs in
      let check_function_call_e expr fun_args =
        match expr.Syntax.nature with
        | Syntax.Identifier "assert" |
          Syntax.Identifier "terminate" |
          Syntax.Identifier "reinit" -> true
        | _ ->
            raise (CompilError
              {err_msg = ["_InvalidWhenEquation"];
               err_info = [];
               err_ctx = {ctx with location = expr.Syntax.info}}) in
      match equ.Syntax.nature with
      | Syntax.Equal (expr, expr') -> check_equal expr expr'
      | Syntax.ConditionalEquationE (alts, None) ->
          List.for_all check_alternative alts
      | Syntax.ConditionalEquationE (alts, Some equs) ->
          (List.for_all check_alternative alts) &&
          (List.for_all check_equation equs)
      | Syntax.ForClauseE (for_inds, equs) ->
          List.for_all check_equation equs
      | Syntax.ConnectClause (expr, expr') ->
          raise (CompilError
            {err_msg = ["_InvalidWhenEquation"];
             err_info = [];
             err_ctx = {ctx with location = equ.Syntax.info}}) (*error*)
      | Syntax.WhenClauseE alts ->
          raise (CompilError
            {err_msg = ["_WhenClausesCannotBeNested"];
             err_info = [];
             err_ctx = {ctx with location = equ.Syntax.info}}) (*error*)
      | Syntax.FunctionCallE (expr, fun_args) ->
          check_function_call_e expr fun_args in
    let resolve_alternative' cpnt_type =
      let cl_spec = evaluate cpnt_type.Types.base_class in
      match cl_spec with
      | Types.ArrayType (Types.DiscreteDimension, _) ->
          raise (CompilError
            {err_msg = ["_InvalidTypeOfWhenCond"];
             err_info =
               [("_ExprKind", "...when A then...");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
             err_ctx = {ctx with location = expr.Syntax.info}}) (*error*)
      | Types.PredefinedType { Types.base_type = Types.BooleanType } |
        Types.ArrayType
          (_, Types.PredefinedType { Types.base_type = Types.BooleanType })
              when List.for_all check_equation equs ->
              let equs' = List.flatten (List.map (resolve_equation ctx) equs) in
              expr', equs'
      | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
        Types.ArrayType _ | Types.TupleType _ ->
          raise (CompilError
            {err_msg = ["_InvalidTypeOfWhenCond"];
             err_info =
               [("_ExprKind", "...when A then...");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
             err_ctx = {ctx with location = expr.Syntax.info}}) (*error*) in
    match expr'.info.type_description with
    | Types.ComponentElement cpnt_type
        when (evaluate cpnt_type.Types.variability) <> Types.Continuous ->
          resolve_alternative' cpnt_type
    | Types.ComponentElement cpnt_type ->
        raise (CompilError
          {err_msg = ["_WhenConditionMustBeDiscrete"];
           err_info = [];
           err_ctx = {ctx with location = expr.Syntax.info}}) (*error*)
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = {ctx with location = expr.Syntax.info}}) (*error*) in
  let alts' = List.map resolve_alternative alts in
  [{
    nature = WhenClauseE alts';
    info = Some equ
  }]

and resolve_functional_call_e ctx equ expr fun_args =
  let ctx = {ctx with location = equ.Syntax.info} in
  let res =
    let nat = Tuple []
    and elt_nat = Types.empty_tuple_type Types.Constant in
    resolved_expression None nat elt_nat in
  let fun_call = resolve_function_call ctx None expr fun_args in
  let resolve_functional_call_e cpnt_type =
    let cl_spec = evaluate cpnt_type.Types.base_class in
    match cl_spec with
      | Types.TupleType [] ->
          [{
            nature = Equal (res, fun_call);
            info = Some equ
          }]
      | _ ->
          raise (CompilError
            {err_msg = ["_NonEmptyFuncCallUsedAsAnEqu"];
             err_info =
               [("_TypeOfFuncValue", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
  match fun_call.info.type_description with
    | Types.ComponentElement cpnt_type -> resolve_functional_call_e cpnt_type
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and equations ctx equ resolved_equation cpnt_type expr cpnt_type' expr' =
  let equivalent_types predef predef' =
    match Types.compare_predefined_types predef predef',
          Types.compare_predefined_types predef' predef with
    | _, Types.NotRelated | Types.NotRelated, _ -> false
    | _ -> true in
  let rec equations' i subs cl_spec expr cl_spec' expr' =
    match cl_spec, cl_spec' with
      | Types.PredefinedType predef, Types.PredefinedType predef'
          when equivalent_types predef predef' ->
            [equation subs expr expr']
      | Types.ComponentType cpnt_type, Types.ComponentType cpnt_type' ->
          raise (CompilError
            {err_msg = ["_NotYetImplemented"; "_ComponentTypeEqu"];
             err_info = [];
             err_ctx = ctx})
      | Types.ClassType cl_type, Types.ClassType cl_type' ->
          record_equations subs cl_type expr cl_type' expr'
      | Types.ArrayType (dim, cl_spec), Types.ArrayType (dim', cl_spec') ->
          [for_equation i subs dim cl_spec expr dim' cl_spec' expr']
      | Types.TupleType cl_specs, Types.TupleType cl_specs' ->
          [{
            nature = Equal (expr, expr');
            info = Some equ
          }]
      | (Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
         Types.TupleType _ | Types.ClassType _),
        (Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
         Types.TupleType _ | Types.ClassType _) ->
          raise (CompilError
            {err_msg = ["_EquTermsNotOfTheSameType"];
             err_info =
               [("_ExprKind", "A = B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = {ctx with location = equ.Syntax.info}}) (*error*)
  and for_equation i subs dim cl_spec expr dim' cl_spec' expr' =
    match dim, dim' with
      | Types.ConstantDimension n, Types.ConstantDimension n' when n <> n' ->
          let type_A = Types.string_of_component_type cpnt_type
          and type_B = Types.string_of_component_type cpnt_type' in
          raise (CompilError
            {err_msg = ["_ArrayDimMismatchInEqu"];
             err_info = [("_ExprKind", "A = B");
                         ("_TypeOfA", type_A);
                         ("_TypeOfB", type_B)];
             err_ctx = ctx}) (*error*)
      | (Types.ConstantDimension _ | Types.ParameterDimension),
        (Types.ConstantDimension _ | Types.ParameterDimension) ->
          let range = resolve_colon ctx expr (Int32.of_int i) dim in
          let subs =
            let nat = LoopVariable (i - 1)
            and elt_nat = Types.integer_type Types.Constant in
            resolved_expression None nat elt_nat :: subs in
          let equs = equations' (i + 1) subs cl_spec expr cl_spec' expr' in
          {
            nature = ForClauseE ([range], equs);
            info = Some equ
          }
      | (Types.ConstantDimension _ | Types.ParameterDimension |
         Types.DiscreteDimension),
        (Types.ConstantDimension _ | Types.ParameterDimension |
         Types.DiscreteDimension) ->
          let type_A = Types.string_of_component_type cpnt_type
          and type_B = Types.string_of_component_type cpnt_type' in
          raise (CompilError
            {err_msg = ["_ArrayDimMismatchInEqu"];
             err_info = [("_ExprKind", "A = B");
                         ("_TypeOfA", type_A);
                         ("_TypeOfB", type_B)];
             err_ctx = ctx}) (*error*)
  and record_equations subs cl_type expr cl_type' expr' =
    let named_elts = cl_type.Types.named_elements
    and named_elts' = cl_type'.Types.named_elements in
    let record_equations' expr expr' =
      let class_spec_of_element_type elt_type =
        let elt_type' = evaluate elt_type in
        element_nature_class ctx elt_type'.Types.element_nature in
      let record_equation (id, elt_type) =
        let elt_type' =
          try
            List.assoc id named_elts'
          with _ ->
            raise (CompilError
              {err_msg = ["_EquTermsNotOfTheSameType"];
               err_info =
                 [("_ExprKind", "A = B");
                  ("_TypeOfA", Types.string_of_component_type cpnt_type);
                  ("_TypeOfB", Types.string_of_component_type cpnt_type')];
               err_ctx = {ctx with location = equ.Syntax.info}}) (*error*) in
        let cl_spec = class_spec_of_element_type elt_type
        and cl_spec' = class_spec_of_element_type elt_type' in
        let expr =
          let nat = FieldAccess (expr, id)
          and flow, var, inout =
            type_prefixes_of_element_nature expr.info.type_description
          and cl_spec = element_nature_class ctx expr.info.type_description in
          let elt_nat =
            element_field_type_nature ctx flow var inout cl_spec id in
          resolved_expression None nat elt_nat
        and expr' =
          let nat = FieldAccess (expr', id)
          and flow, var, inout =
            type_prefixes_of_element_nature expr'.info.type_description
          and cl_spec = element_nature_class ctx expr'.info.type_description in
          let elt_nat =
            element_field_type_nature ctx flow var inout cl_spec id in
          resolved_expression None nat elt_nat in
        equations' 1 [] cl_spec expr cl_spec' expr' in
      List.flatten (List.map record_equation named_elts) in
    match subs with
      | [] -> record_equations' expr expr'
      | subs ->
          let expr =
            let elt_nat = expr.info.type_description in
            let nat = IndexedAccess (expr, subs)
            and elt_nat' = scalar_element_nature elt_nat in
            resolved_expression None nat elt_nat'
          and expr' =
            let elt_nat = expr'.info.type_description in
            let nat = IndexedAccess (expr', subs)
            and elt_nat' = scalar_element_nature elt_nat in
            resolved_expression None nat elt_nat' in
          record_equations' expr expr'
  and equation subs expr expr' = match subs with
    | [] -> resolved_equation (Some equ) expr expr'
    | subs ->
        let expr =
          let elt_nat = expr.info.type_description in
          let nat = IndexedAccess (expr, subs)
          and elt_nat' = scalar_element_nature elt_nat in
          resolved_expression None nat elt_nat'
        and expr' =
          let elt_nat = expr'.info.type_description in
          let nat = IndexedAccess (expr', subs)
          and elt_nat' = scalar_element_nature elt_nat in
          resolved_expression None nat elt_nat' in
        resolved_equation None expr expr' in
  let cl_spec = evaluate cpnt_type.Types.base_class
  and cl_spec' = evaluate cpnt_type'.Types.base_class in
  equations' 1 [] cl_spec expr cl_spec' expr'

and resolve_algorithm ctx algo =
  let ctx = {ctx with location = algo.Syntax.info} in
  match algo.Syntax.nature with
    | Syntax.Assign _ |
      Syntax.FunctionCallA _ |
      Syntax.MultipleAssign _ |
      Syntax.Break |
      Syntax.Return |
      Syntax.ConditionalEquationA _ |
      Syntax.ForClauseA _ |
      Syntax.WhileClause _ |
      Syntax.WhenClauseA _ ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_AlgoClause"];
           err_info = [];
           err_ctx = ctx})

and resolve_expression ctx expr =
  let ctx = {ctx with location = expr.Syntax.info} in
  match expr.Syntax.nature with
    | Syntax.BinaryOperation (kind, arg1, arg2) ->
        resolve_binary_operation ctx expr kind arg1 arg2
    | Syntax.End -> resolve_end ctx expr
    | Syntax.False -> resolve_false ctx expr
    | Syntax.FieldAccess (expr', id) -> resolve_field_access ctx expr expr' id
    | Syntax.FunctionCall (expr', fun_args) ->
        resolve_function_call ctx (Some expr) expr' fun_args
    | Syntax.Identifier id -> resolve_identifier ctx expr id
    | Syntax.If (alts, expr') -> resolve_if ctx expr alts expr'
    | Syntax.IndexedAccess (expr', subs) ->
        resolve_indexed_access ctx expr expr' subs
    | Syntax.Integer s -> resolve_integer ctx expr s
    | Syntax.MatrixConstruction exprss ->
        resolve_matrix_construction ctx expr exprss
    | Syntax.NoEvent expr' ->
        resolve_no_event ctx expr expr'
    | Syntax.Range (start, step, stop) ->
        resolve_range ctx expr start step stop
    | Syntax.Real s -> resolve_real ctx expr s
    | Syntax.String s -> resolve_string ctx expr s
    | Syntax.True -> resolve_true ctx expr
    | Syntax.Tuple exprs -> resolve_tuple ctx expr exprs
    | Syntax.UnaryOperation (kind, arg) ->
        resolve_unuary_operation ctx expr kind arg
    | Syntax.Vector vec_elts -> resolve_vector ctx expr vec_elts

and resolve_binary_operation ctx expr kind arg1 arg2 =
  let arg1' = resolve_expression ctx arg1
  and arg2' = resolve_expression ctx arg2 in
  let args' = apply_binary_coercions [ arg1'; arg2' ] in
  let arg1' = List.nth args' 0
  and arg2' = List.nth args' 1 in
  match kind.Syntax.nature with
    | Syntax.Plus -> resolve_addition ctx expr arg1' arg2'
    | Syntax.And -> resolve_and ctx expr arg1' arg2'
    | Syntax.Divide -> resolve_division ctx expr arg1' arg2'
    | Syntax.EqualEqual -> raise (CompilError
        {err_msg = ["_NotYetImplemented"; "_BinaryOperEQUEQU"];
         err_info = [];
         err_ctx = ctx})
    | Syntax.GreaterEqual ->
        resolve_comparison ctx expr GreaterEqual arg1' arg2'
    | Syntax.Greater -> resolve_comparison ctx expr Greater arg1' arg2'
    | Syntax.LessEqual -> resolve_comparison ctx expr LessEqual arg1' arg2'
    | Syntax.Less -> resolve_comparison ctx expr Less arg1' arg2'
    | Syntax.Times -> resolve_multiplication ctx expr arg1' arg2'
    | Syntax.NotEqual -> raise (CompilError
        {err_msg = ["_NotYetImplemented"; "_BinaryOperDIFF"];
         err_info = [];
         err_ctx = ctx})
    | Syntax.Or -> resolve_or ctx expr arg1' arg2'
    | Syntax.Power -> resolve_power ctx expr arg1' arg2'
    | Syntax.Minus -> resolve_subtraction ctx expr arg1' arg2'

and resolve_end ctx expr =
  let ctx = {ctx with location = expr.Syntax.info} in
  match ctx.context_nature with
    | SubscriptContext (_, _, _, Types.ConstantDimension n) ->
        let nat = Integer n
        and elt_nat = Types.integer_type Types.Constant in
        resolved_expression (Some expr) nat elt_nat
    | SubscriptContext (_, expr', n, Types.ParameterDimension) ->
        size_function_call ctx (Some expr) expr' n
    | SubscriptContext (_, expr', n, Types.DiscreteDimension) ->
        size_function_call ctx (Some expr) expr' n
    | ForContext (ctx', _, _) -> resolve_end ctx' expr
    | ToplevelContext | ClassContext _ ->
        raise (CompilError
          {err_msg = ["_InvalidKeyWordEndInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_false ctx expr =
  resolved_expression (Some expr) False (Types.boolean_type Types.Constant)

and resolve_field_access ctx expr expr' id =
  let expr' = resolve_expression ctx expr' in
  let resolve_field_access' expr' id =
  let nat = FieldAccess (expr', id)
  and flow, var, inout =
    type_prefixes_of_element_nature expr'.info.type_description
  and cl_spec = element_nature_class ctx expr'.info.type_description in
  let elt_nat = element_field_type_nature ctx flow var inout cl_spec id in
    resolved_expression (Some expr) nat elt_nat in
  let is_package cl_spec = match evaluate cl_spec with
    | Types.ClassType cl_type
      when evaluate cl_type.Types.kind = Types.Package -> true
    | _ -> false in
  match expr'.info.type_description with
  | Types.ComponentElement _  ->
      resolve_field_access' expr' id
  | Types.ClassElement cl_spec when is_package cl_spec ->
      resolve_field_access' expr' id
  | _ ->
      raise (CompilError
        {err_msg = ["component or package"; "_ElemExpected"];
         err_info = [];
         err_ctx = { ctx with location = expr.Syntax.info }}) (*error*)

and type_prefixes_of_element_nature = function
  | Types.ComponentElement cpnt_type ->
      evaluate cpnt_type.Types.flow,
      evaluate cpnt_type.Types.variability,
      evaluate cpnt_type.Types.causality
  | Types.ClassElement _ | Types.ComponentTypeElement _ |
    Types.PredefinedTypeElement _ ->
      false, Types.Constant, Types.Acausal

and resolve_function_call ctx syn expr fun_args =
  let ctx = {ctx with location = expr.Syntax.info} in
  let expr' = resolve_expression ctx expr in
  let resolve_function_arguments named_elts =
    let reversed_additional_dimensions input_types args =
      let additional_named_element_dimensions id arg =
        let rec subtract_dimensions fun_dims arg_dims =
          match fun_dims, arg_dims with
            | [], _ ->  arg_dims
            | _, [] ->
                raise (CompilError
                  {err_msg = ["_ArgDimMismatch"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | Types.ConstantDimension i :: _, Types.ConstantDimension i' :: _
              when i <> i' ->
                raise (CompilError
                  {err_msg = ["_ArgDimMismatch"];
                   err_info = [];
                   err_ctx = ctx}) (*error*)
            | _ :: fun_dims, _ :: arg_dims ->
                subtract_dimensions fun_dims arg_dims in
        let elt_type = List.assoc id input_types in
        let elt_type' = evaluate elt_type in
        let fun_dims =
          Types.reversed_element_dimensions elt_type'.Types.element_nature
        and arg_dims =
          Types.reversed_element_dimensions arg.info.type_description in
        subtract_dimensions fun_dims arg_dims in
      let rec reversed_additional_dimensions' ids dims args =
        match args with
          | [] -> ids, dims
          | (id, arg) :: args ->
              let dims' = additional_named_element_dimensions id arg in
              update_additional_dimensions ids dims id dims' args
      and update_additional_dimensions ids dims id dims' args =
        match dims, dims' with
          | _, [] -> reversed_additional_dimensions' ids dims args
          | [], _ :: _ ->
              let ids' = id :: ids in
              reversed_additional_dimensions' ids' dims' args
          | _ :: _, _ :: _ when dims <> dims' ->
              raise (CompilError
                {err_msg = ["_ArgDimMismatchInVectCall"];
                 err_info = [];
                 err_ctx = ctx}) (*error*)
          | _ :: _, _ :: _ ->
              let ids' = id :: ids in
              reversed_additional_dimensions' ids' dims args in
      reversed_additional_dimensions' [] [] args in
    let function_call ids rev_dims input_types output_types args =
      let ndims = List.length rev_dims in
      let rec expressions_of_named_arguments pos input_types =
        let expression_of_default_argument id elt_type =
          let elt_type' = evaluate elt_type in
          let func =
            let nat = FunctionArgument 0
            and elt_nat = expr'.info.type_description in
            resolved_expression None nat elt_nat in
          let nat = FieldAccess (func, id)
          and elt_nat = elt_type'.Types.element_nature in
          resolved_expression None nat elt_nat
        and expression_of_named_argument pos id elt_type =
          let rec loop_variables = function
            | 0 -> []
            | ndims ->
                let nat = LoopVariable (ndims - 1)
                and elt_nat = (Types.integer_type Types.Constant) in
                let loop_var = resolved_expression None nat elt_nat in
                loop_var :: loop_variables (ndims - 1) in
          let elt_type' = evaluate elt_type in
          let elt_nat = elt_type'.Types.element_nature in
          let nat = match List.mem id ids with
            | false -> FunctionArgument pos
            | true ->
                let arg = List.assoc id args in
                let nat = FunctionArgument pos
                and elt_nat = arg.info.type_description in
                let expr = resolved_expression None nat elt_nat in
                IndexedAccess (expr, loop_variables ndims) in
          resolved_expression None nat elt_nat in
        match input_types with
          | [] -> []
          | (id, elt_type) :: input_types when not (List.mem_assoc id args) ->
              let arg = expression_of_default_argument id elt_type in
              arg :: expressions_of_named_arguments pos input_types
          | (id, elt_type) :: input_types ->
              let arg = expression_of_named_argument pos id elt_type in
              arg :: expressions_of_named_arguments (pos + 1) input_types in
      let ranges arg rev_dims =
        let rec ranges' acc n rev_dims =
          let range_of_dimension dim =
            let range_to stop =
              let nat = Range (one, one, stop)
              and elt_nat = Types.integer_array_type Types.Constant dim in
              resolved_expression None nat elt_nat in
            match dim with
              | Types.ConstantDimension i ->
                  let stop =
                    let nat = Integer i
                    and elt_nat = (Types.integer_type Types.Constant) in
                    resolved_expression None nat elt_nat in
                  range_to stop
              | Types.ParameterDimension ->
                  let stop = size_function_call ctx None arg n in
                  range_to stop
              | Types.DiscreteDimension ->
                  let stop = size_function_call ctx None arg n in
                  range_to stop in
          match rev_dims with
            | [] -> acc
            | dim :: rev_dims ->
                let range = range_of_dimension dim in
                ranges' (range :: acc) (Int32.succ n) rev_dims in
        ranges' [] 1l rev_dims in
      let rec sorted_arguments_of_named_arguments = function
        | [] -> []
        | (id, _) :: input_types when not (List.mem_assoc id args) ->
            sorted_arguments_of_named_arguments input_types
        | (id, _) :: input_types ->
            let arg = List.assoc id args in
            arg :: sorted_arguments_of_named_arguments input_types in
      let wrap_function_invocation cpnt_type =
        let add_dimensions cpnt_type =
          let rec add_dimensions cl_spec = function
            | [] -> cl_spec
            | dim :: rev_dims ->
                let cl_spec' = Types.ArrayType (dim, cl_spec) in
                add_dimensions cl_spec' rev_dims in
          let base_class = cpnt_type.Types.base_class in
          { cpnt_type with
            Types.base_class =
              lazy (add_dimensions (evaluate base_class) rev_dims)
          } in
        let wrap_function_invocation' cpnt_type rev_dims =
          let nat =
            let exprs = expressions_of_named_arguments 1 input_types in
            FunctionInvocation exprs
          and elt_nat = Types.ComponentElement cpnt_type in
          match ids with
            | [] ->
                resolved_expression syn nat elt_nat
            | id :: _ ->
                let cpnt_type' = add_dimensions cpnt_type in
                let nat =
                  let ranges =
                    let arg = List.assoc id args in
                    ranges arg rev_dims
                  and expr = resolved_expression None nat elt_nat in
                  VectorReduction (ranges, expr)
                and elt_nat = Types.ComponentElement cpnt_type' in
                resolved_expression None nat elt_nat in
        wrap_function_invocation' cpnt_type rev_dims in
      let component_type_of_output_types output_types =
        let component_type_of_output_type cpnt_type (_, elt_type) =
          let add_class_specifier cl_spec cl_spec' =
            match cl_spec, cl_spec' with
            | Types.TupleType [], _ -> cl_spec'
            | (Types.TupleType cl_specs), _ ->
                Types.TupleType (cl_spec' :: cl_specs)
            | _, _ -> Types.TupleType [cl_spec'; cl_spec] in
          let var = evaluate cpnt_type.Types.variability
          and cl_spec = evaluate cpnt_type.Types.base_class in
          let elt_type' = evaluate elt_type in
          match elt_type'.Types.element_nature with
            | Types.ComponentElement cpnt_type' ->
                let var' = evaluate cpnt_type'.Types.variability
                and cl_spec' = evaluate cpnt_type'.Types.base_class in
                {
                  Types.flow = lazy false;
                  Types.variability = lazy (Types.max_variability var var');
                  Types.causality = lazy Types.Acausal;
                  Types.base_class = lazy (add_class_specifier cl_spec cl_spec')
                }
            | Types.ClassElement _ | Types.ComponentTypeElement _ |
              Types.PredefinedTypeElement _ ->
                raise (CompilError
                  {err_msg = ["_ClassElemFoundInExpr"];
                   err_info = [];
                   err_ctx = ctx}) (*error*) in
        let cpnt_type =
          {
            Types.flow = lazy false;
            Types.variability = lazy Types.Constant;
            Types.causality = lazy Types.Acausal;
            Types.base_class = lazy (Types.TupleType [])
          } in
        List.fold_left component_type_of_output_type cpnt_type output_types in
      let args' = sorted_arguments_of_named_arguments input_types
      and cpnt_type = component_type_of_output_types output_types in
      let func_invoc = wrap_function_invocation cpnt_type in
      let nat = FunctionCall (expr', args', func_invoc)
      and elt_nat = func_invoc.info.type_description in
      resolved_expression syn nat elt_nat in
    let resolve_function_arguments' fun_args =
      match fun_args.Syntax.nature with
      | Syntax.Reduction _ ->
          raise (CompilError
            {err_msg = ["_NotYetImplemented"; "_FuncArgumentReduction"];
             err_info = [];
             err_ctx = ctx})
      | Syntax.ArgumentList args ->
          let input_types, output_types, named_args =
              resolve_function_argument_list ctx expr' named_elts args in
          let ids, rev_dims =
            reversed_additional_dimensions input_types named_args in
          function_call ids rev_dims input_types output_types named_args in
    match fun_args with
      | None ->
          let fun_args = { Syntax.nature = Syntax.ArgumentList [];
                           Syntax.info = ctx.location } in
          resolve_function_arguments' fun_args
      | Some fun_args -> resolve_function_arguments' fun_args in
  let resolve_class_function_call cl_type =
    match evaluate cl_type.Types.kind with
    | Types.Function ->
        resolve_function_arguments cl_type.Types.named_elements
    | Types.Class | Types.Model | Types.Block | Types.Record |
      Types.ExpandableConnector | Types.Connector | Types.Package ->
        raise (CompilError
          {err_msg = ["function"; "_ElemExpected"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
  let resolve_function_call' cl_spec =
    match evaluate cl_spec with
    | Types.ClassType cl_type ->
        resolve_class_function_call cl_type
    | _ ->
        raise (CompilError
          {err_msg = ["function"; "_ElemExpected"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
  match expr'.info.type_description with
  | Types.ClassElement cl_spec -> resolve_function_call' cl_spec
  | Types.ComponentElement cpnt_type ->
      let cl_spec = cpnt_type.Types.base_class in
      resolve_function_call' cl_spec
  | Types.ComponentTypeElement _ | Types.PredefinedTypeElement _ ->
      raise (CompilError
        {err_msg = ["function"; "_ElemExpected"];
         err_info = [];
         err_ctx = ctx}) (*error*)

and resolve_function_argument_list ctx expr' named_elts args =
  let rec class_kind ctx =
    let class_context' cl_spec = match cl_spec with
      | Types.ClassType cl_type ->
          Some (evaluate cl_type.Types.kind)
      | _ -> None in
    match ctx.context_nature with
    | ClassContext cl_def ->
        class_context' (evaluate cl_def.class_type)
    | SubscriptContext (ctx, _, _, _) | ForContext (ctx, _, _) ->
        class_kind ctx
    | _ -> None in
  let add_function_inout_argument ((id, elt_type) as named_elt) inouts =
    let add_function_inout_argument' cpnt_type =
      match inouts, evaluate cpnt_type.Types.causality with
      | (ins, outs), Types.Input -> named_elt :: ins, outs
      | (ins, outs), Types.Output -> ins, named_elt :: outs
      | _, Types.Acausal -> inouts in
    let elt_type' = evaluate elt_type in
    match elt_type'.Types.element_nature with
      | Types.ComponentElement cpnt_type when not elt_type'.Types.protected ->
          add_function_inout_argument' cpnt_type
      | _ -> inouts in
  let add_argument id arg arg' elt_type acc =
    let matchable_types cpnt_type cpnt_type' =
      let cl_spec = evaluate cpnt_type.Types.base_class
      and cl_spec' = evaluate cpnt_type'.Types.base_class in
      let rec matchable_types' cl_spec cl_spec' = match cl_spec, cl_spec' with
        | Types.ArrayType (dim, cl_spec), _ ->
            matchable_types' cl_spec cl_spec'
        | _, Types.ArrayType (dim', cl_spec') ->
            matchable_types' cl_spec cl_spec'
        | _, _ ->
            let type_compare = Types.compare_specifiers cl_spec cl_spec' in
            (type_compare = Types.SameType) ||
            (type_compare = Types.Supertype) in
      matchable_types' cl_spec cl_spec' in
    let matchable_variabilities cpnt_type cpnt_type' =
      let var = evaluate cpnt_type.Types.variability
      and var' = evaluate cpnt_type'.Types.variability in
      Types.higher_variability var var' in
    let elt_type = evaluate elt_type in
    let cpnt_type = match elt_type.Types.element_nature with
      | Types.ComponentElement cpnt_type -> cpnt_type
      | _ -> assert false in
    let arg' = apply_rhs_coercions cpnt_type arg' in
    match arg'.info.type_description with
    | Types.ComponentElement cpnt_type'
        when not (matchable_types cpnt_type cpnt_type') ->
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info =
             [("_ExpectedType", Types.string_of_component_type cpnt_type);
              ("_TypeFound", Types.string_of_component_type cpnt_type')];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | Types.ComponentElement cpnt_type'
        when not (matchable_variabilities cpnt_type cpnt_type') ->
        let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        let var = Types.string_of_variability var
        and var' = Types.string_of_variability var' in
        raise (CompilError
          {err_msg = ["_ArgVariabilityMismatch"];
           err_info = [("_ExpectedVariability", var);
                       ("_VariabilityFound", var')];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | Types.ComponentElement cpnt_type' -> (id, arg') :: acc
    | _ -> raise (CompilError
        {err_msg = ["_ClassElemFoundInExpr"];
         err_info = [];
         err_ctx = {ctx with location = arg.Syntax.info}}) (*error*) in
  let named_arguments_of_arguments input_types args =
    let rec add_positional_arguments acc input_types args =
      match input_types, args with
      | [], [] -> acc
      | [], _ ->
          raise (CompilError
            {err_msg = ["_TooManyArgsInFuncCall"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | _, [] ->
          raise (CompilError
            {err_msg = ["_TooFewArgsInFuncCall"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | (id, elt_type) :: input_types,
        { Syntax.nature = Syntax.Argument arg } :: args ->
          let arg' = resolve_expression ctx arg in
          let acc = add_argument id arg arg' elt_type acc in
          add_positional_arguments acc input_types args
      | _, { Syntax.nature = Syntax.NamedArgument _ } :: _ ->
          add_named_arguments acc input_types args
    and add_named_arguments acc input_types args =
      match input_types, args with
      | [], [] -> acc
      | [], _ ->
          raise (CompilError
            {err_msg = ["_TooManyArgsInFuncCall"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | _, [] ->
          raise (CompilError
            {err_msg = ["_TooFewArgsInFuncCall"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | _, { Syntax.nature = Syntax.Argument _ } :: _ ->
          raise (CompilError
            {err_msg = ["_MixedPositAndNamedFuncArgPass"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | _, { Syntax.nature = Syntax.NamedArgument (id, _) } :: _
          when List.mem_assoc id acc ->
            raise (CompilError
              {err_msg = ["_FuncCallWithDuplicateArg"; id];
               err_info = [];
               err_ctx = ctx}) (*error*)
      | _, { Syntax.nature = Syntax.NamedArgument (id, _) } :: _
          when not (List.mem_assoc id input_types) ->
            raise (CompilError
              {err_msg = ["_NonInputFuncArgElem"; id];
               err_info = [];
               err_ctx = ctx}) (*error*)
      | _, { Syntax.nature = Syntax.NamedArgument (id, arg) } :: args ->
          let arg' = resolve_expression ctx arg
          and elt_type = List.assoc id input_types in
          let acc = add_argument id arg arg' elt_type acc in
          add_named_arguments acc input_types args in
    add_positional_arguments [] input_types args in
  let resolve_built_in_function_argument arg = match arg with
    | { Syntax.nature = Syntax.Argument arg } ->
        arg, (resolve_expression ctx arg)
    | { Syntax.nature = Syntax.NamedArgument _; Syntax.info = info } ->
        raise (CompilError
          {err_msg = ["_CannotUseNamedArgWithBuiltInOper"];
           err_info = [];
           err_ctx = {ctx with location = info}}) (*error*) in
  let rec built_in_function_named_arguments acc input_types args' =
    match input_types, args' with
    | [], [] -> acc
    | [], _ ->
        raise (CompilError
          {err_msg = ["_TooManyArgsInFuncCall"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | _, [] ->
        raise (CompilError
          {err_msg = ["_TooFewArgsInFuncCall"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | (id, elt_type) :: input_types, (arg, arg') :: args' ->
        let acc = add_argument id arg arg' elt_type acc in
        built_in_function_named_arguments acc input_types args' in
  let built_in_function_inout_types ctx id (in_types, out_types) args' =
    let argument_component_type (arg, arg') =
      match arg'.info.type_description with
      | Types.ComponentElement cpnt_type ->
          cpnt_type
      | _ -> raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*) in
    let scalar_base_class_specifier (arg, arg') =
      let rec scalar_base_class_specifier' cl_spec = match cl_spec with
        | Types.ArrayType (dim, cl_spec) -> scalar_base_class_specifier' cl_spec
        | _ -> cl_spec in
      let cpnt_type = argument_component_type (arg, arg') in
      let cl_spec = evaluate cpnt_type.Types.base_class in
      scalar_base_class_specifier' cl_spec in
    let argument_base_type bt (arg, arg') =
      let cl_spec = scalar_base_class_specifier (arg, arg') in
      match cl_spec with
      | Types.PredefinedType predef when predef.Types.base_type = bt -> true
      | _ -> false in
    let argument_base_types bt args =
      List.for_all (argument_base_type bt) args in
    let argument_variability var (arg, arg') =
      let cpnt_type = argument_component_type (arg, arg') in
      let var' = evaluate cpnt_type.Types.variability in
      var = var' in
    let neg f = function x -> not (f x) in
    let ndims arg' =
      let cpnt_type = component_type_of_expression ctx arg' in
      let rec ndims' cl_spec =
        match cl_spec with
        | Types.ArrayType (dim, cl_spec) -> ndims' cl_spec + 1
        | _ -> 0 in
      ndims' (evaluate cpnt_type.Types.base_class) in
    let numeric_base_type arg' =
      let cl_spec = scalar_class_specifier ctx arg' in
      (Types.compare_specifiers Types.integer_class_type cl_spec =
         Types.SameType) ||
      (Types.compare_specifiers Types.real_class_type cl_spec =
         Types.SameType) in
    let rec argument_types i args = match args with
      | [] -> []
      | (arg, arg') :: args ->
          let cpnt_type = component_type_of_expression ctx arg'
          and name = Printf.sprintf "@%d" i in
          (name, cpnt_type) :: (argument_types (i + 1) args) in
    let element_types input_types output_types =
      let element_type inout (id, cpnt_type) =
        (id,
         lazy
           {
             Types.protected = false;
             Types.final = true;
             Types.replaceable = false;
             Types.dynamic_scope = None;
             Types.element_nature =
               Types.ComponentElement
                 { cpnt_type with Types.causality = lazy inout }
           }) in
     (List.map (element_type Types.Input) input_types),
     (List.map (element_type Types.Output) output_types) in
    match id, args' with
    | ("der" | "initial" | "terminal" | "sample" | "pre" | "edge" | "change" |
       "reinit" | "delay"), _ when (class_kind ctx) = Some Types.Function ->
        raise (CompilError
          {err_msg = [id; "_OperCannotBeUsedWithinFuncDef"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | ("pre" | "edge" | "change"), [arg, arg'] | "reinit", [(arg, arg'); _]
        when not (expression_of_variable arg) ->
        raise (CompilError
          {err_msg = [id; "_OperArgMustBeAVar"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | ("ceil" | "floor" | "integer" | "der"), [arg, arg'] |
      "reinit", [(arg, arg'); _] |
      "smooth", [_; (arg, arg')]
        when not (argument_base_type Types.RealType (arg, arg')) ->
        let cl_spec = scalar_base_class_specifier (arg, arg') in
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info =
             [("_ExpectedType", "Real");
              ("_TypeFound", Types.string_of_class_specifier cl_spec)];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | "delay", _
        when not (List.for_all (argument_base_type Types.RealType) args') ->
        let (arg, arg') =
          List.find (neg (argument_base_type Types.RealType)) args' in
        let cl_spec = scalar_base_class_specifier (arg, arg') in
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info =
             [("_ExpectedType", "Real");
              ("_TypeFound", Types.string_of_class_specifier cl_spec)];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | "der", [arg, arg']
        when not (argument_variability Types.Continuous (arg, arg')) ->
        let cpnt_type = argument_component_type (arg, arg') in
        let var = evaluate cpnt_type.Types.variability in
        let var = Types.string_of_variability var in
        raise (CompilError
          {err_msg = ["_ArgVariabilityMismatch"];
           err_info = [("_ExpectedVariability", "Continuous");
                       ("_VariabilityFound", var)];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | "delay", _ when List.length args' = 3 ->
        let input_types =
          [("@1", Types.real_component_type Types.Continuous);
           ("@2", Types.real_component_type Types.Continuous);
           ("@3", Types.real_component_type Types.Parameter)]
        and output_types =
          ["@4", Types.real_component_type Types.Continuous] in
        element_types input_types output_types
    | "abs", [arg, arg']
        when argument_base_type Types.IntegerType (arg, arg') ->
        let input_types = ["@1", Types.integer_component_type Types.Discrete]
        and output_types =
          ["@2", Types.integer_component_type Types.Discrete] in
        element_types input_types output_types
    | ("ones" | "zeros"), _
      when not (argument_base_types Types.IntegerType args') ->
        let (arg, arg') =
          List.find (neg (argument_base_type Types.IntegerType)) args' in
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | "fill", _ :: args'
      when not (argument_base_types Types.IntegerType args') ->
        let (arg, arg') =
          List.find (neg (argument_base_type Types.IntegerType)) args' in
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | ("sum" | "product" | "max" | "min" | "scalar"), [arg, arg']
      when ndims arg' = 0 ->
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | "diagonal", [arg, arg']
      when ndims arg' <> 1 ->
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | ("scalar"), [arg, arg'] ->
        let cpnt_type = component_type_of_expression ctx arg' in
        let input_types = ["@1", cpnt_type]
        and output_types =
          ["@2", Types.scalar_component_type cpnt_type ] in
        element_types input_types output_types
    | ("sum" | "product" | "max" | "min" | "diagonal"), [arg, arg']
      when not (numeric_base_type arg') ->
        raise (CompilError
          {err_msg = ["_ArgTypeMismatch"];
           err_info = [];
           err_ctx = {ctx with location = arg.Syntax.info}}) (*error*)
    | ("sum" | "product" | "max" | "min"), [arg, arg'] ->
        let cpnt_type = component_type_of_expression ctx arg' in
        let input_types = ["@1", cpnt_type]
        and output_types =
          ["@2", Types.scalar_component_type cpnt_type ] in
        element_types input_types output_types
    | ("ones" | "zeros"), _ :: _ ->
        let input_types = argument_types 1 args'
        and output_types =
          let n = List.length args'
          and dims =
            List.map (function _ -> Types.ParameterDimension) args' in
          let cpnt_type =
            { 
              Types.flow = lazy false;
              variability = lazy Types.Parameter;
              Types.causality = lazy Types.Acausal;
              base_class =
                lazy(Types.add_dimensions dims Types.integer_class_type)
            } in
          [ Printf.sprintf "@%d" (n + 1), cpnt_type ] in
        element_types input_types output_types
    | "fill", (arg, arg') :: (_ :: _ as args) ->
        let input_types = argument_types 1 args'
        and output_types =
          let n = List.length args
          and dims =
            List.map (function _ -> Types.ParameterDimension) args in
          let cpnt_type = component_type_of_expression ctx arg' in
          let lcl_spec = lazy
            (Types.add_dimensions
              dims 
              (evaluate cpnt_type.Types.base_class)) in
          [ 
            Printf.sprintf "@%d" (n + 1),
            { cpnt_type with Types.base_class = lcl_spec }
          ] in
        element_types input_types output_types
    | "diagonal", [ arg, arg' ] ->
        let cpnt_type = component_type_of_expression ctx arg' in
        let input_types = [ "@1", cpnt_type ]
        and output_types =
          let dims = [ Types.ParameterDimension ] in
          let lcl_spec = lazy
            (Types.add_dimensions
              dims
              (evaluate cpnt_type.Types.base_class)) in
          [ "@2", { cpnt_type with Types.base_class = lcl_spec } ] in
        element_types input_types output_types
    | ("div" | "mod" | "rem" | "max" | "min"), _
        when List.for_all (argument_base_type Types.IntegerType) args' ->
        let input_types =
          [
            "@1", Types.integer_component_type Types.Discrete;
            "@2", Types.integer_component_type Types.Discrete
          ]
        and output_types =
          ["@3", Types.integer_component_type Types.Discrete] in
        element_types input_types output_types
    | ("pre" | "change"), [arg, arg'] ->
        let cpnt_type = argument_component_type (arg, arg') in
        let input_types =
          ["@1", { cpnt_type with Types.variability = lazy Types.Continuous }]
        and output_types =
          ["@2", { cpnt_type with Types.variability = lazy Types.Discrete }] in
        element_types input_types output_types
    | _, _ -> in_types, out_types in
  match expr'.nature with
  | PredefinedIdentifier id ->
      let args' = List.map resolve_built_in_function_argument args in
      let input_types, output_types =
        let inout_types =
          List.fold_right add_function_inout_argument named_elts ([], []) in
        built_in_function_inout_types ctx id inout_types args' in
      let named_args =
        built_in_function_named_arguments [] input_types args' in
      input_types, output_types, named_args
  | _ ->
      let input_types, output_types =
        List.fold_right add_function_inout_argument named_elts ([], []) in
      let named_args = named_arguments_of_arguments input_types args in
      input_types, output_types, named_args

and resolve_identifier ctx expr id =
  let rec resolve_predefined_identifier ctx expr id = match id with
    | "Boolean" ->
        let nat = PredefinedIdentifier "Boolean"
        and elt_nat = Types.ClassElement (lazy (Types.boolean_class_type)) in
        resolved_expression (Some expr) nat elt_nat
    | "Integer" ->
        let nat = PredefinedIdentifier "Integer"
        and elt_nat = Types.ClassElement (lazy (Types.integer_class_type)) in
        resolved_expression (Some expr) nat elt_nat
    | "Real" ->
        let nat = PredefinedIdentifier "Real"
        and elt_nat = Types.ClassElement (lazy (Types.real_class_type)) in
        resolved_expression (Some expr) nat elt_nat
    | "String" ->
        let nat = PredefinedIdentifier "String"
        and elt_nat = Types.ClassElement (lazy (Types.string_class_type)) in
        resolved_expression (Some expr) nat elt_nat
    | "reinit" ->
        let nat = PredefinedIdentifier "reinit"
        and elt_nat =
          let inputs =
            ["@1", Types.real_component_type Types.Continuous;
             "@2", Types.real_component_type Types.Continuous]
          and outputs = [] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "time" ->
        let nat = PredefinedIdentifier "time"
        and elt_nat = Types.real_type Types.Continuous in
        resolved_expression (Some expr) nat elt_nat
    | "pre" | "change" ->
        let nat = PredefinedIdentifier "pre"
        and elt_nat =
          let inputs = ["@1", Types.real_component_type Types.Continuous]
          and outputs = ["@2", Types.real_component_type Types.Discrete] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "edge" ->
        let nat = PredefinedIdentifier "edge"
        and elt_nat =
          let inputs = ["@1", Types.boolean_component_type Types.Discrete]
          and outputs = ["@2", Types.boolean_component_type Types.Discrete] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "initial" ->
        let nat = PredefinedIdentifier "initial"
        and elt_nat =
          let inputs = []
          and outputs = [] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "terminal" ->
        let nat = PredefinedIdentifier "terminal"
        and elt_nat =
          let inputs = []
          and outputs = [] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "sample" ->
        let nat = PredefinedIdentifier "sample"
        and elt_nat =
          let inputs = [("@1", Types.real_component_type Types.Parameter);
                        ("@2", Types.real_component_type Types.Parameter)]
          and outputs = ["@3", Types.boolean_component_type Types.Parameter] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "delay" ->
        let nat = PredefinedIdentifier "delay"
        and elt_nat =
          let inputs = [("@1", Types.real_component_type Types.Continuous);
                        ("@2", Types.real_component_type Types.Parameter)]
          and outputs = ["@3", Types.real_component_type Types.Continuous] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "assert" ->
        let nat = PredefinedIdentifier "assert"
        and elt_nat =
          let inputs = [("@1", Types.boolean_component_type Types.Discrete);
                        ("@2", Types.string_component_type Types.Discrete)]
          and outputs = [] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "terminate" ->
        let nat = PredefinedIdentifier "terminate"
        and elt_nat =
          let inputs = [("@1", Types.string_component_type Types.Discrete)]
          and outputs = [] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "abs" | "cos" | "sin" | "tan" | "exp" | "log" | "sqrt" |
      "asin" | "acos" | "atan" | "sinh" | "cosh" | "tanh" | "asinh" |
      "acosh" | "atanh" | "log10" | "ceil" | "floor" | "der" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs = ["@1", Types.real_component_type Types.Continuous]
          and outputs = ["@2", Types.real_component_type Types.Continuous] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "sign" | "integer" | "ones" | "zeros" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs = ["@1", Types.real_component_type Types.Continuous]
          and outputs = ["@2", Types.integer_component_type Types.Discrete] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "max" | "min" | "div" | "mod" | "rem" | "fill" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs =
            [
              "@1", Types.real_component_type Types.Continuous;
              "@2", Types.real_component_type Types.Continuous
            ]
          and outputs = ["@3", Types.real_component_type Types.Continuous] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "smooth" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs =
            [
              "@1", Types.integer_component_type Types.Discrete;
              "@2", Types.real_component_type Types.Continuous
            ]
          and outputs = ["@3", Types.real_component_type Types.Continuous] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "identity" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs = [ "@1", Types.integer_component_type Types.Parameter ]
          and outputs =
            let dims =
              [Types.ParameterDimension; Types.ParameterDimension] in
            [ 
              "@2",
              Types.integer_array_component_type Types.Parameter dims
            ] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "diagonal" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs =
            let dim = [ Types.ParameterDimension ] in
            [ "@1", Types.integer_array_component_type Types.Parameter dim ]
          and outputs =
            let dims =
              [Types.ParameterDimension; Types.ParameterDimension] in
            [ 
              "@2",
              Types.integer_array_component_type Types.Parameter dims
            ] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | "sum" | "product" | "scalar" ->
        let nat = PredefinedIdentifier id
        and elt_nat =
          let inputs =
            let dim = [ Types.DiscreteDimension ] in
              [ "@1", Types.integer_array_component_type Types.Discrete dim ]
          and outputs = ["@2", Types.integer_component_type Types.Discrete] in
          Types.function_type inputs outputs in
        resolved_expression (Some expr) nat elt_nat
    | _ -> raise (CompilError
        {err_msg = ["_UnknownIdentifier"; id];
         err_info = [];
         err_ctx = ctx})
  and search_in_toplevel dic =
    try
      let elt_desc = List.assoc id (evaluate dic) in
      let elt_type = evaluate elt_desc.element_type in
      match elt_type.Types.dynamic_scope with
        | None | Some Types.Inner ->
            let nat = ToplevelIdentifier id in
            resolved_expression (Some expr) nat elt_type.Types.element_nature
        | Some Types.Outer | Some Types.InnerOuter ->
            raise (CompilError
              {err_msg = ["_NoInnerDeclForOuterElem"; id];
               err_info = [];
               err_ctx = ctx}) (*error*)
    with Not_found -> resolve_predefined_identifier ctx expr id
  and search_in_class level cl_def = match evaluate cl_def.class_type with
    | Types.ClassType cl_type -> search_in_class_type level cl_def cl_type
    | Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
      Types.TupleType _ ->
        raise (CompilError
          {err_msg = ["_NoInnerDeclForOuterElem"; id];
           err_info = [];
           err_ctx = ctx}) (*error*)
  and search_in_class_type level cl_def cl_type =
    try
      let elt_type = evaluate (List.assoc id cl_type.Types.named_elements) in
      match elt_type.Types.dynamic_scope with
          | None | Some Types.Inner ->
              let nat = LocalIdentifier (level, id) in
              resolved_expression (Some expr) nat elt_type.Types.element_nature
          | Some Types.Outer | Some Types.InnerOuter ->
              let nat = DynamicIdentifier (level, id) in
              resolved_expression (Some expr) nat elt_type.Types.element_nature
    with Not_found -> search_in_parent level cl_def
  and search_in_parent level cl_def = match cl_def.enclosing_class with
    | _ when cl_def.encapsulated -> search_in_toplevel ctx.toplevel
    | Some cl_def -> search_in_class (level + 1) cl_def
    | None -> search_in_toplevel ctx.toplevel
  and search_in_for_loop_variables level ctx = match ctx.context_nature with
    | ToplevelContext -> search_in_toplevel ctx.toplevel
    | ClassContext cl_def -> search_in_class 0 cl_def
    | SubscriptContext (ctx', _, _, _) ->
        search_in_for_loop_variables level ctx'
    | ForContext (_, id', elt_nat) when id' = id ->
        let nat = LoopVariable level in
        resolved_expression (Some expr) nat elt_nat
    | ForContext (ctx', _, _) ->
        search_in_for_loop_variables (level + 1) ctx' in
  search_in_for_loop_variables 0 ctx

(*and resolve_if ctx expr alts expr' =
  let expres' = resolve_expression ctx expr' in
  let elt_nat' = expres'.info.type_description in
  let rec resolve_alternative (cond, expr) =
    resolve_condition cond,
    resolve_alternative_expression expr
  and resolve_condition cond =
    let ctx = {ctx with location = cond.Syntax.info} in
    let cond' = resolve_expression ctx cond in
    let condition cpnt_type =
      let cl_spec = evaluate cpnt_type.Types.base_class in
      match cl_spec with
        | Types.PredefinedType { Types.base_type = Types.BooleanType } -> cond'
        | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
          Types.ArrayType _ | Types.TupleType _ -> 
            raise (CompilError
              {err_msg = ["_NonBooleanIfCondExpr"];
               err_info =
                 [("_ExprKind", "...if A then...");
                  ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
               err_ctx = ctx}) (*error*) in
    match cond'.info.type_description with
      | Types.ComponentElement cpnt_type -> condition cpnt_type
      | _ -> raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = {ctx with location = cond.Syntax.info}}) (*error*)
  and resolve_alternative_expression expr =
    let ctx = {ctx with location = expr.Syntax.info} in
    let expres = resolve_expression ctx expr in
    let elt_nat = expres.info.type_description in
    let display_error elt_nat elt_nat' = match elt_nat, elt_nat' with
      | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
          raise (CompilError
            {err_msg = ["_TypeConflictsInIfAlternExprs"];
             err_info =
               [("_TypeOfThenBranche",
                 Types.string_of_component_type cpnt_type);
                ("_TypeOfElseBranche",
                 Types.string_of_component_type cpnt_type')];
             err_ctx = ctx}) (*error*)
      | Types.ComponentElement cpnt_type, _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info =
               [("_TypeOfThenBranche",
                 Types.string_of_component_type cpnt_type);
                ("_TypeOfElseBranche", "_ClassElement")];
             err_ctx = ctx}) (*error*)
      | _, Types.ComponentElement cpnt_type' ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info =
               [("_TypeOfThenBranche", "_ClassElement");
                ("_TypeOfElseBranche",
                 Types.string_of_component_type cpnt_type')];
             err_ctx = ctx}) (*error*)
      | _, _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info =
               [("_TypeOfThenBranche", "_ClassElement");
                ("_TypeOfElseBranche", "_ClassElement")];
             err_ctx = ctx}) (*error*) in
    match Types.compare_element_natures elt_nat elt_nat' with
    | Types.NotRelated -> display_error elt_nat elt_nat'
    | _ -> expres in
  let alts = List.map resolve_alternative alts in
  let nat = If (alts, expres') in
  resolved_expression (Some expr) nat elt_nat'*)

and resolve_if ctx expr alts expr' =
  let resolve_data_expression ctx expr =
    let expr' = resolve_expression ctx expr in
    match expr'.info.type_description with
    | Types.ComponentElement cpnt_type -> expr'
    | _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = {ctx with location = expr.Syntax.info}}) (*error*) in
  let resolve_condition cond =
    let ctx = {ctx with location = cond.Syntax.info} in
    let cond' = resolve_data_expression ctx cond in
    let condition cpnt_type =
      match evaluate cpnt_type.Types.base_class with
      | Types.PredefinedType { Types.base_type = Types.BooleanType } -> cond'
      | cl_spec -> 
          raise (CompilError
            {err_msg = ["_NonBooleanIfCondExpr"];
             err_info =
               [("_ExprKind", "...if A then...");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    match cond'.info.type_description with
    | Types.ComponentElement cpnt_type -> condition cpnt_type
    | _ ->
        raise (CompilError
          {err_msg = ["_DataElemExpected"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
  let resolve_alternatives (alts, expr') (cond, expr) =
    let ctx = {ctx with location = expr.Syntax.info} in
    let cond' = resolve_condition cond
    and expr = resolve_data_expression ctx expr in
    let exprs = apply_binary_coercions [ expr'; expr] in
    let expr' = List.nth exprs 0
    and expr = List.nth exprs 1 in
    let elt_nat = expr.info.type_description
    and elt_nat' = expr'.info.type_description in
    match Types.compare_element_natures elt_nat elt_nat' with
    | Types.SameType ->
        (alts @ [cond', expr]), expr'
    | _ ->
        raise (CompilError
          {err_msg = ["_TypeConflictsInIfAlternExprs"];
           err_info =
             [("_TypeOfThenBranche",
               Types.string_of_element_nature elt_nat);
              ("_TypeOfElseBranche",
               Types.string_of_element_nature elt_nat')];
           err_ctx = ctx}) (*error*) in
  let expr' = resolve_data_expression ctx expr' in
  let alts, expr' = List.fold_left resolve_alternatives ([], expr') alts in
  let nat = If (alts, expr') in
  resolved_expression (Some expr) nat expr'.info.type_description

and resolve_indexed_access ctx expr expr' subs =
  let expres' = resolve_expression ctx expr' in
  let rec resolve_component_indexed_access cl_spec subs =
    match cl_spec, subs with
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _), [] -> cl_spec
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.TupleType _), _ :: _ ->
          raise (CompilError
            {err_msg = ["_CannotSubscriptANonArrayTypeElem"];
             err_info =
               [("_ExpectedType", "_ArrayType");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*)
      | Types.ArrayType (_, cl_spec'), sub :: subs' ->
          let cl_spec' = resolve_component_indexed_access cl_spec' subs' in
          subarray_access sub cl_spec'
  and subarray_access sub cl_spec =
    let subarray_access' = function
      | Types.PredefinedType { Types.base_type = Types.IntegerType } -> cl_spec
      | Types.ArrayType
          (dim, Types.PredefinedType { Types.base_type = Types.IntegerType }) ->
          Types.ArrayType (dim, cl_spec)
      | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
        Types.ArrayType _ | Types.TupleType _ -> assert false (*error*) in
    match sub.info.type_description with
      | Types.ComponentElement cpnt_type ->
          let cl_spec' = evaluate cpnt_type.Types.base_class in
          subarray_access' cl_spec'
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ -> assert false (*error*) in
  match expres'.info.type_description with
    | Types.ComponentElement cpnt_type ->
        let cl_spec = evaluate cpnt_type.Types.base_class in
        let subs' = resolve_subscripts ctx expres' cl_spec subs in
        let cpnt_type' = 
          { cpnt_type with
            Types.base_class =
              lazy (resolve_component_indexed_access cl_spec subs')
          } in
        let info =
          {
            syntax = Some expr;
            type_description = Types.ComponentElement cpnt_type'
          } in
        { nature = IndexedAccess (expres', subs'); info = info }
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_integer ctx expr s =
  let nat =
    try
      Integer (Int32.of_string s)
    with
    | _ ->
        raise (CompilError
          {err_msg = ["_InvalidInteger"; s];
           err_info = [];
           err_ctx = ctx}) in
  resolved_expression (Some expr) nat (Types.integer_type Types.Constant)

and resolve_matrix_construction ctx expr exprss =
  raise (CompilError
    {err_msg = ["_NotYetImplemented"; "_MatrixExpr"];
     err_info = [];
     err_ctx = ctx})

and resolve_no_event ctx expr expr' =
  let expr' = resolve_expression ctx expr' in
  match expr'.info.type_description with
  | Types.ComponentElement cpnt_type ->
      let nat = NoEvent expr'
      and flow = lazy (evaluate cpnt_type.Types.flow)
      and var = lazy Types.Continuous
      and inout = cpnt_type.Types.causality
      and cl_spec = cpnt_type.Types.base_class in
      let cpnt_type =
        component_element flow var inout cl_spec in
      let elt_nat = Types.ComponentElement cpnt_type in
      resolved_expression (Some expr) nat elt_nat
  | _ ->
      raise (CompilError
        {err_msg = ["_ClassElemFoundInExpr"];
         err_info = [];
         err_ctx = ctx}) (*error*)

and resolve_range ctx expr start step stop =
  let integer_range var start' step' stop' =
    let integer_range' =
      match start'.nature, step'.nature, stop'.nature with
      | _, _, _ when Types.higher_variability var Types.Discrete ->
          let var = Types.string_of_variability var in
          raise (CompilError
            {err_msg = ["_InvalidVarOfRangeExpr"];
             err_info = [("_Expr", Syntax.string_of_range start step stop);
                         ("_ExpectedVariability", "parameter");
                         ("_VariabilityFound", var)];
             err_ctx = ctx}) 
      | Integer i, Integer p, Integer j when p = Int32.zero ->
          raise (CompilError
            {err_msg = ["_RangeStepValueCannotBeNull"];
             err_info = [("_Expr", Syntax.string_of_range start step stop)];
             err_ctx = ctx})
      | Integer i, Integer p, Integer j ->
          let dim = Int32.div (Int32.succ (Int32.sub j i)) p in
          Types.integer_array_type var (Types.ConstantDimension dim)
      | (Integer _ | DynamicIdentifier _ | LocalIdentifier _ |
         LoopVariable _ | PredefinedIdentifier _ | ToplevelIdentifier _),
        (Integer _ | DynamicIdentifier _ | LocalIdentifier _ |
         LoopVariable _ | PredefinedIdentifier _ | ToplevelIdentifier _),
        (Integer _ | DynamicIdentifier _ | LocalIdentifier _ |
         LoopVariable _ | PredefinedIdentifier _ | ToplevelIdentifier _) ->
          Types.integer_array_type var Types.ParameterDimension
      | _, _, _ ->
          raise (CompilError
            {err_msg = ["_NotYetImplemented"; "_NonIntegerRangeExpr"];
             err_info = [("_Expr", Syntax.string_of_range start step stop)];
             err_ctx = ctx}) in
    let nat = Range (start', step', stop') in
    let elt_nat = integer_range' in
    resolved_expression (Some expr) nat elt_nat in
  let start' = resolve_expression ctx start
  and step' = match step with
    | None -> one
    | Some expr -> resolve_expression ctx expr
  and stop' = resolve_expression ctx stop in
  let resolve_range' var start_cl_spec step_cl_spec stop_cl_spec =
    match start_cl_spec, step_cl_spec, stop_cl_spec with
      | Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          integer_range var start' step' stop'
      (*| Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType },
        _ -> assert false*)
      | _ -> raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_NonIntegerRangeExpr"];
           err_info = [("_Expr", Syntax.string_of_range start step stop)];
           err_ctx = ctx}) in
  let start_elt_nat = start'.info.type_description
  and step_elt_nat = step'.info.type_description
  and stop_elt_nat = stop'.info.type_description in
  match start_elt_nat, step_elt_nat, stop_elt_nat with
    | Types.ComponentElement start_cpnt_type,
      Types.ComponentElement step_cpnt_type,
      Types.ComponentElement stop_cpnt_type ->
        let start_cl_spec = evaluate start_cpnt_type.Types.base_class
        and step_cl_spec = evaluate step_cpnt_type.Types.base_class
        and stop_cl_spec = evaluate stop_cpnt_type.Types.base_class
        and start_var = evaluate start_cpnt_type.Types.variability
        and step_var = evaluate step_cpnt_type.Types.variability
        and stop_var = evaluate stop_cpnt_type.Types.variability in
        let var =
          let var' = Types.max_variability step_var stop_var in
          Types.max_variability start_var var' in
        resolve_range' var start_cl_spec step_cl_spec stop_cl_spec
    | _ -> raise (CompilError
        {err_msg = ["_InvalidTypeInRangeExpr"];
         err_info = [("_Expr", Syntax.string_of_range start step stop)];
         err_ctx = ctx}) (*error*)

and resolve_real ctx expr s =
  let nat = Real (float_of_string s) in
  resolved_expression (Some expr) nat (Types.real_type Types.Constant)

and resolve_string ctx expr s =
  resolved_expression (Some expr) (String s) (Types.string_type Types.Constant)

and resolve_true ctx expr =
  resolved_expression (Some expr) True (Types.boolean_type Types.Constant)

and resolve_tuple ctx expr exprs =
  let max_element_variability var expr expr' =
    match expr'.info.type_description with
      | Types.ComponentElement cpnt_type ->
          let var' = evaluate cpnt_type.Types.variability in
          Types.max_variability var var'
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*)
  and class_specifier expr expr' =
    match expr'.info.type_description with
      | Types.ComponentElement cpnt_type -> evaluate cpnt_type.Types.base_class
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*) in
  let exprs' = List.map (resolve_expression ctx) exprs in
  let flow = lazy false
  and var =
    lazy (List.fold_left2 max_element_variability Types.Constant exprs exprs')
  and inout = lazy Types.Acausal
  and cl_spec = lazy (Types.TupleType (List.map2 class_specifier exprs exprs')) in
  {
    nature = Tuple exprs';
    info =
      {
        syntax = Some expr;
        type_description =
          Types.ComponentElement (component_element flow var inout cl_spec)
      }
  }

and resolve_unuary_operation ctx expr kind arg =
  let arg' = resolve_expression ctx arg in
  match kind.Syntax.nature with
    | Syntax.UnaryMinus -> resolve_unary_minus ctx expr arg'
    | Syntax.Not -> resolve_not ctx expr arg'
    | Syntax.UnaryPlus ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_UnaryOperPLUS"];
           err_info = [];
           err_ctx = ctx})

and resolve_vector ctx expr vec_elts = match vec_elts.Syntax.nature with
  | Syntax.VectorReduction (expr', for_inds) ->
      resolve_vector_reduction ctx expr expr' for_inds
  | Syntax.VectorElements exprs -> resolve_vector_elements ctx expr exprs

and resolve_vector_reduction ctx expr expr' for_inds =
  let vector_reduction_type acc expr expr' =
    let add_dimension elt_nat cl_spec =
      let add_dimension' cl_spec' = match cl_spec' with
        | Types.ArrayType (dim, _) -> Types.ArrayType (dim, cl_spec)
        | Types.PredefinedType _ | Types.ClassType _ |
          Types.ComponentType _ | Types.TupleType _ -> 
            raise (CompilError
              {err_msg = ["_InvalidTypeInRangeExpr"];
               err_info =
                 [("_ExpectedType", "_ArrayType");
                  ("_TypeFound",
                   Types.string_of_class_specifier cl_spec')];
               err_ctx = ctx}) (*error*) in
      match elt_nat with
        | Types.ComponentElement cpnt_type ->
            let cl_spec' = evaluate cpnt_type.Types.base_class in
            add_dimension' cl_spec'
        | Types.ClassElement _ | Types.ComponentTypeElement _ |
          Types.PredefinedTypeElement _ ->
            raise (CompilError
              {err_msg = ["_ClassElemFoundInExpr"];
               err_info = [];
               err_ctx = ctx}) (*error*) in
    let rec vector_reduction_type' acc cl_spec = match acc with
      | [] -> cl_spec
      | range :: acc ->
          let elt_nat = range.info.type_description in
          let cl_spec' = add_dimension elt_nat cl_spec in
          vector_reduction_type' acc cl_spec' in
    match expr'.info.type_description with
      | Types.ComponentElement cpnt_type ->
          let cl_spec = evaluate cpnt_type.Types.base_class in
          let cpnt_type' =
            { cpnt_type with
              Types.base_class = lazy (vector_reduction_type' acc cl_spec)
            } in
          Types.ComponentElement cpnt_type'
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*)
  and range_element_type range range' =
    let sub_dimension cl_spec = match cl_spec with
      | Types.ArrayType (dim, cl_spec) -> cl_spec
      | Types.PredefinedType _ | Types.ClassType _ |
        Types.ComponentType _ | Types.TupleType _ -> 
          raise (CompilError
            {err_msg = ["_InvalidTypeInRangeExpr"];
             err_info =
               [("_ExpectedType", "_ArrayType");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    match range'.info.type_description with
      | Types.ComponentElement cpnt_type ->
          let cl_spec = evaluate cpnt_type.Types.base_class in
          let cpnt_type' =
            { cpnt_type with
              Types.base_class = lazy (sub_dimension cl_spec)
            } in
          Types.ComponentElement cpnt_type'
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ ->
          raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*) in
  let rec resolve_vector_reduction' acc ctx = function
    | [] ->
        let expres' = resolve_expression ctx expr' in
        let nat = VectorReduction (List.rev acc, expres')
        and elt_nat = vector_reduction_type acc expr' expres' in
        resolved_expression (Some expr) nat elt_nat
    | (_, None) :: _ ->
        raise (CompilError
          {err_msg = ["_NotYetImplemented"; "_ImplicitIterRange"];
           err_info = [("_Expr", Syntax.string_of_for_inds for_inds)];
           err_ctx = ctx})
    | (id, Some range) :: for_inds ->
        let range' = resolve_expression ctx range in
        let elt_nat = range_element_type range range' in
        let ctx' =
          { ctx with
            context_nature = ForContext (ctx, id, elt_nat)
          } in
        resolve_vector_reduction' (range' :: acc) ctx' for_inds in
  resolve_vector_reduction' [] ctx for_inds

and resolve_vector_elements ctx expr exprs =
  let max_variability var cpnt_type =
    let var' = evaluate cpnt_type.Types.variability in
    Types.max_variability var var' in
  let type_of_elements cpnt_types =
    let rec type_of_elements' cl_spec = function
      | [] -> cl_spec
      | cpnt_type :: cpnt_types ->
          let cl_spec' = evaluate cpnt_type.Types.base_class in
          type_of_elements' (update cl_spec cl_spec') cpnt_types
    and update cl_spec cl_spec' =
      match Types.compare_specifiers cl_spec cl_spec' with
      | Types.SameType | Types.Supertype -> cl_spec
      | Types.Subtype -> cl_spec'
      | _ ->
          raise (CompilError
            {err_msg = ["_TypeConflictsInVectorExpr"];
             err_info =
               [("_MismatchingTypes",
                 Types.string_of_class_specifier cl_spec ^ ", " ^
                 Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) in
    match cpnt_types with
      | [] -> assert false (*error*)
      | cpnt_type :: cpnt_types ->
          let cl_spec' = evaluate cpnt_type.Types.base_class in
          type_of_elements' cl_spec' cpnt_types in
  let exprs' = List.map (resolve_expression ctx) exprs in
  let exprs' = apply_binary_coercions exprs' in
  let cpnt_types = List.map (component_type_of_expression ctx) exprs' in
  let var = lazy (List.fold_left max_variability Types.Constant cpnt_types) in
  let cl_spec = type_of_elements cpnt_types in
  let dim = Types.ConstantDimension (Int32.of_int (List.length exprs')) in
  let cl_spec' = lazy (Types.ArrayType (dim, cl_spec)) in
  let cpnt_type =
    {
      Types.flow = lazy false;
            variability = var;
            causality = lazy Types.Acausal;
            base_class = cl_spec'
    } in
  let nat = Vector exprs'
  and elt_nat = Types.ComponentElement cpnt_type in
  resolved_expression (Some expr) nat elt_nat

and resolve_and ctx expr arg arg' =
  let resolve_and' cpnt_type cpnt_type' =
    let rec and_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.PredefinedType { Types.base_type = Types.BooleanType },
        Types.PredefinedType { Types.base_type = Types.BooleanType } ->
          Types.PredefinedType
            { Types.base_type = Types.BooleanType; attributes = [] }
      | Types.PredefinedType { Types.base_type = Types.BooleanType },
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["and"; "_OperAppliedToNonBoolExpr"];
             err_info =
               [("_ExpectedType", "Boolean");
                ("_TypeFound", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*)
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["and"; "_OperAppliedToNonBoolExpr"];
             err_info =
               [("_ExpectedType", "Boolean");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (
        let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (
        let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        and_type cl_spec cl_spec') in
    let nat = BinaryOperation (And, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_and' cpnt_type cpnt_type'
    | Types.ComponentElement _,
      (Types.ClassElement _ | Types.ComponentTypeElement _ |
       Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)
    | (Types.ClassElement _ | Types.ComponentTypeElement _ |
       Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_or ctx expr arg arg' =
  let resolve_or' cpnt_type cpnt_type' =
    let rec or_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.PredefinedType { Types.base_type = Types.BooleanType },
        Types.PredefinedType { Types.base_type = Types.BooleanType } ->
          Types.PredefinedType
            { Types.base_type = Types.BooleanType; attributes = [] }
      | Types.PredefinedType { Types.base_type = Types.BooleanType },
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["or"; "_OperAppliedToNonBoolExpr"];
             err_info =
               [("_ExpectedType", "Boolean");
                ("_TypeFound", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*)
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["or"; "_OperAppliedToNonBoolExpr"];
             err_info =
               [("_ExpectedType", "Boolean");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        or_type cl_spec cl_spec') in
    let nat = BinaryOperation (Or, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_or' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_addition ctx expr arg arg' =
  let resolve_addition' cpnt_type cpnt_type' =
    let rec addition_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.ArrayType (Types.ConstantDimension n, _),
        Types.ArrayType (Types.ConstantDimension n', _) when n <> n' ->
          raise (CompilError
            {err_msg = ["_ArrayDimMustAgreeToPerform"; "_Addition"];
             err_info =
               [("_ExprKind", "A + B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*)
      | Types.ArrayType (Types.ConstantDimension _, cl_spec),
        Types.ArrayType (dim, cl_spec') |
        Types.ArrayType (dim, cl_spec),
        Types.ArrayType (Types.ConstantDimension _, cl_spec') ->
          Types.ArrayType (dim, addition_type cl_spec cl_spec')
      | Types.ArrayType (Types.ParameterDimension, cl_spec),
        Types.ArrayType (dim, cl_spec') |
        Types.ArrayType (dim, cl_spec),
        Types.ArrayType (Types.ParameterDimension, cl_spec') ->
          Types.ArrayType (dim, addition_type cl_spec cl_spec')
      | Types.ArrayType (Types.DiscreteDimension, cl_spec),
        Types.ArrayType (Types.DiscreteDimension, cl_spec') ->
          Types.ArrayType
            (Types.DiscreteDimension, addition_type cl_spec cl_spec')
      | Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.IntegerType; attributes = [] }
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType },
        Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | Types.PredefinedType _, Types.ArrayType _ 
      | Types.ArrayType _, Types.PredefinedType _ -> 
          raise (CompilError
            {err_msg = ["+"; "_OperBetweenScalarAndArray"];
             err_info =
               [("_ExprKind", "A + B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*)
      | _, _ -> 
          raise (CompilError
            {err_msg = ["+"; "_OperAppliedToNonNumericExpr"];
             err_info =
               [("_ExprKind", "A + B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        addition_type cl_spec cl_spec') in
    let nat = BinaryOperation (Plus, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_addition' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
            {err_msg = ["_ClassElemFoundInExpr"];
             err_info = [];
             err_ctx = ctx}) (*error*)

and resolve_comparison ctx expr kind arg arg' =
  let resolve_comparison' cpnt_type cpnt_type' =
    let rec comparison_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.PredefinedType
          { Types.base_type = Types.IntegerType | Types.RealType },
        Types.PredefinedType
          { Types.base_type = Types.IntegerType | Types.RealType } ->
          Types.PredefinedType
            { Types.base_type = Types.BooleanType; attributes = [] }
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["_TypeInconsistWithComparOper"];
             err_info =
               [("_ExprKind", "A" ^ (string_of_bin_oper_kind kind) ^ "B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*) in
    (*let var =
      let var = evaluate cpnt_type.Types.variability
      and var' = evaluate cpnt_type'.Types.variability in
	Types.max_variability var var'*)
    let var = Types.Discrete
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        comparison_type cl_spec cl_spec') in
    let nat = BinaryOperation (kind, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) (lazy var) (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_comparison' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_division ctx expr arg arg' =
  let resolve_division' cpnt_type cpnt_type' =
    let rec division_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.ArrayType (dim, cl_spec),
        Types.PredefinedType
          { Types.base_type = Types.IntegerType | Types.RealType } ->
          Types.ArrayType (dim, division_type cl_spec cl_spec')
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType },
        Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["_TypeInconsistentWithDivOper"];
             err_info =
               [("_ExprKind", "A / B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        division_type cl_spec cl_spec') in
    let nat = BinaryOperation (Divide, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_division' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and resolve_multiplication ctx expr arg arg' =
  let resolve_multiplication' cpnt_type cpnt_type' =
    let rec multiplication_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.ArrayType (_, Types.ArrayType (Types.ConstantDimension n, _)),
        Types.ArrayType (Types.ConstantDimension n', Types.ArrayType _)
        when n <> n' ->
          raise (CompilError
            {err_msg = ["_ArrayDimsNonCompatibleWithMult"];
             err_info =
               [("_ExprKind", "A * B");
                ("_TypeOfA", Types.string_of_component_type cpnt_type);
                ("_TypeOfB", Types.string_of_component_type cpnt_type')];
             err_ctx = ctx})  (*error*)
      | Types.ArrayType
          (dim, Types.ArrayType
            (_, Types.PredefinedType
              { Types.base_type = Types.IntegerType })),
        Types.ArrayType
          (_, Types.ArrayType
            (dim', Types.PredefinedType
              { Types.base_type = Types.IntegerType })) ->
          Types.ArrayType
            (dim, Types.ArrayType
              (dim', Types.PredefinedType
                { Types.base_type = Types.IntegerType; attributes = [] }))
      | Types.ArrayType
          (dim, Types.ArrayType
            (_, Types.PredefinedType
              { Types.base_type = Types.IntegerType | Types.RealType })),
        Types.ArrayType
          (_, Types.ArrayType
            (dim', Types.PredefinedType
              { Types.base_type = Types.IntegerType | Types.RealType })) ->
          Types.ArrayType
            (dim, Types.ArrayType
              (dim', Types.PredefinedType
                { Types.base_type = Types.RealType; attributes = [] }))
      | Types.ArrayType (_, Types.ArrayType (Types.ConstantDimension n, _)),
        Types.ArrayType (Types.ConstantDimension n', _)
        when n <> n' ->
          raise (CompilError
            {err_msg = ["_ArrayDimsNonCompatibleWithMult"];
             err_info =
               [("_ExprKind", "A * B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx})  (*error*)
      | Types.ArrayType
          (dim, Types.ArrayType
            (_, Types.PredefinedType
              { Types.base_type = Types.IntegerType })),
        Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType }) ->
          Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.IntegerType; attributes = [] })
      | Types.ArrayType
          (dim, Types.ArrayType
            (_, Types.PredefinedType
              { Types.base_type = Types.IntegerType | Types.RealType })),
        Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType | Types.RealType }) ->
          Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.RealType; attributes = [] })
      | Types.ArrayType (Types.ConstantDimension n, _),
        Types.ArrayType (Types.ConstantDimension n', Types.ArrayType _)
        when n <> n' ->
          raise (CompilError
            {err_msg = ["_ArrayDimsNonCompatibleWithMult"];
             err_info =
               [("_ExprKind", "A * B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx})  (*error*)
      | Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType }),
        Types.ArrayType
          (_, Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.IntegerType })) ->
          Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.IntegerType; attributes = [] })
      | Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType | Types.RealType }),
        Types.ArrayType
          (_, Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.IntegerType | Types.RealType })) ->
          Types.ArrayType
            (dim, Types.PredefinedType
              { Types.base_type = Types.RealType; attributes = [] })
      | Types.ArrayType (Types.ConstantDimension n, _),
        Types.ArrayType (Types.ConstantDimension n', _)
        when n <> n' ->
          raise (CompilError
            {err_msg = ["_ArrayDimsNonCompatibleWithMult"];
             err_info =
               [("_ExprKind", "A * B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx})  (*error*)
      | Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType }),
        Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType }) ->
          Types.PredefinedType
            { Types.base_type = Types.IntegerType; attributes = [] }
      | Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType | Types.RealType }),
        Types.ArrayType
          (_, Types.PredefinedType
            { Types.base_type = Types.IntegerType | Types.RealType }) ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | Types.PredefinedType
          { Types.base_type = Types.IntegerType | Types.RealType },
        Types.ArrayType (dim, cl_spec') ->
          Types.ArrayType (dim, multiplication_type cl_spec cl_spec')
      | Types.ArrayType (dim, cl_spec),
        Types.PredefinedType
          { Types.base_type = Types.IntegerType | Types.RealType } ->
          Types.ArrayType (dim, multiplication_type cl_spec cl_spec')
      | Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.IntegerType; attributes = [] }
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType },
        Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["*"; "_OperAppliedToNonNumericExpr"];
             err_info =
               [("_ExprKind", "A * B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx})  (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        multiplication_type cl_spec cl_spec') in
    let nat = BinaryOperation (Times, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match  arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_multiplication' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx})  (*error*)

and resolve_power ctx expr arg arg' =
  let resolve_power' cpnt_type cpnt_type' =
    let rec power_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.ArrayType
          (Types.ConstantDimension n, Types.ArrayType
            (Types.ConstantDimension n', _)),
        Types.PredefinedType { Types.base_type = Types.IntegerType }
        when n <> n' ->
          raise (CompilError
            {err_msg = ["_PowerOperOnNonSquareArray"];
             err_info =
               [("_ExprKind", "A ^ B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*)
      | Types.ArrayType
          (dim, Types.ArrayType
            (dim', Types.PredefinedType
              { Types.base_type = Types.IntegerType })),
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.ArrayType
            (dim, Types.ArrayType
              (dim', Types.PredefinedType
                { Types.base_type = Types.RealType; attributes = [] }))
      | Types.ArrayType
          (dim, Types.ArrayType
            (dim', Types.PredefinedType { Types.base_type = Types.RealType })),
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.ArrayType
            (dim, Types.ArrayType
              (dim', Types.PredefinedType
                { Types.base_type = Types.RealType; attributes = [] }))
      | Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType },
        Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["^"; "_OperAppliedToNonNumericExpr"];
             err_info =
               [("_ExprKind", "A ^ B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        power_type cl_spec cl_spec') in
    let nat = BinaryOperation (Power, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_power' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx})  (*error*)

and resolve_subtraction ctx expr arg arg' =
  let resolve_subtraction' cpnt_type cpnt_type' =
    let rec subtraction_type cl_spec cl_spec' = match cl_spec, cl_spec' with
      | Types.ArrayType (Types.ConstantDimension n, _),
        Types.ArrayType (Types.ConstantDimension n', _) when n <> n' ->
          raise (CompilError
          {err_msg = ["_ArrayDimMustAgreeToPerform"; "_Subtraction"];
           err_info =
             [("_ExprKind", "A - B");
              ("_TypeOfA", Types.string_of_class_specifier cl_spec);
              ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
           err_ctx = ctx})  (*error*)
      | Types.ArrayType (Types.ConstantDimension _, cl_spec),
        Types.ArrayType (dim, cl_spec') |
        Types.ArrayType (dim, cl_spec),
        Types.ArrayType (Types.ConstantDimension _, cl_spec') ->
          Types.ArrayType (dim, subtraction_type cl_spec cl_spec')
      | Types.ArrayType (Types.ParameterDimension, cl_spec),
        Types.ArrayType (dim, cl_spec') |
        Types.ArrayType (dim, cl_spec),
        Types.ArrayType (Types.ParameterDimension, cl_spec') ->
          Types.ArrayType (dim, subtraction_type cl_spec cl_spec')
      | Types.ArrayType (Types.DiscreteDimension, cl_spec),
        Types.ArrayType (Types.DiscreteDimension, cl_spec') ->
          Types.ArrayType
            (Types.DiscreteDimension, subtraction_type cl_spec cl_spec')
      | Types.PredefinedType { Types.base_type = Types.IntegerType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.IntegerType; attributes = [] }
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType },
        Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } ->
          Types.PredefinedType
            { Types.base_type = Types.RealType; attributes = [] }
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _),
        (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.ArrayType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["-"; "_OperAppliedToNonNumericExpr"];
             err_info =
               [("_ExprKind", "A - B");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec);
                ("_TypeOfB", Types.string_of_class_specifier cl_spec')];
             err_ctx = ctx}) (*error*) in
    let var =
      lazy (let var = evaluate cpnt_type.Types.variability
        and var' = evaluate cpnt_type'.Types.variability in
        Types.max_variability var var')
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class
        and cl_spec' = evaluate cpnt_type'.Types.base_class in
        subtraction_type cl_spec cl_spec') in
    let nat = BinaryOperation (Minus, arg, arg') in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description, arg'.info.type_description with
    | Types.ComponentElement cpnt_type, Types.ComponentElement cpnt_type' ->
        resolve_subtraction' cpnt_type cpnt_type'
    | (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _),
      (Types.ComponentElement _ | Types.ClassElement _ |
       Types.ComponentTypeElement _ | Types.PredefinedTypeElement _) ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx})  (*error*)

and resolve_unary_minus ctx expr arg =
  let resolve_unary_minus' cpnt_type =
    let rec unary_minus_type cl_spec = match cl_spec with
      | Types.ArrayType (dim, cl_spec) ->
          Types.ArrayType (dim, unary_minus_type cl_spec)
      | Types.PredefinedType
          { Types.base_type = Types.RealType | Types.IntegerType } -> cl_spec
      | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
         Types.TupleType _) -> 
          raise (CompilError
            {err_msg = ["-"; "_OperAppliedToNonNumericExpr"];
             err_info =
               [("_ExprKind", "- A");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx})  (*error*) in
    let var = cpnt_type.Types.variability
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class in
        unary_minus_type cl_spec) in
    let nat = UnaryOperation (UnaryMinus, arg) in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description with
    | Types.ComponentElement cpnt_type -> resolve_unary_minus' cpnt_type
    | Types.ClassElement _ |
      Types.ComponentTypeElement _ | Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx})  (*error*)

and resolve_not ctx expr arg =
  let resolve_not' cpnt_type =
    let rec not_type cl_spec = match cl_spec with
      | Types.PredefinedType { Types.base_type = Types.BooleanType } -> cl_spec
      | (Types.PredefinedType _ | Types.ArrayType _ | Types.ClassType _ |
        Types.ComponentType _ | Types.TupleType _) ->
          raise (CompilError
            {err_msg = ["not"; "_OperAppliedToNonBoolExpr"];
             err_info =
               [("_ExprKind", "not A");
                ("_TypeOfA", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
    let var = cpnt_type.Types.variability
    and inout = Types.Acausal
    and cl_spec =
      lazy (let cl_spec = evaluate cpnt_type.Types.base_class in
        not_type cl_spec) in
    let nat = UnaryOperation (Not, arg) in
    let elt_nat =
      let cpnt_type =
        component_element (lazy false) var (lazy inout) cl_spec in
      Types.ComponentElement cpnt_type in
    resolved_expression (Some expr) nat elt_nat in
  match arg.info.type_description with
    | Types.ComponentElement cpnt_type -> resolve_not' cpnt_type
    | Types.ClassElement _ |
      Types.ComponentTypeElement _ | Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and component_element flow var inout cl_spec =
  {
    Types.flow = flow;
          variability = var;
          causality = inout;
          base_class = cl_spec
  }

and element_nature_class ctx = function
  | Types.ClassElement cl_spec -> evaluate cl_spec
  | Types.ComponentElement cpnt_type -> evaluate cpnt_type.Types.base_class
  | Types.PredefinedTypeElement predef -> Types.PredefinedType predef
  | Types.ComponentTypeElement _ -> assert false (*error*)

and element_field_type_nature ctx flow var inout cl_spec id =
  let add_dimension dim = function
    | Types.ComponentElement cpnt_type ->
        let cpnt_type' =
          { cpnt_type with
            Types.base_class =
              lazy (Types.ArrayType (dim, evaluate cpnt_type.Types.base_class))
          } in
        Types.ComponentElement cpnt_type'
    | Types.ClassElement _
    | Types.ComponentTypeElement _
    | Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_InvalidClassElemModif"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
  let find_predefined_local_identifier predef id =
    match predef.Types.base_type with
      | Types.BooleanType when id = "start" -> Types.boolean_type Types.Parameter
      | Types.IntegerType when id = "start" ->
          Types.integer_type Types.Parameter
      | Types.RealType when id = "start" ->
          Types.real_type Types.Parameter
      | Types.StringType when id = "start" -> Types.string_type Types.Parameter
      | Types.EnumerationType enum_lits when id = "start" ->
          Types.enumeration_type Types.Parameter enum_lits
      | _ when id = "fixed" -> Types.boolean_type Types.Constant
      | Types.IntegerType when id = "nominal" ->
          Types.integer_type Types.Constant
      | Types.RealType when id = "nominal" ->
          Types.real_type Types.Constant
      | _ ->
          raise (CompilError
            {err_msg = ["_NotYetImplemented"; "_PredefinedTypeAttribModif"; id];
             err_info = [];
             err_ctx = ctx})
  and find_class_local_identifier flow var inout cl_type id =
    let apply_prefixes elt_nat = match elt_nat with
      | Types.ComponentElement cpnt_type ->
          let flow' = lazy (flow || evaluate cpnt_type.Types.flow) in
          Types.ComponentElement { cpnt_type with Types.flow = flow' }
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ -> elt_nat in
    try
      let elt_type =
        evaluate (List.assoc id cl_type.Types.named_elements) in
      match elt_type.Types.dynamic_scope with
        | None | Some Types.Inner | Some Types.InnerOuter
          when not elt_type.Types.protected ->
            apply_prefixes elt_type.Types.element_nature
        | None | Some Types.Inner | Some Types.InnerOuter ->
            raise (CompilError
              {err_msg = ["_CannotAccessProtectElem"; id];
               err_info = [];
               err_ctx = ctx}) (*error*)
        | Some Types.Outer ->
            raise (CompilError
              {err_msg = ["_CannotAccessOuterElem"; id];
               err_info = [];
               err_ctx = ctx}) (*error*)
    with Not_found ->
        raise (CompilError
          {err_msg = ["_UnknownIdentifier"; id];
           err_info = [];
           err_ctx = ctx }) (*error*) in
  let rec find_local_identifier flow var inout = function
    | Types.PredefinedType predef_type ->
        find_predefined_local_identifier predef_type id
    | Types.ClassType cl_type ->
        find_class_local_identifier flow var inout cl_type id
    | Types.ComponentType cpnt_type ->
        let flow = flow || evaluate cpnt_type.Types.flow
        and var =
          Types.max_variability var (evaluate cpnt_type.Types.variability)
        and inout = evaluate cpnt_type.Types.causality
        and base_class = evaluate cpnt_type.Types.base_class in
        find_local_identifier flow var inout base_class
    | Types.ArrayType (dim, cl_spec) ->
        add_dimension dim (find_local_identifier flow var inout cl_spec)
    | Types.TupleType _ -> assert false (*error*) in
  find_local_identifier flow var inout cl_spec

and scalar_element_nature elt_nat =
  let rec scalar_element_nature' cl_spec = match cl_spec with
    | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
      Types.TupleType _ -> cl_spec
    | Types.ArrayType (_, cl_spec) -> scalar_element_nature' cl_spec in
  match elt_nat with
    | Types.ComponentElement cpnt_type ->
        let base_class' =
          lazy (scalar_element_nature' (evaluate cpnt_type.Types.base_class)) in
        Types.ComponentElement { cpnt_type with Types.base_class = base_class' }
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ -> elt_nat

and resolve_lhs_expression ctx expr =
  raise (CompilError
    {err_msg = ["_NotYetImplemented";
                "_ExternalCallWithLeftHandSideExpr"];
     err_info = [];
     err_ctx = ctx})

and resolve_subscripts ctx expr cl_spec subs =
  let rec resolve_subscripts' n cl_spec subs = match cl_spec, subs with
    | _, [] -> []
    | Types.ArrayType (dim, cl_spec'), sub :: subs' ->
        let sub' = resolve_subscript ctx expr n dim sub in
        sub' :: resolve_subscripts' (Int32.add n 1l) cl_spec' subs'
    | (Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
       Types.TupleType _), _ :: _ ->
        raise (CompilError
          {err_msg = ["_CannotSubscriptANonArrayTypeElem"];
           err_info =
             [("_ExpectedType", "_ArrayType");
              ("_TypeFound", Types.string_of_class_specifier cl_spec)];
           err_ctx = ctx}) (*error*) in
  match subs.Syntax.nature with
    | Syntax.Subscripts subs' -> resolve_subscripts' 1l cl_spec subs'

and resolve_subscript ctx expr n dim sub = match sub.Syntax.nature with
  | Syntax.Colon -> resolve_colon ctx expr n dim
  | Syntax.Subscript expr' ->
      let ctx' =
      { ctx with
        context_nature = SubscriptContext (ctx, expr, n, dim);
        location = expr'.Syntax.info } in
      resolve_subscript_expression ctx' expr'

and resolve_colon ctx expr n dim =
  let range var stop =
    let nat = Range (one, one, stop)
    and elt_nat = Types.integer_array_type var dim in
    resolved_expression None nat elt_nat in
  match dim with
    | Types.ConstantDimension n ->
        let stop =
          let nat = Integer n
          and elt_nat = Types.integer_type Types.Constant in
          resolved_expression None nat elt_nat in
        range Types.Constant stop
    | Types.ParameterDimension ->
        let stop = size_function_call ctx None expr n in
        range Types.Parameter stop
    | Types.DiscreteDimension ->
        let stop = size_function_call ctx None expr n in
        range Types.Discrete stop

and resolve_subscript_expression ctx expr =
  let expr' = resolve_expression ctx expr in
  let resolve_subscript_expression' cpnt_type =
    let cl_spec = evaluate cpnt_type.Types.base_class in
    match cl_spec with
      | Types.PredefinedType { Types.base_type = Types.IntegerType } |
        Types.ArrayType
          (_, Types.PredefinedType { Types.base_type = Types.IntegerType }) ->
          expr'
      | Types.PredefinedType _ | Types.ClassType _ | Types.ComponentType _ |
        Types.ArrayType _ | Types.TupleType _ ->
          raise (CompilError
            {err_msg = ["_NonIntegerArraySubscript"];
             err_info =
               [("_ExpectedType", "Integer");
                ("_TypeFound", Types.string_of_class_specifier cl_spec)];
             err_ctx = ctx}) (*error*) in
  match expr'.info.type_description with
    | Types.ComponentElement cpnt_type ->
        resolve_subscript_expression' cpnt_type
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and size_function_call ctx syn arg n =
  let size_function_call' cpnt_type =
    let cpnt_type' =
      { cpnt_type with
        Types.base_class = lazy (Types.integer_class_type)
      } in
    let size =
      let nat = PredefinedIdentifier "size"
      and elt_nat =
        Types.function_type
          [("@1", cpnt_type);
           ("@2", Types.integer_component_type Types.Constant)]
          ["@3", cpnt_type'] in
      resolved_expression None nat elt_nat in
    let elt_nat = Types.ComponentElement cpnt_type' in
    let num =
      let nat = Integer n
      and elt_nat = Types.integer_type Types.Constant in
      resolved_expression None nat elt_nat
    and expr =
      let args =
        let arg1 =
          let nat = FunctionArgument 1
          and elt_nat = arg.info.type_description in
          resolved_expression None nat elt_nat
        and arg2 =
          let nat = FunctionArgument 2
          and elt_nat = Types.integer_type Types.Constant in
          resolved_expression None nat elt_nat in
        [arg1; arg2] in
      let nat = FunctionInvocation args in
      resolved_expression None nat elt_nat in
    let nat = FunctionCall (size, [arg; num], expr) in
    resolved_expression syn nat elt_nat in
  match arg.info.type_description with
    | Types.ComponentElement cpnt_type -> size_function_call' cpnt_type
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ ->
        raise (CompilError
          {err_msg = ["_ClassElemFoundInExpr"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and element_type ctx protect final repl dyn_scope elt_desc =
  {
    Types.protected = protect;
          final = bool_of_final final;
          replaceable = bool_of_replaceable repl;
          dynamic_scope = dynamic_scope_of_dynamic_scope dyn_scope;
          element_nature = element_nature_type ctx elt_desc
  }

and bool_of_replaceable = function
  | None -> false
  | Some Syntax.Replaceable -> true

and dynamic_scope_of_dynamic_scope = function
  | None -> None
  | Some Syntax.Inner -> Some Types.Inner
  | Some Syntax.Outer -> Some Types.Outer
  | Some Syntax.InnerOuter -> Some Types.InnerOuter

and element_nature_type ctx elt_desc =
  let elt_nat = match elt_desc.element_nature with
    | Component cpnt_desc -> Types.ComponentElement (evaluate cpnt_desc.component_type)
    | Class cl_def -> Types.ClassElement cl_def.class_type
    | ComponentType cpnt_type_desc ->
        Types.ComponentTypeElement (evaluate cpnt_type_desc.described_type)
    | PredefinedType predef -> Types.PredefinedTypeElement predef in
  elt_nat

and class_specifier_type ctx part kind cl_def cl_spec =
  let class_kind kind cl_type =
    let check_class () =
      if has_inouts cl_type then
        raise (CompilError
          {err_msg = ["_CannotUseCausPrefixInGenClass";
                      class_specifier_name cl_spec];
           err_info = [];
           err_ctx = ctx}) (*error*)
      else kind
    and check_model () = kind
    and check_block () =
      raise (CompilError
        {err_msg = ["_NotYetImplemented"; "_BlockElem"];
         err_info = [];
         err_ctx = ctx})
    and check_record () = kind
    and check_expandable_connector () =
      raise (CompilError
        {err_msg = ["_NotYetImplemented"; "_ExpandableConnector"];
         err_info = [];
         err_ctx = ctx})
    and check_connector () = kind
    and check_package () = kind
    and check_function () = kind in
  match kind with
    | Types.Class -> check_class ()
    | Types.Model -> check_model ()
    | Types.Block -> check_block ()
    | Types.Record -> check_record ()
    | Types.ExpandableConnector -> check_expandable_connector ()
    | Types.Connector -> check_connector ()
    | Types.Package -> check_package ()
    | Types.Function -> check_function () in
  let rec cl_type =
    {
       Types.partial = bool_of_partial part;
            kind = lazy (class_kind kind cl_type);
            named_elements = class_type_elements ctx kind cl_def
    } in
  Types.ClassType cl_type

and bool_of_partial = function
  | None -> false
  | Some Syntax.Partial -> true

and class_type_elements ctx kind cl_def = match evaluate cl_def.description with
  | LongDescription long_desc -> long_description_type_elements ctx kind long_desc
  | ShortDescription short_desc -> short_description_type_elements ctx kind short_desc

and short_description_type_elements ctx kind short_desc =
  let cl_type = evaluate short_desc.modified_class_type in
  let kind' = evaluate cl_type.Types.kind in
  match kind, kind' with
    | Types.Class, Types.Class |
      Types.Model, Types.Model |
      Types.Block, Types.Block |
      Types.Record, Types.Record |
      Types.ExpandableConnector, Types.ExpandableConnector |
      Types.Connector, Types.Connector |
      Types.Package, Types.Package |
      Types.Function, Types.Function -> cl_type.Types.named_elements
    | (Types.Class | Types.Model | Types.Block | Types.Record |
      Types.ExpandableConnector | Types.Connector | Types.Package | Types.Function),
      (Types.Class | Types.Model | Types.Block | Types.Record |
      Types.ExpandableConnector | Types.Connector | Types.Package | Types.Function) ->
        raise (CompilError
          {err_msg = ["_InheritFromDiffClassKindsNotAllowed"];
           err_info = [];
           err_ctx = ctx}) (*error*)

and long_description_type_elements ctx kind long_desc =
  let type_element (id, elt_desc) = id, elt_desc.element_type in
  let local_elts = List.map type_element long_desc.named_elements in
  let add_extensions kinds exts =
    let add_named_element protected named_elt named_elts =
      let element_type elt_type =
        let elt_type' = evaluate elt_type in
        { elt_type' with Types.protected =
            elt_type'.Types.protected || protected } in
      match named_elt with
      | id, _ when List.mem_assoc id named_elts ->
          raise (CompilError
            {err_msg = [id; "_AlreadyDeclaredInParentClass"];
             err_info = [];
             err_ctx = ctx}) (*error*)
      | id, elt_type -> (id, lazy (element_type elt_type)) :: named_elts in
    let add_extension_contribution (visibility, modif_cl) named_elts =
      let protected = bool_of_visibility visibility
      and cl_type = evaluate modif_cl.modified_class_type in
      let named_elts' = cl_type.Types.named_elements in
      if List.mem (evaluate cl_type.Types.kind) kinds then
        List.fold_right (add_named_element protected) named_elts' named_elts
      else
        raise (CompilError
          {err_msg = ["_InheritFromDiffClassKindsNotAllowed"];
           err_info = [];
           err_ctx = ctx}) (*error*) in
    List.fold_right add_extension_contribution exts local_elts in
  match kind, long_desc.extensions with
  | Types.Function, [] -> local_elts
  | Types.Function, _ :: _ ->
      raise (CompilError
        {err_msg = ["_InheritFromFunctionNotAllowed"];
         err_info = [];
         err_ctx = ctx}) (*error*)
  | (Types.Class | Types.Model | Types.Block | Types.Record | Types.Connector | Types.Package),
    exts -> add_extensions [kind] exts
  | Types.ExpandableConnector, exts ->
      add_extensions [kind; Types.Connector] exts

and bool_of_visibility = function
  | Public -> false
  | Protected -> true

and has_inouts cl_type =
  let is_inout_component cpnt_type =
    match evaluate cpnt_type.Types.causality with
      | Types.Input | Types.Output -> true
      | Types.Acausal -> false in
  let is_inout = function
    | Types.ComponentElement cpnt_type -> is_inout_component cpnt_type
    | Types.ClassElement _ | Types.ComponentTypeElement _ |
      Types.PredefinedTypeElement _ -> false
  and element_nature (_, elt_type) = (evaluate elt_type).Types.element_nature in
  List.exists
    (function named_elt -> is_inout (element_nature named_elt))
    cl_type.Types.named_elements

and component_type_of_expression ctx expr =
  match expr.info.type_description with
  | Types.ComponentElement cpnt_type -> cpnt_type
  | Types.ClassElement _ | Types.ComponentTypeElement _ |
    Types.PredefinedTypeElement _ ->
      raise (CompilError
        {err_msg = ["_ClassElemFoundInExpr"];
         err_info = [];
         err_ctx = ctx}) (*error*)

and scalar_class_specifier ctx expr =
  let rec scalar_class_specifier' cl_spec = match cl_spec with
    | Types.ArrayType (dim, cl_spec) ->
        scalar_class_specifier' cl_spec
    | _ -> cl_spec in
  let cpnt_type = component_type_of_expression ctx expr in
  let cl_spec = evaluate cpnt_type.Types.base_class in
  scalar_class_specifier' cl_spec

and expression_of_variable expr =
  let vector_variables vec_elts = match vec_elts.Syntax.nature with
    | Syntax.VectorReduction _ -> false
    | Syntax.VectorElements exprs ->
        List.for_all expression_of_variable exprs in
  match expr.Syntax.nature with
  | Syntax.Identifier _ -> true
  | Syntax.FieldAccess (expr', _) -> expression_of_variable expr'
  | Syntax.IndexedAccess (expr', subs) ->
      expression_of_variable expr'
  | Syntax.MatrixConstruction exprss ->
      List.for_all (List.for_all expression_of_variable) exprss
  | Syntax.Tuple exprs ->
      List.for_all expression_of_variable exprs
  | Syntax.Vector vec_elts -> vector_variables vec_elts
  | _ -> false

and string_of_bin_oper_kind kind = match kind with
  | And -> " and "
  | Divide -> " / "
  | EqualEqual -> " == "
  | GreaterEqual -> " >= "
  | Greater -> " > "
  | LessEqual -> " <= "
  | Less -> " < "
  | Times -> " * "
  | NotEqual -> " <> "
  | Or -> " or "
  | Plus -> " + "
  | Power -> " ^ "
  | Minus -> " - "

and string_of_un_oper_kind kind = match kind with
  | Not -> " not "
  | UnaryMinus -> "- "
  | UnaryPlus -> "+ "

and apply_binary_coercions exprs =
  let base_type expr =
    let rec base_type' cl_spec = match cl_spec with
      | Types.ArrayType (_, cl_spec) -> base_type' cl_spec
      | Types.PredefinedType pt -> Some pt.Types.base_type
      | _ -> None in
    match expr.info.type_description with
    | Types.ComponentElement cpnt_type ->
        let cl_spec = evaluate cpnt_type.Types.base_class in
        base_type' cl_spec
    | _ -> None
  and real_type bt = match bt with
    | Some Types.RealType -> true
    | _ -> false
  and integer_type bt = match bt with
    | Some Types.IntegerType -> true
    | _ -> false in
  match List.map base_type exprs with
  | [] | [ _ ] -> exprs
  | bts when (List.exists real_type bts) &&
    (List.exists integer_type bts) ->
      let cpnt_type = Types.real_component_type Types.Continuous in
      List.map (apply_rhs_coercions cpnt_type) exprs
  | _ -> exprs

and apply_rhs_coercions cpnt_type expr =
  let apply_real_of_integer cpnt_type cpnt_type' =
    let rec apply_real_of_integer' cl_spec cl_spec' =
      match cl_spec, cl_spec' with
      | Types.ArrayType (dim, cl_spec), _ ->
          apply_real_of_integer' cl_spec cl_spec'
      | _, Types.ArrayType (dim', cl_spec') ->
          let coer, cl_spec' = apply_real_of_integer' cl_spec cl_spec' in
          coer, Types.ArrayType (dim', cl_spec')
      | Types.PredefinedType { Types.base_type = Types.RealType },
        Types.PredefinedType { Types.base_type = Types.IntegerType } ->
          Some RealOfInteger, Types.real_class_type
      | _, _ -> None, cl_spec' in
    let cl_spec = evaluate cpnt_type.Types.base_class
    and cl_spec' = evaluate cpnt_type'.Types.base_class in
    match apply_real_of_integer' cl_spec cl_spec' with
    | Some RealOfInteger, cl_spec' ->
        let cpnt_type' =
          {
            cpnt_type' with
            Types.base_class = lazy cl_spec'
          }
        and nat' = Coercion (RealOfInteger, expr) in
        let elt_nat' = Types.ComponentElement cpnt_type' in
        resolved_expression expr.info.syntax nat' elt_nat'
    | _ -> expr in
  match expr.info.type_description with
  | Types.ComponentElement cpnt_type' ->
      apply_real_of_integer cpnt_type cpnt_type'
  | _ -> expr

(* for debug *)
and string_of_expression expr = match expr.nature with
  | BinaryOperation (bin_oper_kind, expr, expr') ->
      Printf.sprintf "BinaryOperation(_, %s, %s)"
        (string_of_expression expr)
        (string_of_expression expr')
  | DynamicIdentifier (i, s) -> "DynamicIdentifier"
  | False -> "False"
  | FieldAccess (expr, s) -> "FieldAccess"
  | FunctionArgument i -> "FunctionArgument"
  | FunctionCall (expr, exprs, expr') ->
      Printf.sprintf "FunctionCall(%s, {%s}, %s)"
        (string_of_expression expr)
        (String.concat "," (List.map string_of_expression exprs))
        (string_of_expression expr') 
  | FunctionInvocation exprs -> "FunctionInvocation"
  | If (alts, expr) -> "If"
  | IndexedAccess (expr, exprs) -> "IndexedAccess"
  | Integer i ->
      Printf.sprintf "Integer(%d)" (Int32.to_int i)
  | LocalIdentifier (i, s) ->
      Printf.sprintf "LocalIdentifier(%d, %s)" i s
  | LoopVariable i -> "LoopVariable"
  | NoEvent expr -> "NoEvent"
  | PredefinedIdentifier s ->
      Printf.sprintf "PredefinedIdentifier(%s)" s
  | Range (start, step, stop) ->
      Printf.sprintf "Range(%s, %s, %s)"
        (string_of_expression start)
        (string_of_expression step)
        (string_of_expression stop)
  | Real f -> "Real"
  | String s -> "String"
  | ToplevelIdentifier s -> "ToplevelIdentifier"
  | True -> "True"
  | Tuple exprs -> "Tuple"
  | UnaryOperation (un_oper_kind, expr) -> "UnaryOperation"
  | Vector exprs -> "Vector"
  | VectorReduction (exprs, expr) -> "VectorReduction"
  | Coercion _ -> "Coercion"
