function d=%c_triu(a,k)
// g_triu - implement triu function for sparse matrix, rationnal matrix ,..
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

[m,n]=size(a)
if k<=0 then
  mn=mini(m,n-k)
else
  mn=min(m+k,n)
end
a=matrix(a,m*n,1)
i=(1:mn)+((1:mn)+(k-1))*m
d=emptystr(m*n,1)
d(i)=a(i)
d=matrix(d,m,n)
endfunction
