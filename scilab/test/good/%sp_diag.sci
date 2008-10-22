function d=%sp_diag(a,k)
// %sp_diag - implement diag function for sparse matrix, rational matrix ,..
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

  [ij,v,sz]=spget(a)
  m=sz(1);n=sz(2)
  if m>1&n>1 then
    l=find(ij(:,1)==(ij(:,2)-k))
    if k<=0 then
      mn=mini(m+k,n)
      i0=-k
    else
      mn=min(m,n-k)
      i0=0
    end
    kk=abs(k)
    if l==[] then d=sparse([],[],[mn,1]);return;end
    d=sparse([ij(l,1)-i0,ones(ij(l,1))],v(l),[mn,1])
  else
    if m>1 then ij=ij(:,1);else ij=ij(:,2);end
    nn = max(m,n)+abs(k)
    if ij==[] then 
      d=sparse([],[],[nn,nn])
    else
      if k>0 then
	d=sparse([ij,ij+k],v,[nn,nn])
      else
	d=sparse([ij-k,ij],v,[nn,nn])
      end
    end
  end
endfunction
