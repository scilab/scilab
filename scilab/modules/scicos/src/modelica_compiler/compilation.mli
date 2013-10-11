val opt_map : ('a -> 'b) -> 'a option -> 'b option
exception InvalidModification
exception TypeError
type compilation_context =
    ClassContext of compilation_context * compiled_class Lazy.t
  | ForContext of compilation_context * string
  | ModificationContext of compilation_context * compiled_class Lazy.t
  | TopLevelContext of (string * compiled_unit) list
and compiled_unit =
    CompiledClass of compiled_class Lazy.t
  | CompiledFunction of compiled_class Lazy.t
and compiled_class = {
  ccl_public_cpnts : (string * compiled_component Lazy.t) list Lazy.t;
  ccl_initial_equs : compiled_equation list Lazy.t;
  ccl_equs : compiled_equation list Lazy.t;
}
and compiled_modification =
    CompiledModification of field * compiled_modification list *
      compiled_expression option
and field = string * compiled_subscript array
and compiled_reference =
    ParameterReference of level * path
  | VariableReference of level * path
  | LoopVariableReference of level
  | ClassReference of level * string list
and path = field list
and level = int
and compiled_subscript = Indefinite | Definite of compiled_expression
and parameter =
    IntegerParameter of parameter_attributes
  | StringParameter of parameter_attributes
  | RealParameter of parameter_attributes
and parameter_attributes = {
  pat_dimensions : compiled_subscript array;
  pat_comment : string;
  pat_value : compiled_expression option;
  pat_infos : variable_infos;
}
and variable =
    IntegerVariable of variable_attributes
  | StringVariable of variable_attributes
  | DiscreteVariable of variable_attributes
  | RealVariable of variable_attributes
  | CompoundVariable of compiled_class Lazy.t * variable_attributes
and variable_attributes = {
  vat_dimensions : compiled_subscript array;
  vat_nature : nature;
  vat_inout : inout;
  vat_comment : string;
  vat_modifications : compiled_modification list;
  vat_infos : variable_infos;
}
and compiled_component = Parameter of parameter | Variable of variable
and nature = Flow | Potential
and inout = Input | Output | Both
and variable_infos = {
  var_name : string;
  title : string;
  unit : string;
  quantity : string;
  min : string;
  max : string;
  port_name : string;
  port_type : string;
  order : int;
  io : int;
  weight : float;
}
and compiled_equation =
    CompiledEquality of compiled_expression * compiled_expression
  | CompiledFlowConnection of compiled_expression * compiled_expression
  | CompiledIf of (compiled_expression * compiled_equation list) list *
      compiled_equation list
  | CompiledFor of compiled_expression * compiled_expression *
      compiled_expression * compiled_equation list
  | CompiledWhen of
      (compiled_expression * compiled_when_expression list) list
and compiled_when_expression =
    Reinit of compiled_reference * compiled_expression
  | Assign of compiled_reference * compiled_expression
and compiled_expression =
    Abs of compiled_expression
  | Acos of compiled_expression
  | Acosh of compiled_expression
  | Addition of compiled_expression * compiled_expression
  | And of compiled_expression * compiled_expression
  | Asin of compiled_expression
  | Asinh of compiled_expression
  | Atan of compiled_expression
  | Atanh of compiled_expression
  | Boolean of bool
  | Cardinality of compiled_expression
  | Cos of compiled_expression
  | Cosh of compiled_expression
  | Der of compiled_expression
  | Division of compiled_expression * compiled_expression
  | Equals of compiled_expression * compiled_expression
  | Exp of compiled_expression
  | ExternalFunctionCall of string list * compiled_class Lazy.t *
      compiled_argument list
  | Floor of compiled_expression
  | GreaterEqualThan of compiled_expression * compiled_expression
  | GreaterThan of compiled_expression * compiled_expression
  | If of (compiled_expression * compiled_expression) list *
      compiled_expression
  | Integer of int32
  | Log of compiled_expression
  | Max of compiled_expression * compiled_expression
  | Min of compiled_expression * compiled_expression
  | Minus of compiled_expression
  | Mod of compiled_expression * compiled_expression
  | Multiplication of compiled_expression * compiled_expression
  | NoEvent of compiled_expression
  | Not of compiled_expression
  | NotEquals of compiled_expression * compiled_expression
  | Or of compiled_expression * compiled_expression
  | Power of compiled_expression * compiled_expression
  | Pre of compiled_expression
  | Real of float
  | Reference of compiled_reference
  | Sin of compiled_expression
  | Sinh of compiled_expression
  | Sqrt of compiled_expression
  | String of string
  | Subtraction of compiled_expression * compiled_expression
  | Tan of compiled_expression
  | Tanh of compiled_expression
  | Time
  | Vector of compiled_expression array
and compiled_argument =
    ScalarArgument of compiled_expression
  | ArrayArgument of int list * compiled_expression array
val paths : string list ref
val read_class_file : string -> compiled_unit
val last : 'a list -> 'a
val write_class_file : string -> compiled_unit -> unit
val create_filename : string list -> string -> string
val get_compiled_class : compilation_context -> string list -> compiled_unit
val compile_main_class :
  Precompilation.precompiled_class list -> string * compiled_unit
val compile_compound_class :
  compilation_context ->
  Precompilation.precompiled_class -> compiled_class Lazy.t
val compile_components :
  compilation_context ->
  int ->
  (ParseTree.ident * Precompilation.precompiled_component) list ->
  (ParseTree.ident * compiled_component Lazy.t) list
val compile_equation_clauses :
  compilation_context ->
  ParseTree.equation_clause list ->
  compiled_equation list * compiled_equation list
val compile_equation_or_annotation :
  compilation_context ->
  ParseTree.equation_or_annotation -> compiled_equation list
val compile_equation :
  compilation_context -> ParseTree.equation -> compiled_equation list
val find_connector :
  compilation_context -> ParseTree.component_reference -> variable
val compile_connection :
  compilation_context ->
  ParseTree.component_reference ->
  ParseTree.component_reference ->
  compiled_class Lazy.t ->
  variable_attributes ->
  compiled_class Lazy.t -> variable_attributes -> compiled_equation list
val compile_if_clause :
  compilation_context ->
  ParseTree.expression * ParseTree.equation list ->
  compiled_expression * compiled_equation list
val compile_when_clause :
  compilation_context ->
  ParseTree.expression * ParseTree.equation list ->
  compiled_expression * compiled_when_expression list
val compile_when_equation :
  compilation_context -> ParseTree.equation -> compiled_when_expression
val compile_for_clause :
  compilation_context ->
  ParseTree.for_indices -> ParseTree.equation list -> compiled_equation list
val compile_expression :
  compilation_context -> ParseTree.expression -> compiled_expression
val to_argument : compiled_expression -> compiled_argument
val compile_component_reference :
  compilation_context -> ParseTree.component_reference -> compiled_reference
val compile_component_path :
  compilation_context ->
  compilation_context ->
  level -> ParseTree.component_reference -> compiled_reference
val compile_subscripts :
  compilation_context ->
  ParseTree.array_subscripts -> compiled_subscript array
val compile_modifications :
  compilation_context ->
  Precompilation.modification list -> compiled_modification list
val compile_modification :
  compilation_context -> Precompilation.modification -> compiled_modification
val find_local_component_in :
  compiled_class Lazy.t -> ParseTree.ident -> compiled_component
val find_local_component_class_in :
  compiled_class Lazy.t -> ParseTree.ident -> compiled_class Lazy.t
val is_base_type_parameter_of :
  compiled_class Lazy.t -> ParseTree.ident -> bool
val is_base_type_variable_of :
  compiled_class Lazy.t -> ParseTree.ident -> bool
val compile_component_modification :
  compilation_context -> Precompilation.modification -> compiled_modification
val compile_base_type_variable_modifications :
  compilation_context ->
  Precompilation.modification list -> compiled_modification list
val compile_component :
  compilation_context ->
  int ->
  ParseTree.ident ->
  Precompilation.precompiled_component -> compiled_component
val compile_parameter :
  compilation_context ->
  int -> ParseTree.ident -> Precompilation.precompiled_component -> parameter
val compile_variable :
  compilation_context ->
  int -> ParseTree.ident -> Precompilation.precompiled_component -> variable
val start_modifications :
  compiled_modification list -> compiled_modification list
val compile_nature : ParseTree.flow option -> nature
val compile_inout : ParseTree.inout option -> inout
val compile_comment : ParseTree.comment -> string
val compile_infos :
  int ->
  ParseTree.ident ->
  ParseTree.comment -> compiled_modification list -> variable_infos
val compile_annotation :
  ParseTree.annotation -> float * ParseTree.ident * ParseTree.ident
