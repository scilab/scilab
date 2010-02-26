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

module type MatrixElement =
  sig
    type t
    val zero: t
    val infinity: t
    val equal: t -> t -> bool
    val compare: t -> t -> int
    val add: t -> t -> t
    val sub: t -> t -> t
  end

module type Matrix =
  sig
    type elt
    type t
    val init: int -> elt -> (int -> int -> elt) -> t
    val iterij: (int -> int -> elt -> unit) -> t -> unit
    val row_storage_iterj: int -> (int -> elt -> unit) -> t -> unit
    val update_row_storage: int -> (int -> elt -> elt) -> t -> unit
    val column_storage_iteri: int -> (int -> elt -> unit) -> t -> unit
    val update_column_storage: int -> (int -> elt -> elt) -> t -> unit
  end

module type BipartiteGraph =
  sig
    type t
    val create: int -> t
    val link: int -> int -> t -> unit
    val ford_and_fulkerson: t -> int * (int * int option) list
  end

module type S =
  sig
    type elt
    type t
    val init: int -> (int -> int -> elt) -> t
    val perform: t -> (int * int option) list
    val print_with: (elt -> unit) -> t -> unit
  end

module Make
  (E: MatrixElement)
  (M: Matrix with type elt = E.t)
  (B: BipartiteGraph):
    (S with type elt = E.t) =

  struct

    type elt = E.t

    type t =
      {
        size: int;
        row_marks: bool array;
        column_marks: bool array;
        framed_zeros: int array;
        matrix: M.t;
        graph: B.t
      }

    let init size f =
      let matrix =
        M.init size E.infinity f
      in
        {
          size = size;
          row_marks = Array.make size false;
          column_marks = Array.make size false;
          framed_zeros = Array.make size (-1);
          (* -1 ensures is_framed_zero to work properly *)
          matrix = matrix;
          graph = B.create size
        }

    let minimum elt elt' = if E.compare elt elt' < 0 then elt else elt'

    let perform strct =
      let min_row i =
        let min = ref E.infinity in
        M.row_storage_iterj
          i
          (fun _ elt -> min := minimum !min elt)
          strct.matrix;
        !min
      and min_column j =
        let min = ref E.infinity in
        M.column_storage_iteri
          j
          (fun _ elt -> min := minimum !min elt)
          strct.matrix;
        !min
      and sub_row i elt =
        M.update_row_storage i (fun _ elt' -> E.sub elt' elt) strct.matrix
      and sub_column j elt =
        M.update_column_storage j (fun _ elt' -> E.sub elt' elt) strct.matrix
      and is_framed_zero i j = strct.framed_zeros.(j) = i
      and is_marked_row i = strct.row_marks.(i)
      and is_marked_column j = strct.column_marks.(j)
      and mark_row i = strct.row_marks.(i) <- true
      and mark_column j = strct.column_marks.(j) <- true
      and size = strct.size in
      let init_data () =
        for i = 0 to size - 1 do
          sub_row i (min_row i)
        done;
        for j = 0 to size - 1 do
          sub_column j (min_column j)
        done;
        for i = 0 to size - 1 do
          M.row_storage_iterj
            i
            (fun j elt ->
              if E.compare elt E.zero = 0 then B.link i j strct.graph)
            strct.matrix
        done
      and element_to_subtract () =
        let min = ref E.infinity in
        for i = 0 to size - 1 do
          if is_marked_row i then
            M.row_storage_iterj
              i
              (fun j elt ->
                if not (is_marked_column j) then min := minimum !min elt)
              strct.matrix
        done;
        !min
      in
      let update_matrix_and_graph elt =
        for i = 0 to size - 1 do
          if is_marked_row i then
            M.update_row_storage
              i
              (fun j elt' ->
                  if is_marked_column j then elt'
                  else
                    let elt'' = E.sub elt' elt in
                    if E.compare elt'' E.zero = 0 then B.link i j strct.graph;
                    elt'')
              strct.matrix
          else
            M.update_row_storage
              i
              (fun j elt' ->
                  if is_marked_column j then E.add elt' elt
                  else elt')
              strct.matrix
        done
      in
      let rec first_mark () = match B.ford_and_fulkerson strct.graph with
        | size', pairs when size' = size ->
            List.map
              (fun (i, opt) -> match opt with
                | Some j -> i, Some j
                | None -> assert false)
              pairs
        | _, pairs ->
            let marked_rows =
              List.fold_left
                (fun marked_rows (i, opt) -> match opt with
                  | None -> mark_row i; i :: marked_rows (* side effect *)
                  | Some j ->
                      strct.framed_zeros.(j) <- i;
                      marked_rows) (* side effect *)
                []
                pairs
            in second_mark marked_rows
      and second_mark marked_rows =
        let marked_columns_ref = ref [] in
        List.iter
          (fun i ->
            M.row_storage_iterj
              i
              (fun j elt ->
                if not (is_marked_column j) then
                  if E.compare elt E.zero = 0 && not (is_framed_zero i j) then
                    begin
                      mark_column j; (* side effect *)
                      marked_columns_ref := j :: !marked_columns_ref
                      (* side effect *)
                    end)
              strct.matrix)
          marked_rows;
        third_mark !marked_columns_ref
      and third_mark marked_columns =
        let marked_rows =
          List.fold_left
            (fun marked_rows j -> match strct.framed_zeros.(j) with
              | i when i >= 0 && not (is_marked_row i) ->
                  mark_row i; i :: marked_rows (* side effect *)
              | _ -> marked_rows)
            []
            marked_columns
        in match marked_rows with
          | [] -> update_data ()
          | _ -> second_mark marked_rows
      and update_data () =
        let elt = element_to_subtract () in
        match E.compare elt E.infinity with
          | 0 ->
              let res_ref = ref [] in
              Array.iteri
                  (fun i elt -> match elt with
                      | -1 -> res_ref := (i, None) :: !res_ref
                      | j -> res_ref := (i, Some j) :: !res_ref)
                  strct.framed_zeros;
              !res_ref
          | _ ->
              update_matrix_and_graph elt;
              for i = 0 to size - 1 do
                strct.row_marks.(i) <- false;
                strct.column_marks.(i) <- false
              done;
              first_mark ()
      in init_data (); first_mark ()

    let print_with print_fun strct =
      M.iterij
        (fun i j elt ->
          print_int i;
          print_string ", ";
          print_int j;
          print_string " -> ";
          print_fun elt;
          print_newline ())
        strct.matrix

  end
