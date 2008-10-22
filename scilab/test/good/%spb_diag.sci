function d=%spb_diag(a,k)
// g_diag - implement diag function for sparse matrix, rational matrix ,..
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[ij,v,sz]=spget(a)
m=sz(1);n=sz(2)
if m>1&n>1 then
  l=find(ij(:,1)==(ij(:,2)-k))
  if k<=0 then
    mn=mini(m,n-k)
  else
    mn=min(m+k,n)
  end
  kk=abs(k)
  d=sparse([ij(l,1),ones(ij(l,1))],v(l),[mn,1])
else
  nn = max(m,n)+abs(k)
  if ij==[] then 
    d=sparse([],[],[nn,nn])
  else
    d=sparse([ij(:,1),ij(:,1)+k],v,[nn,nn])
  end
end
endfunction
