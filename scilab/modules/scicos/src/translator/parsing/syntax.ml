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

(* 'info denotes the type of the information attached to the syntax nodes
   (location for instance) in the parse tree *)

type ('a, 'info) node =
  {
    nature: 'a;
    info: 'info
  }

and 'info toplevel_element = ('info toplevel_element_desc, 'info) node

and 'info toplevel_element_desc =
  | ClassDefinitions of 'info class_definition list
  | Expression of 'info expression
  | VariablesDefinitions of 'info expression * 'info array_subscripts option *
      'info component_declaration list
  | Command of 'info algorithm
  | Within of string list
  | Import of 'info import_clause

and 'info class_definition = ('info class_definition_desc, 'info) node

and 'info class_definition_desc =
  | ClassDefinition of final option * 'info definition

and final = Final

and 'info definition = ('info definition_desc, 'info) node

and 'info definition_desc =
  | Definition of encapsulated option * partial option * class_kind *
      'info class_specifier

and class_kind =
  | Class
  | Model
  | Block
  | Record
  | ExpandableConnector
  | Connector
  | Type
  | Package
  | Function

and encapsulated = Encapsulated

and partial = Partial

and 'info class_specifier = ('info class_specifier_desc, 'info) node

and 'info class_specifier_desc =
  | LongSpecifier of ident * string list * 'info composition
  | ShortSpecifier of ident * 'info base_prefix * 'info expression *
      'info array_subscripts option * 'info class_modification option *
      'info comment
  | EnumerationSpecifier of ident * 'info enumeration_composition *
      'info comment
  | ExtensionSpecifier of ident * 'info class_modification option *
      string list * 'info composition

and 'info base_prefix = 'info type_prefix

and 'info enumeration_composition = ('info enumeration_composition_desc, 'info) node

and 'info enumeration_composition_desc =
  | EnumList of 'info enumeration_literal list option
  | EnumColon

and 'info enumeration_literal = ('info enumeration_literal_desc, 'info) node

and 'info enumeration_literal_desc =
  | EnumerationLiteral of ident * 'info comment

and 'info composition = ('info composition_desc, 'info) node

and 'info composition_desc =
  | Composition of 'info element list * 'info other_elements list *
      'info externalll option

and 'info element = ('info element_desc, 'info) node

and 'info element_desc =
  | ClassAnnotation of 'info annotation
  | ImportClause of 'info import_clause * 'info annotation option
  | ExtendsClause of 'info extends_clause * 'info annotation option
  | ElementDefinition of redeclare option * final option *
      dynamic_scope option * 'info element_definition *
      'info annotation option

and 'info element_definition = ('info element_definition_desc, 'info) node

and 'info element_definition_desc =
  | ClassDefinitionElement of replaceable option * 'info definition *
      'info constraining_clause list
  | ComponentClauseElement of replaceable option * 'info component_clause *
      'info constraining_clause list

and replaceable = Replaceable

and redeclare = Redeclare

and dynamic_scope = Inner | Outer | InnerOuter

and 'info extends_clause = ('info extends_clause_desc, 'info) node

and 'info extends_clause_desc =
  | Extends of 'info expression * 'info class_modification option *
      'info annotation option

and 'info constraining_clause = ('info constraining_clause_desc, 'info) node

and 'info constraining_clause_desc =
  | Constraint of constraint_kind * 'info expression *
      'info class_modification option * 'info comment

and constraint_kind = Extension | Restriction

and 'info other_elements = ('info other_elements_desc, 'info) node

and 'info other_elements_desc =
  | Public of 'info element list
  | Protected of 'info element list
  | EquationClause of initial option * 'info equation_definition list
  | AlgorithmClause of initial option * 'info algorithm_definition list

and initial = Initial

and 'info externalll = ('info externalll_desc, 'info) node

and 'info externalll_desc =
  | External of string option * 'info external_function_call option *
      'info annotation option * 'info annotation option

and 'info external_function_call = ('info external_function_call_desc, 'info) node

and 'info external_function_call_desc =
  | ExternalFunctionCall of 'info expression option * ident * 'info expression list

and 'info import_clause = ('info import_clause_desc, 'info) node

and 'info import_clause_desc =
  | NewIdentifier of ident * 'info expression * 'info comment
  | OldIdentifier of 'info expression * 'info comment
  | AllIdentifiers of 'info expression * 'info comment

and 'info component_clause = ('info component_clause_desc, 'info) node

and 'info component_clause_desc =
  | ComponentClause of 'info type_prefix * 'info type_specifier *
      'info array_subscripts option * 'info component_declaration list

and 'info type_prefix = ('info type_prefix_desc, 'info) node

and 'info type_prefix_desc =
  | TypePrefix of flow option * variability option * inout option

and flow = Flow

and variability = Discrete | Parameter | Constant

and inout = Input | Output

and 'info type_specifier = 'info expression

and 'info component_declaration = ('info component_declaration_desc, 'info) node

and 'info component_declaration_desc =
  | ComponentDeclaration of 'info declaration * 'info comment

and 'info declaration = ('info declaration_desc, 'info) node

and 'info declaration_desc =
  | Declaration of ident * 'info array_subscripts option *
      'info modification option

and 'info modification = ('info modification_desc, 'info) node

and 'info modification_desc =
  | Modification of 'info class_modification * 'info expression option
  | Eq of 'info expression
  | ColEq of 'info expression

and 'info class_modification = ('info class_modification_desc, 'info) node

and 'info class_modification_desc =
  | ClassModification of 'info argument list

and 'info argument = ('info argument_desc, 'info) node

and 'info argument_desc =
  | ElementModification of each option * final option * 'info expression *
      'info modification option * string list
  | ElementRedeclaration of each option * final option *
      'info element_definition

and each = Each

and 'info equation_definition = ('info equation_definition_desc, 'info) node

and 'info equation_definition_desc =
  | Equation of 'info equation * 'info comment * 'info annotation option

and 'info algorithm_definition = ('info algorithm_definition_desc, 'info) node

and 'info algorithm_definition_desc =
  | Algorithm of 'info algorithm * 'info comment * 'info annotation option

and 'info equation = ('info equation_desc, 'info) node

and 'info equation_desc =
  | Equal of 'info expression * 'info expression
  | ConditionalEquationE of ('info expression * 'info equation list) list *
      'info equation list option
  | ForClauseE of 'info for_indices * 'info equation list
  | ConnectClause of 'info expression * 'info expression
  | WhenClauseE of ('info expression * 'info equation list) list
  | FunctionCallE of 'info expression * 'info function_arguments option

and 'info algorithm = ('info algorithm_desc, 'info) node

and 'info algorithm_desc =
  | Assign of 'info expression * 'info expression
  | FunctionCallA of 'info expression * 'info function_arguments option
  | MultipleAssign of 'info expression list * 'info expression *
      'info function_arguments option
  | Break
  | Return
  | ConditionalEquationA of ('info expression * 'info algorithm list) list *
      'info algorithm list option
  | ForClauseA of 'info for_indices * 'info algorithm list
  | WhileClause of 'info expression * 'info algorithm list
  | WhenClauseA of ('info expression * 'info algorithm list) list

and 'info for_indices = (ident * 'info expression option) list

and 'info expression = ('info expression_desc, 'info) node

and 'info expression_desc =
  | BinaryOperation of 'info binary_operator_kind * 'info expression * 'info expression
  | End
  | False
  | FieldAccess of 'info expression * ident
  | FunctionCall of 'info expression * 'info function_arguments option
  | Identifier of string
  | If of ('info expression * 'info expression) list * 'info expression
  | IndexedAccess of 'info expression * 'info array_subscripts
  | Integer of string
  | MatrixConstruction of 'info expression list list
  | NoEvent of 'info expression
  | Range of 'info expression * 'info expression option * 'info expression
  | Real of string
  | String of string
  | True
  | Tuple of 'info expression list
  | UnaryOperation of 'info unary_operator_kind * 'info expression
  | Vector of 'info vector_elements

and 'info unary_operator_kind = ('info unary_operator_kind_desc, 'info) node

and 'info unary_operator_kind_desc =
  | UnaryMinus
  | Not
  | UnaryPlus

and 'info binary_operator_kind = ('info binary_operator_kind_desc, 'info) node

and 'info binary_operator_kind_desc =
  | Plus
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
  | Power
  | Minus

and 'info vector_elements = ('info vector_elements_desc, 'info) node

and 'info vector_elements_desc =
  | VectorReduction of 'info expression * 'info for_indices
  | VectorElements of 'info expression list

and ident = string

and 'info function_arguments = ('info function_arguments_desc, 'info) node

and 'info function_arguments_desc =
  | Reduction of 'info expression * 'info for_indices
  | ArgumentList of 'info function_arguments_element list

and 'info function_arguments_element = ('info function_arguments_element_desc, 'info) node

and 'info function_arguments_element_desc =
  | Argument of 'info expression
  | NamedArgument of ident * 'info expression

and 'info array_subscripts = ('info array_subscripts_desc, 'info) node

and 'info array_subscripts_desc =
  | Subscripts of 'info array_subscript list

and 'info array_subscript = ('info array_subscript_desc, 'info) node

and 'info array_subscript_desc =
  | Colon
  | Subscript of 'info expression

and 'info comment = ('info comment_desc, 'info) node

and 'info comment_desc =
  | Comment of string list * 'info annotation option

and 'info annotation = ('info annotation_desc, 'info) node

and 'info annotation_desc =
  | Annotation of 'info class_modification

(* Conversion of elements to string, used for error information display *)

let rec string_of_expression expr =
  string_of_subexpression None expr

and string_of_subexpression expr_option subexpr =
  let string_of_subexpression' =
    match subexpr.nature with
      | BinaryOperation (kind, arg1, arg2) ->
          string_of_binOper subexpr kind arg1 arg2
      | End -> "end"
      | False -> "false"
      | FieldAccess (expr, id) ->
          (string_of_expression expr) ^ "." ^ id
      | FunctionCall (expr, fun_args) ->
          string_of_function_call expr fun_args
      | Identifier id -> id
      | If (alts, expr) -> string_of_if expr_option alts expr
      | IndexedAccess (expr, subs) -> string_of_indexedAccess expr subs
      | Integer s -> s
      | MatrixConstruction exprss -> string_of_matrix exprss
      | NoEvent expr -> "noEvent(" ^ (string_of_expression expr) ^ ")"
      | Range (start, step, stop) -> string_of_range start step stop
      | Real s -> s
      | String s -> "\"" ^ s ^ "\""
      | True -> "true"
      | Tuple exprs -> string_of_tuple exprs
      | UnaryOperation (kind, arg) ->
          string_of_unOper subexpr kind arg
      | Vector vec_elts -> string_of_vector vec_elts in
  parenthesize expr_option string_of_subexpression' subexpr

and parenthesize expr_option s subexpr =
  let add_parenthesis =
    "(" ^ s ^ ")" in
  let parenthesize_un_bin_Oper kind kind' =
    match kind.nature, kind'.nature with
      | (UnaryMinus | UnaryPlus),
        (Plus | Minus) ->
          add_parenthesis
      | Not, _ ->
          add_parenthesis
      | _, _ -> s in
  let parenthesize_bin_bin_Oper kind kind' =
    match kind.nature, kind'.nature with
      | Divide,
        (Plus | Minus | Times) ->
        add_parenthesis
      | Times,
        (Plus | Minus | Divide) ->
          add_parenthesis
      | Power, _ ->
          add_parenthesis
      | ( EqualEqual | GreaterEqual | Greater |
          LessEqual | Less | NotEqual | And |
          Or),
        ( EqualEqual | GreaterEqual | Greater |
          LessEqual | Less | NotEqual) ->
          add_parenthesis
      | And, Or -> add_parenthesis
      | Or, And -> add_parenthesis
      | _, _ -> s in
  let parenthesize' expr =
    match expr.nature, subexpr.nature with
      | BinaryOperation (kind, _, _),
        BinaryOperation (kind', _, _) ->
          parenthesize_bin_bin_Oper kind kind'
      | UnaryOperation (kind, _),
        BinaryOperation (kind', _, _) ->
          parenthesize_un_bin_Oper kind kind'
      | ( BinaryOperation (_, _, _) |
          UnaryOperation (_, _) ),
        UnaryOperation (_, _) ->
          add_parenthesis
      | _, _ -> s in
  match expr_option with
    | None -> s
    | Some expr -> parenthesize' expr

and string_of_binOperKind kind =
  match kind.nature with
    | Plus -> " + "
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
    | Power -> " ^ "
    | Minus -> " - "

and string_of_binOper expr kind arg1 arg2 =
  (string_of_subexpression (Some expr) arg1) ^
  (string_of_binOperKind kind) ^
  (string_of_subexpression (Some expr) arg2)

and string_of_range start step stop =
  let sstep = match step with
    | None -> ":"
    | Some step -> ":" ^ (string_of_expression step)  ^ ":" in
  (string_of_expression start) ^ sstep ^ (string_of_expression stop)

and string_of_unOperKind kind =
  match kind.nature with
    | UnaryMinus -> "- "
    | Not -> "not "
    | UnaryPlus -> "+ "

and string_of_unOper expr kind arg =
  (string_of_unOperKind kind) ^
  (string_of_subexpression (Some expr) arg)

and string_of_tuple exprs =
  let rec string_of_tuple' exprs =
    match exprs with
      | [] -> ""
      | [expr] -> string_of_expression expr
      | expr :: exprs ->
          (string_of_expression expr) ^ ", " ^ (string_of_tuple' exprs) in
  "(" ^ (string_of_tuple' exprs) ^ ")"

and string_of_if expr_option alts expr =
  let add_parenthesis s =
    "(" ^ s ^ ")" in
  let rec string_of_alts alts = match alts with
    | [] -> ""
    | (cond, expr) :: alts ->
        "if (" ^ (string_of_expression cond) ^ ") then (" ^
        (string_of_expression expr) ^ ") else" ^
        (string_of_alts alts) in
  let string_of_if' =
    (string_of_alts alts) ^ " " ^
    (add_parenthesis (string_of_expression expr)) in
  match expr_option with
    | None -> string_of_if'
    | Some _ -> add_parenthesis string_of_if'

and string_of_for_inds for_inds =
  let string_of_for_ind for_ind = match for_ind with
    | id, None -> id
    | id, Some expr -> id ^ " in " ^ string_of_expression expr in
  let rec string_of_for_inds' for_inds = match for_inds with
    | [] -> ""
    | [for_ind] -> string_of_for_ind for_ind
    | for_ind :: for_inds ->
        (string_of_for_ind for_ind) ^ ", " ^ (string_of_for_inds' for_inds) in
  "for " ^ (string_of_for_inds' for_inds)

and string_of_function_call expr fun_args =
  let string_of_arg arg = match arg.nature with
    | Argument expr -> string_of_expression expr
    | NamedArgument (id, expr) ->
        id ^ " = " ^ (string_of_expression expr) in
  let rec string_of_args args = match args with
    | [] -> ""
    | [arg] -> string_of_arg arg
    | arg :: args -> (string_of_arg arg) ^ ", " ^ (string_of_args args) in
  let string_of_fun_args fun_args = match fun_args.nature with
    | ArgumentList args -> string_of_args args
    | Reduction (expr, for_inds) when for_inds = [] ->
        string_of_expression expr
    | Reduction (expr, for_inds) ->
        (string_of_expression expr) ^ " " ^
        (string_of_for_inds for_inds) in
  let string_of_fun_args_option fun_args = match fun_args with
    | None -> ""
    | Some fun_args -> string_of_fun_args fun_args in
  (string_of_expression expr) ^
  "(" ^ (string_of_fun_args_option fun_args) ^ ")"

and string_of_indexedAccess expr subs =
  let string_of_sub sub = match sub.nature with
    | Colon -> " : "
    | Subscript expr -> string_of_expression expr in
  let rec string_of_subs subs = match subs with
    | [] -> ""
    | [sub] -> string_of_sub sub
    | sub :: subs ->
        (string_of_sub sub) ^ ", " ^ string_of_subs subs in
  match subs.nature with
    | Subscripts subs ->
        (string_of_expression expr) ^ "[" ^ (string_of_subs subs) ^ "]"

and string_of_vectorElements exprs = match exprs with
  | [] -> ""
  | [expr] -> string_of_expression expr
  | expr :: exprs ->
      (string_of_expression expr) ^ ", " ^
      (string_of_vectorElements exprs)

and string_of_vector vec_elts =
  let string_of_vector' = match vec_elts.nature with
    | VectorReduction (expr, for_inds) ->
        "{" ^ (string_of_expression expr) ^ " " ^
        (string_of_for_inds for_inds) ^ "}"
    | VectorElements exprs ->
        "{" ^ string_of_vectorElements exprs ^ "}" in
  string_of_vector'

and string_of_matrix exprss =
  let rec string_of_matrix' exprss = match exprss with
    | [] -> ""
    | [exprs] -> string_of_vectorElements exprs
    | exprs :: exprss ->
        (string_of_vectorElements exprs) ^ "; " ^
        (string_of_matrix' exprss) in
  "[" ^ (string_of_matrix' exprss) ^ "]"

let string_of_classDefinitions cl_defs = ""

let string_of_within path =
  let rec string_of_path path = match path with
    | [] -> ""
    | [s] -> s
    | s :: path -> s ^ "." ^ (string_of_path path) in
  "within " ^ (string_of_path path) ^ ";"

let string_of_import imprt =
  let string_of_import' = match imprt.nature with
    | NewIdentifier (id, expr, _) ->
        id ^ " = " ^ (string_of_expression expr)
    | OldIdentifier (expr, _) ->
        string_of_expression expr
    | AllIdentifiers (expr, _) ->
        (string_of_expression expr) ^ ".*" in
  "import " ^ string_of_import' ^ ";"

let string_of_MultipleAssign exprs expr func_args =
  let rec string_of_LHS exprs = match exprs with
    | [] -> ""
    | [expr] -> string_of_expression expr
    | expr :: exprs ->
        (string_of_expression expr) ^ ", " ^ (string_of_LHS exprs) in
  (string_of_LHS exprs) ^ " := " ^ (string_of_function_call expr func_args)

let string_of_for_clause for_inds algos =
  (string_of_for_inds for_inds) ^ " loop ... end for"

let string_of_while_clause expr algos =
  "while " ^ (string_of_expression expr) ^ " loop ... end while"

let string_of_when_clause alts = match alts with
  | [] -> ""
  | (expr, algos) :: alts ->
      "when " ^ (string_of_expression expr) ^ " then ... end when"

let string_of_if_cond cond =
  "if (" ^ (string_of_expression cond) ^ ") then ... end if"

let string_of_conditional_equ alts algos = match alts with
  | [] -> ""
  | (expr, algos) :: alts -> string_of_if_cond expr

let string_of_algo algo = match algo.nature with
  | Assign (expr, expr') ->
      (string_of_expression expr) ^ " := " ^ (string_of_expression expr')
  | FunctionCallA (expr, func_args) ->
      string_of_function_call expr func_args
  | MultipleAssign (exprs, expr, func_args) ->
      string_of_MultipleAssign exprs expr func_args
  | Break -> "break"
  | Return -> "return"
  | ConditionalEquationA (alts, algos) ->
      string_of_conditional_equ alts algos
  | ForClauseA (for_inds, algos) ->
      string_of_for_clause for_inds algos
  | WhileClause (expr, algos) ->
      string_of_while_clause expr algos
  | WhenClauseA alts ->
      string_of_when_clause alts

let string_of_toplevel_element node = match node.nature with
  | ClassDefinitions cl_defs -> string_of_classDefinitions cl_defs
  | Expression expr -> string_of_expression expr
  | VariablesDefinitions (expr, subs, cpnt_decls) ->
      string_of_expression expr
  | Command algo -> string_of_algo algo
  | Within path -> string_of_within path
  | Import imprt -> string_of_import imprt

let string_of_base_prefix base_pref =
  let string_of_flow flow_option = match flow_option with
    | None -> ""
    | Some _ -> "flow " in
  let string_of_var var_option = match var_option with
    | None -> ""
    | Some Discrete -> "discrete "
    | Some Parameter -> "parameter "
    | Some Constant -> "constant " in
  let string_of_inout inout_option = match inout_option with
    | None -> ""
    | Some Input -> "input "
    | Some Output -> "output " in
  match base_pref.nature with
    | TypePrefix (flow_option, var_option, inout_option) ->
        (string_of_flow flow_option) ^
        (string_of_var var_option) ^
        (string_of_inout inout_option)
