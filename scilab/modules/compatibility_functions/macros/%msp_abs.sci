function w=%msp_abs(x)
[ij,v,mn]=spget(x);
w=mtlb_sparse(sparse(ij,abs(v),mn));
endfunction
