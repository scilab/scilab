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
