/*
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
 */

/*
 * Parser
 * Grammar for the Modelica language v.2.0
 * V 1.0
 * S. FURIC
 */

%{

open ParseTree

%}

/*names*/
%token <string> IDENT

/*literals*/
%token <string> UNSIGNED_INTEGER UNSIGNED_NUMBER STRING

/*keywords*/
%token ALGORITHM AND ANNOTATION ASSERT BLOCK CLASS CONNECT CONNECTOR CONSTANT
%token DISCRETE EACH ELSE ELSEIF ELSEWHEN ENCAPSULATED END ENUMERATION EQUATION
%token EXTENDS EXTERNAL FALSE FINAL FLOW FOR FUNCTION IF IMPORT IN INITIAL
%token INNER INPUT LOOP MODEL NOT OR OUTER OUTPUT PACKAGE PARAMETER PARTIAL
%token PROTECTED PUBLIC RECORD REDECLARE REPLACEABLE TERMINATE THEN TRUE TYPE
%token WHEN WHILE WITHIN

/*symbols*/
%token LP RP LSB RSB LCB RCB  /*  ( ) [ ] { }  */
%token DOT CM SC CL /*  . , ; :  */
%token PLUS MINUS STAR SLASH EXP /*  + - * / ^  */
%token EQ COLEQ /*  = :=  */
%token LT GT LE GE EE NE /*  < > <= >= == <>  */

/*end of file*/
%token EOF

%type <ParseTree.t> stored_definition_eof
%start stored_definition_eof

%%

/*(0.0)*/
stored_definition_eof
    : stored_definition EOF                                     { $1 }
    ;

/*(2.2.1)*/
stored_definition
    : class_definitions                                         { StoredDefinition (NotWithin, List.rev $1) }
    | WITHIN SC class_definitions                               { StoredDefinition (Within None, List.rev $3) }
    | WITHIN name SC class_definitions                          { StoredDefinition (Within (Some $2), List.rev $4) }
    ;

class_definitions
    :                                                           { [] }
    | class_definitions class_definition SC                     { Definition (NotFinal, $2) :: $1 }
    | class_definitions FINAL class_definition SC               { Definition (Final, $3) :: $1 }
    ;

/*(2.2.2)*/
class_definition
    : encapsulated_option partial_option CLASS
      IDENT class_specifier                                     { ClassDefinition (Class, $4, $1, $2, $5) }
    | encapsulated_option partial_option MODEL
      IDENT class_specifier                                     { ClassDefinition (Model, $4, $1, $2, $5) }
    | encapsulated_option partial_option RECORD
      IDENT class_specifier                                     { ClassDefinition (Record, $4, $1, $2, $5) }
    | encapsulated_option partial_option CONNECTOR
      IDENT class_specifier                                     { ClassDefinition (Connector, $4, $1, $2, $5) }
    | encapsulated_option partial_option TYPE
      IDENT class_specifier                                     { ClassDefinition (Type, $4, $1, $2, $5) }
    | encapsulated_option partial_option PACKAGE
      IDENT class_specifier                                     { ClassDefinition (Package, $4, $1, $2, $5) }
    | encapsulated_option partial_option FUNCTION
      IDENT class_specifier                                     { ClassDefinition (Function, $4, $1, $2, $5) }
    ;

encapsulated_option
    :                                                           { NotEncapsulated }
    | ENCAPSULATED                                              { Encapsulated }
    ;

partial_option
    :                                                           { NotPartial }
    | PARTIAL                                                   { Partial }
    ;

class_specifier
    : string_comment composition END IDENT                      { Specifier ($1, $2, $4) }
    | EQ base_prefix name array_subscripts_option class_modification_option
      comment                                                   { ShortSpecifier ($2, $3, $4, $5, $6) }
    | EQ ENUMERATION LP enum_list_option RP comment             { Enumeration ($4, $6) }
    ;

base_prefix
    : type_prefix                                               { $1 }
    ;

enum_list_option
    :                                                           { [] }
    | enum_list                                                 { List.rev $1 }
    ;

enum_list
    : enumeration_literal                                       { [$1] }
    | enum_list SC enumeration_literal                          { $3 :: $1 }
    ;

enumeration_literal
    : IDENT comment                                             { EnumerationLiteral ($1, $2) }
    ;

composition
    : element_list other_lists external_option                  { Composition ($1, List.rev $2, $3) }
    ;

other_lists
    :                                                           { [] }
    | other_lists PUBLIC element_list                           { Public $3 :: $1 }
    | other_lists PROTECTED element_list                        { Protected $3 :: $1 }
    | other_lists equation_clause                               { EquationClauseElement $2 :: $1 }
    | other_lists algorithm_clause                              { AlgorithmClauseElement $2 :: $1 }
    ;

external_option
    :                                                           { None }
    | EXTERNAL language_specification_option
               external_function_call_option SC annotation_option
                                                                { Some (External ($2, $3, $5)) }
    ;

annotation_option
    :                                                           { None }
    | annotation SC                                             { Some $1 }
    ;

language_specification_option
    :                                                           { None }
    | STRING                                                    { Some $1 }
    ;

external_function_call_option
    :                                                           { None }
    | IDENT LP expressions RP                                   { Some (ExternalFunctionCall (None, $1, $3)) }
    | component_reference EQ IDENT LP expressions RP            { Some (ExternalFunctionCall (Some $1, $3, $5)) }
    ;


expressions
    :                                                           { [] }
    | expression CM expressions                                 { $1 :: $3 }
    ;

array_subscripts_option
    :                                                           { [||] }
    | array_subscripts                                          { Array.of_list $1 }
    ;

class_modification_option
    :                                                           { None }
    | class_modification                                        { Some $1 }
    ;

element_list
    :                                                           { [] }
    | element_list IMPORT import_clause SC                      { ImportClause $3 :: $1 }
    | element_list EXTENDS extends_clause SC                    { ExtendsClause $3 :: $1 }
    | element_list final_option dynamic_scope_option
        class_definition SC                                     { ClassDefinitionElement ($4, $2, $3) :: $1 }
    | element_list final_option dynamic_scope_option
        component_clause SC                                     { ComponentClauseElement ($4, $2, $3) :: $1 }
    | element_list final_option dynamic_scope_option
        REPLACEABLE class_definition
        constraining_clause_option SC                           { ReplaceableClassDefinition ($5, $6, $2, $3) :: $1 }
    | element_list final_option dynamic_scope_option
        REPLACEABLE component_clause
        constraining_clause_option SC                           { ReplaceableComponentClause ($5, $6, $2, $3) :: $1 }
    | element_list annotation SC                                { AnnotationElement $2 :: $1 }
    ;

final_option
    :                                                           { NotFinal }
    | FINAL                                                     { Final }
    ;

dynamic_scope_option
    :                                                           { NoDynamicScope }
    | INNER                                                     { Inner }
    | OUTER                                                     { Outer }
    ;

import_clause
    : IDENT EQ name comment                                     { NewIdentifier ($1, $3, $4) }
    | name comment                                              { Identifier ($1, $2) }
    | name DOT STAR comment                                     { AllIdentifiers ($1, $4) }
    ;

constraining_clause_option
    :                                                           { None }
    | constraining_clause comment                               { Some ($1, $2) }
    ;

/*(2.2.3)*/
extends_clause
    : name class_modification_option                            { ($1, $2) }
    ;

constraining_clause
    : extends_clause                                            { $1 }
    ;

/*(2.2.4)*/
component_clause
    : type_prefix type_specifier array_subscripts_option
        component_list                                          { ComponentClause ($1, $2, $3, List.rev $4) }
    ;

type_prefix
    : flow_option variability_option inout_option               { TypePrefix ($1, $2, $3) }
    ;

flow_option
    :                                                           { None }
    | FLOW                                                      { Some Flow }
    ;

variability_option
    :                                                           { None }
    | DISCRETE                                                  { Some Discrete }
    | PARAMETER                                                 { Some Parameter }
    | CONSTANT                                                  { Some Constant }
    ;

inout_option
    :                                                           { None }
    | INPUT                                                     { Some Input }
    | OUTPUT                                                    { Some Output }
    ;

type_specifier
    : name                                                      { $1 }
    ;

component_list
    : component_declaration                                     { [$1] }
    | component_list CM component_declaration                   { $3 :: $1 }
    ;

component_declaration
    : declaration comment                                       { ComponentDeclaration ($1, $2) }
    ;

declaration
    : IDENT array_subscripts_option modification_option         { ($1, $2, $3) }
    ;

modification_option
    :                                                           { None }
    | modification                                              { Some $1 }
    ;

/*(2.2.5)*/
modification
    : class_modification EQ expression                          { Modification ($1, Some $3) }
    | class_modification                                        { Modification ($1, None) }
    | EQ expression                                             { Eq $2 }
    | COLEQ expression                                          { ColEq $2 }
    ;

class_modification
    : LP argument_list RP                                       { ClassModification (List.rev $2) }
    ;

argument_list
    : argument                                                  { [$1] }
    | argument CM argument_list                                 { $1 :: $3 }
    ;

argument
    : element_modification                                      { $1 }
    | element_redeclaration                                     { $1 }
    ;

element_modification
    : component_reference modification string_comment           { ElementModification (NotEach, NotFinal,$1, $2, $3) }
    | EACH component_reference modification string_comment      { ElementModification (Each, NotFinal,$2, $3, $4) }
    | FINAL component_reference modification string_comment     { ElementModification (NotEach, Final,$2, $3, $4) }
    | EACH FINAL
      component_reference modification string_comment           { ElementModification (Each, Final,$3, $4, $5) }
    ;

element_redeclaration
    : REDECLARE  class_definition_or_component_clause1          { ElementRedeclaration (NotEach, NotFinal, $2) }
    | REDECLARE EACH class_definition_or_component_clause1      { ElementRedeclaration (Each, NotFinal, $3) }
    | REDECLARE FINAL class_definition_or_component_clause1     { ElementRedeclaration (NotEach, Final, $3) }
    | REDECLARE EACH FINAL
      class_definition_or_component_clause1                     { ElementRedeclaration (Each, Final, $4) }
    ;

class_definition_or_component_clause1
    : class_definition                                          { Redeclaration (NotReplaceable, RedeclaredClassDefinition $1, None) }
    | type_prefix type_specifier component_declaration          { Redeclaration (NotReplaceable, RedeclaredComponentClause ($1, $2, $3), None) }
    | REPLACEABLE class_definition constraining_clause_option   { Redeclaration (Replaceable, RedeclaredClassDefinition $2, $3) }
    | REPLACEABLE type_prefix type_specifier
      component_declaration constraining_clause_option          { Redeclaration (Replaceable, RedeclaredComponentClause ($2, $3, $4), $5) }
    ;

/*(2.2.6)*/
equation_clause
    : INITIAL EQUATION equations                                { EquationClause (Initial, List.rev $3) }
    | EQUATION equations                                        { EquationClause (NotInitial, List.rev $2) }
    ;

equations
    :                                                           { [] }
    | equations equation comment SC                             { Equation ($2, $3) :: $1 }
    | equations annotation SC                                   { EquationAnnotation $2 :: $1 }
    ;

algorithm_clause
    : INITIAL ALGORITHM algorithms                              { AlgorithmClause (Initial, List.rev $3) }
    | ALGORITHM algorithms                                      { AlgorithmClause (NotInitial, List.rev $2) }
    ;

algorithms
    :                                                           { [] }
    | algorithms algorithm comment SC                           { Algorithm ($2, $3) :: $1 }
    | algorithms annotation SC                                  { AlgorithmAnnotation $2 :: $1 }
    ;

equation
    : simple_expression EQ expression                           { Equality ($1, $3) }
    | conditional_equation_e                                    { $1 }
    | for_clause_e                                              { $1 }
    | connect_clause                                            { $1 }
    | when_clause_e                                             { $1 }
    | component_reference LP RP                                 { FunctionCallE ($1, None) }
    | component_reference LP function_arguments RP              { FunctionCallE ($1, Some $3) }
    ;

algorithm
    : component_reference COLEQ expression                      { Assignment ($1, $3) }
    | component_reference LP RP                                 { FunctionCallA ($1, None) }
    | component_reference LP function_arguments RP              { FunctionCallA ($1, Some $3) }
    | LP expression_list RP COLEQ
      component_reference LP RP                                 { MultipleAssignment ($2, $5, None) }
    | LP expression_list RP COLEQ
      component_reference LP function_arguments RP              { MultipleAssignment ($2, $5, Some $7) }
    | conditional_equation_a                                    { $1 }
    | for_clause_a                                              { $1 }
    | while_clause                                              { $1 }
    | when_clause_a                                             { $1 }
    ;

conditional_equation_e
    : IF expression THEN
        equations_e
      else_if_expressions_e
      else_option_e
      END IF                                                    { ConditionalEquationE (($2, (List.rev $4)) :: $5, $6) }
    ;

else_if_expressions_e
    :                                                           { [] }
    | ELSEIF expression THEN
        equations_e
      else_if_expressions_e                                     { ($2, (List.rev $4)) :: $5 }
    ;

else_option_e
    :                                                           { [] }
    | ELSE equations_e                                          { List.rev $2 }
    ;

equations_e
    : equation SC                                               { [$1] }
    | equations_e equation SC                                   { $2 :: $1 }
    ;

conditional_equation_a
    : IF expression THEN
        algorithms_a
      else_if_expressions_a
      else_option_a
      END IF                                                    { ConditionalEquationA (($2, (List.rev $4)) :: $5, $6) }
    ;

else_if_expressions_a
    :                                                           { [] }
    | ELSEIF expression THEN
        algorithms_a
      else_if_expressions_a                                     { ($2, (List.rev $4)) :: $5 }
    ;

else_option_a
    :                                                           { [] }
    | ELSE algorithms_a                                         { List.rev $2 }
    ;

algorithms_a
    : algorithm SC                                              { [$1] }
    | algorithms_a algorithm SC                                 { $2 :: $1 }
    ;

for_clause_e
    : FOR for_indices LOOP
        equations_e
      END FOR                                                   { ForClauseE ($2, List.rev $4) }
    ;

for_clause_a
    : FOR for_indices LOOP
        algorithms_a
      END FOR                                                   { ForClauseA ($2, List.rev $4) }
    ;

for_indices
    : for_index                                                 { [$1] }
    | for_index CM for_indices                                  { $1 :: $3 }
    ;

for_index
    : IDENT in_expression_option                                { ($1, $2) }
    ;

in_expression_option
    :                                                           { None }
    | IN expression                                             { Some $2 }
    ;

while_clause
    : WHILE expression LOOP
        algorithms_a
      END WHILE                                                 { WhileClause ($2, List.rev $4) }
    ;

when_clause_e
    : WHEN expression THEN
        equations_e
      else_when_expressions_e
      END WHEN                                                  { WhenClauseE (($2, $4) :: $5) }
    ;

when_clause_a
    : WHEN expression THEN
        algorithms_a
      else_when_expressions_a
      END WHEN                                                  { WhenClauseA (($2, $4) :: $5) }
    ;

else_when_expressions_e
    :                                                           { [] }
    | ELSEWHEN expression THEN
        equations_e
      else_when_expressions_e                                   { ($2, $4) :: $5 }
    ;

else_when_expressions_a
    :                                                           { [] }
    | ELSEWHEN expression THEN
        algorithms_a
      else_when_expressions_a                                   { ($2, $4) :: $5 }
    ;

connect_clause
    : CONNECT LP connector_ref CM connector_ref RP              { ConnectClause ($3, $5) }
    ;

connector_ref
    : IDENT array_subscripts_option                             { [($1, $2)] }
    | IDENT array_subscripts_option
        DOT IDENT array_subscripts_option                       { [($1, $2); ($4, $5)] }
    ;

/*(2.2.7)*/
expression
    : simple_expression                                         { $1 }
    | IF expression THEN expression
      elseifs_option
      ELSE expression                                           { If (($2, $4) :: $5, $7) }
    ;

elseifs_option
    :                                                           { [] }
    | ELSEIF expression THEN expression
      elseifs_option                                            { ($2, $4) :: $5 }
    ;

simple_expression
    : logical_expression                                        { $1 }
    | logical_expression CL logical_expression                  { Range ($1, $3, None) }
    | logical_expression
        CL logical_expression CL logical_expression             { Range ($1, $3, Some $5) }
    ;

logical_expression
    : logical_term                                              { $1 }
    | logical_expression OR logical_term                        { Or ($1, $3) }
    ;

logical_term
    : logical_factor                                            { $1 }
    | logical_term AND logical_factor                           { And ($1, $3) }
    ;

logical_factor
    : relation                                                  { $1 }
    | NOT relation                                              { Not $2 }
    ;

relation
    : arithmetic_expression                                     { $1 }
    | arithmetic_expression LT arithmetic_expression            { LessThan ($1, $3) }
    | arithmetic_expression GT arithmetic_expression            { GreaterThan ($1, $3) }
    | arithmetic_expression LE arithmetic_expression            { LessEqualThan ($1, $3) }
    | arithmetic_expression GE arithmetic_expression            { GreaterEqualThan ($1, $3) }
    | arithmetic_expression EE arithmetic_expression            { Equals ($1, $3) }
    | arithmetic_expression NE arithmetic_expression            { NotEquals ($1, $3) }
    ;

arithmetic_expression
    : signed_term                                               { $1 }
    | arithmetic_expression PLUS term                           { Addition ($1, $3) }
    | arithmetic_expression MINUS term                          { Subtraction ($1, $3) }
    ;

signed_term
    : term                                                      { $1 }
    | PLUS term                                                 { Plus $2 }
    | MINUS term                                                { Minus $2 }
    ;

term
    : factor                                                    { $1 }
    | term STAR factor                                          { Multiplication ($1, $3) }
    | term SLASH factor                                         { Division ($1, $3) }
    ;

factor
    : primary                                                   { $1 }
    | primary EXP primary                                       { Power ($1, $3) }
    ;

primary
    : UNSIGNED_INTEGER                                          { Integer $1 }
    | UNSIGNED_NUMBER                                           { Real $1 }
    | STRING                                                    { String $1 }
    | FALSE                                                     { False }
    | TRUE                                                      { True }
    | component_reference                                       { Reference $1 }
    | component_reference LP RP                                 { FunctionCall ($1, None) }
    | component_reference LP function_arguments RP              { FunctionCall ($1, Some $3) }
    | LP expression_list RP                                     { ExpressionList (Array.of_list $2) }
    | LSB expression_lists RSB                                  { ArrayConcatenation $2 }
    | LCB function_arguments RCB                                { VectorOrRecord $2 }
    ;

expression_lists
    : expression_list                                           { [$1] }
    | expression_list SC expression_lists                       { $1 :: $3 }
    ;

name
    : IDENT                                                     { [$1] }
    | name DOT IDENT                                            { $1 @ [$3] }
    ;

component_reference
    : IDENT array_subscripts_option                             { [$1, $2] }
    | IDENT array_subscripts_option DOT component_reference     { ($1, $2) :: $4 }
    ;

function_arguments
    : expression FOR for_indices                                { ArgList ([$1], Some $3) }
    | expression CM expression_list FOR for_indices             { ArgList ($1 :: $3, Some $5) }
    | expression                                                { ArgList ([$1], None) }
    | expression CM expression_list                             { ArgList ($1 :: $3, None) }
    | named_arguments FOR for_indices                           { NamedArgList ($1, Some $3) }
    | named_arguments                                           { NamedArgList ($1, None) }
    ;

named_arguments
    : named_argument                                            { [$1] }
    | named_argument CM named_arguments                         { $1 :: $3 }
    ;

named_argument
    : IDENT EQ expression                                       { ($1, $3) }
    ;

expression_list
    : expression                                                { [$1] }
    | expression CM expression_list                             { $1 :: $3 }
    ;

array_subscripts
    : LSB subscripts RSB                                        { $2 }
    ;

subscripts
    : subscript                                                 { [$1] }
    | subscript CM subscripts                                   { $1 :: $3 }
    ;

subscript
    : CL                                                        { All }
    | expression                                                { Subscript $1 }
    ;

comment
    : string_comment                                            { Comment ($1, None) }
    | string_comment annotation                                 { Comment ($1, Some $2) }
    ;

string_comment
    :                                                           { StringComment [] }
    | strings                                                   { StringComment (List.rev $1) }
    ;

strings
    : STRING                                                    { [$1] }
    | strings PLUS STRING                                       { $3 :: $1 }
    ;

annotation
    : ANNOTATION class_modification                             { Annotation $2 }
    ;

%%

let parse filename token_fun lexbuf =
  try stored_definition_eof token_fun lexbuf with
    | Parsing.Parse_error ->
        let linenum, linebeg =
          Linenum.for_position filename (Lexing.lexeme_start lexbuf) in
        let first_char = Lexing.lexeme_start lexbuf - linebeg in
        Printf.eprintf
          "Syntax error at line %d, characters %d to %d\n"
          linenum
          first_char 
          ((Lexing.lexeme_end lexbuf - Lexing.lexeme_start lexbuf) + first_char);
        raise Parsing.Parse_error

