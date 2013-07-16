module ExpressionElement :
  sig
    type t = SymbolicExpression.t
    val equal : 'a -> 'a -> bool
    val hash : SymbolicExpression.t -> int
  end
module ExpressionTable :
  sig
    type key = ExpressionElement.t
    type 'a t = 'a Hashtbl.Make(ExpressionElement).t
    val create : int -> 'a t
    val clear : 'a t -> unit
    val copy : 'a t -> 'a t
    val add : 'a t -> key -> 'a -> unit
    val remove : 'a t -> key -> unit
    val find : 'a t -> key -> 'a
    val find_all : 'a t -> key -> 'a list
    val replace : 'a t -> key -> 'a -> unit
    val mem : 'a t -> key -> bool
    val iter : (key -> 'a -> unit) -> 'a t -> unit
    val fold : (key -> 'a -> 'b -> 'b) -> 'a t -> 'b -> 'b
    val length : 'a t -> int
  end
val eq_array : 'a array -> 'a array -> bool
module ArrayElement :
  sig
    type t = int list * SymbolicExpression.t array
    val equal : 'a * 'b array -> 'a * 'b array -> bool
    val hash : 'a * SymbolicExpression.t array -> int
  end
module ArrayStore :
  sig
    type key = ArrayElement.t
    type 'a t = 'a Hashtbl.Make(ArrayElement).t
    val create : int -> 'a t
    val clear : 'a t -> unit
    val copy : 'a t -> 'a t
    val add : 'a t -> key -> 'a -> unit
    val remove : 'a t -> key -> unit
    val find : 'a t -> key -> 'a
    val find_all : 'a t -> key -> 'a list
    val replace : 'a t -> key -> 'a -> unit
    val mem : 'a t -> key -> bool
    val iter : (key -> 'a -> unit) -> 'a t -> unit
    val fold : (key -> 'a -> 'b -> 'b) -> 'a t -> 'b -> 'b
    val length : 'a t -> int
  end
type model_info = {
  model : Optimization.model;
  final_index_of_integer_parameters : int array;
  final_index_of_string_parameters : int array;
  final_index_of_real_parameters : int array;
  final_index_of_variables : int array;
  surfaces : SymbolicExpression.t list;
  occurrence_table : occurrence_record ExpressionTable.t;
  mutable current_index : int;
  mutable max_index : int;
  code_buffer : Buffer.t;
  real_array_store_size : int;
  real_arrays : int ArrayStore.t;
}
and occurrence_record = {
  mutable occurrences : int;
  mutable label : int option;
}
val postprocess_residue : Optimization.model -> unit
val next_index : model_info -> int
val create_index_array : 'a array -> ('a -> bool) -> int array
val final_index_of_integer_parameters : Optimization.model -> int array
val final_index_of_string_parameters : Optimization.model -> int array
val final_index_of_real_parameters : Optimization.model -> int array
val final_index_of_variables : Optimization.model -> int array
val collect_surfaces : Optimization.model -> SymbolicExpression.t list
val is_atomic : SymbolicExpression.t -> bool
val add_to_occurrence_table :
  bool -> ExpressionTable.key -> model_info -> unit
val has_multiple_occurrences : ExpressionTable.key -> model_info -> bool
val has_alias_binding : ExpressionTable.key -> model_info -> bool
val real_array_store_size : Optimization.model -> int
val bufferize_float : float -> model_info -> unit
val bufferize_rhs :
  model_info -> int -> bool -> string -> ExpressionTable.key -> unit
val bufferize_equations : model_info -> unit
val bufferize_jacobian : int -> model_info -> unit
val bufferize_outputs : model_info -> unit
val bufferize_surface_expression : model_info -> SymbolicExpression.t -> unit
val bufferize_surface_equation :
  model_info -> int -> SymbolicExpression.t -> unit
val bufferize_when_equations : model_info -> unit
val bufferize_surfaces : model_info -> unit
val bufferize_initializations :
  string option * 'a -> int -> int -> int -> int -> model_info -> unit
val bufferize_variable_store :
  'a * string option -> 'b -> int -> int -> model_info -> unit
val bufferize_work_deallocation : 'a option * 'b -> int -> model_info -> unit
val bufferize_parameter_value : 'a option * 'b -> int -> model_info -> unit
val bufferize_variable_nature : int -> model_info -> unit
val last : 'a list -> 'a
val string_of_c_type : Instantiation.expression_type -> string
val generate_c_function_prototype :
  out_channel ->
  string ->
  Instantiation.expression_type list ->
  Instantiation.expression_type list -> unit
val generate_code :
  'a ->
  string ->
  string ->
  Optimization.model -> bool -> 'b -> string option * string option -> unit
