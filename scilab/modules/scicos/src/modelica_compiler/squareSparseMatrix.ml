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
        val equal : t -> t -> bool
    end

module type S =
    sig
        type elt
        type t
        val make : int -> elt -> t
        val init : int -> elt -> (int -> int -> elt) -> t
        val size : t -> int * int
        val get : t -> int -> int -> elt
        val set : t -> int -> int -> elt -> unit
        val iter : (elt -> unit) -> t -> unit
        val iterij : (int -> int -> elt -> unit) -> t -> unit
        val row_iter : int -> (elt -> unit) -> t -> unit
        val row_iterj : int -> (int -> elt -> unit) -> t -> unit
        val row_storage_iterj : int -> (int -> elt -> unit) -> t -> unit
        val update_row_storage : int -> (int -> elt -> elt) -> t -> unit
        val column_iter : int -> (elt -> unit) -> t -> unit
        val column_iteri : int -> (int -> elt -> unit) -> t -> unit
        val column_storage_iteri : int -> (int -> elt -> unit) -> t -> unit
        val update_column_storage : int -> (int -> elt -> elt) -> t -> unit
    end

module Make (M : MatrixElement) : (S with type elt = M.t) =

    struct

        type elt = M.t
        type t =
          {
            default : elt;
            rows : (int * elt ref) list array;
            columns : (int * elt ref) list array
          }

        let make size elt =
          {
            default = elt;
            rows = Array.make size [];
            columns = Array.make size []
          }

        let init size elt f =
            let rows = Array.make size []
            and columns = Array.make size [] in
            for i = 0 to size - 1 do
                for j = 0 to size - 1 do
                    let elt' = f i j in
                    if M.equal elt' elt then ()
                    else
                        let elt'_ref = ref elt' in
                        rows.(i) <- (j, elt'_ref) :: rows.(i);
                        columns.(j) <- (i, elt'_ref) :: columns.(j)
                done
            done;
              {
                default = elt;
                rows = rows;
                columns = columns
              }

        let size mtrx = let size = Array.length mtrx.rows in (size, size)

        let get mtrx i j = try !(List.assoc j mtrx.rows.(i)) with
            | Not_found -> mtrx.default

        let set mtrx i j elt = try (List.assoc j mtrx.rows.(i)) := elt with
            | Not_found ->
                mtrx.rows.(i) <- (j, ref elt) :: mtrx.rows.(i);
                mtrx.columns.(j) <- (i, ref elt) :: mtrx.columns.(j)
        
        let row_iter i f mtrx =
            for j = 0 to Array.length mtrx.rows - 1 do
                f (get mtrx i j)
            done

        let row_iterj i f mtrx =
            for j = 0 to Array.length mtrx.rows - 1 do
                f j (get mtrx i j)
            done

        let row_storage_iterj i f mtrx =
            List.iter (fun (j, elt_ref) -> f j !elt_ref) mtrx.rows.(i)

        let update_row_storage i f mtrx =
            List.iter (fun (j, elt_ref) -> elt_ref := f j !elt_ref) mtrx.rows.(i)

        let column_iter j f mtrx =
            for i = 0 to Array.length mtrx.rows - 1 do
                f (get mtrx i j)
            done

        let column_iteri j f mtrx =
            for i = 0 to Array.length mtrx.rows - 1 do
                f i (get mtrx i j)
            done

        let column_storage_iteri j f mtrx =
            List.iter (fun (i, elt_ref) -> f i !elt_ref) mtrx.columns.(j)

        let update_column_storage j f mtrx =
            List.iter (fun (i, elt_ref) -> elt_ref := f i !elt_ref) mtrx.columns.(j)

        let iter f mtrx =
            for i = 0 to Array.length mtrx.rows - 1 do row_iter i f mtrx done

        let iterij f mtrx =
            for i = 0 to Array.length mtrx.rows - 1 do
                row_iterj i (fun j -> f i j) mtrx
            done

    end

(*
module IntElement =
    struct
        type t = int
        let equal = ( = )
    end

module IntSparseMatrix = SquareSparseMatrix.Make(IntElement)

open IntSparseMatrix;;

let m = make 10 0;;

row_iter 0 (fun elt -> print_int elt; print_newline ()) m;;

row_iterj 0 (fun j elt -> print_int j; print_string " -> ";print_int elt; print_newline ()) m;;

row_iterj 1 (fun j elt -> set m 1 j j) m;;

swap_rows m 0 1;;

row_iterj 0 (fun j elt -> print_int j; print_string " -> ";print_int elt; print_newline ()) m;;

iterij (fun i j elt -> print_int i; print_string ", "; print_int j; print_string " -> ";print_int elt; print_newline ()) m;;

let m = init 10 0 (fun i j -> i * 10 + j);;

iterij (fun i j elt -> print_int i; print_string ", "; print_int j; print_string " -> ";print_int elt; print_newline ()) m;;

*)
