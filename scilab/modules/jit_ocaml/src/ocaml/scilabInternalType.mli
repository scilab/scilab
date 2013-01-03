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

type t = ScilabContext.t

type binop =
  Unknown_oper (* first, because our C code starts with 1 for plus *)

            (* Arithmetics *)
            (* "+" *)        | Plus
            (* "-" *)        | Minus
            (* "*" *)        | Times
            (* "/" *)        | Rdivide
            (* \  *)         | Ldivide
            (* "**" or "^" *)| Power

            (*                       Element Ways     *)
            (* ".*" *)       | Dottimes
            (* "./" *)       | Dotrdivide
            (* .\ *)         | Dotldivide (* not run. used ? *)
            (* ".^" *)       | Dotpower

            (* Kroneckers *)
            (* ".*." *)      | Krontimes
            (* "./." *)      | Kronrdivide
            (* ".\." *)      | Kronldivide

            (*                       Control *)
            (* FIXME : What the hell is this ??? *)
            (* "*." *)       | Controltimes (* not run. used ? *)
            (* "/." *)       | Controlrdivide  (* not run. used ? *)
            (* "\." *)       | Controlldivide (* not run. used ? *)

            (*                       Comparison     *)
            (* "==" *)       | Eq
           (* "<>" or "~=" *)| Ne
            (* "<" *)        | Lt
            (* "<=" *)       | Le
            (* "<" *)        | Gt
            (* ">=" *)       | Ge

            (*                       Unary minus *)
            (* "-" *)        | UnaryMinus

            (*                       Logical operators *)
            (* "&" *)   | LogicalAnd
            (* "|" *)   | LogicalOr
            (* "&&" *)  | LogicalShortCutAnd
            (* "||" *)  | LogicalShortCutOr


type realType =
(* Internal Type *)
| RealInternal
            (* Generic Types *)
| RealGeneric
| RealInt8
| RealUInt8
| RealInt16
| RealUInt16
| RealInt32
| RealUInt32
| RealInt64
| RealUInt64
| RealString
| RealDouble
| RealBool
| RealFloat
| RealPoly
| RealSinglePoly
            (* Callable *)
| RealFunction
| RealMacro
| RealMacroFile
            (* Implicit List *)
| RealImplicitList
            (* Container *)
| RealContainer
| RealList
| RealTList
| RealMList
| RealSingleStruct
| RealStruct
| RealCell
            (* User *)
| RealUserType
            (*For list operation*)
| RealListOperation (* parent type *)
| RealListInsertOperation
| RealListDeleteOperation
| RealListUndefinedOperation
| RealFile
| RealColon
| RealDollar
| RealThreadId
| RealSparse
| RealSparseBool
| RealSingleHandle
| RealHandle

| RealUnknown (* Unknown Scilab type ! *)

val string_of_realType : realType -> string
val get_type : t -> realType
val bool : bool -> t
val double : float -> t
val float : float -> t
val string : string -> t
val int8 : int32 -> t
val int16 : int32 -> t
val int32 : int32 -> t
val implicitlist : t -> t -> t -> t (* start -> step -> end -> list *)

val get_implicitlist : t -> t * t * t (* start, step, end *)

val get_double : t -> int -> float
val get_string : t -> int -> string
val get_int8 : t -> int -> int32
val get_int16 : t -> int -> int32
val get_int32 : t -> int -> int32
val get_bool : t -> int -> bool

val add_int8 : int32 -> int32 -> int32
val add_int16 : int32 -> int32 -> int32
val add_int32 : int32 -> int32 -> int32

val unsafe_set_double : t -> int -> float -> unit
val unsafe_set_int8 : t -> int -> int32 -> unit
val unsafe_set_int16 : t -> int -> int32 -> unit
val unsafe_set_int32 : t -> int -> int32 -> unit

val set_double : t -> int -> float -> unit
val set_int8 : t -> int -> int32 -> unit
val set_int16 : t -> int -> int32 -> unit
val set_int32 : t -> int -> int32 -> unit

val unsafe_get_size : t -> int

val empty_double : unit -> t

val to_string : t -> string

type ocaml_function = t array -> (string * t) array -> int -> t array option
val ocamlfunction : string -> ocaml_function -> t
val call : t -> ocaml_function
val clone : t -> t
val operation : binop -> t -> t -> t

val refcount : t -> int
val incr_refcount : t -> unit
val decr_refcount : t -> unit

val is_true : t -> bool

val get_funlist : unit -> string array
val context_get : string -> t

val iterator_of_implicitlist : t -> (unit -> t option) option
