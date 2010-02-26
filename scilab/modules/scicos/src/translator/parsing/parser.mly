/*
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
 */

/*
 * Parser
 * Grammar for the Modelica language v2.2
 * V 1.0
 * S. FURIC
 */

%{

(** Modelica parser. *)

(** Implementation based on {i Modelica language specification 2.0 } *)

open Syntax

exception Unclosed of int * string * int * string
exception Invalid_matrix of int * int
exception Invalid_array of int * int
exception End_of_file

type location =
  {
    start: int; (* offset in the parsed stream *)
    enddd: int;  (* offset in the parsed stream *)
    filename: filename
  }

and filename =
  | LibraryFile of string
  | CommandLine

type error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: err_ctx
  }

and err_ctx =
  {
    location: location;
  }

exception SyntacticError of error_description

let inputfile = ref CommandLine

let node nature =
  {
    nature = nature;
    info = { start = Parsing.symbol_start ();
             enddd = Parsing.symbol_end ();
             filename = !inputfile }
  }

let rhs_nodes n n' nature =
  {
    nature = nature;
    info = { start = Parsing.rhs_start n;
             enddd = Parsing.rhs_end n';
             filename = !inputfile }
  }

let unclosed opening_symbol opening_pos closing_symbol closing_pos =
  let offset = Parsing.rhs_start opening_pos
  and offset' = Parsing.rhs_start closing_pos in
  raise (Unclosed (offset, opening_symbol, offset', closing_symbol))

let invalid_matrix_construction opening_pos error_pos =
  raise (Invalid_matrix (
    Parsing.rhs_start opening_pos, Parsing.rhs_start error_pos))

let invalid_literal_array_construction opening_pos error_pos =
  raise (Invalid_array (
    Parsing.rhs_start opening_pos, Parsing.rhs_start error_pos))

%}

/*names*/
%token <string> IDENT END_IDENT

/*literals*/
%token <string> UNSIGNED_INTEGER UNSIGNED_REAL STRING

/*keywords*/
%token ALGORITHM AND ANNOTATION BLOCK BREAK CLASS CONNECT CONNECTOR CONSTANT
%token DISCRETE EACH ELSE ELSEIF ELSEWHEN ENCAPSULATED END
%token END_IF END_FOR END_WHEN END_WHILE
%token ENUMERATION EQUATION EXPANDABLE
%token EXTENDS EXTERNAL FALSE FINAL FLOW FOR FUNCTION IF IMPORT IN
%token INITIAL_ALGORITHM INITIAL_EQUATION
%token INNER INPUT LOOP MODEL NOT NOEVENT OR OUTER OUTPUT
%token PACKAGE PARAMETER PARTIAL
%token PROTECTED PUBLIC RECORD REDECLARE REPLACEABLE RESTRICTS RETURN
%token THEN TRUE TYPE WHEN WHILE WITHIN

/*symbols*/
%token LP RP LSB RSB LCB RCB  /*  ( ) [ ] { }  */
%token DOT CM SC CL /*  . , ; :  */
%token PLUS MINUS STAR SLASH EXP /*  + - * / ^  */
%token EQ COLEQ /*  = :=  */
%token LT GT LE GE EE NE /*  < > <= >= == <>  */

/*end of file*/
%token EOF

%type <(location Syntax.toplevel_element_desc, location) Syntax.node> definition
%start definition

%%


/*(2.2.1)*/
definition
  : class_definitions
      { node (ClassDefinitions (List.rev $1)) }
  | toplevel_expression SC
      { $1 }
  | WITHIN SC
      { node (Within []) }
  | WITHIN within_name SC
      { node (Within (List.rev $2)) }
  | import_clause SC
      { node (Import $1) }
  | EOF
      { raise End_of_file }
  ;

class_definitions
  : FINAL class_definition SC
      { [node (ClassDefinition (Some Final, $2))] }
  | class_definition SC
      { [node (ClassDefinition (None, $1))] }
  | class_definitions FINAL class_definition SC
      { node (ClassDefinition (Some Final, $3)) :: $1 }
  | class_definitions class_definition SC
      { node (ClassDefinition (None, $2)) :: $1 }

toplevel_expression
  : expression
      { node (Expression $1) }
  | component_reference component_list
      { match $1.nature with
          | IndexedAccess (type_spec_node, subscripts_node) ->
              node (VariablesDefinitions (type_spec_node, Some subscripts_node, (List.rev $2)))
          | type_spec ->
              node (VariablesDefinitions (rhs_nodes 1 1 type_spec, None, (List.rev $2))) }
  | component_reference COLEQ expression
      { node (Command (node (Assign ($1, $3)))) }
  | LP expression_list RP COLEQ component_reference LP RP
      { node (Command (node (MultipleAssign ($2, $5, None)))) }
  | LP expression_list RP COLEQ component_reference LP function_arguments RP
      { node (Command (node (MultipleAssign ($2, $5, Some $7)))) }
  | LP expression_list RP COLEQ component_reference LP function_arguments
    error
      { unclosed "(" 6 ")" 8 }
  | LP expression_list RP COLEQ component_reference LP error
      { unclosed "(" 6 ")" 7 }
  | BREAK
      { node (Command (node Break)) }
  | RETURN
      { node (Command (node Return)) }
  | conditional_equation_a
      { node (Command (node $1)) }
  | for_clause_a
      { node (Command (node $1)) }
  | while_clause
      { node (Command (node $1)) }
  | when_clause_a
      { node (Command (node $1)) }
  ;

within_name
  : IDENT
      { [$1] }
  | within_name DOT IDENT
      { $3 :: $1 }
  ;

/*(2.2.2)*/
class_definition
  : encapsulated_option partial_option class_type class_specifier
      { node (Definition ($1, $2, $3, $4)) }
  ;

class_type
  : CLASS
      { Class }
  | MODEL
      { Model }
  | BLOCK
      { Block }
  | RECORD
      { Record }
  | EXPANDABLE CONNECTOR
      { ExpandableConnector }
  | CONNECTOR
      { Connector }
  | TYPE
      { Type }
  | PACKAGE
      { Package }
  | FUNCTION
      { Function }
  ;

encapsulated_option
  :
      { None }
  | ENCAPSULATED
      { Some Encapsulated }
  ;

partial_option
  :
      { None }
  | PARTIAL
      { Some Partial }
  ;

class_specifier
  : IDENT string_comment composition END_IDENT
      { if $1 <> $4 then unclosed $1 1 $1 4
        else node (LongSpecifier ($1, $2, $3)) }
  | IDENT EQ base_prefix name array_subscripts_option class_modification_option
    comment
      { node (ShortSpecifier ($1, $3, $4, $5, $6, $7)) }
  | IDENT EQ ENUMERATION LP enum_composition_option RP comment
      { node (EnumerationSpecifier ($1, rhs_nodes 5 5 $5, $7)) }
  | IDENT EQ ENUMERATION LP enum_composition_option error
      { unclosed "(" 4 ")" 6 }
  | EXTENDS IDENT class_modification_option string_comment composition END_IDENT
      { if $2 <> $6 then unclosed $2 2 $2 6
        else node (ExtensionSpecifier ($2, $3, $4, $5)) }
  ;

base_prefix
  : type_prefix
      { $1 }
  ;

enum_composition_option
  :
      { EnumList None }
  | enum_composition
      { $1 }
  ;

enum_composition
  : enum_list
      { EnumList (Some (List.rev $1)) }
  | CL
      { EnumColon}
  ;

enum_list
  : enumeration_literal
      { [$1] }
  | enum_list CM enumeration_literal
      { $3 :: $1 }
  ;

enumeration_literal
  : IDENT comment
      { node (EnumerationLiteral ($1, $2)) }
  ;

composition
  : other_lists external_option
      { node (Composition ([], List.rev $1, $2)) }
  | element_list other_lists external_option
      { node (Composition (List.rev $1, List.rev $2, $3)) }
  ;

other_lists
  :
      { [] }
  | other_lists PUBLIC element_list
      { rhs_nodes 2 3 (Public (List.rev $3)) :: $1 }
  | other_lists PROTECTED element_list
      { rhs_nodes 2 3 (Protected (List.rev $3)) :: $1 }
  | other_lists equation_clause
      { rhs_nodes 2 2 $2 :: $1 }
  | other_lists algorithm_clause
      { rhs_nodes 2 2 $2 :: $1 }
  ;

external_option
  :
      { None }
  | EXTERNAL language_specification_option
    external_function_call_option SC
    annotation_sc_option
      { Some (node (External ($2, $3, None, $5))) }
  | EXTERNAL language_specification_option
    external_function_call_option annotation SC
    annotation_sc_option
      { Some (node (External ($2, $3, Some $4, $6))) }
  ;

annotation_option
  :
      { None }
  | annotation
      { Some $1 }
  ;

annotation_sc_option
  :
      { None }
  | annotation SC
      { Some $1 }
  ;

language_specification_option
  :
      { None }
  | STRING
      { Some $1 }
  ;

external_function_call_option
  :
      { None }
  | IDENT LP RP
      { Some (node (ExternalFunctionCall (None, $1, []))) }
  | IDENT LP expressions RP
      { Some (node (ExternalFunctionCall (None, $1, $3))) }
  | IDENT LP expressions error
      { unclosed "(" 2 ")" 4 }
  | IDENT LP error
      { unclosed "(" 2 ")" 3 }
  | component_reference EQ IDENT LP RP
      { Some (node (ExternalFunctionCall (Some $1, $3, []))) }
  | component_reference EQ IDENT LP expressions RP
      { Some (node (ExternalFunctionCall (Some $1, $3, $5))) }
  | component_reference EQ IDENT LP expressions error
      { unclosed "(" 4 ")" 6 }
  | component_reference EQ IDENT LP error
      { unclosed "(" 4 ")" 5 }
  ;

expressions
  : expression
      { [$1] }
  | expression CM expressions
      { $1 :: $3 }
  ;

array_subscripts_option
  :
      { None }
  | array_subscripts
      { Some $1 }
  ;

class_modification_option
  :
      { None }
  | class_modification
      { Some $1 }
  ;

element_list
  : annotation SC
      { [node (ClassAnnotation $1)] }
  | import_clause SC annotation_sc_option
      { [node (ImportClause ($1, $3))] }
  | extends_clause SC annotation_sc_option
      { [node (ExtendsClause ($1, $3))] }
  | redeclare_option final_option dynamic_scope_option
    element_definition SC annotation_sc_option
      { [node (ElementDefinition ($1, $2, $3, $4, $6))] }
  | element_list import_clause SC annotation_sc_option
      { rhs_nodes 2 3 (ImportClause ($2, $4)) :: $1 }
  | element_list extends_clause SC annotation_sc_option
      { rhs_nodes 2 3 (ExtendsClause ($2, $4)) :: $1 }
  | element_list redeclare_option final_option dynamic_scope_option
    element_definition SC annotation_sc_option
      { rhs_nodes 2 5 (ElementDefinition ($2, $3, $4, $5, $7)) :: $1 }
  ;

element_definition
  : class_definition
      { node (ClassDefinitionElement (None, $1, [])) }
  | component_clause
      { node (ComponentClauseElement (None, $1, [])) }
  | REPLACEABLE class_definition
      { node (ClassDefinitionElement (Some Replaceable, $2, [])) }
  | REPLACEABLE class_definition constraining_clauses
      { node (ClassDefinitionElement (Some Replaceable, $2, List.rev $3)) }
  | REPLACEABLE component_clause
      { node (ComponentClauseElement (Some Replaceable, $2, [])) }
  | REPLACEABLE component_clause constraining_clauses
      { node (ComponentClauseElement (Some Replaceable, $2, List.rev $3)) }
  ;

redeclare_option
  :
      { None }
  | REDECLARE
      { Some Redeclare }
  ;

final_option
  :
      { None }
  | FINAL
      { Some Final }
  ;

dynamic_scope_option
  :
      { None }
  | INNER
      { Some Inner }
  | OUTER
      { Some Outer }
  | INNER OUTER
      { Some InnerOuter }
  ;

import_clause
  : IMPORT IDENT EQ name comment
      { node (NewIdentifier ($2, $4, $5)) }
  | IMPORT name comment
      { node (OldIdentifier ($2, $3)) }
  | IMPORT name DOT STAR comment
      { node (AllIdentifiers ($2, $5)) }
  ;

constraining_clauses
  : constraining_clause
      { [$1] }
  | constraining_clauses constraining_clause
      { $2 :: $1 }
  ;

/*(2.2.3)*/
extends_clause
  : EXTENDS name class_modification_option annotation_option
      { node (Extends ($2, $3, $4)) }
  ;

constraining_clause
  : EXTENDS name class_modification_option comment
      { node (Constraint (Extension, $2, $3, $4)) }
  | RESTRICTS name class_modification_option comment
      { node (Constraint (Restriction, $2, $3, $4)) }
  ;

/*(2.2.4)*/
component_clause
  : type_prefix type_specifier array_subscripts_option component_list
      { node (ComponentClause ($1, $2, $3, List.rev $4)) }
  ;

type_prefix
  : flow_option variability_option inout_option
      { node (TypePrefix ($1, $2, $3)) }
  ;

flow_option
  :
      { None }
  | FLOW
      { Some Flow }
  ;

variability_option
  :
      { None }
  | DISCRETE
      { Some Discrete }
  | PARAMETER
      { Some Parameter }
  | CONSTANT
      { Some Constant }
  ;

inout_option
  :
      { None }
  | INPUT
      { Some Input }
  | OUTPUT
      { Some Output }
  ;

type_specifier
  : name
      { $1 }
  ;

component_list
  : component_declaration
      { [$1] }
  | component_list CM component_declaration
      { $3 :: $1 }
  ;

component_declaration
  : declaration comment
      { node (ComponentDeclaration ($1, $2)) }
  ;

declaration
  : IDENT array_subscripts_option modification_option
      { node (Declaration ($1, $2, $3)) }
  ;

modification_option
  :
      { None }
  | modification
      { Some $1 }
  ;

/*(2.2.5)*/
modification
  : class_modification EQ expression
      { node (Modification ($1, Some $3)) }
  | class_modification
      { node (Modification ($1, None)) }
  | EQ expression
      { node (Eq $2) }
  | COLEQ expression
      { node (ColEq $2) }
  ;

class_modification
  : LP RP
      { node (ClassModification []) }
  | LP argument_list RP
      { node (ClassModification (List.rev $2)) }
  | LP argument_list error
      { unclosed "(" 1 ")" 3 }
  | LP error
      { unclosed "(" 1 ")" 2 }
  ;

argument_list
  : argument
      { [$1] }
  | argument CM argument_list
      { $1 :: $3 }
  ;

argument
  : element_modification
      { $1 }
  | element_redeclaration
      { $1 }
  ;

element_modification
  : each_option final_option component_reference modification_option
    string_comment
      { node (ElementModification ($1, $2, $3, $4, $5)) }
  ;

each_option
  :
      { None }
  | EACH
      { Some Each }
  ;

element_redeclaration
  : REDECLARE each_option final_option class_definition_or_component_clause1
      { node (ElementRedeclaration ($2, $3, $4)) }
  ;

class_definition_or_component_clause1
  : class_definition
      { node (ClassDefinitionElement (None, $1, [])) }
  | type_prefix type_specifier component_declaration
      { let cpnt = node (ComponentClause ($1, $2, None, [$3])) in
        node (ComponentClauseElement (None, cpnt, [])) }
  | REPLACEABLE class_definition
      { node (ClassDefinitionElement (Some Replaceable, $2, [])) }
  | REPLACEABLE class_definition constraining_clauses
      { node (ClassDefinitionElement (Some Replaceable, $2, List.rev $3)) }
  | REPLACEABLE type_prefix type_specifier
    component_declaration
      { let cpnt = node (ComponentClause ($2, $3, None, [$4])) in
        node (ComponentClauseElement (Some Replaceable, cpnt, [])) }
  | REPLACEABLE type_prefix type_specifier
    component_declaration constraining_clauses
      { let cpnt = node (ComponentClause ($2, $3, None, [$4])) in
        node (ComponentClauseElement (Some Replaceable, cpnt, List.rev $5)) }
  ;

/*(2.2.6)*/
equation_clause
  : INITIAL_EQUATION
      { EquationClause (Some Initial, []) }
  | INITIAL_EQUATION equations
      { EquationClause (Some Initial, List.rev $2) }
  | EQUATION
      { EquationClause (None, []) }
  | EQUATION equations
      { EquationClause (None, List.rev $2) }  ;

equations
  : equation comment SC annotation_sc_option
      { [node (Equation (rhs_nodes 1 3 $1, $2, $4))] }
  | equations equation comment SC annotation_sc_option
      { rhs_nodes 2 5 (Equation (rhs_nodes 2 4 $2, $3, $5)) :: $1 }
  ;

algorithm_clause
  : INITIAL_ALGORITHM
      { AlgorithmClause (Some Initial, []) }
  | INITIAL_ALGORITHM algorithms
      { AlgorithmClause (Some Initial, List.rev $2) }
  | ALGORITHM
      { AlgorithmClause (None, []) }
  | ALGORITHM algorithms
      { AlgorithmClause (None, List.rev $2) }
  ;

algorithms
  : algorithm comment SC annotation_sc_option
      { [node (Algorithm (rhs_nodes 1 3 $1, $2, $4))] }
  | algorithms algorithm comment SC annotation_sc_option
      { rhs_nodes 2 5 (Algorithm (rhs_nodes 2 4 $2, $3, $5)) :: $1 }
  ;

equation
  : simple_expression EQ expression
      { Equal ($1, $3) }
  | conditional_equation_e
      { $1 }
  | for_clause_e
      { $1 }
  | connect_clause
      { $1 }
  | when_clause_e
      { $1 }
  | component_reference LP RP
      { FunctionCallE ($1, None) }
  | component_reference LP function_arguments RP
      { FunctionCallE ($1, Some $3) }
  | component_reference LP function_arguments error
      { unclosed "(" 2 ")" 4 }
  | component_reference LP error
      { unclosed "(" 2 ")" 3 }
;

algorithm
  : component_reference COLEQ expression
      { Assign ($1, $3) }
  | component_reference LP RP
      { FunctionCallA ($1, None) }
  | component_reference LP function_arguments RP
      { FunctionCallA ($1, Some $3) }
  | component_reference LP function_arguments error
      { unclosed "(" 2 ")" 4 }
  | component_reference LP error
      { unclosed "(" 2 ")" 3 }
  | LP expression_list RP COLEQ component_reference LP RP
      { MultipleAssign ($2, $5, None) }
  | LP RP COLEQ component_reference LP RP
      { MultipleAssign ([], $4, None) }
  | LP expression_list RP COLEQ component_reference LP function_arguments RP
      { MultipleAssign ($2, $5, Some $7) }
  | LP RP COLEQ component_reference LP function_arguments RP
      { MultipleAssign ([], $4, Some $6) }
  | LP expression_list RP COLEQ component_reference LP function_arguments
    error
      { unclosed "(" 6 ")" 8 }
  | LP RP COLEQ component_reference LP function_arguments error
      { unclosed "(" 5 ")" 7 }
  | LP expression_list RP COLEQ component_reference LP error
      { unclosed "(" 6 ")" 7 }
  | LP RP COLEQ component_reference LP error
      { unclosed "(" 5 ")" 6 }
  | LP expression_list error
      { unclosed "(" 1 ")" 3 }
  | LP error
      { unclosed "(" 1 ")" 2 }
  | BREAK
      { Break }
  | RETURN
      { Return }
  | conditional_equation_a
      { $1 }
  | for_clause_a
      { $1 }
  | while_clause
      { $1 }
  | when_clause_a
      { $1 }
  ;

conditional_equation_e
  : IF expression THEN
      equations_e
    else_if_expressions_e
    else_option_e
    END_IF
      { ConditionalEquationE (($2, List.rev $4) :: $5, $6) }
  ;

else_if_expressions_e
  :
      { [] }
  | ELSEIF expression THEN
      equations_e
    else_if_expressions_e
      { ($2, List.rev $4) :: $5 }
  ;

else_option_e
  :
      { None }
  | ELSE equations_e
      { Some (List.rev $2) }
  ;

equations_e
  : equation SC
      { [node $1] }
  | equations_e equation SC
      { rhs_nodes 2 3 $2 :: $1 }
  ;

conditional_equation_a
  : IF expression THEN
      algorithms_a
    else_if_expressions_a
    else_option_a
    END_IF
      { ConditionalEquationA (($2, List.rev $4) :: $5, $6) }
  ;

else_if_expressions_a
  :
      { [] }
  | ELSEIF expression THEN
      algorithms_a
    else_if_expressions_a
      { ($2, List.rev $4) :: $5 }
  ;

else_option_a
  :
      { None }
  | ELSE algorithms_a
      { Some (List.rev $2) }
  ;

algorithms_a
  : algorithm SC
      { [node $1] }
  | algorithms_a algorithm SC
      { rhs_nodes 2 3 $2 :: $1 }
  ;

for_clause_e
  : FOR for_indices LOOP
      equations_e
    END_FOR
      { ForClauseE ($2, List.rev $4) }
  ;

for_clause_a
  : FOR for_indices LOOP
      algorithms_a
    END_FOR
      { ForClauseA ($2, List.rev $4) }
  ;

for_indices
  : for_index
      { [$1] }
  | for_index CM for_indices
      { $1 :: $3 }
  ;

for_index
  : IDENT
      { ($1, None) }
  | IDENT IN expression
      { ($1, Some $3) }
  ;

while_clause
  : WHILE expression LOOP
      algorithms_a
    END_WHILE
      { WhileClause ($2, List.rev $4) }
  ;

when_clause_e
  : WHEN expression THEN
      equations_e
    else_when_expressions_e
    END_WHEN
      { WhenClauseE (($2, List.rev $4) :: $5) }
  ;

when_clause_a
  : WHEN expression THEN
      algorithms_a
    else_when_expressions_a
    END_WHEN
      { WhenClauseA (($2, List.rev $4) :: $5) }
  ;

else_when_expressions_e
  :
      { [] }
  | ELSEWHEN expression THEN
      equations_e
    else_when_expressions_e
      { ($2, List.rev $4) :: $5 }
  ;

else_when_expressions_a
  :
      { [] }
  | ELSEWHEN expression THEN
      algorithms_a
    else_when_expressions_a
      { ($2, List.rev $4) :: $5 }
  ;

connect_clause
  : CONNECT LP component_reference CM component_reference RP
      { ConnectClause ($3, $5) }
  | CONNECT LP component_reference CM component_reference error
      { unclosed "(" 2 ")" 6 }
  ;

/*(2.2.7)*/
expression
  : simple_expression
      { $1 }
  | IF expression THEN expression
    elseifs_option
    ELSE expression
      { node (If (($2, $4) :: $5, $7)) }
  ;

elseifs_option
  :
      { [] }
  | ELSEIF expression THEN expression
    elseifs_option
      { ($2, $4) :: $5 }
  ;

simple_expression
  : logical_expression
      { $1 }
  | logical_expression CL logical_expression
      { node (Range ($1, None, $3)) }
  | logical_expression CL logical_expression CL logical_expression
      { node (Range ($1, Some $3, $5)) }
  ;

logical_expression
  : logical_term
      { $1 }
  | logical_expression OR logical_term
      { node (BinaryOperation (rhs_nodes 2 2 Or, $1, $3)) }
  ;

logical_term
  : logical_factor
      { $1 }
  | logical_term AND logical_factor
      { node (BinaryOperation (rhs_nodes 2 2 And, $1, $3)) }
  ;

logical_factor
  : relation
      { $1 }
  | NOT relation
      { node (UnaryOperation (rhs_nodes 1 1 Not, $2)) }
  ;

relation
  : arithmetic_expression
      { $1 }
  | arithmetic_expression LT arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 Less, $1, $3)) }
  | arithmetic_expression GT arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 Greater, $1, $3)) }
  | arithmetic_expression LE arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 LessEqual, $1, $3)) }
  | arithmetic_expression GE arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 GreaterEqual, $1, $3)) }
  | arithmetic_expression EE arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 EqualEqual, $1, $3)) }
  | arithmetic_expression NE arithmetic_expression
      { node (BinaryOperation (rhs_nodes 2 2 NotEqual, $1, $3)) }
  ;

arithmetic_expression
  : term
      { $1 }
  | arithmetic_expression PLUS term
      { node (BinaryOperation (rhs_nodes 2 2 Plus, $1, $3)) }
  | arithmetic_expression MINUS term
      { node (BinaryOperation (rhs_nodes 2 2 Minus, $1, $3)) }
  ;

term
  : unary_factor
      { $1 }
  | term STAR unary_factor
      { node (BinaryOperation (rhs_nodes 2 2 Times, $1, $3)) }
  | term SLASH unary_factor
      { node (BinaryOperation (rhs_nodes 2 2 Divide, $1, $3)) }
  ;

unary_factor
  : factor
      { $1 }
  | PLUS unary_factor
      { node (UnaryOperation (rhs_nodes 1 1 UnaryPlus, $2)) }
  | MINUS unary_factor
      { node (UnaryOperation (rhs_nodes 1 1 UnaryMinus, $2)) }

factor
  : primary
      { $1 }
  | factor EXP primary
      { node (BinaryOperation (rhs_nodes 2 2 Power, $1, $3)) }
  ;

primary
  : UNSIGNED_INTEGER
      { node (Integer $1) }
  | UNSIGNED_REAL
      { node (Real $1) }
  | STRING
      { node (String $1) }
  | FALSE
      { node False }
  | TRUE
      { node True }
  | NOEVENT LP expression RP
      { node (NoEvent $3) }
  | component_reference
      { $1 }
  | component_reference LP RP
      { node (FunctionCall ($1, None)) }
  | component_reference LP function_arguments RP
      { node (FunctionCall ($1, Some $3)) }
  | component_reference LP function_arguments error
      { unclosed "(" 2 ")" 4 }
  | component_reference LP error
      { unclosed "(" 2 ")" 3 }
  | LP expression_list RP
      { match $2 with
          | [expr] -> node expr.Syntax.nature
          | _ -> node (Tuple $2) }
  | LP expression_list error
      { unclosed "(" 1 ")" 3 }
  | LSB expression_lists RSB
      { node (MatrixConstruction $2) }
  | LSB error
      { invalid_matrix_construction 1 2 }
  | LCB vector_elements RCB
      { node (Vector $2) }
  | LCB error
      { invalid_literal_array_construction 1 2 }
  | END
      { node End }
  ;

expression_lists
  : expression_list
      { [$1] }
  | expression_list SC expression_lists
      { $1 :: $3 }
  ;

vector_elements
  : expression FOR for_indices
      { node (VectorReduction ($1, $3)) }
  | expression_list
      { node (VectorElements $1) }
  ;

name
  : IDENT
      { node (Identifier $1) }
  | name DOT IDENT
      { node (FieldAccess ($1, $3)) }
  ;

component_reference
  : IDENT
      { node (Identifier $1) }
  | IDENT array_subscripts
      { node (IndexedAccess (rhs_nodes 1 1 (Identifier $1), $2)) }
  | component_reference DOT IDENT
      { node (FieldAccess ($1, $3)) }
  | component_reference DOT IDENT array_subscripts
      { node (IndexedAccess (rhs_nodes 1 3 (FieldAccess ($1, $3)), $4)) }
  ;

function_arguments
  : function_arguments_elements
      { node (ArgumentList (List.rev $1)) }
  | expression FOR for_indices
      { node (Reduction ($1, $3)) }
  ;

function_arguments_elements
  : expression
      { [node (Argument $1)] }
  | named_argument
      { [$1] }
  | function_arguments_elements CM expression
      { node (Argument $3) :: $1 }
  | function_arguments_elements CM named_argument
      { $3 :: $1 }
  ;

named_argument
  : IDENT EQ expression
      { node (NamedArgument ($1, $3)) }
  | FUNCTION IDENT
      { failwith "Not yet implemented" }
  | FUNCTION IDENT LP RP
      { failwith "Not yet implemented" }
  | FUNCTION IDENT LP function_arguments RP
      { failwith "Not yet implemented" }
  | FUNCTION IDENT LP function_arguments error
      { unclosed  "(" 3 ")" 5 }
  | FUNCTION IDENT LP error
      { unclosed  "(" 3 ")" 4 }
  ;

expression_list
  : expression
      { [$1] }
  | expression CM expression_list
      { $1 :: $3 }
  ;

array_subscripts
  : LSB subscripts RSB
      { node (Subscripts $2) }
  ;

subscripts
  : subscript
      { [$1] }
  | subscript CM subscripts
      { $1 :: $3 }
  ;

subscript
  : CL
      { node Colon }
  | expression
      { node (Subscript $1) }
  ;

comment
  : string_comment annotation_option
      { node (Comment ($1, $2)) }
  ;

string_comment
  :
      { [] }
  | strings
      { List.rev $1 }
  ;

strings
  : STRING
      { [$1] }
  | strings PLUS STRING
      { $3 :: $1 }
  ;

annotation
  : ANNOTATION class_modification
      { node (Annotation $2) }
  ;

%%

let parse filename token_fun lexbuf =
  inputfile := filename;
  try
    definition token_fun lexbuf
  with
    | Unclosed (pos, symbol, pos', symbol') ->
        raise (SyntacticError
          {err_msg = ["_Unclosed"; symbol];
           err_info = [];
           err_ctx =
             {location = {start = pos;
                          enddd = pos';
                          filename = filename}}})
    | Invalid_matrix (pos, pos') ->
        raise (SyntacticError
          {err_msg = ["_InvalidMatrixConstruct"];
           err_info = [];
           err_ctx =
             {location = {start = pos;
                          enddd = pos';
                          filename = filename}}})
    | Invalid_array (pos, pos') ->
        raise (SyntacticError
          {err_msg = ["_InvalidArrayConstruct"];
           err_info = [];
           err_ctx =
             {location = {start = pos;
                          enddd = pos';
                          filename = filename}}})
    | Parsing.Parse_error ->
        raise (SyntacticError
          {err_msg = ["_SyntaxError"];
           err_info = [];
           err_ctx =
             {location = {start = Lexing.lexeme_start lexbuf;
                          enddd = Lexing.lexeme_end lexbuf;
                          filename = filename}}})
