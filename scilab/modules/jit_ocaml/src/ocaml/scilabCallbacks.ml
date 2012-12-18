(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

let debug = false
let _ =
  Printf.fprintf stderr "OCaml JIT in !\n%!"

let print_ast =
  try
    ignore (Sys.getenv "SCILAB_AST_DEBUG"); true
  with _ -> false

external jit_ocaml_register_callback_ml :
  (string -> string) -> unit = "jit_ocaml_register_callback_c"

let _ =
  jit_ocaml_register_callback_ml
    (fun s_c ->
      try
        let ast = ScilabString2Ast.ast_of_string s_c in
        let s1 = ScilabString2Ast.copy_string s_c in
        let s2 = ScilabAst2String.string_of_ast ast in
        if debug then begin
          ScilabString2Ast.diff_strings s1 s2;
          print_string (ScilabAstPrinter.to_string ast);
          print_newline ()
        end else
          if print_ast then begin
            print_string (ScilabAstPrinter.to_string ast);
            print_newline ()
          end;
        s2
      with e ->
        Printf.fprintf stderr "jit_ocaml_register_callback_ml: exception %S\n%!"
          (Printexc.to_string e);
        s_c
    )

let main () = ()
