(*
 *  Translator from Modelica 2.x to flat Modelica
 *
 *  Copyright (C) 2005 - 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

(* Exception handler module *)

open Parser

type execution_step =
  | NameResolution
  | Instantiation of Instantiation.instance_nature
  | CodeGeneration of Instantiation.instance_nature
  | SyntacticAnalysis

let display s =
  print_string (MsgDico.translate s)

let print_offset =
  function s ->
      Printf.printf "\n";
      for i = 1 to 3 do print_string s done;
      Printf.printf " "

let display_header exn err_msg =
  let display_code exn err_msg =
    let rec display_code' err_msg =
      match err_msg with
        | [] -> ()
        | s :: err_msg when s.[0] = '_' -> display (ErrorDico.getCode exn s)
        | _ :: err_msg -> display_code' err_msg in
    Printf.printf "\n";
    display "_ERROR";
    Printf.printf " ";
    display_code' err_msg;
    Printf.printf ":" in
  let display_message err_msg =
    let display_elem s =
      match s with
        | "" -> ()
        | s when s.[0] = '_' -> display s; Printf.printf " "
        | s ->
            Printf.printf "\"";
            print_string s;
            Printf.printf "\" " in
    let rec display_message' err_msg =
      match err_msg with
        | [] -> ()
        | s :: err_msg ->
            display_elem s;
            display_message' err_msg in
    print_offset("-");
    display_message' err_msg in
  display_code exn err_msg;
  display_message err_msg
  
let display_info err_info =
  let display_value sn sv =
    match sv with
      | "" -> ()
      | s when s.[0] = '_' -> display s
      | s when sn = "_ExprKind" -> Printf.printf "\"%s\"" s
      | s -> print_string s in
  let rec display_info' err_info =
    match err_info with
      | [] -> ()
      | (sn, sv) :: err_info ->
          print_offset("-");
          display sn;
          Printf.printf ": ";
          display_value sn sv;
          display_info' err_info in
  Printf.printf "\n";
  display_info' err_info

(*let string_of_path path =
  let rec string_of_path' elem path = match elem, path with
    | None, []
    | Some (Instantiation.Name _), [] -> ""
    | Some (Instantiation.Index _), [] -> "]"
    | None, (Instantiation.Name s) :: path ->
        s ^ (string_of_path' (Some (Instantiation.Name s)) path)
    | Some (Instantiation.Name _), (Instantiation.Name s) :: path ->
        "." ^ s ^ (string_of_path' (Some (Instantiation.Name s)) path)
    | Some (Instantiation.Index _), (Instantiation.Name s) :: path ->
        "]." ^ s ^ (string_of_path' (Some (Instantiation.Name s)) path)
    | Some (Instantiation.Index _), (Instantiation.Index i) :: path ->
        ", " ^ (string_of_int i) ^
        (string_of_path' (Some (Instantiation.Index i)) path)
    | _, (Instantiation.Index i) :: path ->
        "[" ^ (string_of_int i) ^
        (string_of_path' (Some (Instantiation.Index i)) path) in
  string_of_path' None path*)

let last path =
  let rec last' id path = match path with
    | [] -> id
    | (Instantiation.Name id) :: path -> last' id path
    | (Instantiation.Index _) :: path -> last' id path in
  last' "" path

let loc_info loc =
  match loc.filename with
    | Parser.CommandLine ->
        [("_Source", "_CommandLine");
         ("_CharacterPosition", string_of_int (loc.Parser.start + 1))]
    | Parser.LibraryFile lib_file ->
        let linenum, linebeg =
          Linenum.for_position lib_file loc.Parser.start in
        [("_Source", lib_file);
         ("_LineNumber", string_of_int linenum);
         ("_ColumnNumber", string_of_int (loc.Parser.start - linebeg + 1))]

let class_name_info instance_nature = match instance_nature with
  | Instantiation.ClassElement -> []
  | Instantiation.ComponentElement s -> [("_ClassName", s)]

let exec_step_info step opath =
  let string_of_step = match step with
    | NameResolution -> "_NameResolution"
    | Instantiation Instantiation.ClassElement ->
        "_InstantiationOfClass"
    | Instantiation Instantiation.ComponentElement _ ->
        "_InstantiationOfComponent"
    | CodeGeneration Instantiation.ClassElement ->
        "_CodeGenerationForClass"
    | CodeGeneration Instantiation.ComponentElement _ ->
        "_CodeGenerationForComponent"
    | SyntacticAnalysis -> "_SyntacticAnalysis" in
  let path_info = match opath with
    | None -> ""
    | Some path -> " \"" ^ (last path) ^ "\"" in
  [("_ExecutionStep", (MsgDico.translate string_of_step) ^ path_info)]

(* This function is'nt called if all exception types are correctly handled. *)
let handle_unhandledException exn =
  Printf.printf "\nUnhandled exception: %s\n" (Printexc.to_string exn)

let handle exn =
  let handle' =
    match exn with
      | NameResolve.CompilError { NameResolve.err_msg = err_msg;
                                  NameResolve.err_info = err_info;
                                  NameResolve.err_ctx = ctx } ->
          display_header exn err_msg;
          display_info (exec_step_info NameResolution None);
          display_info
            (loc_info ctx.NameResolve.location);
          display_info err_info;
          Printf.printf "\n"
      | Instantiation.InstantError { Instantiation.err_msg = err_msg;
                                     Instantiation.err_info = err_info;
                                     Instantiation.err_ctx = ctx } ->
          display_header exn err_msg;
          let opath = (Some ctx.Instantiation.path)
          and step = Instantiation ctx.Instantiation.instance_nature in
          display_info (exec_step_info step opath);
          display_info
            ((class_name_info ctx.Instantiation.instance_nature) @
            (loc_info ctx.Instantiation.location));
          display_info err_info;
          Printf.printf "\n"
      | ErrorDico.GenericError { ErrorDico.err_msg = err_msg;
                                 ErrorDico.err_info = err_info;
                                 ErrorDico.err_ctx = ctx } ->
          display_header exn err_msg;
          let step = CodeGeneration ctx.ErrorDico.instance_nature in
          display_info (exec_step_info step (Some ctx.ErrorDico.path));
          display_info
            ((class_name_info ctx.ErrorDico.instance_nature) @
            (loc_info ctx.ErrorDico.location));
          display_info err_info;
          Printf.printf "\n"
      | Parser.SyntacticError { Parser.err_msg = err_msg;
                                Parser.err_info = err_info;
                                Parser.err_ctx = ctx } ->
          display_header exn err_msg;
          display_info (exec_step_info SyntacticAnalysis None);
          display_info
            (loc_info ctx.Parser.location);
          display_info err_info;
          Printf.printf "\n"
      | _ -> 
          handle_unhandledException exn in
  handle'

