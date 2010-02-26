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

(* Imperative implementation by Esko Nuutila and Eljas Soisalon-Soininen in   *)
(* ON FINDING THE STRONGLY CONNECTED COMPONENTS IN A DIRECTED GRAPH (1994)    *)

type t = Graph of bool ref * nodes
and nodes = node array
and node =
  {
    contents : int;
    mutable index : int;
    mutable not_visited : bool;
    mutable root : node;
    mutable in_component : bool;
    mutable next_nodes : node list
  }

let min node node' = if node.index < node'.index then node else node'
let gt node node' = node.index > node'.index
let empty stack = !stack = []
let push elt stack = stack := elt :: !stack
let pop stack = match !stack with
    | [] -> failwith "top"
    | node :: nodes -> stack := nodes; node
let top stack = match !stack with
    | [] -> failwith "top"
    | node :: _ -> node

let visit1 nodes =
    let stack = ref []
    and index = ref 0
    and res_ref = ref [] in
    let rec visit1' current_node =
        current_node.index <- !index;
        current_node.not_visited <- false;
        current_node.root <- current_node;
        current_node.in_component <- false;
        index := !index + 1;
        List.iter
            (fun node ->
                if node.not_visited then visit1' node;
                if not node.in_component then current_node.root <- min node.root current_node.root)
            current_node.next_nodes;
        if current_node.root == current_node then begin
            current_node.in_component <- true;
            let comp_ref = ref [current_node.contents] in
            while (not (empty stack) && gt (top stack) current_node) do
                let node = pop stack in
                node.in_component <- true;
                comp_ref := node.contents :: !comp_ref
            done;
            res_ref := !comp_ref :: !res_ref
        end else push current_node stack
    in
        Array.iter (fun node -> if node.not_visited then visit1' node) nodes;
        !res_ref

let erase_marks nodes =
    Array.iter
        (fun node ->
            node.index <- 0;
            node.not_visited <- true;
            node.root <- node;
            node.in_component <- false)
        nodes

let create size =
    let nodes = Array.init size
        (fun i ->
            let rec node =
              {
                contents = i;
                index = 0;
                not_visited = true;
                root = node;
                in_component = false;
                next_nodes = []
              }
            in node)
    in Graph (ref true, nodes)

let connect i j (Graph (_, nodes)) =
    let node = nodes.(i)
    and node' = nodes.(j)
    in node.next_nodes <- node' :: node.next_nodes

let strongly_connected_components (Graph (ready, nodes)) = match !ready with
    | true -> ready := false; visit1 nodes
    | false -> erase_marks nodes; ready := false; visit1 nodes

let print_with print_fun (Graph (_, nodes)) =
    Array.iter
        (fun node ->
            List.iter
                (fun node' ->
                    print_fun node.contents;
                    print_string " --> ";
                    print_fun node'.contents;
                    print_newline ())
                node.next_nodes)
        nodes
