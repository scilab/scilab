function d=%r_diag(a,k)
// %r_diag - implement diag function for  rational matrix ,..
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[m,n]=size(a('num'))
if m<>1&n<>1 then
  d=syslin(a('dt'),diag(a('num'),k),diag(a('den'),k))
else
  mn=max(m,n)
  den=ones(mn,mn)
  den=den-diag(diag(den,k))+diag(a('den'),k)
  d=syslin(a('dt'),diag(a('num'),k),den)
end
