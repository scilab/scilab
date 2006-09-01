function sp=%sp_sin(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
sp=sparse(ij,sin(v),mn)
endfunction
