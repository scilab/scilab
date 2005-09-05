(** This module includes some utility functions for XML (experimental)
   export of modelica models *)

val generate_XML : string -> string -> Optimization.model -> unit
(** [generate_XML filename class_name model] generates an XML version
of the Modelica model [model] with name [class_name] into [filename] *)
