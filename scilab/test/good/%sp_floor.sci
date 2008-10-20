function x=%sp_floor(a)
// only to be called by function floor
//!
// Copyright INRIA
[ij,v,mn]=spget(a)
x=sparse(ij,floor(v),mn)
endfunction
