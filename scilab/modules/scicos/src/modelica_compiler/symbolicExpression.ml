(*
 *  Modelicac
 *
 *  Copyright (C) 2005 - 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

open Num

exception Infinite_result of string


(* Type definitions *)

type t =
  {
    nature: nature;
    sortingHash: int;
    hash: int;
    mutable count: int;
    mutable replacement: t
  }
and nature =
  | Addition of t list
  | And of t list
  | ArcCosine of t
  | ArcHyperbolicCosine of t
  | ArcHyperbolicSine of t
  | ArcHyperbolicTangent of t
  | ArcSine of t
  | ArcTangent of t
  | BlackBox of string * argument list
  | BooleanValue of bool
  | Constant of string
  | Cosine of t
  | Derivative of t * num
  | DiscreteVariable of int
  | Equality of t * t
  | Exponential of t
  | Floor of t
  | Greater of t * t
  | GreaterEqual of t * t
  | HyperbolicCosine of t
  | HyperbolicSine of t
  | HyperbolicTangent of t
  | If of t * t * t
  | Integer of int32
  | Logarithm of t
  | Multiplication of t list
  | Not of t
  | Number of num
  | Or of t list
  | Parameter of int
  | PartialDerivative of t * t
  | Pre of t
  | RationalPower of t * num
  | Sign of t
  | Sine of t
  | String of string
  | Tangent of t
  | TimeVariable
  | Variable of int

and argument =
  | ScalarArgument of t
  | ArrayArgument of int list (* dimensions *) * t array (* flatten array *)

(* Node utilities *)

let nature node = node.nature

let hash node = node.hash

(* Argument utilities *)

let argument_hash = function
  | ScalarArgument node -> node.hash
  | ArrayArgument (dims, nodes) ->
      (Hashtbl.hash dims lxor
      Array.fold_left
        (fun acc node -> acc lsl 2 + node.hash)
        0x32fb7a88
        nodes) land max_int

(* Array utilities *)

let eq_array nodes nodes' =
  let l = Array.length nodes in
  let rec eq_array_from i =
    i = l || nodes.(i) == nodes'.(i) && eq_array_from (i + 1) in
  l = Array.length nodes' && eq_array_from 0

let exists_array p xs =
  let l = Array.length xs in
  let rec exists_array_from i =
    i < l && (p xs.(i) || exists_array_from (i + 1)) in
  exists_array_from 0

(* List utilities *)

let rec eq_list nodes nodes' = match nodes, nodes' with
  | [], [] -> true
  | node :: nodes, node' :: nodes' when node == node' -> eq_list nodes nodes'
  | _ -> false

let insert node nodes =
  let rec insert_into nodes = match nodes with
    | [] -> [node]
    | node' :: _ when node.sortingHash <= node'.sortingHash -> node :: nodes
    | node' :: nodes' -> node' :: insert_into nodes'
  in insert_into nodes

let remove node nodes =
  let rec remove_from = function
    | [] -> []
    | node' :: nodes' when node == node' -> nodes'
    | node' :: nodes' -> node' :: remove_from nodes'
  in remove_from nodes

let sort nodes =
  List.sort
    (fun node node' ->
      if node.sortingHash < node'.sortingHash then -1
      else if node.sortingHash > node'.sortingHash then 1
      else 0)
    nodes

let union nodes nodes' =
  List.fold_left
    (fun acc node -> if List.exists (( == ) node) acc then acc else node :: acc)
    nodes
    nodes'

let intersection nodes nodes' =
  List.fold_left
    (fun acc node ->
      if List.exists (( == ) node) nodes then node :: acc else acc)
    []
    nodes'


(* Node collections *)

type node = t

module Node =
  struct
    type t = node
    let hash node = node.hash
  end

module NodeSet = GraphNodeSet.Make(Node)

let global_count = ref 0

let unique_integer =
  let i = ref 0
  in function () -> let v = !i in begin incr i; v end

let create_node nature hash =
  let rec node =
    {
      nature = nature;
      sortingHash = unique_integer ();
      hash = hash;
      count = !global_count;
      replacement = node
    }
  in node

let additionNodeSet =
  let equal nodeList node = match node.nature with
    | Addition nodes -> eq_list nodeList nodes
    | _ -> invalid_arg "Argument mismatch."
  and hash nodes =
    List.fold_left
      (fun sum elt -> (sum lsl 3) + elt.hash)
      0x248c44
      nodes
  and create nodes hash = create_node (Addition nodes) hash
  in NodeSet.create 101 equal hash create

let andNodeSet =
  let equal nodeList node = match node.nature with
    | And nodes -> eq_list nodeList nodes
    | _ -> invalid_arg "Argument mismatch."
  and hash nodes =
    List.fold_left
      (fun sum elt -> (sum lsl 6) + elt.hash)
      0xe41e902
      nodes
  and create nodes hash = create_node (And nodes) hash
  in NodeSet.create 101 equal hash create

let arcCosineNodeSet =
  let equal node1 node = match node.nature with
    | ArcCosine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 6) lxor 0x4211038
  and create node1 hash = create_node (ArcCosine node1) hash
  in NodeSet.create 101 equal hash create

let arcHyperbolicCosineNodeSet =
  let equal node1 node = match node.nature with
    | ArcHyperbolicCosine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 4) lxor 0x24a5b008
  and create node1 hash = create_node (ArcHyperbolicCosine node1) hash
  in NodeSet.create 101 equal hash create

let arcHyperbolicSineNodeSet =
  let equal node1 node = match node.nature with
    | ArcHyperbolicSine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 3) lxor 0x21a8d400
  and create node1 hash = create_node (ArcHyperbolicSine node1) hash
  in NodeSet.create 101 equal hash create

let arcHyperbolicTangentNodeSet =
  let equal node1 node = match node.nature with
    | ArcHyperbolicTangent node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x2bd00c4
  and create node1 hash = create_node (ArcHyperbolicTangent node1) hash
  in NodeSet.create 101 equal hash create

let arcSineNodeSet =
  let equal node1 node = match node.nature with
    | ArcSine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 3) lxor 0x114500a0
  and create node1 hash = create_node (ArcSine node1) hash
  in NodeSet.create 101 equal hash create

let arcTangentNodeSet =
  let equal node1 node = match node.nature with
    | ArcTangent node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 4) lxor 0x200aa000
  and create node1 hash = create_node (ArcTangent node1) hash
  in NodeSet.create 101 equal hash create

let blackBoxNodeSet =
  let rec same_arguments args args' = match args, args' with
    | [], [] -> true
    | [], _ :: _ | _ :: _, [] -> false
    | ScalarArgument node :: args, ScalarArgument node' :: args'
      when node == node' -> same_arguments args args'
    | ArrayArgument (dims, nodes) :: args,
      ArrayArgument (dims', nodes') :: args'
      when dims = dims' && eq_array nodes nodes' ->
        same_arguments args args'
    | _ -> false in
  let equal (string, argList) node = match node.nature with
    | BlackBox (name, args) -> name = string && same_arguments argList args
    | _ -> invalid_arg "Argument mismatch."
  and hash (string, args) =
    (GraphNodeSet.hash string +
    List.fold_left
      (fun sum arg -> (sum lsl 3) + argument_hash arg)
      0x20a0
      args) land max_int
  and create (string, args) hash = create_node (BlackBox (string, args)) hash
  in NodeSet.create 101 equal hash create

let constantNodeSet =
  let equal string node = match node.nature with
    | Constant name -> name = string
    | _ -> invalid_arg "Argument mismatch."
  and hash string = GraphNodeSet.hash string lxor 0x709dc0
  and create string hash = create_node (Constant string) hash
  in NodeSet.create 101 equal hash create

let cosineNodeSet =
  let equal node1 node = match node.nature with
    | Cosine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x208af001
  and create node1 hash = create_node (Cosine node1) hash
  in NodeSet.create 101 equal hash create

let derivativeNodeSet =
  let equal (node1, num) node = match node.nature with
    | Derivative (node', num') -> node1 == node' && eq_num num num'
    | _ -> invalid_arg "Argument mismatch."
  and hash (node, num) =
    (GraphNodeSet.hash num lsl 10) lxor (node.hash + 0x1410200d)
  and create (node, num) hash = create_node (Derivative (node, num)) hash
  in NodeSet.create 101 equal hash create

let discreteVariableNodeSet =
  let equal i node = match node.nature with
    | DiscreteVariable i' -> i = i'
    | _ -> invalid_arg "Argument mismatch."
  and hash i = GraphNodeSet.hash i lxor 0x2fb049a3
  and create i hash = create_node (DiscreteVariable i) hash
  in NodeSet.create 101 equal hash create

let equalityNodeSet =
  let equal (node1, node2) node = match node.nature with
    | Equality (leftHandNode, rightHandNode) ->
        leftHandNode == node1 && rightHandNode == node2
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, node2) = (node1.hash lsl 5) lxor (node2.hash + 0x5218c660)
  and create (node1, node2) hash = create_node (Equality (node1, node2)) hash
  in NodeSet.create 101 equal hash create

let exponentialNodeSet =
  let equal node1 node = match node.nature with
    | Exponential node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 4) lxor 0x20c22a00
  and create node1 hash = create_node (Exponential node1) hash
  in NodeSet.create 101 equal hash create

let floorNodeSet =
  let equal node1 node = match node.nature with
    | Floor node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 2) lxor 0x8788b02
  and create node1 hash = create_node (Floor node1) hash
  in NodeSet.create 101 equal hash create

let greaterNodeSet =
  let equal (node1, node2) node = match node.nature with
    | Greater (leftHandNode, rightHandNode) ->
        leftHandNode == node1 && rightHandNode == node2
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, node2) =
    (node1.hash lsl 18) lxor (node2.hash + 0x11e02c02)
  and create (node1, node2) hash = create_node (Greater (node1, node2)) hash
  in NodeSet.create 101 equal hash create

let greaterEqualNodeSet =
  let equal (node1, node2) node = match node.nature with
    | GreaterEqual (leftHandNode, rightHandNode) ->
        leftHandNode == node1 && rightHandNode == node2
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, node2) =
    (node1.hash lsl 18) lxor (node2.hash + 0x11e02c02)
  and create (node1, node2) hash =
    create_node (GreaterEqual (node1, node2)) hash
  in NodeSet.create 101 equal hash create

let hyperbolicCosineNodeSet =
  let equal node1 node = match node.nature with
    | HyperbolicCosine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 4) lxor 0x2a091f0c
  and create node1 hash = create_node (HyperbolicCosine node1) hash
  in NodeSet.create 101 equal hash create

let hyperbolicSineNodeSet =
  let equal node1 node = match node.nature with
    | HyperbolicSine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x24f002f1
  and create node1 hash = create_node (HyperbolicSine node1) hash
  in NodeSet.create 101 equal hash create

let hyperbolicTangentNodeSet =
  let equal node1 node = match node.nature with
    | HyperbolicTangent node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 3) lxor 0x2a221c8
  and create node1 hash = create_node (HyperbolicTangent node1) hash
  in NodeSet.create 101 equal hash create

let ifNodeSet =
  let equal (node1, node2, node3) node = match node.nature with
    | If (testNode, alternativeNode1, alternativeNode2) ->
        testNode == node1 && alternativeNode1 == node2 &&
        alternativeNode2 == node3
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, node2, node3) =
    (node1.hash lsl 17) lxor (node2.hash lsl 5) lxor
    (node3.hash + 0x5d403a30)
  and create (node1, node2, node3) hash =
    create_node (If (node1, node2, node3)) hash
  in NodeSet.create 101 equal hash create

let integerNodeSet =
  let equal i node = match node.nature with
    | Integer i' -> i' = i
    | _ -> invalid_arg "Argument mismatch."
  and hash i = (Hashtbl.hash i lsl 2) lxor 0x11bf004
  and create i hash = create_node (Integer i) hash
  in NodeSet.create 101 equal hash create

let logarithmNodeSet =
  let equal node1 node = match node.nature with
    | Logarithm node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 2) lxor 0x11bf004
  and create node1 hash = create_node (Logarithm node1) hash
  in NodeSet.create 101 equal hash create

let multiplicationNodeSet =
  let equal nodeList node = match node.nature with
    | Multiplication nodes -> eq_list nodeList nodes
    | _ -> invalid_arg "Argument mismatch."
  and hash nodes =
    List.fold_left
      (fun sum elt -> (sum lsl 3) + elt.hash)
      0x25a50039
      nodes
  and create nodes hash = create_node (Multiplication nodes) hash
  in NodeSet.create 101 equal hash create

let numberNodeSet =
  let equal num node = match node.nature with
    | Number num' -> eq_num num' num
    | _ -> invalid_arg "Argument mismatch."
  and hash num = GraphNodeSet.hash num lxor 0x59710f0
  and create num hash =
    let rec node =
      {
        nature = Number num;
        sortingHash = -unique_integer ();
        (* The sorting hash is nagated to ensure that numbers appear
           at the first positions in commutative n-ary expressions.  *)
        hash = hash;
        count = !global_count;
        replacement = node
      }
    in node
  in NodeSet.create 101 equal hash create

let notNodeSet =
  let equal node1 node = match node.nature with
    | Not node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x18a20a0
  and create node1 hash = create_node (Not node1) hash
  in NodeSet.create 101 equal hash create

let orNodeSet =
  let equal nodes node = match node.nature with
    | Or nodes' -> eq_list nodes nodes'
    | _ -> invalid_arg "Argument mismatch."
  and hash nodes =
    List.fold_left
      (fun sum elt -> (sum lsl 3) + elt.hash)
      0x4102f024
      nodes
  and create nodes hash = create_node (Or nodes) hash
  in NodeSet.create 101 equal hash create

let parameterNodeSet =
  let equal i node = match node.nature with
    | Parameter i' -> i' = i
    | _ -> invalid_arg "Argument mismatch."
  and hash i = GraphNodeSet.hash i lxor 0x709dc0
  and create i hash = create_node (Parameter i) hash
  in NodeSet.create 101 equal hash create

let partialDerivativeNodeSet =
  let equal (node1, node2) node = match node.nature with
    | PartialDerivative (variableNode, argumentNode) ->
        variableNode == node1 && argumentNode == node2
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, node2) =
    (node1.hash lsl 12) lxor (node2.hash + 0x2f301000)
  and create (node1, node2) hash =
    create_node (PartialDerivative (node1, node2)) hash
  in NodeSet.create 101 equal hash create

let preNodeSet =
  let equal node1 node = match node.nature with
    | Pre node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x100050f0
  and create node1 hash = create_node (Pre node1) hash
  in NodeSet.create 101 equal hash create

let rationalPowerNodeSet =
  let equal (node1, num) node = match node.nature with
    | RationalPower (argumentNode, num')  ->
        argumentNode == node1 && (eq_num num' num)
    | _ -> invalid_arg "Argument mismatch."
  and hash (node1, num) =
    (GraphNodeSet.hash num lsl 20) lxor (node1.hash + 0xd41000d)
  and create (node1, num) hash = create_node (RationalPower (node1, num)) hash
  in NodeSet.create 101 equal hash create

let signNodeSet =
  let equal node1 node = match node.nature with
    | Sign node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 5) lxor 0x100050f0
  and create node1 hash = create_node (Sign node1) hash
  in NodeSet.create 101 equal hash create

let sineNodeSet =
  let equal node1 node = match node.nature with
    | Sine node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 6) lxor 0x8a8f00
  and create node1 hash = create_node (Sine node1) hash
  in NodeSet.create 101 equal hash create

let stringNodeSet =
  let equal s node = match node.nature with
    | String s' -> s' = s
    | _ -> invalid_arg "Argument mismatch."
  and hash s = (Hashtbl.hash s lsl 6) lxor 0x38a8f002
  and create s hash = create_node (String s) hash
  in NodeSet.create 101 equal hash create

let tangentNodeSet =
  let equal node1 node = match node.nature with
    | Tangent node -> node == node1
    | _ -> invalid_arg "Argument mismatch."
  and hash node1 = (node1.hash lsl 8) lxor 0x655001c9
  and create node1 hash = create_node (Tangent node1) hash
  in NodeSet.create 101 equal hash create

let variableNodeSet =
  let equal i node = match node.nature with
    | Variable i' -> i' = i
    | _ -> invalid_arg "Argument mismatch."
  and hash i = GraphNodeSet.hash i lxor 0x2fb04900
  and create i hash = create_node (Variable i) hash
  in NodeSet.create 101 equal hash create


(* Node creation *)

let zero_num = Int 0
let one_num = Int 1
let two_num = Int 2

let minus_one = NodeSet.find_or_add (Int (-1)) numberNodeSet
let zero = NodeSet.find_or_add zero_num numberNodeSet
let one = NodeSet.find_or_add one_num numberNodeSet
let two = NodeSet.find_or_add two_num numberNodeSet
let ten = NodeSet.find_or_add (Int 10) numberNodeSet
let one_over_two = NodeSet.find_or_add (div_num (one_num) two_num) numberNodeSet
let minus_one_over_two =
  NodeSet.find_or_add (div_num (Int (-1)) two_num) numberNodeSet
let pi = NodeSet.find_or_add "3.14159265359" constantNodeSet
let pi_over_two =
  NodeSet.find_or_add (insert one_over_two [pi]) multiplicationNodeSet
let minus_pi_over_two =
  NodeSet.find_or_add (insert minus_one_over_two [pi]) multiplicationNodeSet
let e = NodeSet.find_or_add "2.71828182846" constantNodeSet
let eps = NodeSet.find_or_add "0.00001" constantNodeSet
let false_value = create_node (BooleanValue false) 0
let true_value = create_node (BooleanValue true) 1
let time = create_node TimeVariable 0

let addition_neutral = zero
let multiplication_neutral = one
let and_neutral = true_value
let or_neutral = false_value

let create_addition = function
  | [] -> addition_neutral
  | [node] -> node
  | nodes -> NodeSet.find_or_add nodes additionNodeSet

let create_and = function
  | [] -> and_neutral
  | [node] -> node
  | nodes -> NodeSet.find_or_add nodes andNodeSet

let create_arcCosine node = NodeSet.find_or_add node arcCosineNodeSet

let create_arcHyperbolicCosine node =
  NodeSet.find_or_add node arcHyperbolicCosineNodeSet

let create_arcHyperbolicSine node =
  NodeSet.find_or_add node arcHyperbolicSineNodeSet

let create_arcHyperbolicTangent node =
  NodeSet.find_or_add node arcHyperbolicTangentNodeSet

let create_arcSine node = NodeSet.find_or_add node arcSineNodeSet

let create_arcTangent node = NodeSet.find_or_add node arcTangentNodeSet

let create_blackBox s nodes = NodeSet.find_or_add (s, nodes) blackBoxNodeSet

let create_booleanValue b = if b then true_value else false_value

let create_constant s = NodeSet.find_or_add s constantNodeSet

let create_cosine node = NodeSet.find_or_add node cosineNodeSet

let create_derivative node num =
  NodeSet.find_or_add (node, num) derivativeNodeSet

let create_discrete_variable i = NodeSet.find_or_add i discreteVariableNodeSet

let create_equality node node' =
  NodeSet.find_or_add (node, node') equalityNodeSet

let create_exponential node = NodeSet.find_or_add node exponentialNodeSet

let create_floor node =  NodeSet.find_or_add node floorNodeSet

let create_greater node node' = NodeSet.find_or_add (node, node') greaterNodeSet

let create_greater_equal node node' =
  NodeSet.find_or_add (node, node') greaterEqualNodeSet

let create_hyperbolicCosine node =
  NodeSet.find_or_add node hyperbolicCosineNodeSet

let create_hyperbolicSine node = NodeSet.find_or_add node hyperbolicSineNodeSet

let create_hyperbolicTangent node =
  NodeSet.find_or_add node hyperbolicTangentNodeSet

let create_if node node' node'' =
  NodeSet.find_or_add (node, node', node'') ifNodeSet

let create_integer i = NodeSet.find_or_add i integerNodeSet

let create_logarithm node = NodeSet.find_or_add node logarithmNodeSet

let create_multiplication = function
  | [] -> multiplication_neutral
  | [node] -> node
  | nodes -> NodeSet.find_or_add nodes multiplicationNodeSet

let create_not node = NodeSet.find_or_add node notNodeSet

let create_number = function
  | Int 0 -> zero
  | Int 1 -> one
  | num -> NodeSet.find_or_add num numberNodeSet

let create_or = function
  | [] -> or_neutral
  | [node] -> node
  | nodes -> NodeSet.find_or_add nodes orNodeSet

let create_parameter i = NodeSet.find_or_add i parameterNodeSet

let create_partialDerivative node node' =
  NodeSet.find_or_add (node, node') partialDerivativeNodeSet

let create_pre node = NodeSet.find_or_add node preNodeSet

let create_rationalPower node num =
  NodeSet.find_or_add (node, num) rationalPowerNodeSet

let create_sign node = NodeSet.find_or_add node signNodeSet

let create_sine node = NodeSet.find_or_add node sineNodeSet

let create_string s = NodeSet.find_or_add s stringNodeSet

let create_tangent node = NodeSet.find_or_add node tangentNodeSet

let create_timeVariable () = time

let create_variable i = NodeSet.find_or_add i variableNodeSet


(* Reductions *)

let rec apply_if_possible create op node nodes =
  let rec apply_if_possible' = function
    | [] -> Some (create (insert node nodes))
    | node' :: nodes' ->
        begin match op node node' with
          | Some node'' ->
              apply_if_possible create op node'' (remove node' nodes)
          | None -> apply_if_possible' nodes'
        end
  in apply_if_possible' nodes

and symbolic_abs node =
  symbolic_if (symbolic_ge node zero) node (symbolic_minus node)

and symbolic_acos node =
  if node == minus_one then pi
  else if node == zero then pi_over_two
  else if node == one then zero
  else create_arcCosine node

and symbolic_acosh node =
  if node == one then zero
  else create_arcHyperbolicCosine node

and symbolic_add node node' = match symbolic_add_if_possible node node' with
  | Some node -> node
  | None -> create_addition (insert node [node'])

and symbolic_and node node' = match symbolic_and_if_possible node node' with
  | Some node -> node
  | None -> create_and (insert node [node'])

and symbolic_asin node =
  if node == minus_one then minus_pi_over_two
  else if node == zero then zero
  else if node == one then pi_over_two
  else create_arcSine node

and symbolic_asinh node =
  if node == zero then zero
  else create_arcHyperbolicSine node

and symbolic_atan node =
  if node == zero then zero
  else create_arcTangent node

and symbolic_atanh node =
  if node == zero then zero
  else create_arcHyperbolicTangent node

and symbolic_blackBox name nodes = create_blackBox name nodes

and symbolic_cos node =
  if node == zero then one
  else create_cosine node

and symbolic_cosh node =
  if node == zero then one
  else create_arcHyperbolicCosine node

and symbolic_derive node' num = match num with
  | Int 0 -> node'
  | Int n when n > 0 ->
      symbolic_derive (symbolic_derivative node') (pred_num num)
  | _ -> assert false

and symbolic_derivative node' =
  let ( + ) = symbolic_add
  and ( - ) = symbolic_sub
  and ( * ) = symbolic_mult
  and ( / ) = symbolic_div
  and ( ** ) = symbolic_rationalPower
  in match node'.nature with
    | Integer _ | Number _ | Constant _ | DiscreteVariable _ | Floor _ |
      Parameter _ | Sign _ | BlackBox (_, []) -> zero
    | BlackBox _ | PartialDerivative _| Variable _ ->
        create_derivative node' one_num
    | Addition nodes ->
        List.fold_left (fun sum elt -> symbolic_derivative elt + sum) zero nodes
    | ArcCosine node ->
        symbolic_derivative node * minus_one /
        symbolic_sqrt (one - node ** two_num)
    | ArcHyperbolicCosine node ->
        symbolic_derivative node / symbolic_sqrt (node ** two_num - one)
    | ArcHyperbolicSine node ->
        symbolic_derivative node / symbolic_sqrt (node ** two_num + one)
    | ArcHyperbolicTangent node ->
        symbolic_derivative node / (node ** two_num - one)
    | ArcSine node ->
        symbolic_derivative node / symbolic_sqrt (one - node ** two_num)
    | ArcTangent node -> symbolic_derivative node / (one + node ** two_num)
    | Cosine node ->
        symbolic_derivative node * symbolic_minus (symbolic_sin node)
    | Derivative (node, num) -> create_derivative node (succ_num num)
    | Exponential node -> symbolic_derivative node * symbolic_exp node
    | HyperbolicCosine node -> symbolic_derivative node * symbolic_sinh node
    | HyperbolicSine node -> symbolic_derivative node * symbolic_cosh node
    | HyperbolicTangent node ->
        symbolic_derivative node * (one - node' ** two_num)
    | If (node, node', node'') ->
        create_if node (symbolic_derivative node') (symbolic_derivative node'')
    | Logarithm node -> symbolic_derivative node / node
    | Multiplication [] -> zero
    | Multiplication [node] -> symbolic_derivative node
    | Multiplication (node :: nodes) ->
        let mult = create_multiplication nodes in
        symbolic_derivative node * mult + node * symbolic_derivative mult
    | Pre node -> create_pre (symbolic_derivative node)
    | RationalPower (node, num) ->
        symbolic_derivative node * create_number num * node ** pred_num num
    | Sine node -> symbolic_derivative node * symbolic_cos node
    | Tangent node -> symbolic_derivative node * (one + node' ** two_num)
    | TimeVariable -> one
    | And _ | BooleanValue _ | Equality _ | Not _ | Or _ | Greater _ |
      GreaterEqual _ | String _ -> invalid_arg "symbolic_derivative"

and symbolic_div node node' =
  if node' == zero then raise (Infinite_result "Division by zero.")
  else if node == node' then one
  else symbolic_mult node (symbolic_invert node')

and symbolic_eq node node' =
  if node == node' then true_value
  else match node.nature, node'.nature with
    | Number num, Number num' -> create_booleanValue (eq_num num num')
    | _ -> create_equality node' node

and symbolic_exp node =
  if node == zero then one
  else if node == one then e
  else create_exponential node

and symbolic_ge node node' = match node.nature, node'.nature with
  | Number num, Number num' -> create_booleanValue (ge_num num num')
  | _ -> create_greater_equal node node'

and symbolic_gt node node' = match node.nature, node'.nature with
  | Number num, Number num' -> create_booleanValue (gt_num num num')
  | _ -> create_greater node node'

and symbolic_if node node' node'' =
  if node' == node'' then node'
  else match node.nature with
    | BooleanValue b -> if b then node' else node''
    | Not bool -> symbolic_if bool node'' node'
    | _ -> create_if node node' node''

and symbolic_floor node = match node.nature with
  | Number num -> create_number (floor_num num)
  | Floor node' -> node
  | _ -> create_floor node

and symbolic_invert node =
  if node == zero then raise (Infinite_result "Division by zero.")
  else match node.nature with
    | Number num -> create_number (div_num (one_num) num)
    | Multiplication nodes ->
        create_multiplication (
          sort (List.map (fun elt -> symbolic_invert elt) nodes))
    | _ -> symbolic_power node minus_one

and symbolic_le node node' = symbolic_ge node' node

and symbolic_log node =
  if node == zero then raise (Infinite_result "Logarithm of zero.")
  else create_logarithm node

and symbolic_log10 node = symbolic_div (symbolic_log node) (symbolic_log ten)

and symbolic_lt node node' = symbolic_gt node' node

and symbolic_max node node' = symbolic_if (symbolic_gt node node') node node'

and symbolic_min node node' = symbolic_if (symbolic_gt node node') node' node

and symbolic_minus node =
  if node == zero then zero
  else match node.nature with
    | Integer i -> create_integer (Int32.neg i)
    | Number num -> create_number (minus_num num)
    | Addition nodes ->
        create_addition (
          sort (List.rev_map (fun elt -> symbolic_minus elt) nodes))
    | _ -> symbolic_mult minus_one node

and symbolic_mult node node' = match symbolic_mult_if_possible node node' with
  | Some node -> node
  | None -> create_multiplication (insert node [node'])

and symbolic_neq node node' = symbolic_not (symbolic_eq node node')

and symbolic_not node = match node.nature with
  | BooleanValue b -> create_booleanValue (not b)
  | Not bool -> bool
  | _ -> create_not node

and symbolic_or node node' = match symbolic_or_if_possible node node' with
  | Some node -> node
  | None -> create_or (insert node [node'])

and symbolic_partial_derivative var node' =
  symbolic_partial_derivative_with eps var node'

and symbolic_partial_derivative_with step var node' =
  let ( + ) = symbolic_add
  and ( - ) = symbolic_sub
  and ( * ) = symbolic_mult
  and ( / ) = symbolic_div
  and ( ** ) = symbolic_rationalPower in
  let rec partial_derivative node =
    if node == var then one
    else match node.nature with
      | Integer _ | Number _ | Constant _ | Derivative _ | DiscreteVariable _ |
        Floor _ | Parameter _ | Sign _ | TimeVariable | Variable _ |
        BlackBox (_, []) -> zero
      | BlackBox _ ->
          let node' = replace var (var + step) node in (node' - node) / step
      | PartialDerivative _ ->
          create_partialDerivative var node
      | Addition nodes ->
          List.fold_left (fun sum elt -> partial_derivative elt + sum) zero nodes
      | ArcCosine node' ->
          partial_derivative node' * minus_one /
          symbolic_sqrt (one - node' ** two_num)
      | ArcHyperbolicCosine node' ->
          partial_derivative node' / symbolic_sqrt (node' ** two_num - one)
      | ArcHyperbolicSine node' ->
          partial_derivative node' / symbolic_sqrt (node' ** two_num + one)
      | ArcHyperbolicTangent node' ->
          partial_derivative node' / (node ** two_num - one)
      | ArcSine node' ->
          partial_derivative node' / symbolic_sqrt (one - node' ** two_num)
      | ArcTangent node' -> partial_derivative node' / (one + node' ** two_num)
      | Cosine node' ->
          partial_derivative node' * symbolic_minus (symbolic_sin node')
      | Exponential node' -> partial_derivative node' * symbolic_exp node'
      | HyperbolicCosine node' -> partial_derivative node' * symbolic_sinh node'
      | HyperbolicSine node' -> partial_derivative node' * symbolic_cosh node'
      | HyperbolicTangent node' ->
          partial_derivative node' * (one - node ** two_num)
      | If (cond, node', node'') ->
          symbolic_if cond (partial_derivative node') (partial_derivative node'')
      | Logarithm node' -> partial_derivative node' / node'
      | Multiplication [] -> zero
      | Multiplication [node'] -> partial_derivative node'
      | Multiplication (node' :: nodes) ->
          let mult = create_multiplication nodes in
          partial_derivative node' * mult + node' * partial_derivative mult
      | Pre node' -> create_pre (partial_derivative node')
      | RationalPower (node', num) ->
          partial_derivative node' * create_number num * node' ** pred_num num
      | Sine node' -> partial_derivative node' * symbolic_cos node'
      | Tangent node' -> partial_derivative node' * (one + node ** two_num)
      | And _ | BooleanValue _ | Equality _ | Not _ | Or _ | Greater _ |
        GreaterEqual _ | String _ ->
          invalid_arg "partial_derivative : Invalid argument."
  in partial_derivative node'

and symbolic_power node node' = match node'.nature with
  | Integer i -> symbolic_rationalPower node (num_of_int (Int32.to_int i))
  | Number num' -> symbolic_rationalPower node num'
  | _ -> create_exponential (symbolic_mult node' (symbolic_log node))

and symbolic_pre node = match node.nature with
  | Number _ -> node
  | _ -> create_pre node

and symbolic_rationalPower node num' =
  if node == zero && num' = zero_num then
    raise (Infinite_result "Zero raised to zero.")
  else if node == one || node == zero || num' = one_num then node
  else if num' = zero_num then one
  else match node.nature with
    | Number num when is_integer_num num' ->
        create_number (power_num num num')
    | RationalPower (node', num) ->
        symbolic_power node' (create_number (mult_num num num'))
    | _ -> create_rationalPower node num'

and symbolic_sgn node = match node.nature with
  | Number num -> create_number (Int (sign_num num))
  | _ -> create_sign node

and symbolic_sin node =
  if node == zero then zero
  else create_sine node

and symbolic_sinh node =
  if node == zero then zero
  else create_hyperbolicSine node

and symbolic_sqrt node = symbolic_power node one_over_two

and symbolic_sub node node' =
  if node' == zero then node
  else if node == node' then zero
  else symbolic_add node (symbolic_minus node')

and symbolic_tan node =
  if node == zero then zero
  else create_tangent node

and symbolic_tanh node =
  if node == zero then zero
  else create_hyperbolicTangent node


(* Intermediate functions *)

and symbolic_add_if_possible node node' =
  if node == zero then Some node'
  else if node' == zero then Some node
  else match node.nature, node'.nature with
    | Number num, Number num' -> Some (create_number (add_num num num'))
    | Number _, Addition nodes' ->
        add_number_to_addition_if_possible node nodes'
    | Multiplication _, Multiplication _ ->
        add_multiplication_to_multiplication_if_possible node node'
    | Multiplication _, Addition nodes' ->
        apply_if_possible create_addition symbolic_add_if_possible node nodes'
    | Multiplication _, _ ->
        add_expression_to_multiplication_if_possible node' node
    | Addition nodes, Number _ -> add_number_to_addition_if_possible node' nodes
    | Addition nodes, Multiplication _ ->
        apply_if_possible create_addition symbolic_add_if_possible node' nodes
    | Addition nodes, Addition _ ->
        Some (List.fold_left (fun sum elt -> symbolic_add elt sum) node' nodes)
    | Addition nodes, _ ->
        apply_if_possible create_addition symbolic_add_if_possible node' nodes
    | _, Multiplication _ ->
        add_expression_to_multiplication_if_possible node node'
    | _, Addition nodes' ->
        apply_if_possible create_addition symbolic_add_if_possible node nodes'
    | _ when node == node' -> Some (create_multiplication (insert two [node]))
    | _ -> None

and add_number_to_addition_if_possible node nodes =
  match node.nature, nodes with
    | Number _, [] -> Some node
    | Number num, node' :: nodes' ->
        begin match node'.nature with
          | Number num' -> let sum = add_num num num' in
              if sum == zero_num then Some (create_addition nodes')
              else Some (create_addition (insert (create_number sum) nodes'))
          | _ -> Some (create_addition (insert node nodes))
        end
    | _ -> invalid_arg "First Argument must be a number node."

and add_factors_if_possible num nodes num' nodes' =
  if eq_list nodes nodes' then
    let sum = create_number (add_num num num') in
      if sum == zero then Some zero
      else Some (create_multiplication (insert sum nodes))
  else None

and add_multiplication_to_multiplication_if_possible node node' =
  match node.nature, node'.nature with
    | Multiplication nodes, Multiplication nodes' ->
        begin match nodes, nodes' with
          | [], _ -> symbolic_add_if_possible one node'
          | _, [] -> symbolic_add_if_possible one node
          | node :: nodes2, node' :: nodes2' ->
              begin match node.nature, node'.nature with
                | Number num, Number num' ->
                    add_factors_if_possible num nodes2 num' nodes2'
                | Number num, _ ->
                    add_factors_if_possible num nodes2 one_num nodes'
                | _, Number num' ->
                    add_factors_if_possible one_num nodes num' nodes2'
                | _ ->
                    add_factors_if_possible one_num nodes one_num nodes'
              end
        end
    | _ -> invalid_arg "Arguments must be multiplication nodes."

and add_expression_to_multiplication_if_possible node node' =
  match node'.nature with
    | Multiplication [] -> symbolic_add_if_possible one node
    | Multiplication (node' :: nodes') ->
        begin match node'.nature with
          | Number num' -> add_factors_if_possible one_num [node] num' nodes'
          | _ -> None
        end
    | _ -> invalid_arg "Second argument must be a multiplication node."

and symbolic_mult_if_possible node node' =
  if node == zero || node' == zero then Some zero
  else if node == one then Some node'
  else if node' == one then Some node
  else match node.nature, node'.nature with
    | Number num, Number num' -> Some (create_number (mult_num num num'))
    | Number _, Addition nodes' ->
        Some (
          create_addition (
            sort (List.map (fun elt -> symbolic_mult node elt) nodes')))
    | Addition nodes, Number _ ->
        Some (
          create_addition (
            sort (List.map (fun elt -> symbolic_mult node' elt) nodes)))
    | Multiplication nodes, Multiplication _ ->
        Some (
          List.fold_left (fun prod elt -> symbolic_mult elt prod) node' nodes)
    | _, Multiplication nodes' ->
        apply_if_possible
          create_multiplication
          symbolic_mult_if_possible
          node
          nodes'
    | Multiplication nodes, _ ->
        apply_if_possible
          create_multiplication
          symbolic_mult_if_possible
          node'
          nodes
    | RationalPower _,RationalPower _ ->
        mult_power_by_power_if_possible node node'
    | RationalPower _, _ -> mult_expression_by_power_if_possible node' node
    | _, RationalPower _ -> mult_expression_by_power_if_possible node node'
    |_ when node == node' -> Some (symbolic_rationalPower node two_num)
    | _ -> None

and mult_expression_by_power_if_possible node node' = match node'.nature with
  | RationalPower (node', num') when node == node' ->
      Some (symbolic_rationalPower node (add_num one_num num'))
  | RationalPower _ -> None
  | _ -> invalid_arg "Second argument must be a power node."

and mult_power_by_power_if_possible node node' =
  match node.nature, node'.nature with
    | RationalPower (node, num), RationalPower (node', num')
      when node == node' ->
        Some (symbolic_rationalPower node (add_num num num'))
    | RationalPower _, RationalPower _ -> None
    | _ -> invalid_arg "Arguments must be power nodes."

and symbolic_and_if_possible node node' =
  if node == false_value || node' == false_value then Some false_value
  else if node == true_value then Some node'
  else if node' == true_value then Some node
  else if node == node' then Some node else
  match node.nature, node'.nature with
    | BooleanValue b, BooleanValue b' -> Some (create_booleanValue (b && b'))
    | Not bool, Not bool' -> Some (symbolic_not (symbolic_or bool bool'))
    | Not bool, _ when bool == node' -> Some false_value
    | _, Not bool' when node == bool' -> Some false_value
    | And nodes, And _ ->
        Some(List.fold_left (fun sum elt -> symbolic_and elt sum) node' nodes)
    | And nodes, _ ->
        apply_if_possible create_and symbolic_and_if_possible node' nodes
    | _, And nodes' ->
        apply_if_possible create_and symbolic_and_if_possible node nodes'
    | _ -> None

and symbolic_or_if_possible node node' =
  if node == true_value || node' == true_value then Some true_value
  else if node == false_value then Some node'
  else if node' == false_value then Some node
  else if node == node' then Some node else
  match node.nature, node'.nature with
    | BooleanValue b, BooleanValue b' -> Some (create_booleanValue (b || b'))
    | Not bool, Not bool' -> Some (symbolic_not (symbolic_and bool bool'))
    | Not bool, _ when bool == node' -> Some true_value
    | _, Not bool' when node == bool' -> Some true_value
    | Or nodes, Or _ ->
        Some(List.fold_left (fun sum elt -> symbolic_or elt sum) node' nodes)
    | Or nodes, _ ->
        apply_if_possible create_or symbolic_or_if_possible node' nodes
    | _, Or nodes' ->
        apply_if_possible create_or symbolic_or_if_possible node nodes'
    | _ -> None


(* Symbolic n-ary function application *)

and apply_addition nodes =
  List.fold_left (fun acc elt -> symbolic_add elt acc) addition_neutral nodes

and apply_and nodes =
  List.fold_left (fun acc elt -> symbolic_and elt acc) and_neutral nodes

and apply_blackBox s nodes = symbolic_blackBox s nodes

and apply_max = function
  | [] -> raise (Infinite_result "apply_max")
  | node :: nodes ->
      List.fold_left (fun acc elt -> symbolic_max elt acc) node nodes

and apply_min = function
  | [] -> raise (Infinite_result "apply_min")
  | node :: nodes ->
      List.fold_left (fun acc elt -> symbolic_min elt acc) node nodes

and apply_multiplication nodes =
  List.fold_left
    (fun acc elt -> symbolic_mult elt acc)
    multiplication_neutral
    nodes

and apply_or nodes =
  List.fold_left (fun acc elt -> symbolic_or elt acc) or_neutral nodes


(* Input/output *)

and output out_channel node =
  let mult_precedence = 50 in
  let rec precedence node = match node.nature with
    | Addition [] | And [] | BooleanValue _ | Constant _ | DiscreteVariable _ |
      Multiplication [] | Or [] | Parameter _ | TimeVariable | Variable _ |
      String _ ->
        1000
    | ArcCosine _ | ArcHyperbolicCosine _ | ArcHyperbolicSine _ |
      ArcHyperbolicTangent _ | ArcSine _ | ArcTangent _ | BlackBox _ |
      Cosine _ | Derivative _ | Exponential _ | Floor _ | HyperbolicCosine _ |
      HyperbolicSine _ | HyperbolicTangent _ | If _ | Logarithm _ | Not _ |
      PartialDerivative _ | Pre _ | Sign _ | Sine _| Tangent _ -> 10000
    | Addition _ -> 10
    | And _ -> 5
    | Equality _ -> 3
    | Multiplication _ -> mult_precedence
    | Integer i when i < 0l -> 75
    | Integer _ -> 1000
    | Number (Ratio _) -> mult_precedence
    | Number num when lt_num num zero_num -> 75
    | Number (Int _) | Number (Big_int _)  -> 1000
    | Or _ -> 7
    | RationalPower (_, num) when lt_num num zero_num -> mult_precedence
    | RationalPower _ -> 100
    | Greater _ -> 9
    | GreaterEqual _ -> 9
  and output'' node = match node.nature with
    | Addition [] -> output_char' '0'
    | Addition nodes' ->
      let not_negatives, negatives =
        List.partition
          (fun node -> match node.nature with
            | Number num when lt_num num zero_num -> false
            | Multiplication nodes ->
                begin
                  let not_reciprocals =
                    List.filter
                      (fun node -> match node.nature with
                        | RationalPower (_, num) when lt_num num zero_num ->
                            false
                        | _ -> true)
                      nodes
                  in match not_reciprocals with
                    | [] -> true
                    | node' :: _ ->
                        begin match node'.nature with
                          | Number num when lt_num num zero_num -> false
                          | _ -> true
                        end
                end
            | _ -> true)
          nodes'
      in
      begin match not_negatives with
        | [] -> ()
        | node' :: nodes' ->
            output' (precedence node) node';
            List.iter (fun elt -> output_string' " + ";
            output' (precedence node) elt) nodes'
      end;
      begin match negatives with
        | [] -> ()
        | nodes' -> List.iter (output' (precedence node)) nodes'
      end
    | And [] -> output_string' "true"
    | And (node' :: nodes') ->
        output' (precedence node) node';
        List.iter (fun elt -> output_string' " and ";
        output' (precedence node) elt) nodes'
    | ArcCosine node' -> output_string' "acos"; output' (precedence node) node'
    | ArcHyperbolicCosine node' ->
        output_string' "acosh"; output' (precedence node) node'
    | ArcHyperbolicSine node' ->
        output_string' "asinh"; output' (precedence node) node'
    | ArcHyperbolicTangent node' ->
        output_string' "atanh"; output' (precedence node) node'
    | ArcSine node' -> output_string' "asin"; output' (precedence node) node'
    | ArcTangent node' -> output_string' "atan"; output' (precedence node) node'
    | BlackBox (name, args) ->
        output_string' name;
        output_char' '('; output_arguments args; output_char' ')'
    | BooleanValue b -> output_string' (if b then "true" else "false")
    | Constant s -> output_string' s
    | Cosine node' -> output_string' "cos"; output' (precedence node) node'
    | Derivative (node', num') ->
        output_string' "der("; output' 0 node'; output_string' ", ";
        output_string' (string_of_num num'); output_char' ')'
    | Equality (node', node'') ->
        output' (precedence node) node'; output_string' " = ";
        output' (precedence node) node''
    | Exponential node' -> output_string' "exp"; output' (precedence node) node'
    | Floor node' -> output_string' "int"; output' (precedence node) node'
    | HyperbolicCosine node' ->
        output_string' "cosh"; output' (precedence node) node'
    | HyperbolicSine node' ->
        output_string' "sinh"; output' (precedence node) node'
    | HyperbolicTangent node' ->
        output_string' "tanh"; output' (precedence node) node'
    | If (node', node'', node''') ->
      output_string' "if("; output' 0 node';
      output_string' ", "; output' 0 node'';
      output_string' ", "; output' 0 node'''; output_char' ')'
    | DiscreteVariable i -> output_string' "discrete"; output_int' i
    | Logarithm node' -> output_string' "ln"; output' (precedence node) node'
    | Multiplication [] -> output_char' '1'
    | Multiplication nodes' ->
      let not_reciprocals, reciprocals = List.fold_left
        (fun (nodes, nodes') node -> match node.nature with
          | Number (Ratio _ as num) ->
            let ratio = ratio_of_num num in
            let numerator = num_of_big_int (Ratio.numerator_ratio ratio)
            and denominator = num_of_big_int (Ratio.denominator_ratio ratio) in
            if eq_num numerator one_num then
              nodes, (create_number denominator :: nodes')
            else
              (create_number numerator :: nodes),
              (create_number denominator :: nodes')
          | RationalPower (node'', num) when eq_num num (Int (-1)) ->
              nodes, (node'' :: nodes')
          | RationalPower (node'', num) when lt_num num zero_num ->
              nodes, (create_rationalPower node'' (minus_num num) :: nodes')
          | _ -> (node :: nodes), nodes')
        ([], [])
        nodes'
      in
      begin match List.rev not_reciprocals with
        | [] -> output_char' '1'
        | node' :: nodes' ->
            begin match node'.nature with
              | Number (Int -1) ->
                output_char' '-';
                begin match nodes' with
                  | [] -> output_char' '1'
                  | node' :: nodes' ->
                      output' (precedence node) node';
                      List.iter
                        (fun elt ->
                          output_string' " * "; output' (precedence node) elt)
                        nodes'
                end
              | _ ->
                output' (precedence node) node';
                List.iter
                  (fun elt ->
                    output_string' " * "; output' (precedence node) elt)
                  nodes'
            end
      end;
      begin match reciprocals with
        | [] -> ()
        | [node'] -> output_string' " / "; output' (precedence node + 1) node'
        | node' :: nodes' ->
            output_string' " / (";
            output' (precedence node) node';
            List.iter
              (fun elt -> output_string' " * "; output' (precedence node) elt)
              nodes';
            output_char' ')'
      end
    | Not node' -> output_string' "not"; output' (precedence node) node'
    | Integer i -> output_string' (Printf.sprintf "%ld" i)
    | Number num ->
        let s = string_of_float (float_of_num num) in
        if String.contains s '.' then output_string' s
        else output_string' (s ^ ".")
    | String s ->  output_string' (Printf.sprintf "\"%s\"" s)
    | Or [] -> output_string' "false"
    | Or (node' :: nodes') ->
        output' (precedence node) node';
        List.iter
          (fun elt -> output_string' " or "; output' (precedence node) elt)
          nodes'
    | Parameter i ->
        output_string' "parameter("; output_int' i; output_char' ')'
    | PartialDerivative (node', node'') ->
        output_string' "pder("; output' 0 node'';
        output_string' ", "; output' 0 node'; output_char' ')'
    | Pre node' -> output_string' "pre"; output' (precedence node) node'
    | RationalPower (node', num) when ge_num num zero_num ->
        output' (precedence node) node'; output_string' " ^ ";
        begin match num with
          | Int _ | Big_int _ -> output_string' (string_of_num num)
          | Ratio _ ->
              output_char' '('; output_string' (string_of_num num);
              output_char' ')'
        end
    | RationalPower (node', num) when eq_num num (Int (-1)) ->
        output_string' "1 / "; output' (mult_precedence + 1) node'
    | RationalPower (node', num) ->
        output_string' "1 / "; output' (precedence node) node';
        output_string' " ^ ";
        begin match num with
          | Int _ | Big_int _ -> output_string' (string_of_num (minus_num num))
          | Ratio _ ->
              output_char' '('; output_string' (string_of_num (minus_num num));
              output_char' ')'
        end
    | Sign node' -> output_string' "sgn"; output' (precedence node) node'
    | Sine node' -> output_string' "sin"; output' (precedence node) node'
    | Greater (node', node'') ->
        output' (precedence node) node'; output_string' " > ";
        output' (precedence node) node''
    | GreaterEqual (node', node'') ->
        output' (precedence node) node'; output_string' " >= ";
        output' (precedence node) node''
    | Tangent node' -> output_string' "tan"; output' (precedence node) node'
    | TimeVariable -> output_string' "time"
    | Variable i -> output_string' "variable("; output_int' i; output_char' ')'
  and output_char' c = output_char out_channel c
  and output_int' i = output_string out_channel (string_of_int i)
  and output_string' s = output_string out_channel s
  and output' prec node =
    if precedence node <= prec then begin
      output_string' "(";  output'' node; output_string' ")"
    end else output'' node
  and output_arguments args =
    let rec output_arguments' = function
      | [] -> ()
      | [arg] -> output_argument arg
      | arg :: args ->
          output_argument arg; output_string' ", "; output_arguments' args in
    output_char' '('; output_arguments' args; output_char' ')'
  and output_argument arg =
    match arg with
    | ScalarArgument node -> output' 0 node
    | ArrayArgument (dims, nodes) -> output_array_argument dims nodes
  and output_array_argument dims nodes =
    let rec repeat n printf i =
      if n = 0 then i
      else if n = 1 then printf i
      else
        let i = printf i in
        output_string' ", ";
        repeat (n - 1) printf i in
    let rec output_array_argument' dim dims i = match dims with
      | [] -> repeat dim (fun i -> output' 0 nodes.(i); i + 1) i
      | dim' :: dims ->
          repeat
            dim
            (fun i ->
              output_char' '{';
              let i = output_array_argument' dim' dims i in
              output_char' '}'; i)
            i in
    match dims with
    | [] -> assert false
    | dim :: dims ->
        output_char' '{';
        let _ = output_array_argument' dim dims 0 in ();
        output_char' '}' in
  output' 0 node


(* Symbolic manipulation helpers *)

and exists p node =
  let exists_in_argument = function
    | ScalarArgument node -> exists p node
    | ArrayArgument (_, nodes) -> exists_array p nodes in
  p node || match node.nature with
    | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> false
    | ArcCosine node | ArcHyperbolicCosine node |
      ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
      ArcTangent node | Cosine node | Derivative (node, _) |
      Exponential node | Floor node | HyperbolicCosine node |
      HyperbolicSine node | HyperbolicTangent node | Logarithm node |
      Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
      Tangent node -> exists p node
    | Equality (node1, node2) | Greater (node1, node2) |
      GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
        exists p node1 || exists p node2
    | If (node1, node2, node3) ->
        exists p node1 || exists p node2 || exists p node3
    | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
        List.exists (exists p) nodes
    | BlackBox (_, args) -> List.exists exists_in_argument args

and is_subnode_of node node' = exists (fun node -> node == node') node

and assignable_parameters_of node =
  let assignable_parameters_of_argument = function
    | ScalarArgument node -> assignable_parameters_of node
    | ArrayArgument (_, nodes) ->
        Array.fold_left
          (fun acc node -> union (assignable_parameters_of node) acc)
          []
          nodes in
  match node.nature with
  | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ | Pre _ |
    Variable _ | TimeVariable | Integer _ | String _ -> []
  | Parameter _ -> [node]
  | ArcCosine node | ArcHyperbolicCosine node |
    ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
    ArcTangent node | Cosine node | Derivative (node, _) |
    Exponential node | Floor node | HyperbolicCosine node |
    HyperbolicSine node | HyperbolicTangent node | Logarithm node |
    Not node | RationalPower (node, _) | Sign node | Sine node |
    Tangent node -> assignable_parameters_of node
  | Equality (node1, node2) | Greater (node1, node2) |
    GreaterEqual (node1, node2) | PartialDerivative (node1, node2) |
    If (_, node1, node2) ->
      union
        (assignable_parameters_of node1)
        (assignable_parameters_of node2)
  | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
      List.fold_left
        (fun acc node -> union (assignable_parameters_of node) acc)
        []
        nodes
  | BlackBox (_, args) ->
      List.fold_left
        (fun acc arg -> union (assignable_parameters_of_argument arg) acc)
        []
        args

and variables_of node =
  let variables_of_argument = function
    | ScalarArgument node -> variables_of node
    | ArrayArgument (_, nodes) ->
        Array.fold_left
          (fun acc node -> union (variables_of node) acc)
          []
          nodes in
  match node.nature with
  | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ | Parameter _ |
    TimeVariable | Integer _ | String _ -> []
  | Variable _ -> [node]
  | ArcCosine node | ArcHyperbolicCosine node |
    ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
    ArcTangent node | Cosine node | Derivative (node, _) |
    Exponential node | Floor node | HyperbolicCosine node |
    HyperbolicSine node | HyperbolicTangent node | Logarithm node |
    Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
    Tangent node -> variables_of node
  | Equality (node1, node2) | Greater (node1, node2) |
    GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
      union (variables_of node1) (variables_of node2)
  | If (node1, node2, node3) ->
      union
        (variables_of node1)
        (union (variables_of node2) (variables_of node3))
  | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
      List.fold_left (fun acc node -> union (variables_of node) acc) [] nodes
  | BlackBox (_, args) ->
      List.fold_left
        (fun acc arg -> union (variables_of_argument arg) acc)
        []
        args

and assignable_variables_of node =
  let assignable_variables_of_argument = function
    | ScalarArgument node -> assignable_variables_of node
    | ArrayArgument (_, nodes) ->
        Array.fold_left
          (fun acc node -> union (assignable_variables_of node) acc)
          []
          nodes in
  match node.nature with
  | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ | Parameter _ |
    Pre _ | TimeVariable | Integer _ | String _ -> []
  | Variable _ -> [node]
  | ArcCosine node | ArcHyperbolicCosine node |
    ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
    ArcTangent node | Cosine node | Derivative (node, _) |
    Exponential node | Floor node | HyperbolicCosine node |
    HyperbolicSine node | HyperbolicTangent node | Logarithm node |
    RationalPower (node, _) | Sign node | Sine node | Tangent node ->
      assignable_variables_of node
  | Equality (node1, node2) | Greater (node1, node2) |
    GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
      union (assignable_variables_of node1) (assignable_variables_of node2)
  | If (_, node1, node2) ->
      union (* intersection is too pessimistic (since v.1.1.4. *)
        (assignable_variables_of node1)
        (assignable_variables_of node2)
  | Addition nodes | Multiplication nodes ->
      List.fold_left
        (fun acc node -> union (assignable_variables_of node) acc)
        []
        nodes
  | BlackBox (_, args) ->
      List.fold_left
        (fun acc arg -> union (assignable_variables_of_argument arg) acc)
        []
        args
  | And _ | Or _ | Not _ -> []

and derivatives_of node =
  let derivatives_of_argument = function
    | ScalarArgument node -> derivatives_of node
    | ArrayArgument (_, nodes) ->
        Array.fold_left
          (fun acc node -> union (derivatives_of node) acc)
          []
          nodes in
  match node.nature with
  | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ | Parameter _ |
    TimeVariable | Variable _ | Integer _ | String _ -> []
  | ArcCosine node' | ArcHyperbolicCosine node' |
    ArcHyperbolicSine node' | ArcHyperbolicTangent node' | ArcSine node' |
    ArcTangent node' | Cosine node' | Exponential node' | Floor node' |
    HyperbolicCosine node' | HyperbolicSine node' | HyperbolicTangent node' |
    Logarithm node' | Not node' | Pre node' | RationalPower (node', _) |
    Sign node' | Sine node' | Tangent node' -> derivatives_of node'
  | Derivative _ -> [node]
  | Equality (node1, node2) | Greater (node1, node2) |
    GreaterEqual (node1, node2) | If (_, node1, node2) |
    PartialDerivative (node1, node2) ->
      union (derivatives_of node1) (derivatives_of node2)
  | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
      List.fold_left (fun acc node -> union (derivatives_of node) acc) [] nodes
  | BlackBox (_, args) ->
      List.fold_left
        (fun acc arg -> union (derivatives_of_argument arg) acc)
        []
        args

and inputs_of node =
  let inputs_of_argument = function
    | ScalarArgument node -> inputs_of node
    | ArrayArgument (_, nodes) ->
        Array.fold_left
          (fun acc node -> union (inputs_of node) acc)
          []
          nodes in
  match node.nature with
  | BooleanValue _  | Constant _ | Variable _ | Number _ | Parameter _ |
    TimeVariable | Integer _ | String _ -> []
  | DiscreteVariable i when i < 0 -> [node]
  | DiscreteVariable _ -> []
  | ArcCosine node | ArcHyperbolicCosine node |
    ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
    ArcTangent node | Cosine node | Derivative (node, _) |
    Exponential node | Floor node | HyperbolicCosine node |
    HyperbolicSine node | HyperbolicTangent node | Logarithm node |
    Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
    Tangent node -> inputs_of node
  | Equality (node1, node2) | Greater (node1, node2) |
    GreaterEqual (node1, node2) | PartialDerivative (node1, node2) ->
      union (inputs_of node1) (inputs_of node2)
  | If (node1, node2, node3) ->
      union
        (inputs_of node1)
        (union (inputs_of node2) (inputs_of node3))
  | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
      List.fold_left (fun acc node -> union (inputs_of node) acc) [] nodes
  | BlackBox (_, args) ->
      List.fold_left
        (fun acc arg -> union (inputs_of_argument arg) acc)
        []
        args

and invert_if_possible_with_respect_to node left right =
  let not_null node = match node.nature with
    | Constant _ -> true
    | Number num -> num <>/ zero_num
    | _ -> false
  in
  let invert_addition_if_possible nodes =
    match List.partition (fun node' -> is_subnode_of node' node) nodes with
      | [node'], nodes' -> Some (node', nodes')
      | [], _ -> invalid_arg "invert_addition_if_possible"
      | _ -> None
  and invert_multiplication_if_possible nodes =
    match List.partition (fun node' -> is_subnode_of node' node) nodes with
      | [node'], nodes' when List.for_all not_null nodes' -> Some (node', nodes')
      | [], _ -> invalid_arg "invert_multiplication_if_possible"
      | _ -> None
  in
  if node == left then Some right
  else match left.nature with
    | BlackBox _ | Cosine _ | Derivative _ | Floor _ | HyperbolicCosine _ |
      PartialDerivative _ | Pre _ | Sign _ | Sine _ | Tangent _ -> None
    | Addition nodes ->
        begin match invert_addition_if_possible nodes with
          | None -> None
          | Some (node', nodes') ->
              invert_if_possible_with_respect_to
                node
                node'
                (symbolic_sub right (create_addition (sort nodes')))
        end
    | ArcCosine node' ->
        invert_if_possible_with_respect_to node node' (symbolic_cos right)
    | ArcHyperbolicCosine node' ->
        invert_if_possible_with_respect_to node node' (symbolic_cosh right)
    | ArcHyperbolicSine node' ->
        invert_if_possible_with_respect_to node node' (symbolic_sinh right)
    | ArcHyperbolicTangent node' ->
        invert_if_possible_with_respect_to node node' (symbolic_tanh right)
    | ArcSine node' ->
        invert_if_possible_with_respect_to node node' (symbolic_sin right)
    | ArcTangent node' ->
        invert_if_possible_with_respect_to node node' (symbolic_tan right)
    | Exponential node' ->
        invert_if_possible_with_respect_to node node' (symbolic_log right)
    | HyperbolicSine node' ->
        invert_if_possible_with_respect_to node node' (symbolic_asinh right)
    | HyperbolicTangent node' ->
        invert_if_possible_with_respect_to node node' (symbolic_atanh right)
    | If (cond, node', node'') -> None
    | Logarithm node' ->
        invert_if_possible_with_respect_to node node' (symbolic_exp right)
    | Multiplication nodes ->
        begin match invert_multiplication_if_possible nodes with
          | None -> None
          | Some (node', nodes') ->
              invert_if_possible_with_respect_to
                node
                node'
                (symbolic_div right (create_multiplication (sort nodes')))
        end
    | RationalPower (node', num)
      when is_integer_num num && eq_num (mod_num num two_num) zero_num -> None
    | RationalPower (node', num)
      when is_integer_num num ->
        invert_if_possible_with_respect_to
          node
          node'
          (symbolic_rationalPower right (one_num // num))
    | RationalPower _ -> None
    | And _ | Constant _ | BooleanValue _ | Equality _ | Greater _ |
      GreaterEqual _ | DiscreteVariable _ | Not _ | Number _ | Or _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ ->
        invalid_arg "invert_if_possible_with_respect_to"

and exists_except_in_conditions p node =
  let exists_except_in_conditions_argument = function
    | ScalarArgument node -> exists_except_in_conditions p node
    | ArrayArgument (_, nodes) ->
        exists_array (exists_except_in_conditions p) nodes in
  p node || match node.nature with
    | BooleanValue _  | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> false
    | ArcCosine node | ArcHyperbolicCosine node |
      ArcHyperbolicSine node | ArcHyperbolicTangent node | ArcSine node |
      ArcTangent node | Cosine node | Derivative (node, _) |
      Exponential node | Floor node | HyperbolicCosine node |
      HyperbolicSine node | HyperbolicTangent node | Logarithm node |
      Not node | Pre node | RationalPower (node, _) | Sign node | Sine node |
      Tangent node -> exists_except_in_conditions p node
    | Equality (node1, node2) | Greater (node1, node2) |
      GreaterEqual (node1, node2) | If (_, node1, node2) |
      PartialDerivative (node1, node2) ->
        exists_except_in_conditions p node1 ||
        exists_except_in_conditions p node2
    | And nodes | Addition nodes | Multiplication nodes | Or nodes ->
        List.exists (exists_except_in_conditions p) nodes
    | BlackBox (_, args) ->
        List.exists exists_except_in_conditions_argument args

and inversion_difficulty node left right =
  let is_derivative_of_node node' = match node'.nature with
    | Derivative (node', _) -> node' == node
    | _ -> false
  in match derivatives_of left with
    | ders when List.exists is_derivative_of_node ders -> 0
    | [] ->
        begin
          try match invert_if_possible_with_respect_to node left right with
            | Some _ -> 1
            | None -> 2
          with
            | Invalid_argument _ -> 3
        end
    | _ -> 2

and replace node node' node'' =
  let rec rewrite_argument = function
    | ScalarArgument node -> ScalarArgument (rewrite node)
    | ArrayArgument (dims, nodes) ->
        ArrayArgument (dims, Array.map rewrite nodes)
  and rewrite node'' =
    if node'' == node then node'
    else match node''.nature with
    | Addition nodes -> apply_addition (List.map rewrite nodes)
    | And nodes -> apply_and (List.map rewrite nodes)
    | ArcCosine node -> symbolic_acos (rewrite node)
    | ArcHyperbolicCosine node -> symbolic_acosh (rewrite node)
    | ArcHyperbolicSine node -> symbolic_asinh (rewrite node)
    | ArcHyperbolicTangent node -> symbolic_atanh (rewrite node)
    | ArcSine node -> symbolic_asin (rewrite node)
    | ArcTangent node -> symbolic_atan (rewrite node)
    | BlackBox (s, args) -> apply_blackBox s (List.map rewrite_argument args)
    | Cosine node -> symbolic_cos (rewrite node)
    | Derivative (node, num) -> symbolic_derive (rewrite node) num
    | Equality (node, node') -> symbolic_eq (rewrite node) (rewrite node')
    | Exponential node -> symbolic_exp (rewrite node)
    | Floor node -> symbolic_floor (rewrite node)
    | Greater (node, node') -> symbolic_gt (rewrite node) (rewrite node')
    | GreaterEqual (node, node') -> symbolic_ge (rewrite node) (rewrite node')
    | HyperbolicCosine node -> symbolic_cosh (rewrite node)
    | HyperbolicSine node -> symbolic_sinh (rewrite node)
    | HyperbolicTangent node -> symbolic_tanh (rewrite node)
    | If (node, node', node'') ->
        symbolic_if (rewrite node) (rewrite node') (rewrite node'')
    | Logarithm node -> symbolic_log (rewrite node)
    | Multiplication nodes -> apply_multiplication (List.map rewrite nodes)
    | Not node -> symbolic_not (rewrite node)
    | Or nodes -> apply_or (List.map rewrite nodes)
    | PartialDerivative (node, node') ->
        create_partialDerivative (rewrite node) (rewrite node')
    | Pre node -> symbolic_pre (rewrite node)
    | RationalPower (node, num) -> symbolic_rationalPower (rewrite node) num
    | Sign node -> symbolic_sgn (rewrite node)
    | Sine node -> symbolic_sin (rewrite node)
    | Tangent node -> symbolic_tan (rewrite node)
    | BooleanValue _ | Constant _ | DiscreteVariable _ | Number _ |
      Parameter _ | TimeVariable | Variable _ | Integer _ | String _ -> node''
  in
  rewrite node''
