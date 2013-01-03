(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

module TYPES : sig

  type t =
  | Scalar of scalar
  | Matrix of matrix
  | ImplicitList of t * t * t
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
  | Float of float
  | Bool of bool

end

open TYPES

type t = TYPES.t

val empty : t
val to_string : t -> string
val bool : bool -> t
val int : int32 -> t
val double : float -> t
val float : float -> t
val string : string -> t


val lt : t -> t -> bool
val add : t -> t -> t
val is_true : t -> bool
