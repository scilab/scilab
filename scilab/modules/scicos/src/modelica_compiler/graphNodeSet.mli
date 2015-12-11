val hash_param : int -> int -> 'a -> int
val hash : 'a -> int
module type HashableType = sig type t val hash : t -> int end
module type S =
  sig
    type elt
    type 'a t
    val create :
      int -> ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
    val find_or_add : 'a -> 'a t -> elt
    val iter : (elt -> unit) -> 'a t -> unit
  end
module Make :
  functor (H : HashableType) ->
    sig
      type elt = H.t
      type 'a t
      val create :
        int ->
        ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
      val find_or_add : 'a -> 'a t -> elt
      val iter : (elt -> unit) -> 'a t -> unit
    end
