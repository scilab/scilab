function d=%r_triu(a,k)
// g_triu - implement triu function for sparse matrix, rationnal matrix ,..
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
d=syslin(a('dt'),triu(a('num'),k),triu(a('den'),k)+tril(ones(a('den')),k-1))
endfunction
