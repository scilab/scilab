function d=%b_tril(a,k)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

[m,n]=size(a)
i=find(tril(ones(a),k))
a=matrix(a,m*n,1)
d(m*n,1)=%f
d(i)=a(i)
d=matrix(d,m,n)
endfunction
