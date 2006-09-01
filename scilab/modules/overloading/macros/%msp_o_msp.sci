function r=%msp_o_msp(l1,l2)
[ij,v,mn]=spget(l1);l1=sparse(ij,v,mn)
[ij,v,mn]=spget(l2);l2=sparse(ij,v,mn)
r=l1==l2
endfunction
