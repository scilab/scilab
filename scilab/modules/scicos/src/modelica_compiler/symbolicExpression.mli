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

(** This module provides the data structures and functions that are necessary
to perform the symbolic manipulations involved in the transformation of Modelica
models to simulation code. *)

open Num

exception Infinite_result of string
(** [Infinite_result] is raised when the result of symbolic computation
evaluates to an infinite result. *)

type t
(** The type of the objects manipulated as symbolic expressions. *)

type nature =
    | Addition of t list
    | And of t list
    | ArcCosine of t
    | ArcHyperbolicCosine of t
    | ArcHyperbolicSine of t
    | ArcHyperbolicTangent of t
    | ArcSine of t
    | ArcTangent of t
    | BlackBox of string * argument list
    | BooleanValue of bool
    | Constant of string
    | Cosine of t
    | Derivative of t * num
    | DiscreteVariable of int
    | Equality of t * t
    | Exponential of t
    | Floor of t
    | Greater of t * t
    | GreaterEqual of t * t
    | HyperbolicCosine of t
    | HyperbolicSine of t
    | HyperbolicTangent of t
    | If of t * t * t
    | Integer of int32
    | Logarithm of t
    | Multiplication of t list
    | Not of t
    | Number of num
    | Or of t list
    | Parameter of int
    | PartialDerivative of t * t (* PartialDerivative (variable, expression) *)
    | Pre of t
    | RationalPower of t * num
    | Sign of t
    | Sine of t
    | String of string
    | Tangent of t
    | TimeVariable
    | Variable of int
(** The type of the nature of an expression. *)

and argument =
  | ScalarArgument of t
  | ArrayArgument of int list (* dimensions *) * t array (* flatten array *)
(** The type of the arguments of functions. *)

(* Node utilities *)

val nature: t -> nature
(** [nature node] returns an object of {!SymbolicExpression.nature} type. *)

val hash: t -> int
(** [hash node] returns an integer associated to [node] such that verifies the
usual properties of a hash number. *)

(* Node lists utilities *)

val sort: t list -> t list
(** [sort nodes] sorts [nodes] using internal criterion. This function is used
to ensure that two lists containing the same nodes (with respect to the
{!Pervasives.( == )} predicate) but in different relative positions are sorted
such that the relative positions are the same in both lists. *)

(* Useful constants *)

val minus_one: t
(* The object that represents the -1 value. *)

val zero: t
(** The object that represents the 0 value. *)

val one: t
(** The object that represents the 1 value. *)

val two: t
(** The object that represents the 2 value. *)

val ten: t
(** The object that represents the 10 value. *)

val one_over_two: t
(** The object that represents the 1/2 value. *)

val minus_one_over_two: t
(** The object that represents the -1/2 value. *)

val pi: t
(** The object that represents the pi value. *)

val pi_over_two: t
(** The object that represents the pi/2 value. *)

val minus_pi_over_two: t
(** The object that represents the -pi/2 value. *)

val e: t
(** The object that represents the e (= exp (1)) value. *)

val true_value: t
(** The object that represents the true value. *)

val false_value: t
(** The object that represents the false value. *)

val time: t
(** The object that represents the independant time variable. *)

val addition_neutral: t
(** The object that represents the neutral element of the addition over reals. *)

val multiplication_neutral: t
(** The object that represents the neutral element of the multiplication over
reals. *)

val and_neutral: t
(** The object that represents the neutral element of the 'and' over booleans. *)

val or_neutral: t
(** The object that represents the neutral element of the 'or' over booleans. *)


(* Creation of objects representing expressions *)

val create_addition: t list -> t
(** [create_addition exprs] returns an object representing the addition of
the expressions represented by the object of the list [exprs]. *)

val create_and: t list -> t
(** [create_and exprs] returns an object representing the 'and' of
the expressions represented by the object of the list [exprs]. *)

val create_arcCosine: t -> t
(** [create_arcCosine expr] returns an object representing the arc cosine of
the expression represented by [expr]. *)

val create_arcHyperbolicCosine: t -> t
(** [create_arcHyperbolicCosine expr] returns an object representing the arc
hyperbolic cosine of the expression represented by [expr]. *)

val create_arcHyperbolicSine: t -> t
(** [create_arcHyperbolicSine expr] returns an object representing the arc
hyperbolic sine of the expression represented by [expr]. *)

val create_arcHyperbolicTangent: t -> t
(** [create_arcHyperbolicTangent expr] returns an object representing the arc
hyperbolic tangent of the expression represented by [expr]. *)

val create_arcSine: t -> t
(** [create_arcSine expr] returns an object representing the arc sine of the
expression represented by [expr]. *)

val create_arcTangent: t -> t
(** [create_arcTangent expr] returns an object representing the arc tangent of
the expression represented by [expr]. *)

val create_blackBox: string -> argument list -> t
(** [create_blackBox name args] returns an object representing a black box
function application expression. [name] is the name of the function and [args]
is the list of the object representing the arguments of the function. *)

val create_booleanValue: bool -> t
(** [create_booleanValue bool] returns true_value if bool is [true] and
false_value if bool is [false]. *)

val create_constant: string -> t
(** [create_constant name] returns an object representing a symbolic constant
named [name]. *)

val create_cosine: t -> t
(** [create_cosine expr] returns an object representing the cosine of the
expression represented by [expr]. *)

val create_derivative: t -> num -> t
(** [create_derivative expr num] returns an object representing the [num]th
derivative of the expression represented by [expr]. *)

val create_discrete_variable: int -> t
(** [create_discrete_variable id] returns an object representing a discrete
variable whose id is [id]. *)

val create_equality: t -> t -> t
(** [create_equality expr expr'] returns an object representing the equality of
the expressions represented by [expr] and [expr']. *)

val create_exponential: t -> t
(** [create_exponential expr] returns an object representing the exponential of
the expression represented by [expr]. *)

val create_floor: t -> t
(** [create_floor expr] returns an object representing the floor (i.e. the
greatest integer smaller than) of the expression represented by [expr]. *)

val create_greater: t -> t -> t
(** [create_greater expr expr'] returns an object representing the strict
'greater than' relation involving the expressions represented by [expr] and
[expr'] in that order. *)

val create_greater_equal: t -> t -> t
(** [create_greater_equal expr expr'] returns an object representing the
'greater than' relation involving the expressions represented by [expr] and
[expr'] in that order. *)

val create_hyperbolicCosine: t -> t
(** [create_hyperbolicCosine expr] returns an object representing the hyperbolic
cosine of the expression represented by [expr]. *)

val create_hyperbolicSine: t -> t
(** [create_hyperbolicSine expr] returns an object representing the hyperbolic
sine of the expression represented by [expr]. *)

val create_hyperbolicTangent: t -> t
(** [create_hyperbolicTangent expr] returns an object representing the hyperbolic
tangent of the expression represented by [expr]. *)

val create_if: t -> t -> t -> t
(** [create_if expr expr' expr''] returns an object representing the 'if' whose
condition is the expression represented by [expr] and first alternative the
expression represented by [expr'] and second alternative the expression
represented by [expr'']. *)

val create_integer: int32 -> t
(** [create_integer i] returns an object representing the integer [i]. *)

val create_logarithm: t -> t
(** [create_logarithm expr] returns an object representing the logarithm of the
expression represented by [expr]. *)

val create_multiplication: t list -> t
(** [create_multiplication exprs] returns an object representing the
multiplication of the expressions represented by the objects of the list
[exprs]. *)

val create_not: t -> t
(** [create_not expr] returns an object representing the 'no' of the expression
represented by [expr]. *)

val create_number: num -> t
(** [create_number num] returns an object representing the number (integer or
rational) whose value is [num]. *)

val create_or: t list -> t
(** [create_or exprs] returns an object representing the 'or' of the expressions
represented by the objects of the list [exprs]. *)

val create_parameter: int -> t
(** [create_parameter id] returns an object representing a parameter whose id is
[id]. *)

val create_partialDerivative: t -> t -> t
(** [create_partialDerivative expr expr'] returns an object representing the
partial derivative of the expression represented by [expr'] with respect to the
variable represented by [expr]. If [expr] does not represent a variable,
Invalid_argument is raised. *)

val create_pre: t -> t
(** [create_pre expr] returns an object representing the 'previous value' of
the expression represented by [expr]. *)

val create_rationalPower: t -> num -> t
(** [create_rationalPower expr num] returns an object representing the expression
represented by [expr] raised to the power of the rational number represented by
[num]. *)

val create_sign: t -> t
(** [create_sign expr] returns an object representing the 'sign' of the
expression represented by [expr]
(sign (x) = -1, if x < 0, 0 if x = 0, 1 if x > 0). *)

val create_sine: t -> t
(** [create_sine expr] returns an object representing the sine of the expression
represented by [expr]. *)

val create_string: string -> t
(** [create_string s] returns an object representing the string [s]. *)

val create_tangent: t -> t
(** [create_tangent expr] returns an object representing the tangent of the
expression represented by [expr]. *)

val create_timeVariable: unit -> t
(** [create_timeVariable ()] returns an object representing the time variable.
*)

val create_variable: int -> t
(** [create_variable id] returns an object representing a variable whose id is
[id]. *)


(* Symbolic manipulation of expressions *)

val symbolic_abs: t -> t
(** [symbolic_abs expr] returns an object that represents the result of applying
the 'abs' function to the expression represented by [expr]. *)

val symbolic_acos: t -> t
(** [symbolic_acos expr] returns an object that represents the result of applying
the arc cosine function to the expression represented by [expr]. *)

val symbolic_acosh: t -> t
(** [symbolic_acosh expr] returns an object that represents the result of
applying the hyperbolic arc cosine function to the expression represented by
[expr]. *)

val symbolic_add: t -> t -> t
(** [symbolic_add expr expr'] returns an object that represents the result of
applying the addition to the expressions represented by [expr] and [expr']. *)

val symbolic_and: t -> t -> t
(** [symbolic_and expr expr'] returns an object that represents the result of
applying the 'and' function to the expressions represented by [expr] and
[expr']. *)

val symbolic_asin: t -> t
(** [symbolic_asin expr] returns an object that represents the result of
applying the hyperbolic sine function to the expression represented by [expr].
*)

val symbolic_asinh: t -> t
(** [symbolic_asinh expr] returns an object that represents the result of
applying the hyperbolic arc cosine function to the expression represented by
[expr]. *)

val symbolic_atan: t -> t
(** [symbolic_atan expr] returns an object that represents the result of applying
the arc tangent function to the expression represented by [expr]. *)

val symbolic_atanh: t -> t
(** [symbolic_abs expr] returns an object that represents the result of applying
the hyperbolic arc tangent function to the expression represented by [expr]. *)

val symbolic_blackBox: string -> argument list -> t
(** [symbolic_blackBox name args] returns an object that represents the
result of applying the black box function named [name] to the expressions
represented by the objects of the list [args]. *)

val symbolic_cos: t -> t
(** [symbolic_cos expr] returns an object that represents the result of applying
the cosine function to the expression represented by [expr]. *)

val symbolic_cosh: t -> t
(** [symbolic_cosh expr] returns an object that represents the result of
applying the hyperbolic cosine function to the expression represented by [expr].
*)

val symbolic_derive: t -> num -> t
(** [symbolic_derive expr num] returns the [num]-th derivative of [expr] with
respect to the implicit time variable. *)

val symbolic_derivative: t -> t
(** [symbolic_derivative expr] returns an object that represents the result of
applying the derivative function with respect to the implicit time variable to
the expression represented by [expr]. *)

val symbolic_div: t -> t -> t
(** [symbolic_div expr expr'] returns an object that represents the result of
applying the division to the expressions represented by [expr] and
[expr']. *)

val symbolic_eq: t -> t -> t
(** [symbolic_eq expr expr'] returns an object that represents the result of
applying the equality to the expressions represented by [expr] and
[expr']. *)

val symbolic_exp: t -> t
(** [symbolic_exp expr] returns an object that represents the result of applying
the exponential function to the expression represented by [expr]. *)

val symbolic_floor: t -> t
(** [symbolic_floor expr] returns an object that represents the result of
applying the 'floor' function to the expression represented by [expr]. *)

val symbolic_ge: t -> t -> t
(** [symbolic_ge expr expr'] returns an object that represents the result of
applying the 'greater than' function to the expressions represented by [expr]
and [expr'], in that order. *)

val symbolic_gt: t -> t -> t
(** [symbolic_gt expr expr'] returns an object that represents the result of
applying the 'strict greater than' function to the expressions represented by
[expr] and [expr'], in that order. *)

val symbolic_if: t -> t -> t -> t
(** [symbolic_if expr expr' expr''] returns an object that represents the result
of applying the 'if then else' function to the expressions represented by
[expr], [expr'] and [expr''], in that order. *)

val symbolic_invert: t -> t
(** [symbolic_invert expr] returns an object that represents the result of
applying the (multiplicative) 'invert' function to the expression represented by
[expr]. *)

val symbolic_log: t -> t
(** [symbolic_log expr] returns an object that represents the result of
applying the (natural) 'log' function to the expression represented by [expr].
Raise Infinite_result if [expr] represent zero.*)

val symbolic_log10: t -> t
(** [symbolic_log10 expr] returns an object that represents the result of
applying the 'decimal log' function to the expression represented by [expr].
Raise Infinite_result if [expr] represent zero.*)

val symbolic_le: t -> t -> t
(** [symbolic_le expr expr'] returns an object that represents the result of
applying the 'lower than' function to the expressions represented by [expr]
and [expr'], in that order. *)

val symbolic_lt: t -> t -> t
(** [symbolic_le expr expr'] returns an object that represents the result of
applying the 'strict lower than' function to the expressions represented by
[expr] and [expr'], in that order. *)

val symbolic_max: t -> t -> t
(** [symbolic_max expr expr'] returns an object that represents the result of
applying the 'max' function to the expressions represented by [expr] and
[expr']. *)

val symbolic_min: t -> t -> t
(** [symbolic_min expr expr'] returns an object that represents the result of
applying the 'min' function to the expressions represented by [expr] and
[expr']. *)

val symbolic_minus: t -> t
(** [symbolic_minus expr] returns an object that represents the result of
applying the unary minus function to the expression represented by [expr]. *)

val symbolic_mult: t -> t -> t
(** [symbolic_mult expr expr'] returns an object that represents the result of
applying the multiplication to the expressions represented by [expr] and
[expr']. *)

val symbolic_neq: t -> t -> t
(** [symbolic_neq expr expr'] returns an object that represents the result of
applying the inequality to the expressions represented by [expr] and
[expr']. *)

val symbolic_not: t -> t
(** [symbolic_not expr] returns an object that represents the result of applying
the 'not' function to the expression represented by [expr]. *)

val symbolic_or: t -> t -> t
(** [symbolic_or expr expr'] returns an object that represents the result of
applying the 'or' function to the expressions represented by [expr] and
[expr']. *)

val symbolic_partial_derivative_with: t -> t -> t -> t
(** [symbolic_partial_derivative step expr expr'] returns an object that represents
the result of applying the partial derivative function to the expression
represented by [expr'] with respect to the variable represented by [expr]. If
[expr]'s nature is [And _], [BooleanValue _], [Equality _], [Not _], [Or _],
[Greater _] or [GreaterEqual _], raise Invalid_argument. [step] is used to
specify a default step value when generating finite-difference schemes for
black box functions. *)

val symbolic_partial_derivative: t -> t -> t
(** [symbolic_partial_derivative expr expr'] returns an object that represents
the result of applying the partial derivative function to the expression
represented by [expr'] with respect to the variable represented by [expr]. If
[expr]'s nature is [And _], [BooleanValue _], [Equality _], [Not _], [Or _],
[Greater _] or [GreaterEqual _], raise Invalid_argument. *)

val symbolic_power: t -> t -> t
(** [symbolic_power expr expr'] returns an object that represents the result of
raising the expression represented by [expr] to the power of the expression
represented by [expr']. Raise Infinite_result if both [expr] and [num] represent
zero.*)

val symbolic_pre: t -> t
(** [symbolic_pre expr] returns an object that represents the result of
applying the 'pre' operator to the expression represented by [expr]. *)

val symbolic_rationalPower: t -> num -> t
(** [symbolic_rationalPower expr num] returns an object that represents the
result of raising the expression represented by [expr] to the power of the
rational number represented by [num]. Raise Infinite_result if both [expr] and
[num] represent zero. *)

val symbolic_sgn: t -> t
(** [symbolic_sgn expr] returns an object that represents the result of
applying the 'sign' function to the expression represented by [expr]. *)

val symbolic_sin: t -> t
(** [symbolic_sin expr] returns an object that represents the result of
applying the sine function to the expression represented by [expr]. *)

val symbolic_sinh: t -> t
(** [symbolic_sinh expr] returns an object that represents the result of
applying the hyperbolic sine function to the expression represented by [expr].
*)

val symbolic_sqrt: t -> t
(** [symbolic_sqrt expr] returns an object that represents the result of
applying the 'square root' function to the expression represented by [expr]. *)

val symbolic_sub: t -> t -> t
(** [symbolic_sub expr expr'] returns an object that represents the result of
applying the subtraction to the expressions represented by [expr] and
[expr'], in that order. *)

val symbolic_tan: t -> t
(** [symbolic_tan expr] returns an object that represents the result of
applying the tangent function to the expression represented by [expr]. *)

val symbolic_tanh: t -> t
(** [symbolic_tanh expr] returns an object that represents the result of
applying the hyperbolic tangent function to the expression represented by
[expr]. *)


(* Symbolic a-ary function application *)

val apply_addition: t list -> t
(** [apply_addition exprs] returns an object that represents the result of
applying the addition to the expressions represented by the objects in the list
[exprs]. *)

val apply_and: t list -> t
(** [apply_and exprs] returns an object that represents the result of applying
the 'and' function to the expressions represented by the objects in the list
[exprs]. *)

val apply_blackBox: string -> argument list -> t
(** [apply_blackBox name args] returns an object that represents the result
of applying the black box function named [name] to the expressions represented
by the objects in the list [args]. *)

val apply_max: t list -> t
(** [apply_max exprs] returns an object that represents the result of applying
the 'max' function to the expressions represented by the objects in the list
[exprs]. Raise Infinite_result if the list is empty. *)

val apply_min: t list -> t
(** [apply_min exprs] returns an object that represents the result of applying
the 'min' function to the expressions represented by the objects in the list
[exprs]. Raise Infinite_result if the list is empty. *)

val apply_multiplication: t list -> t
(** [apply_multiplication exprs] returns an object that represents the result of
applying the multiplication to the expressions represented by the objects in the
list [exprs]. *)

val apply_or: t list -> t
(** [apply_or exprs] returns an object that represents the result of applying
the 'or' function to the expressions represented by the objects in the list
[exprs]. *)


(* Input/output *)

val output: out_channel -> t -> unit
(** [output oc expr] outputs to [oc] a string representing the expression
represented by [expr]. *)


(* Formal manipulation helpers *)

val is_subnode_of: t -> t -> bool
(** [is_subnode_of node node'] returns [true] if [node'] appears as a subnode
in the internal representation of [node] viewed as a tree. *)

val assignable_parameters_of: t -> t list
(** [assignable_parameters_of node] returns the parameters that, considered as
variables, might be selected by the Hungarian method applied to [node] as a
list of symbolic expressions. *)

val variables_of: t -> t list
(** [variables_of node] returns the variables that appear in [node] as a list of
symbolic expressions. *)

val assignable_variables_of: t -> t list
(** [assignable_variables_of node] returns the variables that might be selected
by the Hungarian method applied to [node] as a list of symbolic expressions. *)

val derivatives_of: t -> t list
(** [derivatives_of node] returns the derivatives that appear in [node] as a list
of symbolic expressions. *)

val inputs_of: t -> t list
(** [inputs_of node] returns the inputs that appear in [node] as a list of
symbolic expressions. *)

val invert_if_possible_with_respect_to: t -> t -> t -> t option
(** [invert_if_possible_with_respect_to node left right] returns [Some node'] if
a way to rewrite the equation [left] = [right] as [node] = ... by applying
inversions to both the left hand part and the right hand part of the initial
equation has been found and [None] otherwise.*)

val inversion_difficulty: t -> t -> t -> int
(** [inversion_difficulty node left right] returns a coefficient whose value
is 0 if [node] (usually, a variable) appears derivated in [left], 1 if
[invert_if_possible_with_respect_to node left right] returns [Some _] (i.e.,
if it is "easy" to invert [left] with respect to [node] in [left] = [right]),
2 if the inversion cannot be done and 3 if [node] does not appear in [left].
*)

val replace: t -> t -> t -> t
(** [replace node node' node''] rewrites [nodes''] by replacing all occurrences
of [node] by [node'] within [nodes'']. *)

val exists: (t -> bool) -> t -> bool
(** [exists p node] checks if at least one subnode of [node] verifies p. *)

val exists_except_in_conditions: (t -> bool) -> t -> bool
(** [exists_except_in_conditions p node] checks if at least one subnode of
[node] except if conditions verifies p. *)
