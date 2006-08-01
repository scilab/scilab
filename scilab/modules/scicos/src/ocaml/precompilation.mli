(** This module eases the compilation phase by sorting elements of the parse
tree obtained by calling the parsing functions. *)

(** The type of a preprocessed parse tree element. *)
type precompiled_class = {
  class_kind : ParseTree.class_kind;
  mutable extensions : extension list;
  mutable public_classes : (string * precompiled_class) list;
  mutable protected_classes : (string * precompiled_class) list;
  mutable public_cpnts : (string * precompiled_component) list;
  mutable protected_cpnts : (string * precompiled_component) list;
  mutable equs : ParseTree.equation_clause list;
  mutable algs : ParseTree.algorithm_clause list;
  mutable class_comment : ParseTree.string_comment;
}

and extension = string list * modification list

and modification =
    Modification of (string * ParseTree.array_subscripts) list *
      modification list * ParseTree.expression option

and precompiled_component = {
  class_name : string list;
  subscripts : ParseTree.array_subscripts;
  flow : ParseTree.flow option;
  variability : ParseTree.variability option;
  inout : ParseTree.inout option;
  modification : modification list * ParseTree.expression option;
  comment : ParseTree.comment;
}

(** [precompile tree] yields a precompiled class given a parse tree. Only one
class description is allowed per parse tree. *)
val precompile : ParseTree.t -> precompiled_class
