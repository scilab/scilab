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

(** Main module *)

(** {4 compiler options } *)

(** List of library files added using the "-lib" compiler option *)
let lib_files = ref []

let add_library_file s = lib_files := !lib_files @ [s]

(** Generation of and XML description of the instantiated model *)
let xml = ref false

(** option for the generation of initialization code *)
let init = ref false

(** option "-command" value *)
let command = ref None

let define_command s =
  if !command <> None then failwith "More than one command issued."
  else command := Some s

(** target file name, option "-o" *)
let filename = ref None

let define_filename s =
  if !filename <> None then failwith "More than one target filename specified."
  else filename := Some s

(** {4 functions } *)

(** Error displayed when the compiler is called with an invalid argument *)
let set_input s = failwith ("Invalid command line argument: " ^ s)

(** Parsing of compiler command line arguments *)
let parse_args () =
  Arg.parse
    [("-lib", Arg.String add_library_file,
      "<filename> Define <filename> to be a file containing a Modelica \
      library of classes.");
     ("-o", Arg.String define_filename,
      "<filename> Specify target filename (default is `<instance name>.mo`).");
     ("-command", Arg.String define_command,
      "\"command\" Define the component to be created as a regular Modelica \
      component declaration.");
     ("-xml", Arg.Set xml,
      "Generate an XML version of the model instead of flat Modelica code");
     ("-with-init", Arg.Set init,
      "Generate an XML version of the initialization problem in a file \
      named <filename>_init.xml.")]
    set_input
    (Printf.sprintf
       "usage: %s [-lib <filename>] [-o <filename>] [-fundir <dirname>] \
        -command \"command\""
       Sys.argv.(0))

(** Compilation of libraries *)
let compile_libraries lib_paths =
  try
    let lib_syns = LibraryManager.parse_stored_libraries lib_paths in
    let comp_defs' = NameResolve.resolve_toplevel [] lib_syns in
    let inst_defs' =
      Instantiation.evaluate_toplevel_definitions [] comp_defs' in
    comp_defs', inst_defs'
  with
    exn ->
      ExceptHandler.handle exn;
      exit 1

(** Creation of components defined using the "-command" option argument *)
let create_component comp_defs inst_defs filename cmd =
  try
    let inst_syn = LibraryManager.parse_command cmd in
    let comp_defs' = NameResolve.resolve_toplevel comp_defs [ inst_syn ] in
    let inst_defs' =
      Instantiation.evaluate_toplevel_definitions inst_defs comp_defs' in
    CodeGeneration.generate_code !xml !init filename inst_defs'
  with 
    exn ->
      ExceptHandler.handle exn;
      exit 1

(** Main function *)
let run () =
  Printf.printf "---------------------------------------------------\n";
  Printf.printf "Translator v1.2 for Xcos from Modelica 2.x to flat Modelica\n";
  Printf.printf "Copyright (C) \n2005-2007 Imagine,\n2007-2008 LMS-Imagine\n";
  Versiondate.print_versiondate();
  Printf.printf "---------------------------------------------------\n";
  flush stderr;
  flush stdout;
  parse_args ();
  let comp_defs, inst_defs =
    compile_libraries !lib_files in
  let cmd = match !command with
    | None -> failwith "Command missing"
    | Some cmd -> cmd in
  create_component comp_defs inst_defs !filename cmd

;;

run ()
