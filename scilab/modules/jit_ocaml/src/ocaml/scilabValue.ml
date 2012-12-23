
exception NotImplemented

module TYPES = struct
  type t =
  | Scalar of scalar
  | Matrix of matrix
  | List of t * t * t
  | Macro of string * (t array -> t array)
  | String of string

  and matrix =
    {
      matrix_dims : int;
      matrix_sizes : int array;
      matrix_matrix : t array;
    }

  and scalar =
    Int of int32
  | Double of float
  | Float of float
  | Bool of bool

end

open TYPES
type t = TYPES.t
let empty = Matrix {
  matrix_dims = 0;
  matrix_sizes = [||];
  matrix_matrix = [||];
}


let string_of_unicode s =
  let len = String.length s in
  let b = Buffer.create len in
  for i = 0 to len-1 do
    let c= s.[i] in
    if c <> '\000' then Buffer.add_char b c
  done;
  Buffer.contents b

let bool b = Scalar (Bool b)
let int n = Scalar (Int n)
let double d = Scalar (Double d)
let float d = Scalar (Float d)
let string s = String s

let b = Buffer.create 1000

let rec buf_value v =
  match v with
  | Scalar s ->
    begin
      match s with
        Int i -> Printf.bprintf b "int32(%ld)" i
      | Double d -> Printf.bprintf b "%f" d
      | Float d -> Printf.bprintf b "%f" d
      | Bool true -> Buffer.add_char b 'T'
      | Bool false -> Buffer.add_char b 'F'
    end
  | Matrix m ->
    begin
      match m.matrix_dims with
      | 1 ->
        let size = m.matrix_sizes.(0) in
        Buffer.add_string b "[ ";
        for i = 0 to size - 2 do
          buf_value m.matrix_matrix.(i); Buffer.add_string b " ,"
        done;
        buf_value m.matrix_matrix.(size-1);
        Buffer.add_string b " ]";
      | dims -> Printf.bprintf b "<matrix%d>" dims
    end
  | List (start, step, stop) ->
    buf_value start;
    Buffer.add_char b ':';
    buf_value step;
    Buffer.add_char b ':';
    buf_value stop
  | Macro (name, _) ->
    Printf.bprintf b "Macro<%s>" name
  | String s ->
    Printf.bprintf b "%S" (string_of_unicode s)

let to_string v =
  Buffer.clear b;
  buf_value v;
  Buffer.contents b

let lt v1 v2 =
  match v1, v2 with
    Scalar v1, Scalar v2 -> v1 <= v2
  | _ ->
    Printf.fprintf stderr "ScilabValue.lt %s %s not implemented\n%!"
      (to_string v1) (to_string v2);
    raise NotImplemented

let to_int x =
  match x with
    Int n -> n
  | Double d -> Int32.of_float d
  | Float d -> Int32.of_float d
  | Bool _ -> assert false

let add v1 v2 =
  match v1, v2 with
    Scalar s1, Scalar s2 ->
      begin
        match s1, s2 with
        | Bool _, _
        | _, Bool _ ->
          Printf.fprintf stderr "ScilabValue.add %s %s not implemented\n%!"
            (to_string v1) (to_string v2);
          raise NotImplemented
        | Int _, _
        | _, Int _ ->
          Scalar (Int (Int32.add (to_int s1) (to_int s2)))
        | Float d1, Float d2 ->
          Scalar (Float (d1 +. d2))
        | (Double d1 | Float d1), (Double d2 | Float d2) ->
          Scalar (Double (d1 +. d2))
(*        | _ ->
          Printf.fprintf stderr "ScilabValue.add %s %s not implemented\n%!"
            (to_string v1) (to_string v2);
          raise NotImplemented
*)
      end
  | _ ->
    Printf.fprintf stderr "ScilabValue.add %s %s not implemented\n%!"
      (to_string v1) (to_string v2);
    raise NotImplemented

let is_true v =
  match v with
  | Scalar (Bool b) -> b
  | Scalar (Double 0. | Float 0. | Int 0l) -> false
  | Scalar (Double _ | Float _ | Int _ ) -> true
  | Matrix { matrix_dims = 0 } -> false
  | Matrix _ -> true
  | Macro _ -> assert false
  | List _ -> assert false
  | String _ -> assert false

