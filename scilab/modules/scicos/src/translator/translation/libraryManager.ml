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

let evaluate x = Lazy.force x

let rec parse_stored_libraries lib_paths =
  List.fold_left parse_library [] lib_paths

and parse_command cmd =
  let token_fun = Lexer.token
  and lexbuf = Lexing.from_string cmd in
  Parser.parse Parser.CommandLine token_fun lexbuf

and parse_library lib_syns lib_path =
  try
    let file_names = modelica_file_names_of lib_path in
    lib_syns @ (List.fold_left parse_file [] file_names)
  with
    exn ->
      ExceptHandler.handle exn;
      lib_syns

and parse_file lib_syns file_name =
  let ic = open_in_bin file_name in
  try
    let token_fun = Lexer.token
    and lexbuf = Lexing.from_channel ic in
    let src = Parser.LibraryFile file_name in
    let lib_syn = Parser.parse src token_fun lexbuf in
    close_in ic;
    lib_syn :: lib_syns
  with
    exn ->
      close_in ic;
      ExceptHandler.handle exn;
      lib_syns

and modelica_file_names_of lib_path =
  let rec modelica_file_names_of' acc lib_path =
    let base_name = basename lib_path
    and dir_name = dirname lib_path in
    let lib_path = Filename.concat dir_name base_name in
    match is_directory lib_path with
    | true ->
        let names = Array.to_list (Sys.readdir lib_path) in
        let lib_paths =
          List.map (function s -> Filename.concat lib_path s) names in
        List.fold_left modelica_file_names_of' acc lib_paths
    | false when Filename.check_suffix lib_path ".mo" -> lib_path :: acc
    | false -> acc in
  modelica_file_names_of' [] lib_path

and basename lib_path =
  match Filename.basename lib_path with
  | "." -> basename (Filename.dirname lib_path)
  | s -> s

and dirname lib_path =
  match Filename.basename lib_path with
  | "." -> dirname (Filename.dirname lib_path)
  | _ -> Filename.dirname lib_path

and is_directory dirname =
  try
    Sys.is_directory dirname
  with _ -> false

