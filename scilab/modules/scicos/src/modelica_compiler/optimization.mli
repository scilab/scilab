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

(** This module contains functions that perform symbolic and topologic
simplifications over an instantiated Modelica model. *)

(** The type of the records used to store information about parameters. *)
type parameter_description = {
  mutable main : bool; (** If a parameter is a "main" one, it is not inlined. *)
  mutable p_type : parameter_type; (** The parameter's type *)
  mutable p_name : string; (** The parameter's user name *)
  mutable p_comment : string; (** The comment associated to the parameter. *)
  mutable value : SymbolicExpression.t;
}

and parameter_type = IntegerType | StringType | RealType

(** The type of the records used to store information about discrete variables.
*)
type discrete_variable_description =
  {
    mutable d_output: int option; (** The index of the variable in the output
      vector *)
    mutable d_v_name: string; (** The discrete variable's user name *)
    mutable d_v_comment: string; (** The comment associated to the discrete
      variable. *)
    mutable d_start_value: SymbolicExpression.t option
  }

(** The type of the records used to store information about variables. *)
type variable_description = {
  mutable output : int option; (** The index of the variable in the output
    vector *)
  mutable state : bool; (** [true] = state variable, [false] = algebraic
    variable *)
  mutable v_name : string; (** The variable's user name *)
  mutable depth_in_hierarchy : int; (** The number of dots in the variable
    name that is used as a criterion to choose the variables to eliminate
    (deepest-defined variables are eliminated when possible). *)
  mutable v_comment : string; (** The comment associated to the variable. *)
  mutable start_value : SymbolicExpression.t option;
}

(** The type of the records used to store information about equations. *)
type equation_description = {
  mutable solved : bool; (** [true] = the equation has been inverted with
    respect to its associated variable (see model type) and thus does no longer
    appear in the set of equations submitted to the numeric solver. *)
  mutable inner_variables : SymbolicExpression.t list; (** The variables that
    appear in the equation. *)
  mutable inner_derivatives : SymbolicExpression.t list; (** The derivatives
    that appear in the equation. *)
  mutable assignable_variables : SymbolicExpression.t list; (** The variables
    it is possible to invert the equation with respect to. *)
  mutable expression : SymbolicExpression.t; (** If the equation has been
    solved, this expression tells how it is possible to obtain the associated
    variable's value from the set of unknown variables. Otherwise, this
    expression is the right side hand of the residual equation submitted to the
    numerical solver. *)
}

(** The type of a model on which it is possible to perform various optimizations
passes. *)
type model = {
  mutable inputs : string array; (** inputs.(i) contains the user name of the
    associated input. *)
  mutable parameters : parameter_description array;
  mutable discrete_variables : discrete_variable_description array;
  mutable variables : variable_description array;
  mutable equations : equation_description array;
  mutable reinitializable_variables : SymbolicExpression.t list; (** The list of
    the variables that may be reinitialized during the numerical simulation.
    These variables are not eliminated during the symbolic simplification
    passes. *)
  mutable when_clauses :
    (SymbolicExpression.t * when_expression list) list; (** The list of the when
      clauses. Each clause is stored as a condition along with its associated
      reinitializations. *)
  mutable io_dependency : bool; (** [true] = there is a direct dependency
    between inputs and outputs in the model. *)
  mutable external_functions :
    (string list *
    Instantiation.expression_type list *
    Instantiation.expression_type list) list; (** The list of the paths
    where to find external function bodies and their respective signatures. *)
  trace: string option; (** The file where optional tracing information of
    external function calls is generated *)
  variables_infos: (string * Compilation.variable_infos) list
}

and when_expression =
  | Assign of SymbolicExpression.t * SymbolicExpression.t
  | Reinit of SymbolicExpression.t * SymbolicExpression.t

val create_model_with_parameters: string option ->
  Instantiation.typed_expression -> model
(** [create_model trace iexpr] builds a model given the instantiated Modelica
model [iexpr]. [trace], if not [None], indicates a filename where tracing
information of external functions calls can be generated.
The generated model includes all the parameters present in the original
specification (i.e., no parameter inlining is performed).
The resulting data structure can be used to perform various
optimization passes over it. *)

val create_model: string option -> Instantiation.typed_expression -> model
(** [create_model trace iexpr] builds a model given the instantiated Modelica
model [iexpr]. The resulting data structure can be used to perform various
optimization passes over it. [trace], if not [None], indicates a filename
where tracing information of external functions calls can be generated. *)

val eliminate_trivial_relations: int -> model -> int
(** [eliminate_trivial_relations max_simplifs model] eliminates at most
[max_simplifs] trivial relations from the model (i.e. relations involving linear
relations between at most two variables). Returns the number of remaining
simplifications that was allowed. *)

val perform_simplifications: int -> model -> unit
(** [perform_simplifications max_simplifs model] simplifies [model] by making the
appropriate substitutions in order to eliminate at most [max_simplifs] auxiliary
variables from the model. *)

val find_submodels: model -> int list list
(** [find_submodels model] splits [model] in dependent submodels such that there
is no cyclic dependency between them. The result is given as a list of index
lists (each index corresponding to a variable index). *)

val print_model: out_channel -> model -> unit
(** [print_model oc model] prints [model] in an implementation-dependent format
to [oc]. *)
