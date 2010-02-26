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

(* Functorial interface *)

let hash_param = Hashtbl.hash_param

let hash x = hash_param 10 100 x

module type HashableType =
  sig
    type t
    val hash: t -> int
  end

module type S =
  sig
    type elt
    type 'a t
    val create:
      int -> ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
    val find_or_add: 'a -> 'a t -> elt
    val iter: (elt -> unit) -> 'a t -> unit
  end

module Make(H: HashableType): (S with type elt = H.t) =
  struct

    type elt = H.t

    type 'a t =
      {
        equal : 'a -> elt -> bool;        (* equality function      *)
        hash : 'a -> int;                 (* hash function          *)
        create : 'a -> int -> elt;        (* creation function      *)
        mutable max_len : int;            (* max length of a bucket *)
        mutable data : elt Weak.t array   (* the buckets            *)
      }

    let create initial_size equalfun hashfun createfun =
      let s = if initial_size < 1 then 1 else initial_size in
      let s = if s > Sys.max_array_length then Sys.max_array_length else s in
      {
        equal = equalfun;
        hash = hashfun;
        create = createfun;
        max_len = 3;
        data = Array.init s (function n -> Weak.create 3)
      }

    let rec insert_from buckt some_elt n =
      if n < 0 then failwith "Insertion error" else
      match Weak.get buckt n with
        | None -> Weak.set buckt n some_elt
        | _ -> insert_from buckt some_elt (n - 1)

    let resize s =
      let odata = s.data in
      let osize = Array.length odata in
      let nsize = min (2 * osize + 1) Sys.max_array_length in
      begin
        s.max_len <- 2 * s.max_len;
        let ndata = Array.init nsize (function n -> Weak.create s.max_len) in
        let insert_bucket buckt =
          for i = 0 to Weak.length buckt - 1 do
            match Weak.get buckt i with
                | None -> ()
                | Some elt as some_elt ->
                    insert_from
                      ndata.((H.hash elt land max_int) mod nsize)
                      some_elt
                      (s.max_len - 1)
          done
        in
          for i = 0 to osize - 1 do
              insert_bucket odata.(i)
          done;
          s.data <- ndata;
      end

    let rec bucket_too_long n bucket =
      if n < 0 then true else
      match Weak.get bucket n with
        | None -> false
        | _ -> bucket_too_long (n - 1) bucket

    let find_or_add elt_as_atoms s =
      let equalfun = s.equal
      and hash = s.hash elt_as_atoms land max_int
      and createfun = s.create in
      let rec add' bucket n option_pos =
        if n < 0 then match option_pos with
          | None ->
              resize s;
              add' s.data.(hash mod (Array.length s.data)) (s.max_len - 1) None
          | Some pos ->
              let elt = createfun elt_as_atoms hash in
                Weak.set bucket pos (Some elt); elt
        else match Weak.get bucket n with
          | None ->
              begin match option_pos with
                | None -> add' bucket (n - 1) (Some n)
                | _ -> add' bucket (n - 1) option_pos
              end
          | Some elt when equalfun elt_as_atoms elt -> elt
          | _ -> add' bucket (n - 1) option_pos
      in add' s.data.(hash mod (Array.length s.data)) (s.max_len - 1) None

    let iter f s =
      let iter_bucket bucket =
        for i = 0 to Weak.length bucket - 1 do
          match Weak.get bucket i with
            | None -> ()
            | Some elt -> f elt
        done
      in Array.iter iter_bucket s.data

  end
