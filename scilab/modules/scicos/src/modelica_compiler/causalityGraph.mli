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

(** This module provides a graph structure over which it is possible to apply
an algorithm that finds the strongly connected components of this graph. *)

type t
(** The type of the graph used to perform the strongly connected component
finding algorithm. *)

val create: int -> t
(** [create size] creates a graph with [size] unconnected nodes. *)

val connect: int -> int -> t -> unit
(** [connect i j g] connects the [i]th node to the [j]th one in g. *)

val strongly_connected_components: t -> int list list
(** [strongly_connected_components g] returns the stronly connected components
of [g] as a list of index lists. *)

val print_with: (int -> unit) -> t -> unit
(** [print_with print_fun g] prints the connections in [g] using [print_fun]. *)
