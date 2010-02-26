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

(** This module provides the functions that yield instances of compiled
Modelica classes.*)


(** The type of a Modelica instantiated class. *)
type instantiated_class = {
  icl_components: (string * instantiated_component Lazy.t) list Lazy.t;
  icl_init_equs: equation list Lazy.t;
  icl_equs: equation list Lazy.t;
}

and instantiated_component =
    InstantiatedParameter of instantiated_parameter
  | InstantiatedVariable of instantiated_variable

and instantiated_parameter =
    InstantiatedIntegerParameter of string * parameter_kind *
      typed_expression * Compilation.variable_infos
  | InstantiatedStringParameter of string * parameter_kind *
      typed_expression * Compilation.variable_infos
  | InstantiatedRealParameter of string * parameter_kind * typed_expression *
      Compilation.variable_infos

and parameter_kind = Main | Sub

and instantiated_variable =
    InstantiatedIntegerVariable of string * Compilation.inout *
    typed_expression * Compilation.variable_infos
  | InstantiatedStringVariable of string * Compilation.inout *
    typed_expression * Compilation.variable_infos
  | InstantiatedDiscreteVariable of string * Compilation.inout *
      typed_expression * Compilation.variable_infos
  | InstantiatedRealVariable of string * Compilation.inout *
      Compilation.nature * typed_expression * Compilation.variable_infos
  | InstantiatedCompoundVariable of string * typed_expression

and equation =
    Equation of typed_expression * typed_expression
  | ConditionalEquation of (typed_expression * equation list) list *
      equation list
  | FlowConnection of typed_expression * typed_expression
  | When of when_clause_type

and when_clause_type =
  (typed_expression * typed_when_expression list) list

and typed_when_expression =
  | Reinit of typed_expression * typed_expression
  | Assign of typed_expression * typed_expression

and typed_expression = {
  tex_type: expression_type;
  tex_expression: expression option;
}

and expression_type =
    BooleanType of int array
  | CartesianProduct of expression_type list
  | CompoundType of int array
  | IntegerType of int array
  | RealType of int array
  | StringType of int array

and expression =
  | Abs of typed_expression
  | Acos of typed_expression
  | Acosh of typed_expression
  | Addition of typed_expression * typed_expression
  | And of typed_expression * typed_expression
  | Asin of typed_expression
  | Asinh of typed_expression
  | Atan of typed_expression
  | Atanh of typed_expression
  | Boolean of bool
  | Cardinality of typed_expression
  | CompoundElement of instantiated_class
  | Cos of typed_expression
  | Cosh of typed_expression
  | Der of typed_expression
  | Division of typed_expression * typed_expression
  | Equals of typed_expression * typed_expression
  | Exp of typed_expression
  | ExternalFunctionCall of
      string list * expression_type list * expression_type list *
      typed_argument list
  | Floor of typed_expression
  | GreaterEqualThan of typed_expression * typed_expression
  | GreaterThan of typed_expression * typed_expression
  | If of (typed_expression * typed_expression) list * typed_expression
  | Integer of int32
  | Log of typed_expression
  | Max of typed_expression * typed_expression
  | Min of typed_expression * typed_expression
  | Mod of typed_expression * typed_expression
  | Minus of typed_expression
  | Multiplication of typed_expression * typed_expression
  | NoEvent of typed_expression
  | Not of typed_expression
  | NotEquals of typed_expression * typed_expression
  | Or of typed_expression * typed_expression
  | ParameterValue of int * reference
  | Power of typed_expression * typed_expression
  | Pre of typed_expression
  | Real of float
  | Sin of typed_expression
  | Sinh of typed_expression
  | Sqrt of typed_expression
  | String of string
  | Subtraction of typed_expression * typed_expression
  | Tan of typed_expression
  | Tanh of typed_expression
  | Time
  | VariableStart of int * reference
  | VariableValue of int * reference
  | Vector of typed_expression array

and typed_argument =
  | ScalarArgument of typed_expression
  | ArrayArgument of int list * typed_expression array

and reference = (string * int array) list

(** The type of an instantiation context. *)
type instantiation_context =
  | ToplevelContext (** The default instantiation context *)
  | ClassContext of instantiation_context * instantiated_class Lazy.t
  | ForContext of instantiation_context * int

(** The type of an instantiated modification. *)
type modification =
    Modification of (string * int array) * modification list *
      typed_expression option

val string_of_expression: typed_expression -> string
(** [string_of_expression iexpr] yields a string containing an implementation
dependent representation of the typed expression [iexpr] (for debugging
purposes).*)

val instantiate_main_class:
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.compiled_class -> typed_expression
(** [instantiate_main_class ctx modifs ccl] returns an instance of the model
given in [ccl] under context [ctx] and modifications [modifs] (usually, [ctx] is
[ToplevelContext] and [modifs] is [[]]. *)

val expand_class :
  typed_expression ->
  (string * instantiated_component) list * equation list * equation list
(** [expand_class iexpr] returns a list of instantiated components and two lists
 of instantiated equations (initial and dynamic ones) given an instance of a
 Modelica model [iexpr]. *)
