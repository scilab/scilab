function x=%sp_real(a)
// Copyright INRIA
[ij,v,mn]=spget(a)
x=sparse(ij,real(v),mn)
