(** This module contains functions that perform symbolic and topologic
simplifications over an instantiated Modelica model. *)

(** The type of the records used to store information about parameters. *)
type parameter_description = {
  mutable main : bool; (** If a parameter is a "main" one, it is not inlined. *)
  mutable p_name : string; (** The parameter's user name *)
  mutable p_comment : string; (** The comment associated to the parameter. *)
  mutable value : SymbolicExpression.t;
}

(** The type of the records used to store information about discrete variables.
*)
type discrete_variable_description =
  {
    mutable d_output: int option; (** The index of the variable in the output
      vector *)
    mutable d_v_name: string; (** The discrete variable's user name *)
    mutable d_v_comment: string; (** The comment associated to the discrete
      variable. *)
    mutable d_start_value: SymbolicExpression.t
  }

(** The type of the records used to store information about variables. *)
type variable_description = {
  mutable output : int option; (** The index of the variable in the output
    vector *)
  mutable state : bool; (** [true] = state variable, [false] = algebraic
    variable *)
  mutable v_name : string; (** The variable's user name *)
  mutable v_comment : string; (** The comment associated to the variable. *)
  mutable start_value : SymbolicExpression.t;
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
  mutable external_functions : string list list; (** The list of the paths
    where to find external function bodies. *)
}

and when_expression =
  | Assign of SymbolicExpression.t * SymbolicExpression.t
  | Reinit of SymbolicExpression.t * SymbolicExpression.t

val create_model : Instantiation.typed_expression -> model
(** [create_model iexpr] builds a model given the instantiated Modelica model
[iexpr]. The resulting data structure can be used to perform various
optimization passes over it. *)

val perform_simplifications : model -> unit
(** [perform_simplifications model] simplifies [model] by making the appropriate
substitutions in order to eliminate auxiliary variables from the system. *)

val find_submodels : model -> int list list
(** [find_submodels model] splits [model] in dependent submodels such that there
is no cyclic dependency between them. The result is given as a list of index
lists (each index corresponding to a variable index). *)

val print_model : out_channel -> model -> unit
(** [print_model oc model] prints [model] in an implementation-dependent format
to [oc]. *)
