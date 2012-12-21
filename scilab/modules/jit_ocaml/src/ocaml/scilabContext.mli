
type context
type symbol

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

val clear : unit -> unit
val symbol_name : symbol -> string
val new_symbol : string -> symbol
val getInstance : unit -> context
val put : context -> symbol -> ScilabValue.t -> unit
val get : context -> symbol -> ScilabValue.t
val global : context -> symbol -> unit
val resume : context -> symbol -> ScilabValue.t -> unit
val begin_scope : unit -> unit
val end_scope : unit -> unit

val to_string : unit -> string
