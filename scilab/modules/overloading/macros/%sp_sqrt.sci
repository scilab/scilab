function sp=%sp_sqrt(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
sp=sparse(ij,sqrt(v),mn)
endfunction
