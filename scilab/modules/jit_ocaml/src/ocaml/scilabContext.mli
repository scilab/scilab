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

type t
type context
type symbol
type scope

(*
type box = {
  mutable box_value : ScilabValue.t;
  mutable box_counter : int;
 }

type symbol = {
  symbol_name : string;
  mutable symbol_binding : binding option;
}

and binding = {
             binding_name : string;
     mutable binding_locals : local_binding list;
     mutable binding_global : global_binding option;
}

and local_binding = {
    mutable local_box : box;
    local_scope : scope;
}


and global_binding = {
    mutable global_box : box;
    mutable global_scopes : scope list;
}

and scope = {
  scope_level : int;
     mutable scope_locals : binding list;
     mutable scope_globals : binding list;
}

and context = {
    mutable context_scopes : scope list; (* never [] *)
}
*)


exception ErrorUndefinedVariable of string
exception CannotResumeFromToplevelScope

val clear : context -> unit
val symbol_name : symbol -> string
val new_symbol : string -> symbol
val getInstance : unit -> context
val put : context -> symbol -> t -> unit
val get : context -> symbol -> t
val global : context -> symbol -> unit
val resume : context -> symbol -> t -> unit
val begin_scope : context -> unit
val end_scope : context -> unit
val current_scope : context -> scope
val restore_scope : context -> scope -> unit

val to_string : unit -> string

val set_context_create_empty_value : context -> (unit -> t) -> unit
val set_context_string_of_value : context -> (t -> string) -> unit
val set_context_refcount_setters :
  context -> (t -> t) -> (t -> unit) -> unit

