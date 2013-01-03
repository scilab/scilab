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

(*****
Where the context is also used in Scilab:
* Some functions from the foreigh api (api_scilab)
* getStructFromExp in visitor_common.cpp
* Set the "%onprompt" variable
* SciNotes completion
* Set the "nargout" and "nargin" variables
* "sci_clear", "sci_clearglobal", "sci_existsOrIsdef", "sci_global",
      "sci_isglobal", "sci_who"
* Add_{Poly,Double,Boolean,String}_Constant
* Dump stack trace
* "%ODEOPTIONS" of "sci_ode", "sci_odedc"
* "%_gsort" of "sci_gsort"
* "ans" in "sci_exec", "sci_execstr"
* "sci_funclist", "sci_whereis"
* "sci_load"-> getMacros()
* "%eps" in mexlib
* Setting "home", "SCIHOME", "SCI", "WSCI", "TMPDIR"
* sci_inspectorGetFunctionList
* Macro->call(), MacroFile->parse()

*****)

type t

type box = {
  mutable box_value : t;
  mutable box_refcount : int;
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
  mutable context_create_empty_value : (unit -> t);
  mutable context_string_of_value : (t -> string);
  mutable context_increase_refcount : (t -> t);
  mutable context_decrease_refcount : (t -> unit);

  mutable context_scopes : scope list; (* never [] *)
}

let set_context_create_empty_value ctx f =
  ctx.context_create_empty_value <- f
let set_context_string_of_value ctx f =
  ctx.context_string_of_value <- f
let set_context_refcount_setters ctx increase_ref decrease_ref =
  ctx.context_increase_refcount <- increase_ref;
  ctx.context_decrease_refcount <- decrease_ref;
  ()

let increase_box_refcount ctx box =
  box.box_refcount <- box.box_refcount + 1

let decrease_box_refcount ctx box =
  box.box_refcount <- box.box_refcount - 1;
  if box.box_refcount = 0 then
    ctx.context_decrease_refcount box.box_value

exception ErrorUndefinedVariable of string
exception CannotResumeFromToplevelScope

let new_symbol name = {
  symbol_name = name;
  symbol_binding = None;
}

let new_scope level = {
  scope_level = level;
  scope_locals = [];
  scope_globals = [];
}

let new_binding name = {
  binding_name = name;
  binding_locals = [];
  binding_global = None;
}

let symbols = Hashtbl.create 133
let context = {
  context_scopes = [new_scope 0];
  context_create_empty_value = (fun _ ->
    failwith "context_create_empty_value not initialized");
  context_increase_refcount = (fun v -> v);
  context_decrease_refcount = (fun _ -> ());
  context_string_of_value = (fun _ -> "<abstract>");
}

let getInstance () = context

let current_scope ctx =
    match ctx.context_scopes with
      [] -> assert false
    | scope :: _ -> scope


let begin_scope ctx =
  match ctx.context_scopes with
  [] -> assert false
  | s :: _ ->
    ctx.context_scopes <- (new_scope (s.scope_level+1)) :: ctx.context_scopes

let end_scope ctx =
  match ctx.context_scopes with
    [] -> assert false
  | scope :: scopes ->
     ctx.context_scopes <- scopes;
     List.iter (fun b ->
       match b.binding_locals with
        | [] -> assert false
        | { local_box = box; local_scope = s } :: locals ->
          assert (s == scope);
          decrease_box_refcount ctx box;
          b.binding_locals <- locals
     ) scope.scope_locals;
     List.iter (fun b ->
       match b.binding_global with
        | None -> assert false
        | Some g ->
	  match g.global_scopes with
	  | [] -> assert false
          | s :: globals ->
            assert (s == scope);
            g.global_scopes <- globals
     ) scope.scope_globals

let rec restore_scope context s0 =
  match context.context_scopes with
    [] -> assert false
  | scope :: scopes ->
    if scope != s0 then begin
      end_scope context;
      restore_scope context s0
    end

let get_symbol_binding sy =
  match sy.symbol_binding with
  | Some b -> b
  | None ->
    let b = try
              Hashtbl.find symbols sy.symbol_name
      with Not_found ->
        let b = new_binding sy.symbol_name in
        Hashtbl.add symbols sy.symbol_name b;
        b
    in
    sy.symbol_binding <- Some b;
    b

let get ctx sy =
  let s0 = current_scope ctx in
  let b = get_symbol_binding sy in

  match b.binding_global with
    | Some { global_scopes = ss :: _; global_box = box }
                             when ss == s0
        -> box.box_value
    | _ ->
      match b.binding_locals with
       | l0 :: _ -> l0.local_box.box_value
       | _ -> raise (ErrorUndefinedVariable sy.symbol_name)

let new_local s box =
  { local_box = box; local_scope = s }

let set_box_value ctx box v =
  if box.box_value != v then begin
    ctx.context_decrease_refcount box.box_value;
    let v = ctx.context_increase_refcount v in
    box.box_value <- v
  end

let new_box ctx v =
  let v = ctx.context_increase_refcount v in
  {
    box_value = v;
    box_refcount = 1;
  }

let put ctx sy v =
  let s0 = current_scope ctx in
  let b = get_symbol_binding sy in
  match b.binding_global with
    | Some { global_scopes = ss :: _; global_box = box }
                             when ss == s0
        -> set_box_value ctx box v
    | _ ->
      match b.binding_locals with
       | l0 :: _ when
           l0.local_scope == s0 -> set_box_value ctx l0.local_box v
       | locals ->
           let l0 = new_local s0 (new_box ctx v) in
           b.binding_locals <- l0 :: locals;
           s0.scope_locals <- b :: s0.scope_locals

(* [resume ctx sy v] assigns value [v] to symbol [sy] in the previous
   scope.

   In the implementation, we first lookup the previous scope [s1], and
   then search for it in the two first slots of the symbol bindings.
*)
let resume ctx sy v =
  match ctx.context_scopes with
  | [] -> assert false
  | [ _ ] -> raise CannotResumeFromToplevelScope
  | s0 :: s1 :: _ ->
    (* we will bind 'sy' to 'v' into scope 's1' *)
    let b = get_symbol_binding sy in
    match b.binding_locals with
    | l1 :: _ when
        l1.local_scope == s1 ->
      (* no scope 's0', first scope 'l1' is already 's1' ! *)
      set_box_value ctx l1.local_box v

    | l0 :: locals when
        l0.local_scope == s0 ->
      (* first scope is  's0', maybe 's1' is just after ? *)
      begin
        match locals with
        | l1 :: _ when
            l1.local_scope == s1 ->
          (* second scope is 's1' *)
          set_box_value ctx l1.local_box v

        | locals ->
          (* scope 's1' is not present after 's0' *)
          let l1 = new_local s1 (new_box ctx v) in
          b.binding_locals <- l0 :: l1 :: locals; (* insert 's1' after 's0' *)
          s1.scope_locals <- b :: s1.scope_locals
      end

    | locals ->
          (* scopes 's0' and 's1' are not available *)
      let l1 = new_local s1 (new_box ctx v) in
      b.binding_locals <- l1 :: locals;
      s1.scope_locals <- b :: s1.scope_locals

(* [global ctx sy] binds [sy] to a global value. *)
let global ctx sy =
  let s0 = current_scope ctx in
  let b = get_symbol_binding sy in

  (* first case, [sy] is not global, make it global ! *)
  match b.binding_global with
  | None ->
    let box =
      match b.binding_locals with
      | l0 :: _ when
          l0.local_scope == s0 ->
        let box = l0.local_box in
        box
      | _ ->
        let v = match b.binding_locals with
            [] -> ctx.context_create_empty_value ()
          | l :: _ ->
            l.local_box.box_value
        in
        let box = new_box ctx v in
        let l0 = new_local s0 box in
        s0.scope_locals <- b :: s0.scope_locals;
        b.binding_locals <- l0 :: b.binding_locals;
        box
    in
    increase_box_refcount ctx box;
    let g = {
      global_box = box;
      global_scopes = [s0];
    } in
    b.binding_global <- Some g;
    s0.scope_globals <- b :: s0.scope_globals

  | Some g ->

    match b.binding_locals with
    | l0 :: _ when
        l0.local_scope == s0 ->
      decrease_box_refcount ctx l0.local_box;
      l0.local_box <- g.global_box;
      increase_box_refcount ctx l0.local_box

    | _ ->
      let l0 = new_local s0 g.global_box in
      increase_box_refcount ctx g.global_box;
      s0.scope_locals <- b :: s0.scope_locals;
      b.binding_locals <- l0 :: b.binding_locals

(* remove all local scopes, and keep only the global one *)
let rec clear ctx =
  match ctx.context_scopes with
    [ _ ] -> ()
  | [] -> assert false
  | s :: _ -> end_scope ctx; clear ctx

let to_string () =

  let buf = Buffer.create 1000 in
  List.iter (fun s ->
    Printf.bprintf buf "Scope %d:\n" s.scope_level;
    List.iter (fun b ->
      Printf.bprintf buf "\t%s\n" b.binding_name
    ) s.scope_locals;
    List.iter (fun b ->
      Printf.bprintf buf "\t%s\n" b.binding_name
    ) s.scope_globals;

  ) context.context_scopes;

  Hashtbl.iter (fun _ b ->
    Printf.bprintf buf "Symbol %s:\n" b.binding_name;
    List.iter (fun l ->
      Printf.bprintf buf "\t%d -> %s\n" l.local_scope.scope_level
        (context.context_string_of_value l.local_box.box_value)
    ) b.binding_locals
  ) symbols;

  Buffer.contents buf

let symbol_name sy = sy.symbol_name
