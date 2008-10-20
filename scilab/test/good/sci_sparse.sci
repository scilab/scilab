function x=sci_sparse(x)
//mtlb_sparse --> scilab sparse
[ij,v,mn]=spget(x);
x=sparse(ij,v,mn);
endfunction
