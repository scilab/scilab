function [r] = %msp_spones(s)
// Replace nonzero elements with ones in matlab sparse matrix
// Copyright INRIA
// r=spones(s) generates a matrix with the sane sparsity structure as s,
// but with ones in the nonzero positions;
//
[ij,v,mn]=spget(s);
r=sparse(ij,ones(v),mn);
endfunction
