(** Modelicac for Scicos. *)

module Compiler = OptimizingCompiler.Make (ScicosCodeGeneration)

let sccs_id =
  "@(#)Modelicac - Copyright (C) 2005 Imagine"

let name = Sys.argv.(0)

let _ =
  Printf.printf "This is Modelicac v.%s for Scicos.\n" Compiler.version;
  Printf.printf "%s\n" (String.sub sccs_id 4 (String.length sccs_id - 4));
  flush stdout

let () = Compiler.run ()
