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
    type t
    val init: int -> elt -> (int -> int -> elt) -> t
    val iterij: (int -> int -> elt -> unit) -> t -> unit
    val row_storage_iterj: int -> (int -> elt -> unit) -> t -> unit
    val update_row_storage: int -> (int -> elt -> elt) -> t -> unit
    val column_storage_iteri: int -> (int -> elt -> unit) -> t -> unit
    val update_column_storage: int -> (int -> elt -> elt) -> t -> unit
  end

module type BipartiteGraph =
  sig
    type t
    val create: int -> t
    val link: int -> int -> t -> unit
    val ford_and_fulkerson: t -> int * (int * int option) list
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
