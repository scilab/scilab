function d=%r_tril(a,k)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
d=syslin(a('dt'),tril(a('num'),k),tril(a('den'),k)+triu(ones(a('den')),k+1))
endfunction
