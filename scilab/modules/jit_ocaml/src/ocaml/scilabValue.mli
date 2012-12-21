
module TYPES : sig

  type t =
  | Scalar of scalar
  | Matrix of matrix
  | List of t * t * t
  | Macro of string * (t array -> t array)
  | String of string

  and matrix =
    {
      matrix_dims : int;
      matrix_sizes : int array;
      matrix_matrix : t array;
    }

  and scalar =
    Int of int32
  | Double of float
  | Bool of bool

end

open TYPES

type t = TYPES.t

val empty : t
val to_string : t -> string
val bool : bool -> t
val int : int32 -> t
val double : float -> t
val string : string -> t


val lt : t -> t -> bool
val add : t -> t -> t

