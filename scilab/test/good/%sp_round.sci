function sp=%sp_round(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
sp=sparse(ij,round(v),mn)
endfunction
