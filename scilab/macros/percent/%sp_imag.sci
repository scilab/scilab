function x=%sp_imag(a)
// only to be called by function imag
//!
// Copyright INRIA
[ij,v,mn]=spget(a)
x=sparse(ij,imag(v),mn)
endfunction
