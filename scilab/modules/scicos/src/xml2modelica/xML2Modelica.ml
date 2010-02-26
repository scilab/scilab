(*
 *  XML to Modelica
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

let sccs_id =
  "@(#)XML2Modelica - Copyright (C) 2008 LMS Imagine"

let version = "1.0"

let output = ref ""

let input = ref ""

let init = ref false

let check_filename () =
  if Filename.check_suffix !input "xml" then ()
  else failwith "check_filename: Filename suffix must be 'xml'"

let check_output () =
  if !output = "" then failwith "check_output: No output filename specified"

let set_output s =
  if !output <> "" then failwith "set_output: More than one output specified";
  output := s

let set_input s =
  if !input <> "" then failwith "set_input: More than one input specified";
  input := s

let construct_output_filename () =
  if !output = "" then begin
    output := Filename.chop_suffix !input "mo"
  end;
  !output

let parse_args () =
  Arg.parse
    [("-o", Arg.String set_output,
      "<outputfile>  Set output file name to <outputfile>");
     ("-init", Arg.Set init,
      "Generate flat Modelica code for initialization")]
    set_input
    ("usage: XML2modelica -o <outputfile> <inputfile>")

let run () =
  parse_args ();
  check_filename ();
  check_output ();
  let ic = open_in !input in
  Printf.printf "Input file name = %s\n" !input; flush stdout;
  let lexbuf = Lexing.from_channel ic in
  Printf.printf "Parsing..."; flush stdout;
  let model = XMLParser.parse !input XMLLexer.token lexbuf in
  let filename = construct_output_filename () in
  Printf.printf "OK\nGenerating code..."; flush stdout;
  ModelicaCodeGenerator.generate_code !init filename model;
  Printf.printf " OK\nOutput file name = %s\n" !output; flush stdout;
  Printf.printf "Terminated.\n"; flush stdout;
  close_in ic

let () = run ()
