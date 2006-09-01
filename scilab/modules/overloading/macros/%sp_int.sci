function x=%sp_int(a)
// only to be called by function int
//!
// Copyright INRIA
[ij,v,mn]=spget(a)
x=sparse(ij,int(v),mn)
endfunction
