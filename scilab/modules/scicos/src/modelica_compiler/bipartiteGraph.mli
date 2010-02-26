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

(** This module provides the necessary structures and functions to solve a
simple assignment problem using the Ford and Fulkerson method. *)

type t
(** The type of the bipartite graphs. *)

val create : int -> t
(** [create size] creates a bipartite graph of size [size]. *)

val link : int -> int -> t -> unit
(** [link i j bg] links the [i]th left-side node of [bg] to the [j]th
right-side node of [bg]. If [i] or [j] are outside \[0, size) where size
is the size of [bg], Invalid_argument is raised. *)

val fill : int -> int -> t -> unit
(** [fill i j bg] fills the edge between the [i]th and the [j]th nodes. If
the edge doesn't exists, Not_found is raised. If [i] or [j] are outside
\[0, size) where size is the size of [bg], Invalid_argument is raised. *)

val ford_and_fulkerson : t -> int * (int * int option) list
(** [ford_and_fulkerson bg] performs the Ford and Fulkerson method over the
bipartite graph [bg], returning a couple whose first element is the
number of successful coupling an second one a list whose elements are of
the form [(i, None)] if no right-side node could be associated to the
[i]th left-side one and [(i, Some j)] in case of success. *)
