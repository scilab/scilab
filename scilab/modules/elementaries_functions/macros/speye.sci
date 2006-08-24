function [sp]=speye(m,n)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then [m,n]=size(m),end
mn=mini(m,n)
sp=sparse([(1:mn)' (1:mn)'],ones(mn,1),[m,n])
endfunction
