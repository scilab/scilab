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

(* Precompilation *)

type precompiled_class =
  {
    class_kind: ParseTree.class_kind;
    mutable extensions: extension list;
    mutable public_classes: (string * precompiled_class) list;
    mutable protected_classes: (string * precompiled_class) list;
    mutable public_cpnts: (string * precompiled_component) list;
    mutable protected_cpnts: (string * precompiled_component) list;
    mutable equs: ParseTree.equation_clause list;
    mutable algs: ParseTree.algorithm_clause list;
    mutable class_comment: ParseTree.string_comment
  }

and extension = string list * modification list

and modification =
  | Modification of (string * ParseTree.array_subscripts) list *
    modification list * ParseTree.expression option

and precompiled_component =
  {
    class_name: string list;
    subscripts: ParseTree.array_subscripts;
    flow: ParseTree.flow option;
    variability: ParseTree.variability option;
    inout: ParseTree.inout option;
    modification: modification list * ParseTree.expression option;
    comment: ParseTree.comment
  }

let create_precompiled_class kind parent_opt =
  let empty_comment = ParseTree.StringComment [] in
  {
    class_kind = kind;
    extensions = [];
    public_classes = [];
    protected_classes = [];
    public_cpnts = [];
    protected_cpnts = [];
    equs = [];
    algs = [];
    class_comment = empty_comment
  }

let rec precompile = function
  | ParseTree.StoredDefinition (ParseTree.NotWithin, defs) ->
      let root = create_precompiled_class ParseTree.Package None in
      root.public_classes <- precompile_class_definitions root defs;
      root
  | ParseTree.StoredDefinition (ParseTree.Within _, _) ->
      failwith "within not allowed"

and precompile_class_definitions parent defs =
  List.fold_left (precompile_class_definition parent) [] defs

and precompile_class_definition parent map = function
  | ParseTree.Definition (ParseTree.NotFinal, class_def) ->
      precompile_not_final_class_definition parent map class_def
  | ParseTree.Definition (ParseTree.Final, _) ->
      failwith "final class definitions not allowed"

and precompile_not_final_class_definition parent map = function
  | ParseTree.ClassDefinition (ParseTree.Class, id,
    ParseTree.NotEncapsulated, ParseTree.NotPartial, class_spec) ->
      let cl = create_precompiled_class ParseTree.Class (Some parent) in
      precompile_class_specifier_into cl id class_spec;
      (id, cl) :: map
  | ParseTree.ClassDefinition (ParseTree.Function, id,
    ParseTree.NotEncapsulated, ParseTree.NotPartial, class_spec) ->
      let cl = create_precompiled_class ParseTree.Function (Some parent) in
      precompile_class_specifier_into cl id class_spec;
      (id, cl) :: map
  | _ -> failwith "precompile_class_definition: Unsupported class definition"

and precompile_class_specifier_into cl id = function
  | ParseTree.Specifier (cmt, compo, id') when id = id' ->
      cl.class_comment <- cmt;
      precompile_composition_into cl compo
  | ParseTree.Specifier _ -> failwith "Wrong class definition"
  | ParseTree.ShortSpecifier _ ->
      failwith "Short class definitions not allowed"
  | ParseTree.Enumeration _ -> failwith "Enumeration definitions not allowed"

and precompile_composition_into cl = function
  | ParseTree.Composition (elts, other_elts_list, None) ->
      precompile_public_elements_into cl elts;
      precompile_other_elements_into cl other_elts_list
  | ParseTree.Composition (elts, other_elts_list,
    Some (ParseTree.External (None, None, None))) ->
      precompile_public_elements_into cl elts;
      precompile_other_elements_into cl other_elts_list
  | ParseTree.Composition (_, _, Some _) ->
      failwith "precompile_composition_into: invalid external function call"

and precompile_public_elements_into cl elts =
  List.iter (precompile_public_element_into cl) elts

and precompile_public_element_into cl = function
  | ParseTree.AnnotationElement _ -> failwith "Annotations not allowed"
  | ParseTree.ImportClause _ -> failwith "Import statements not allowed"
  | ParseTree.ExtendsClause (name, None) ->
      cl.extensions <- (name, []) :: cl.extensions
  | ParseTree.ExtendsClause (name, Some class_modif) ->
      let modifs = precompile_class_modification class_modif in
      cl.extensions <- (name, modifs) :: cl.extensions
  | ParseTree.ClassDefinitionElement (class_def, ParseTree.NotFinal,
    ParseTree.NoDynamicScope) ->
      let map =
        precompile_not_final_class_definition cl cl.public_classes class_def
      in
      cl.public_classes <- map
  | ParseTree.ClassDefinitionElement (_, ParseTree.Final, _) ->
      failwith "Final class definitions not allowed"
  | ParseTree.ClassDefinitionElement (_, _,
    (ParseTree.Inner | ParseTree.Outer)) ->
      failwith "Dynamic scope not allowed"
  | ParseTree.ComponentClauseElement (cpnt_clause, ParseTree.NotFinal,
    ParseTree.NoDynamicScope) ->
      precompile_public_component_clause_into cl cpnt_clause
  | ParseTree.ComponentClauseElement (_, ParseTree.Final, _) ->
      failwith "Final component definitions not allowed"
  | ParseTree.ComponentClauseElement (_, _,
    (ParseTree.Inner | ParseTree.Outer)) ->
      failwith "Dynamic scope not allowed"
  | ParseTree.ReplaceableClassDefinition _ ->
      failwith "Replaceable classes not allowed"
  | ParseTree.ReplaceableComponentClause _ ->
      failwith "Replaceable components not allowed"

and precompile_other_elements_into cl elts =
  List.iter (precompile_other_element_into cl) elts

and precompile_other_element_into cl = function
  | ParseTree.Public elt_list ->
      precompile_public_elements_into cl elt_list
  | ParseTree.Protected element_list ->
      failwith "Protected elements not allowed"
  | ParseTree.EquationClauseElement equ_clause ->
      cl.equs <- equ_clause :: cl.equs
  | ParseTree.AlgorithmClauseElement alg_clause ->
      cl.algs <- alg_clause :: cl.algs

and precompile_public_component_clause_into cl = function
  | ParseTree.ComponentClause (type_prefix, type_spec,
    array_subscrs, cpnt_decl_list) ->
      let flow, var_opt, inout_opt = precompile_type_prefix type_prefix in
      let precompile_component_declaration = function
        | ParseTree.ComponentDeclaration ((id, subscrs, modif_opt), comment) ->
            let subscripts =
              Array.append subscrs array_subscrs
            in
            id,
            {
              class_name = type_spec;
              subscripts = subscripts;
              flow = flow;
              variability = var_opt;
              inout = inout_opt;
              modification = precompile_modification modif_opt;
              comment = comment
            }
      in
      cl.public_cpnts <-
        List.fold_right
          (fun cpnt_decl cpnts ->
            precompile_component_declaration cpnt_decl :: cpnts)
          cpnt_decl_list
          cl.public_cpnts

and precompile_type_prefix = function
  | ParseTree.TypePrefix (flow, var_opt, inout_opt) ->
      flow, var_opt, inout_opt

and precompile_modification = function
  | Some (ParseTree.Modification (class_modif, expr_opt)) ->
      precompile_class_modification class_modif, expr_opt
  | Some (ParseTree.Eq expr) | Some (ParseTree.ColEq expr) -> [], Some expr
  | None -> [], None

and precompile_class_modification = function
  | ParseTree.ClassModification args ->
      List.map precompile_argument args

and precompile_argument = function
  | ParseTree.ElementModification (ParseTree.NotEach, ParseTree.NotFinal,
    cpnt_ref, ParseTree.Modification (class_modif, expr_opt), _) ->
      let modifs = precompile_class_modification class_modif in
      Modification (cpnt_ref, modifs, expr_opt)
  | ParseTree.ElementModification (ParseTree.NotEach, ParseTree.NotFinal,
    cpnt_ref, ParseTree.Eq expr, _) |
    ParseTree.ElementModification (ParseTree.NotEach, ParseTree.NotFinal,
    cpnt_ref, ParseTree.ColEq expr, _) -> Modification (cpnt_ref, [], Some expr)
  | _ -> failwith "Unsupported modification"
