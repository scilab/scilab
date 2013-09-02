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

(** This module implements a sparse representation of square matrices using
list associations. The retrieval of an element takes O(m) where m is the
number of elements on the current row that are not equal to the default
element (in the sense of the compare function provided by a module
furnishing matrix elements). *)

(* Functorial interface *)

(** The module type that specifies the interface of a suitable module
providing matrix elements. *)
module type MatrixElement =
  sig
    type t
    val equal: t -> t -> bool
  end

module type S =
  sig
    type elt
    (** the type of the elements of the matrix *)
    type t
    (** the type of the sparse matrices        *)
    val make: int -> elt -> t
    (** [make size default] creates a square sparse matrix of
    [size]*[size] elements whose default element is [default]. *)
    val init: int -> elt -> (int -> int -> elt) -> t
    (** [init size default f] creates a square sparse matrix of
    [size]*[size] elements whose default element is [default] and whose
    initial elements are given by [f] ([f i j] returns the element to be
    placed at ([i], [j])). *)
    val size: t -> int * int
    (** [size mtrx] returns a couple whose first element is the number of
    rows of [mtrx] and second element the number of columns. In this module
    the two values are always equal, since matrices are square. *)
    val get: t -> int -> int -> elt
    (** [get mtrx i j] returns the element stored into [mtrx] at
    ([i], [j])). *)
    val set: t -> int -> int -> elt -> unit
    (** [set mtrx i j elt] stores [elt] in [mtrx] at ([i], [j])). *)
    val iter: (elt -> unit) -> t -> unit
    (** [iter f mtrx] applies [f] in turn to all the elements of [mtrx].
    Traversal is performed by iterating over the rows starting at 0. *)
    val iterij: (int -> int -> elt -> unit) -> t -> unit
    (** [iterij f mtrx] behaves like [iter f mtrx] except that f receives
    the row index as first argument, the column index as second argument
    and the current element as third argument. *)
    val row_iter: int -> (elt -> unit) -> t -> unit
    (** [row_iter i f mtrx] is a restriction of [iter f mtrx] to row
    [i]. *)
    val row_iterj: int -> (int -> elt -> unit) -> t -> unit
    (** [row_iterj i f mtrx] is a restriction of [iterij f mtrx] to row
    [i]. *)
    val row_storage_iterj: int -> (int -> elt -> unit) -> t -> unit
    (** [row_storage_iterj i f mtrx] is a restriction of
    [row_iterj i f mtrx] to the elements effectively stored into the matrix,
    in an unspecified order. Provided for efficiency. *)
    val update_row_storage: int -> (int -> elt -> elt) -> t -> unit
    (** [update_row_storage i f mtrx] replaces the elements effectively
    stored into [mtrx] at row [i] by the results of [f] applyied to the
    current column index as first argument and the current element as
    second argument, in an unspecified order. Provided for efficiency. *)
    val column_iter: int -> (elt -> unit) -> t -> unit
    (** [column_iter j f mtrx] is a restriction of [iter f mtrx] to
    column [j]. *)
    val column_iteri: int -> (int -> elt -> unit) -> t -> unit
    (** [column_iteri j f mtrx] is a restriction of [iterij f mtrx] to
    column [j]. *)
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

module Make (M: MatrixElement): (S with type elt = M.t)
