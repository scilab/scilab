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

open XMLTree

let start_equations = ref []

(* Decode XML special characters *)
let decode_spc s =
  let token_fun = StringLexer.token
  and lexbuf = Lexing.from_string s in
  StringParser.parse token_fun lexbuf

let generate_attributes oc attribs =
  let rec modified_attributes attribs = match attribs with
    | [] -> []
    | (attrib, value) :: attribs when value = "" ->
        modified_attributes attribs
    | (attrib, value) :: attribs ->
        (attrib, value) :: (modified_attributes attribs) in
  let rec generate_attributes' attribs = match attribs with
    | [] -> ()
    | [ (attrib, value) ] ->
        Printf.fprintf oc "%s = %s" attrib value
    | (attrib, value) :: attribs ->
        Printf.fprintf oc "%s = %s, " attrib value;
        generate_attributes' attribs in
  match modified_attributes attribs with
  | [] -> ()
  | attribs ->
      Printf.fprintf oc "(";
      generate_attributes' attribs;
      Printf.fprintf oc ")"

let generate_code init filename model =
  let generate_type_specifier oc t = match t.kind, t.output with
    | Input, _ -> Printf.fprintf oc "  input Real"
    | (FixedParameter | Parameter), _ -> Printf.fprintf oc "  parameter Real"
    | Variable, true -> Printf.fprintf oc "  output Real"
    | DiscreteVariable, _ when init -> Printf.fprintf oc "  Real"
    | DiscreteVariable, _ -> Printf.fprintf oc "  discrete Real"
    | _ -> Printf.fprintf oc "  Real"
  and generate_identifier oc t =
    Printf.fprintf oc " `%s`" (decode_spc t.id)
  and generate_comment oc t =
    Printf.fprintf oc " \"%s\";\n" (decode_spc t.comment) in
  let generate_start_attribute oc t = match t.kind with
    | _ when t.initial_value = "" -> ()
    | Variable | DiscreteVariable when t.fixed <> "true"  ->
        generate_attributes oc [ "start", t.initial_value ]
    | Variable | DiscreteVariable ->
        let equ =
          Printf.sprintf "`%s` = %s;" (decode_spc t.id) t.initial_value in
        start_equations := equ :: !start_equations
    | FixedParameter | Parameter when t.fixed <> "false"  ->
        let equ =
          Printf.sprintf "`%s` = %s;" (decode_spc t.id) t.initial_value in
        start_equations := equ :: !start_equations
    | FixedParameter | Parameter ->
        Printf.fprintf oc " = %s" t.initial_value
    | Input -> () in
  let rec generate_sub_elements oc = function
    | [] -> ()
    | Struct s :: elts ->
        generate_sub_elements oc s.subnodes;
        generate_sub_elements oc elts
    | Terminal t :: elts ->
        generate_terminal oc t;
        generate_sub_elements oc elts
  and generate_terminal oc t =
    generate_type_specifier oc t;
    generate_identifier oc t;
    generate_start_attribute oc t;
    generate_comment oc t
  and generate_sub_equations oc = function
    | [] -> ()
    | equ :: equs ->
        Printf.fprintf oc "  %s\n" (decode_spc equ);
        generate_sub_equations oc equs in
  let oc = open_out filename in
  start_equations := [];
  Printf.fprintf oc
    "class %s\n" (Filename.chop_suffix (Filename.basename filename) ".mo");
  generate_sub_elements oc model.elements;
  Printf.fprintf oc "equation\n";
  generate_sub_equations oc
    (!start_equations @ model.equations @ model.when_clauses);
  Printf.fprintf oc
    "end %s;\n" (Filename.chop_suffix (Filename.basename filename) ".mo");
  close_out oc

