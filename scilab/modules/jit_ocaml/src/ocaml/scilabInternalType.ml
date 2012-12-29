(*

  The goal of this module is to provide easy access to Scilab
  gateways.

  Scilab values are stored as finalizable OCaml custom types, for
  which the finalizer decreases the ref counter, and if it is not
  referenced anymore, destroy the value.

  The polymorphic compare function will work on scalar values, but not
  on matrices. To be improved.

  We have to be careful, as some functions only work on specific
  types, and we currently don't check runtime types. For example,
  ocpsci_sci2ml_double_ml assumes that the Scilab value is a Scilab
  Double, and will silently fail, with unspecified behavior, if it is
  not the case.

*)

(* This is an abstract type, giving access to Scilab internal
   type.  *)
type t

type binop =
  Unknown_oper (* first, because our C code starts with 1 for plus *)

            (* Arithmetics *)
            (* "+" *)        | Plus
            (* "-" *)        | Minus
            (* "*" *)        | Times
            (* "/" *)        | Rdivide
            (* \  *)         | Ldivide
            (* "**" or "^" *)| Power

            (*                       Unary minus *)
            (* "-" *)        | UnaryMinus

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


let string_of_realType = function
| RealInternal -> "RealInternal"
| RealGeneric -> "RealGeneric"
| RealInt8 -> "RealInt8"
| RealUInt8 -> "RealUInt8"
| RealInt16 -> "RealInt16"
| RealUInt16 -> "RealUInt16"
| RealInt32 -> "RealInt32"
| RealUInt32 -> "RealUInt32"
| RealInt64 -> "RealInt64"
| RealUInt64 -> "RealUInt64"
| RealString -> "RealString"
| RealDouble -> "RealDouble"
| RealBool -> "RealBool"
| RealFloat -> "RealFloat"
| RealPoly -> "RealPoly"
| RealSinglePoly -> "RealSinglePoly"
| RealFunction -> "RealFunction"
| RealMacro -> "RealMacro"
| RealMacroFile -> "RealMacroFile"
| RealImplicitList -> "RealImplicitList"
| RealContainer -> "RealContainer"
| RealList -> "RealList"
| RealTList -> "RealTList"
| RealMList -> "RealMList"
| RealSingleStruct -> "RealSingleStruct"
| RealStruct -> "RealStruct"
| RealCell -> "RealCell"
| RealUserType -> "RealUserType"
| RealListOperation -> "RealListOperation (* parent type *)"
| RealListInsertOperation -> "RealListInsertOperation"
| RealListDeleteOperation -> "RealListDeleteOperation"
| RealListUndefinedOperation -> "RealListUndefinedOperation"
| RealFile -> "RealFile"
| RealColon -> "RealColon"
| RealDollar -> "RealDollar"
| RealThreadId -> "RealThreadId"
| RealSparse -> "RealSparse"
| RealSparseBool -> "RealSparseBool"
| RealSingleHandle -> "RealSingleHandle"
| RealHandle -> "RealHandle"
| RealUnknown -> "RealUnknown"


external ocpsci_get_RealType_ml : t -> realType = "noalloc"
  "ocpsci_get_RealType_c"

external ocpsci_ml2sci_double_ml : float -> t = "ocpsci_ml2sci_double_c"
external ocpsci_sci2ml_double_ml : t -> float = "ocpsci_sci2ml_double_c"


external ocpsci_ml2sci_bool_ml : bool -> t = "ocpsci_ml2sci_bool_c"
external ocpsci_sci2ml_bool_ml : t -> bool = "ocpsci_sci2ml_bool_c"
external ocpsci_get_funlist_ml : unit ->  string array = "ocpsci_get_funlist_c"
external ocpsci_context_get_ml : string -> t = "ocpsci_context_get_c"

external ocpsci_call_ml :
  t -> t array -> (string * t) array -> int -> t array option
    = "ocpsci_call_c"

external ocpsci_ml2sci_double_matrix_ml : float array array -> t =
  "ocpsci_ml2sci_double_matrix_c"
external ocpsci_create_double_matrix_ml : int -> int -> float -> t =
  "ocpsci_create_double_matrix_c"
external ocpsci_operation_ml : binop -> t -> t -> t =
  "ocpsci_operation_c"


(*********************************************************************)
(*                                                                   *)
(*                                                                   *)
(*                            TESTS                                  *)
(*                                                                   *)
(*                                                                   *)
(*********************************************************************)

(* The first milestone should be:

- create a Scilab double from OCaml
- create a Scilab matrix from OCaml
- Multiply the Scilab matrix by the Scilab double
- Recover an OCaml matrix from the Scilab matrix
*)



let multiply_matrix_by_double matrix double =
  let d = ocpsci_ml2sci_double_ml double in
  ()

(* We sort an array of Scilab doubles. It works because our
generic comparison is compatible with Scilab comparison on
scalars. It would not work on matrices. *)
let test_double_and_compare () =
  let list = ref [] in
  for i = 1 to 1000 do
    let d1 = Random.float 1000. in
    let s = ocpsci_ml2sci_double_ml d1 in
    let d2 = ocpsci_sci2ml_double_ml s in
    list := s :: !list;
    Printf.fprintf stderr "%f --> %f\n%!" d1 d2;
  done;
  let list = List.sort compare !list in
  List.iter (fun d ->
    let d = ocpsci_sci2ml_double_ml d in
    Printf.fprintf stderr "%f %!" d;
  ) list;
  Printf.fprintf stderr "\n%!"


let print_all_primitives () =
  let fun_names = ocpsci_get_funlist_ml () in

  let map = Array.mapi (fun  i s ->
    Printf.fprintf stderr "Primitive %S%!"
      (ScilabMisc.string_of_unicode s);
    try
      let f = ocpsci_context_get_ml s in
      let t = ocpsci_get_RealType_ml f in
      Printf.fprintf stderr " OK type %S\n%!"
      (string_of_realType t);

      (s, Some f)
    with Not_found ->
      Printf.fprintf stderr " NOT FOUND\n%!";
      (s, None)
  ) fun_names
  in
  ()

let disp t =
  let disp = ocpsci_context_get_ml (ScilabMisc.unicode_of_ascii "disp") in
  let res = ocpsci_call_ml disp [| t |] [||] 0 in
  match res with
    Some _ -> Printf.fprintf stderr "disp OK\n%!"
  | None -> Printf.fprintf stderr "disp ERRRO\n%!"

let multiply_matrix_per_double () =
  let d = ocpsci_ml2sci_double_ml 2.5 in
  let m = ocpsci_ml2sci_double_matrix_ml
          [|
            [| 1.; 2.; 3. |];
            [| 5.; 6.; 7. ; 8. |];
          |] in
  let r = ocpsci_operation_ml Times d m in
  disp d;
  disp m;
  disp r;
