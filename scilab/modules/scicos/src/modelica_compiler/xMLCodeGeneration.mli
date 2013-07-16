type 'a tree = Leaf of (string * 'a) | Node of string * 'a tree list
val hide_spc : string -> string
val insert : string list -> 'a -> 'a tree list -> 'a tree list
val cut_on_dot : string -> string * string option
val split : string -> string list
type element = {
  kind : element_kind;
  id : string;
  comment : string;
  initial_value : SymbolicExpression.t option;
  output : bool;
}
and element_kind = Input | Parameter | Variable | DiscreteVariable
val build_tree : Optimization.model -> element tree list
val print_expression :
  out_channel -> Optimization.model -> SymbolicExpression.t -> unit
val print_expression_option :
  out_channel -> Optimization.model -> SymbolicExpression.t option -> unit
val print_tree :
  out_channel -> Optimization.model -> element tree list -> unit
val print_equations : out_channel -> Optimization.model -> unit
val print_when_clauses : out_channel -> Optimization.model -> unit
val generate_XML : string -> string -> Optimization.model -> unit
