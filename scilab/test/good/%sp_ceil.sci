function x=%sp_ceil(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
x=sparse(ij,ceil(v),mn)
endfunction
