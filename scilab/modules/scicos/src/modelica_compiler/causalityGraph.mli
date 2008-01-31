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
(** [print_with print_fun g] prints the connexions in [g] using [print_fun]. *)