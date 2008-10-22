
(*  Scicos *)
(* *)
(*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr> *)
(* *)
(* This program is free software; you can redistribute it and/or modify *)
(* it under the terms of the GNU General Public License as published by *)
(* the Free Software Foundation; either version 2 of the License, or *)
(* (at your option) any later version. *)
(* *)
(* This program is distributed in the hope that it will be useful, *)
(* but WITHOUT ANY WARRANTY; without even the implied warranty of *)
(* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *)
(* GNU General Public License for more details. *)
(* *) 
(* You should have received a copy of the GNU General Public License *)
(* along with this program; if not, write to the Free Software *)
(* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. *)
(*  *)
(* See the file ./license.txt *)

(** This module performs the compilation of a subset of the Modelica language.
*)

exception InvalidModification
exception TypeError

type compiled_unit =
    CompiledClass of compiled_class Lazy.t
  | CompiledFunction of compiled_class Lazy.t
(** The type of a compiled Modelica class *)

and compiled_class = {
  ccl_public_cpnts : (string * compiled_component Lazy.t) list Lazy.t;
  ccl_initial_equs : compiled_equation list Lazy.t;
  ccl_equs : compiled_equation list Lazy.t;
}

and compiled_modification =
    CompiledModification of field * compiled_modification list *
      compiled_expression option

and field = string * compiled_subscript array

and compiled_reference =
    ParameterReference of level * path
  | VariableReference of level * path
  | LoopVariableReference of level
  | ClassReference of level * string list

and path = field list

and level = int

and compiled_subscript = Indefinite | Definite of compiled_expression

and parameter =
    IntegerParameter of parameter_attributes
  | RealParameter of parameter_attributes

and parameter_attributes = {
  pat_dimensions : compiled_subscript array;
  pat_comment : string;
  pat_value : compiled_expression option;
}

and variable =
    DiscreteVariable of variable_attributes
  | RealVariable of variable_attributes
  | CompoundVariable of compiled_class Lazy.t * variable_attributes

and variable_attributes = {
  vat_dimensions : compiled_subscript array;
  vat_nature : nature;
  vat_inout : inout;
  vat_comment : string;
  vat_modifications : compiled_modification list;
}

and compiled_component = Parameter of parameter | Variable of variable

and nature = Flow | Potential

and inout = Input | Output | Both

and compiled_equation =
    CompiledEquality of compiled_expression * compiled_expression
  | CompiledFlowConnection of compiled_expression * compiled_expression
  | CompiledIf of (compiled_expression * compiled_equation list) list *
      compiled_equation list
  | CompiledFor of compiled_expression * compiled_expression *
      compiled_expression * compiled_equation list
  | CompiledWhen of (compiled_expression * compiled_when_expression list) list

and compiled_when_expression =
  | Reinit of compiled_reference * compiled_expression
  | Assign of compiled_reference * compiled_expression

and compiled_expression =
  | Abs of compiled_expression
  | Addition of compiled_expression * compiled_expression
  | And of compiled_expression * compiled_expression
  | Boolean of bool
  | Cardinality of compiled_expression
  | Cos of compiled_expression
  | Der of compiled_expression
  | Division of compiled_expression * compiled_expression
  | Equals of compiled_expression * compiled_expression
  | Exp of compiled_expression
  | ExternalFunctionCall of string list * compiled_class Lazy.t *
    compiled_expression list
  | Floor of compiled_expression
  | GreaterEqualThan of compiled_expression * compiled_expression
  | GreaterThan of compiled_expression * compiled_expression
  | If of (compiled_expression * compiled_expression) list * compiled_expression
  | Integer of int32
  | Log of compiled_expression
  | Max of compiled_expression * compiled_expression
  | Min of compiled_expression * compiled_expression
  | Minus of compiled_expression
  | Mod of compiled_expression * compiled_expression
  | Multiplication of compiled_expression * compiled_expression
  | NoEvent of compiled_expression
  | Not of compiled_expression
  | NotEquals of compiled_expression * compiled_expression
  | Or of compiled_expression * compiled_expression
  | Power of compiled_expression * compiled_expression
  | Real of float
  | Reference of compiled_reference
  | Sin of compiled_expression
  | Sqrt of compiled_expression
  | String of string
  | Subtraction of compiled_expression * compiled_expression
  | Tan of compiled_expression
  | Tanh of compiled_expression
  | Time
  | Vector of compiled_expression array

val paths : string list ref
(** Global variable used to store the paths where to find compiled Modelica
classes. *)

val read_class_file : string -> compiled_unit
(** [read_class_file name] finds then loads the compiled class named [name].
A compiled Modelica class named "Class" is usually stored in a file named
"Class.moc". The search is performed in the file system using
{!Compilation.path}. *)

val write_class_file : string -> compiled_unit -> unit
(** [write_class_file name cu] stores the compiled Modelica class [cu] in a file
named [name]. See {!Compilation.read_class_file} for more information about file
naming conventions. *)

val compile_main_class : Precompilation.precompiled_class -> compiled_unit
(** [compile_main_class pcl] yields the compiled Modelica class associated to
[pcl]. *)
