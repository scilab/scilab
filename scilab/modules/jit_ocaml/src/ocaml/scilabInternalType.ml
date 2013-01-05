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
| RealSingleHandle (* unused *)
| RealHandle  (* RealGraphicHandle *)

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


external ocpsci_sci2ml_implicitlist_ml : t -> t * t * t =
  "ocpsci_sci2ml_implicitlist_c"
external ocpsci_ml2sci_implicitlist_ml : t -> t -> t -> t =
  "ocpsci_ml2sci_implicitlist_c"

(* extract the type of a scilab value *)
external ocpsci_get_RealType_ml : t -> realType = "noalloc"
  "ocpsci_get_RealType_c"

(* conversion between Scilab and OCaml doubles *)
external ocpsci_ml2sci_double_ml : float -> t = "ocpsci_ml2sci_double_c"
external ocpsci_ml2sci_float_ml : float -> t = "ocpsci_ml2sci_float_c"
external ocpsci_ml2sci_bool_ml : bool -> t = "ocpsci_ml2sci_bool_c"
external ocpsci_ml2sci_int8_ml : int32 -> t = "ocpsci_ml2sci_int8_c"
external ocpsci_ml2sci_int16_ml : int32 -> t = "ocpsci_ml2sci_int16_c"
external ocpsci_ml2sci_int32_ml : int32 -> t = "ocpsci_ml2sci_int32_c"
external ocpsci_ml2sci_string_ml : string -> t = "ocpsci_ml2sci_string_c"

external ocpsci_generic_getSize_ml : t -> int = "noalloc" "ocpsci_generic_getSize_c"

external ocpsci_sci2ml_double_ml : t -> int -> float = "ocpsci_sci2ml_double_c"
external ocpsci_sci2ml_bool_ml : t -> int -> bool = "ocpsci_sci2ml_bool_c"
external ocpsci_sci2ml_int8_ml : t -> int -> int32 = "ocpsci_sci2ml_int8_c"
external ocpsci_sci2ml_int16_ml : t -> int -> int32 = "ocpsci_sci2ml_int16_c"
external ocpsci_sci2ml_int32_ml : t -> int -> int32 = "ocpsci_sci2ml_int32_c"
external ocpsci_sci2ml_string_ml : t -> int -> string = "ocpsci_sci2ml_string_c"

external ocpsci_set_bool_ml : t -> int -> bool -> unit = "noalloc" "ocpsci_set_bool_c"
external ocpsci_set_double_ml : t -> int -> float -> unit = "noalloc" "ocpsci_set_double_c"
external ocpsci_set_int8_ml : t -> int -> int32 -> unit = "noalloc" "ocpsci_set_int8_c"
external ocpsci_set_int16_ml : t -> int -> int32 -> unit = "noalloc" "ocpsci_set_int16_c"
external ocpsci_set_int32_ml : t -> int -> int32 -> unit = "noalloc" "ocpsci_set_int32_c"

external ocpsci_sparsebool_set_ml : t -> int -> int -> bool -> unit =
  "noalloc" "ocpsci_sparsebool_set_c"
external ocpsci_sparsebool_get_ml : t -> int -> int -> bool =
  "noalloc" "ocpsci_sparsebool_get_c"

external ocpsci_empty_double_ml : unit -> t = "ocpsci_empty_double_c"

(* get the list of all available functions in the current scope *)
external ocpsci_get_funlist_ml : unit ->  string array = "ocpsci_get_funlist_c"

(* extract a value by name from the current scope *)
external ocpsci_context_get_ml : string -> t = "ocpsci_context_get_c"


external ocpsci_clone_ml : t -> t = "ocpsci_clone_c"

type ocaml_function = t array -> (string * t) array -> int -> t array option

external ocpsci_set_ocaml_functions_ml : ocaml_function array -> unit =
  "ocpsci_set_ocaml_functions_c"

external ocpsci_ml2sci_ocamlfunction_ml : string -> int -> t =
  "ocpsci_ml2sci_ocamlfunction_c"

(* call a Scilab function:
  [ocpsci_call_ml f args optionan_args nbrExpectedResults]
*)
external ocpsci_call_ml : t -> t array -> (string * t) array -> int -> t array option
  = "ocpsci_call_c"

(* generate a Scilab matrix of doubles from an OCaml matrix *)
external ocpsci_ml2sci_double_matrix_ml : float array array -> t =
  "ocpsci_ml2sci_double_matrix_c"

(* generate a Scilab matrix with a given size and initial value.
   We should probably use "ones(dy,dx) * initial" instead.
 *)
external ocpsci_create_double_matrix_ml : int -> int -> float -> t =
  "ocpsci_create_double_matrix_c"

(* perform a binary operation. Logical operations are not yet implemented *)
external ocpsci_operation_ml : binop -> t -> t -> t =
  "ocpsci_operation_c"

external ocpsci_refcount_ml : t -> int = "noalloc" "ocpsci_refcount_c"
external ocpsci_incr_refcount_ml : t -> unit = "noalloc" "ocpsci_incr_refcount_c"
external ocpsci_decr_refcount_ml : t -> unit = "noalloc" "ocpsci_decr_refcount_c"
external ocpsci_dollar_ml : unit -> t = "ocpsci_dollar_c"
external ocpsci_colon_ml : unit -> t = "ocpsci_colon_c"
external ocpsci_list_get_ml : t -> int -> t = "ocpsci_list_get_c"

external ocpsci_generic_getCols_ml : t -> int =
  "noalloc" "ocpsci_generic_getCols_c"
external ocpsci_generic_getRows_ml : t -> int =
  "noalloc" "ocpsci_generic_getRows_c"
external ocpsci_generic_getColumnValues_ml : t -> int -> t =
  "ocpsci_generic_getColumnValues_c"

external ocpsci_implicitlist_extractFullMatrix_ml : t -> t =
  "ocpsci_implicitlist_extractFullMatrix_c"

external ocpsci_arrayof_get_ml : t -> int -> t =
  "ocpsci_arrayof_get_c"

external ocpsci_arrayof_set_ml : t -> int -> t -> unit =
  "noalloc" "ocpsci_arrayof_set_c"

external ocpsci_map_ml : t -> t =  "ocpsci_map_c"

external ocpsci_new_bool_ml : int array -> t = "ocpsci_new_bool_c"
external ocpsci_generic_getDimsArray_ml : t -> int array = "ocpsci_generic_getDimsArray_c"

(*********************************************************************)
(*                                                                   *)
(*                                                                   *)
(*                 Higher order functions                            *)
(*                                                                   *)
(*                                                                   *)
(*********************************************************************)


let dummy_function _ _ _ = None
let ocaml_functions = ref [| |]
let function_counter = ref 0
let ocamlfunction name f =
  let function_num = !function_counter in
  function_counter := function_num + 1;
  if Array.length !ocaml_functions < !function_counter then begin
    let old_ocaml_functions = !ocaml_functions in
    let new_ocaml_functions = Array.create (2 * !function_counter + 16) dummy_function
    in
    Array.blit old_ocaml_functions 0 new_ocaml_functions 0 function_num;
    ocaml_functions := new_ocaml_functions;
    ocpsci_set_ocaml_functions_ml new_ocaml_functions
  end;
  (!ocaml_functions).(function_num) <- f;
  ocpsci_ml2sci_ocamlfunction_ml name function_num

let get_type = ocpsci_get_RealType_ml
let bool = ocpsci_ml2sci_bool_ml
let double = ocpsci_ml2sci_double_ml
let float = ocpsci_ml2sci_float_ml
let string = ocpsci_ml2sci_string_ml
let int8 = ocpsci_ml2sci_int8_ml
let int16 = ocpsci_ml2sci_int16_ml
let int32 = ocpsci_ml2sci_int32_ml
let implicitlist = ocpsci_ml2sci_implicitlist_ml


let isGeneric typ =
  match typ with
  | RealGeneric
  | RealContainer
  | RealList
  | RealTList
  | RealMList
  | RealFloat
  | RealSinglePoly
  | RealBool
  | RealCell
  | RealDouble
  | RealHandle (* RealGraphicHandle *)
  | RealInt8
  | RealInt16
  | RealInt32
  | RealInt64
  | RealPoly
  | RealDollar
  | RealString
  | RealStruct
  | RealUInt8
  | RealUInt16
  | RealUInt32
  | RealUInt64 -> true
  | _ -> false

let unsafe_get_double = ocpsci_sci2ml_double_ml
let unsafe_get_bool = ocpsci_sci2ml_bool_ml
let get_double t pos =
  match get_type t with
  | RealDouble ->  unsafe_get_double t pos
  | _ -> assert false

let get_bool t pos =
  match get_type t with
  | RealBool -> unsafe_get_bool t pos
  | _ -> assert false

let int32_0xff = Int32.of_int 0xff
let int32_0xffff = Int32.of_int 0xffff

let mod_int8 x = Int32.logand x int32_0xff
let mod_int16 x = Int32.logand x int32_0xffff

let add_int8 x y = mod_int8 (Int32.add x y)
let add_int16 x y = mod_int16 (Int32.add x y)
let add_int32 x y = Int32.add x y

let get_int8 t pos =
  match get_type t with
  | RealInt8 -> ocpsci_sci2ml_int8_ml t pos
  | RealInt16 -> mod_int8 (ocpsci_sci2ml_int16_ml t pos)
  | RealInt32 -> mod_int8 (ocpsci_sci2ml_int32_ml t pos)
  | RealDouble -> mod_int8 (Int32.of_float (ocpsci_sci2ml_double_ml t pos))
  | _ -> assert false

let get_int16 t pos =
  match get_type t with
  | RealInt16 -> ocpsci_sci2ml_int16_ml t pos
  | RealInt8 -> ocpsci_sci2ml_int8_ml t pos
  | RealInt32 -> mod_int16 (ocpsci_sci2ml_int32_ml t pos)
  | RealDouble -> mod_int16 (Int32.of_float (ocpsci_sci2ml_double_ml t pos))
  | _ -> assert false

let get_int32 t pos =
  match get_type t with
  | RealInt8 -> ocpsci_sci2ml_int8_ml t pos
  | RealInt16 -> ocpsci_sci2ml_int16_ml t pos
  | RealInt32 -> ocpsci_sci2ml_int32_ml t pos
  | RealDouble -> Int32.of_float (ocpsci_sci2ml_double_ml t pos)
  | _ -> assert false

let get_string t pos =
  match get_type t with
  | RealDouble -> ocpsci_sci2ml_string_ml t pos
  | _ -> assert false

let get_implicitlist t =
  match get_type t with
  | RealImplicitList -> ocpsci_sci2ml_implicitlist_ml t
  | _ -> assert false

let set_double t pos d =
  match get_type t with
  | RealDouble -> ocpsci_set_double_ml t pos d
  | _ -> assert false

let set_int8 t pos d =
  match get_type t with
  | RealInt8 -> ocpsci_set_int8_ml t pos d
  | _ -> assert false

let set_int16 t pos d =
  match get_type t with
  | RealInt16 -> ocpsci_set_int16_ml t pos d
  | _ -> assert false

let set_int32 t pos d =
  match get_type t with
  | RealInt32 -> ocpsci_set_int32_ml t pos d
  | _ -> assert false

let unsafe_set_double = ocpsci_set_double_ml
let unsafe_set_int8 = ocpsci_set_int8_ml
let unsafe_set_int16 = ocpsci_set_int16_ml
let unsafe_set_int32 = ocpsci_set_int32_ml

let empty_double = ocpsci_empty_double_ml
let call t args opt_args iRetCount =
  match get_type t with
  | RealFunction -> ocpsci_call_ml t args opt_args iRetCount
  | _ -> assert false

let clone = ocpsci_clone_ml

let to_string t = "<abstract>"

let operation = ocpsci_operation_ml

let refcount = ocpsci_refcount_ml
let incr_refcount = ocpsci_incr_refcount_ml
let decr_refcount = ocpsci_decr_refcount_ml

let get_size t =
  if isGeneric (get_type t) then ocpsci_generic_getSize_ml t
  else assert false

let unsafe_get_size =
  (* TODO: we should probably check that the type has a size ! *)
  ocpsci_generic_getSize_ml


let iterator_of_implicitlist t =
  match get_type t with
  | RealImplicitList ->
    let (start, step, stop) = get_implicitlist t in
    let iterator =
      fun get add create set to_string ->
        let start = get start 0 in
        let step = get step 0 in
        let stop = get stop 0 in
        let r = ref start in
        let v = create start in
(*
        Printf.eprintf "Iterator: %s:%s:%s\n%!\n%!"
          (to_string start)
          (to_string step)
          (to_string stop);
*)
        if !r <= stop then
          Some (fun () ->
(*
            Printf.eprintf "  test %s <= %s\n%!"
              (to_string !r) (to_string stop);
*)
            if !r <= stop then begin
              set v 0 !r;
              r := add !r step;
              Some v
            end else None)
        else None
    in
    begin
      match get_type start,
        get_type step,
        get_type stop with
        | RealDouble, RealDouble, RealDouble ->
          iterator get_double (+.) double unsafe_set_double string_of_float
        | RealInt8, _, _
        | RealDouble, RealInt8, _
        | RealDouble, RealDouble, RealInt8 ->
          iterator get_int8 add_int8 int8 unsafe_set_int8 Int32.to_string
        | RealInt16, _, _
        | RealDouble, RealInt16, _
        | RealDouble, RealDouble, RealInt16 ->
          iterator get_int16 add_int16 int8 unsafe_set_int16 Int32.to_string
        | RealInt32, _, _
        | RealDouble, RealInt32, _
        | RealDouble, RealDouble, RealInt32 ->
          iterator get_int32 add_int32 int32 unsafe_set_int32 Int32.to_string
        | _ -> assert false
    end
  | _ -> assert false

let iterator_of_list t =
  match get_type t with
  | RealList
  | RealTList
  | RealMList ->
    let size = unsafe_get_size t in
    if size = 0 then None else
      let count = ref 0 in
      Some (fun () ->
        let pos = !count in
        if pos < size then begin
          incr count;
          Some (ocpsci_list_get_ml t pos)
        end else None
      )
  | _ -> assert false

let iterator_of_generic t =
  if isGeneric (get_type t) then
    let size = ocpsci_generic_getCols_ml t in
    if size = 0 then None else
      let count = ref 0 in
      Some (fun () ->
        let pos = !count in
        if pos < size then begin
          incr count;
          Some (ocpsci_generic_getColumnValues_ml t pos)
        end else None
      )
  else
    assert false

let rec is_true t =
  match get_type t with
  | RealDouble ->
    let size = unsafe_get_size t in
    if size = 0 then false else
      let rec iter t pos size =
        if pos = size then true else
          if unsafe_get_double t pos = 0. then false else
            iter t (pos+1) size
      in
      iter t 0 size
  | RealBool ->
    let size = unsafe_get_size t in
    if size = 0 then false else
      let rec iter t pos size =
        if pos = size then true else
          if unsafe_get_bool t pos then
            iter t (pos+1) size
          else false
      in
      iter t 0 size
  | RealImplicitList ->
    begin match iterator_of_implicitlist t with
      None -> false (* empty list *)
    | Some iterator ->
      let rec iter iterator =
        match iterator () with
          None -> true
        | Some v ->
          if is_true v then iter iterator else false
      in
      iter iterator
    end
  | _ -> false


let get_funlist = ocpsci_get_funlist_ml
let context_get = ocpsci_context_get_ml

let dollar = ocpsci_dollar_ml
let colon = ocpsci_colon_ml

let list_get t pos =
  match get_type t with
    RealList
  | RealMList
  | RealTList ->
    ocpsci_list_get_ml t pos
  | _ -> assert false

let extractFullMatrix t =
  match get_type t with
    RealImplicitList -> ocpsci_implicitlist_extractFullMatrix_ml t
  | _ -> t

let not_exp t =
  match get_type t with
  | RealDouble ->
    let t2 = ocpsci_new_bool_ml (ocpsci_generic_getDimsArray_ml t) in
    for i = 0 to ocpsci_generic_getSize_ml t - 1 do
      let d = ocpsci_sci2ml_double_ml t i in
      ocpsci_set_double_ml t2 i
        (if d = 0. then 1. else 0.)
    done;
    t2
  | RealBool ->
    let t2 = ocpsci_new_bool_ml (ocpsci_generic_getDimsArray_ml t) in
    for i = 0 to ocpsci_generic_getSize_ml t - 1 do
      let b = ocpsci_sci2ml_bool_ml t i in
      ocpsci_set_bool_ml t2 i (not b)
    done;
    t2
  | RealSparseBool ->
    let t2 = ocpsci_map_ml t in
    for row = 0 to ocpsci_generic_getRows_ml t - 1 do
      for col = 0 to ocpsci_generic_getCols_ml t - 1 do
        let b = ocpsci_sparsebool_get_ml t row col in
        ocpsci_sparsebool_set_ml t2 row col (not b)
      done;
    done;
    t2
  | _ -> assert false


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

(* We use Scilab 'disp' function to print values on stdout. *)


let multiply_matrix_by_double matrix double =
  let _d = ocpsci_ml2sci_double_ml double in
  ()

(* We sort an array of Scilab doubles. It works because our
generic comparison is compatible with Scilab comparison on
scalars. It would not work on matrices. *)
let test_double_and_compare () =
  let list = ref [] in
  for i = 1 to 1000 do
    let d1 = Random.float 1000. in
    let s = ocpsci_ml2sci_double_ml d1 in
    let d2 = ocpsci_sci2ml_double_ml s 0 in
    list := s :: !list;
    Printf.fprintf stderr "%f --> %f\n%!" d1 d2;
  done;
  let list = List.sort compare !list in
  List.iter (fun d ->
    let d = ocpsci_sci2ml_double_ml d 0 in
    Printf.fprintf stderr "%f %!" d;
  ) list;
  Printf.fprintf stderr "\n%!"


let print_all_primitives () =
  let fun_names = ocpsci_get_funlist_ml () in

  let _map = Array.mapi (fun  i s ->
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
  let d = double 2.5 in
  let m = ocpsci_ml2sci_double_matrix_ml
          [|
            [| 1.; 2.; 3. |];
            [| 5.; 6.; 7. ; 8. |];
          |] in
  let r = ocpsci_operation_ml Times d m in
  disp d;
  disp m;
  disp r;

