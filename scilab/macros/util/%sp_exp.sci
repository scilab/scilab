function sp=%sp_exp(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
sp=sparse(ij,exp(v),mn)

