function d=%b_diag(a,k)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[m,n]=size(a)
if m>1&n>1 then
  if k<=0 then
    mn=mini(m+k,n)
    i0=-k+1
  else
    mn=min(m,n-k)
    i0=k*m+1
  end
  a=matrix(a,m*n,1)
  i=i0+((0:mn-1)*(m+1))
  d=a(i)
else
  nn = max(m,n)+abs(k)
  mn=max(m,n)
  i=(1:mn)+((1:mn)+(k-1))*nn
  d(i)=a
  d=matrix(d,nn,nn)
end
endfunction
