(** This module provides functions that generate code for the Scicos target. *)

val generate_code : string -> string -> string -> Optimization.model -> unit
(** [generate_code path filename fun_name model] generates the code that allows
the numeric simulation of [model]. [path] is the path to the external functions
referenced in [model]. [filename] is the name of the file where the code is
generated. [fun_name] is the name of the entry point in the generated code. *)
