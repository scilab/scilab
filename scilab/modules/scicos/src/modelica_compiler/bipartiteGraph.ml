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

type t = Bipartite of node array * node array
and node =
  {
    index : int;
    side : side;
    mutable solved : bool;
    mutable mark : mark;
    mutable edges : edge list;
  }
and side = Left | Right
and mark = NoMark | Source | Mark of edge
and edge = Edge of node * node * flow ref
and flow = Empty | Full
and result = Failed | Succeed

let create size =
    let create_node side i =
          {
            index = i;
            side = side;
            solved = false;
            mark = NoMark;
            edges = []
          } in
    let left_nodes = Array.init size (fun i -> create_node Left i)
    and right_nodes = Array.init size (fun i -> create_node Right i)
    in Bipartite (left_nodes, right_nodes)

(* In a bipartite graph, left-side nodes can only be linked to right-side ones and vice-versa *)
let link i j (Bipartite (left_nodes, right_nodes)) =
    let node = left_nodes.(i)
    and node' = right_nodes.(j) in
    let edge = Edge (node, node', ref Empty)
    in node.edges <- edge :: node.edges; node'.edges <- edge :: node'.edges

(* Invariant: when an edge's flow is full, its source and destination nodes are solved *)
let fill i j (Bipartite (left_nodes, right_nodes)) =
    let node = left_nodes.(i)
    and node' = right_nodes.(j) in
    let Edge (node, node', flow) = List.find
        (fun (Edge (_, node'', _)) -> node' == node'')
        node.edges
    in
        if node.solved = true || node'.solved = true && !flow = Empty then invalid_arg "fill"
        else flow := Full; node.solved <- true; node'.solved <- true

let ford_and_fulkerson (Bipartite (left_nodes, right_nodes)) =
    let rec first_mark i marked_left_nodes =
        if i < 0 then mark_right_nodes [] marked_left_nodes
        else match left_nodes.(i) with
            | x when not x.solved -> x.mark <- Source; first_mark (i - 1) (x :: marked_left_nodes)
            | _ -> first_mark (i - 1) marked_left_nodes
    and mark_right_nodes marked_right_nodes = function
        | [] -> mark_left_nodes [] marked_right_nodes
        | x :: xs -> mark_right_nodes (add_right_nodes marked_right_nodes x.edges) xs
    and add_right_nodes marked_right_nodes = function
        | [] -> marked_right_nodes
        | (Edge (_, node, flow) as x) :: xs when node.mark = NoMark && !flow = Empty ->
            node.mark <- Mark x; add_right_nodes (node :: marked_right_nodes) xs
        | _ :: xs -> add_right_nodes marked_right_nodes xs
    and mark_left_nodes marked_left_nodes = function
        | [] when marked_left_nodes = [] -> Failed
        | [] -> mark_right_nodes [] marked_left_nodes
        | x :: _ when not x.solved -> x.solved <- true; update_edges_from x; Succeed
        | x :: xs -> mark_left_nodes (add_left_nodes marked_left_nodes x.edges) xs
    and add_left_nodes marked_left_nodes = function
        | [] -> marked_left_nodes
        | (Edge (node, _, flow) as x) :: xs when node.mark = NoMark && !flow = Full ->
            node.mark <- Mark x; add_left_nodes (node :: marked_left_nodes) xs
        | _ :: xs -> add_left_nodes marked_left_nodes xs
    and update_edges_from node = match node with
        | { mark = Source } -> node.solved <- true
        | { mark = Mark (Edge (node', node'', flow)) } when node == node' ->
            flow := Empty; update_edges_from node''
        | { mark = Mark (Edge (node', node'', flow)) } when node == node'' ->
            flow := Full; update_edges_from node'
        | _ -> assert false in
    let erase_marks () =
        Array.iter (fun node -> node.mark <- NoMark) left_nodes;
        Array.iter (fun node -> node.mark <- NoMark) right_nodes;
    and mark () = first_mark (Array.length left_nodes - 1) []
    and return_pairs () =
        let rec succ_from = function
            | [] -> None
            | Edge (_, node, flow) :: xs when !flow = Full -> Some node.index
            | _ :: xs -> succ_from xs
        in Array.fold_left
            (fun (n, pairs) node ->
                match succ_from node.edges with
                    | Some index as res -> (n + 1, ((node.index, res) :: pairs))
                    | None -> (n, ((node.index, None) :: pairs)))
            (0, [])
            left_nodes
    in
    erase_marks ();
    while (mark () = Succeed) do erase_marks () done;
    return_pairs ()
