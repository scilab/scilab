(** This module provides a structure that allow object sharing in order to
create DAGs. Weak pointers are used to avoid keeping into the collection
some objects that are no longer used elsewhere. *)

(** Functorial interface *)

module type HashableType =
  sig
    type t
    val hash: t -> int
  end
    (** A hashable type is a type provided with one function whose purpose is to
       get an integer value that represents the hash code of objects, i.e. an
       integer associated. *)

module type S =
  sig
    type elt
    (** The type of the elements in the weak collection. *)
    type 'a t
    (** The type of the weak collection. *)
    val create:
      int -> ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
    (** The parameters of create are:
           - the initial size of the collection;
           - an equality function that does does create a new object but rather
             uses some atoms;
           - a hash function that computes the value from the atoms (without
             creating a new object);
           - a constructor that creates a new object, given the necessary atoms
             plus a hash value. *)
    val find_or_add: 'a -> 'a t -> elt
    (** find_or_add either returns an existing object whose atoms are the same
         as the ones given as the first argument, either builds a new object and
         insert it into the collection before returning it. *)
    val iter: (elt -> unit) -> 'a t -> unit
    (** iter applies its first argument to every argument of the collection. *)
  end

module Make:
  functor (H: HashableType) ->
    sig
      type elt = H.t
      and 'a t
      val create:
        int -> ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
      val find_or_add: 'a -> 'a t -> elt
      val iter: (elt -> unit) -> 'a t -> unit
    end

val hash_param: int -> int -> 'a -> int
(** See {!Hashtbl.hash_param}. *)
val hash: 'a -> int
(** See {!Hashtbl.hash}. *)
