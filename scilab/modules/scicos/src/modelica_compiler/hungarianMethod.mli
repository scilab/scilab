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

(** This modules provides structures and functions that performs the
causality analysis using the Hungarian Method. *)

(** The module type that specifies the interface of a suitable module
providing matrix elements. The following properties must be verified by
the elements in order to get correct results:
[compare x y] must have the following behavior:

{v
\ y  | +Inf |   y'
x \  |      |
-----+------+-----------
+Inf |  0   |   z>0
-----+------+-----------
 x'  | z<0  | 0, if x'=y'
     |      | z<0, if x'<y'
     |      | z>0, if x'>y'
v}

[x = y] is [true] iff [compare x y] returns [0].

[add x y] must have the following behavior:

{v
\ y  | +Inf |   y'
x \  |      |
-----+------+-----------
+Inf | +Inf |  +Inf
-----+------+-----------
 x'  | +Inf | z=x'+y'
v}

[sub x y] must have the following behavior:

{v
\ y  | +Inf |   y'
x \  |      |
-----+------+-----------
+Inf |  ?   |  +Inf
-----+------+-----------
 x'  |  ?   | z=x'-y'
v}

*)
module type MatrixElement =
  sig
    type t
    (** the type of the matrix elements *)
    val zero: t (** the object of type t representing 0 *)
    val infinity: t (** the object of type t representing +Inf *)
    val equal: t -> t -> bool
    val compare: t -> t -> int
    val add: t -> t -> t
    val sub: t -> t -> t
  end

module type Matrix =
  sig
    type elt
    (** the type of the elements of the matrix *)
    type t
    (** the type of the sparse matrices        *)
    val init: int -> elt -> (int -> int -> elt) -> t
    (** [init size default f] creates a square sparse matrix of
    [size]*[size] elements whose default element is [default] and whose
    initial elements are given by [f] ([f i j] returns the element to be
    placed at ([i], [j])). *)
    val iterij: (int -> int -> elt -> unit) -> t -> unit
    (** [iterij f mtrx] behaves like [iter f mtrx] except that f receives
    the row index as first argument, the column index as second argument
    and the current element as third argument. *)
    val row_storage_iterj: int -> (int -> elt -> unit) -> t -> unit
    (** [row_storage_iterj i f mtrx] is a restriction of
    [row_iterj i f mtrx] to the elements effectively stored into the matrix,
    in an unspecified order. Provided for efficiency. *)
    val update_row_storage: int -> (int -> elt -> elt) -> t -> unit
    (** [update_row_storage i f mtrx] replaces the elements effectively
    stored into [mtrx] at row [i] by the results of [f] applyied to the
    current column index as first argument and the current element as
    second argument, in an unspecified order. Provided for efficiency. *)
    val column_storage_iteri: int -> (int -> elt -> unit) -> t -> unit
    (** [column_storage_iteri j f mtrx] is a restriction of
    [column_iteri j f mtrx] to the elements effectively stored into the
    matrix, in an unspecified order. Provided for efficiency. *)
    val update_column_storage: int -> (int -> elt -> elt) -> t -> unit
    (** [update_column_storage j f mtrx] replaces the elements effectively
    stored into [mtrx] at column [j] by the results of [f] applyied to the
    current row index as first argument and the current element as
    second argument, in an unspecified order. Provided for efficiency. *)
  end

module type BipartiteGraph =
  sig
    type t
    (** The type of the bipartite graphs. *)

    val create : int -> t
    (** [create size] creates a bipartite graph of size [size]. *)

    val link : int -> int -> t -> unit
    (** [link i j bg] links the [i]th left-side node of [bg] to the [j]th
    right-side node of [bg]. If [i] or [j] are outside \[0, size) where size
    is the size of [bg], Invalid_argument is raised. *)

    val ford_and_fulkerson : t -> int * (int * int option) list
    (** [ford_and_fulkerson bg] performs the Ford and Fulkerson method over the
    bipartite graph [bg], returning a couple whose first element is the
    number of successful coupling an second one a list whose elements are of
    the form [(i, None)] if no right-side node could be associated to the
    [i]th left-side one and [(i, Some j)] in case of success. *)
  end

module type S =
  sig
    type elt
    (** the type of the objects manipulated by the method *)
    type t
     (** the type of the structure used to hold the data   *)
    val init: int -> (int -> int -> elt) -> t
    (** [init size fun] returns a structure on which it is possible
    to perform the Hungarian Method. [size] is the size of the square matrix
    used to perform the method. Given a pair of ints [(i, j)], [fun i j]
    returns the weight associated with [(i, j)]. *)
    val perform: t -> (int * int option) list
    (** [perform struct] effectively performs the Hungarian Method, given
    [struct] created using [init]. *)
    val print_with: (elt -> unit) -> t -> unit
    (** [print_with print_fun mtrx] prints the matrix used to perform the
    Hungarian Method. [print_fun] is the function that prints matrix elements.
    *)
  end

module Make
  (E: MatrixElement)
  (M: Matrix with type elt = E.t)
  (B: BipartiteGraph):
    (S with type elt = E.t)
