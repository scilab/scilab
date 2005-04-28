(** Modelicac for Scicos. *)

module Compiler = OptimizingCompiler.Make (ScicosCodeGeneration)

let name = Sys.argv.(0)

let _ =
  Printf.printf "This is Modelicac v.%s for Scicos.\n" Compiler.version;
  Printf.printf "%s\n"
    (String.sub Compiler.sccs_id 4 (String.length Compiler.sccs_id - 4));
  flush stdout

let () = Compiler.run ()
