function w=%msp_abs(x)
[ij,v,mn]=spget(a);
w=mtlb_sparse(sparse(ij,abs(v),mn));
endfunction
