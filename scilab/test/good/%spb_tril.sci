function d=%spb_tril(a,k)

// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

[ij,v,sz]=spget(a)
m=sz(1);n=sz(2)
l=find(ij(:,1)>=(ij(:,2)-k))
d=sparse(ij(l,:),v(l),[m,n])
endfunction
