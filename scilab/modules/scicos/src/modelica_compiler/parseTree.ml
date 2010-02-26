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

type t =
  | StoredDefinition of within * definition list

and within =
  | Within of name option
  | NotWithin

and definition =
  | Definition of final * class_definition

and final =
  | Final
  | NotFinal

and class_definition =
  | ClassDefinition of class_kind * ident * encapsulated * partial *
      class_specifier

and class_kind =
  | Class
  | Model
  | Record
  | Connector
  | Type
  | Package
  | Function

and encapsulated =
  | Encapsulated
  | NotEncapsulated

and partial =
  | Partial
  | NotPartial

and class_specifier =
  | Specifier of string_comment * composition * ident
  | ShortSpecifier of base_prefix * name * array_subscripts *
      class_modification option * comment
  | Enumeration of enumeration_literal list * comment

and base_prefix = type_prefix

and enumeration_literal =
  | EnumerationLiteral of ident * comment

and composition =
  | Composition of element list * other_elements list * externalll option

and element =
  | AnnotationElement of annotation
  | ImportClause of import_clause
  | ExtendsClause of extends_clause
  | ClassDefinitionElement of class_definition * final * dynamic_scope
  | ComponentClauseElement of component_clause * final * dynamic_scope
  | ReplaceableClassDefinition of class_definition *
      (constraining_clause * comment) option * final * dynamic_scope
  | ReplaceableComponentClause of component_clause *
      (constraining_clause * comment) option * final * dynamic_scope

and dynamic_scope =
  | Inner
  | Outer
  | NoDynamicScope

and extends_clause = name * class_modification option

and constraining_clause = extends_clause

and other_elements =
  | Public of element list
  | Protected of element list
  | EquationClauseElement of equation_clause
  | AlgorithmClauseElement of algorithm_clause

and externalll =
  | External of string option * external_function_call option *
      annotation option

and external_function_call =
  | ExternalFunctionCall of component_reference option * ident *
      expression list

and import_clause =
  | NewIdentifier of ident * name * comment
  | Identifier of name * comment
  | AllIdentifiers of name * comment

and component_clause =
  | ComponentClause of type_prefix * type_specifier *
      array_subscripts * component_declaration list

and type_prefix =
  | TypePrefix of flow option * variability option * inout option

and flow =
  | Flow

and variability =
  | Discrete
  | Parameter
  | Constant

and inout =
  | Input
  | Output

and type_specifier = name

and component_declaration =
  | ComponentDeclaration of declaration * comment

and declaration = ident * array_subscripts * modification option

and modification =
  | Modification of class_modification * expression option
  | Eq of expression
  | ColEq of expression

and class_modification =
  | ClassModification of argument list

and argument =
  | ElementModification of each * final * component_reference *
      modification * string_comment
  | ElementRedeclaration of each * final * redeclaration

and each =
  | Each
  | NotEach

and redeclaration =
  | Redeclaration of replaceable * redeclared_element *
      (constraining_clause * comment) option

and replaceable =
  | Replaceable
  | NotReplaceable

and redeclared_element =
  | RedeclaredClassDefinition of class_definition
  | RedeclaredComponentClause of type_prefix * type_specifier *
      component_declaration

and equation_clause =
  | EquationClause of initial * equation_or_annotation list

and equation_or_annotation =
  | Equation of equation * comment
  | EquationAnnotation of annotation

and algorithm_clause =
  | AlgorithmClause of initial * algorithm_or_annotation list

and algorithm_or_annotation =
  | Algorithm of algorithm * comment
  | AlgorithmAnnotation of annotation

and initial =
  | Initial
  | NotInitial

and equation =
  | Equality of (* simple *) expression * expression
  | ConditionalEquationE of (expression * equation list) list * equation list
  | ForClauseE of for_indices * equation list
  | ConnectClause of component_reference * component_reference
  | WhenClauseE of (expression * equation list) list
  | FunctionCallE of component_reference * function_arguments option

and algorithm =
  | Assignment of component_reference * expression
  | FunctionCallA of component_reference * function_arguments option
  | MultipleAssignment of expression list * component_reference *
      function_arguments option
  | ConditionalEquationA of (expression * algorithm list) list * algorithm list
  | ForClauseA of for_indices * algorithm list
  | WhileClause of expression * algorithm list
  | WhenClauseA of (expression * algorithm list) list

and for_indices = (ident * expression option) list

and expression =
  | Addition of expression * expression
  | And of expression * expression
  | Division of expression * expression
  | End
  | Equals of expression * expression
  | ExpressionList of expression array
  | False
  | FunctionCall of component_reference * function_arguments option
  | GreaterEqualThan of expression * expression
  | GreaterThan of expression * expression
  | If of (expression * expression) list * expression
  | Integer of string
  | LessEqualThan of expression * expression
  | LessThan of expression * expression
  | ArrayConcatenation of expression list list
  | Minus of expression
  | Multiplication of expression * expression
  | Not of expression
  | NotEquals of expression * expression
  | Or of expression * expression
  | Plus of expression
  | Power of expression * expression
  | Range of expression * expression * expression option
  | Real of string
  | Reference of component_reference
  | String of string
  | Subtraction of expression * expression
  | True
  | VectorOrRecord of function_arguments

and ident = string

and name = ident list

and component_reference = (ident * array_subscripts) list

and function_arguments =
  | ArgList of expression list * for_indices option
  | NamedArgList of (ident * expression) list * for_indices option

and array_subscripts = array_subscript array

and array_subscript =
  | All
  | Subscript of expression

and comment =
  | Comment of string_comment * annotation option

and string_comment =
  | StringComment of string list

and annotation =
  | Annotation of class_modification
