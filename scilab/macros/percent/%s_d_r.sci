function f=%s_d_r(n1,f2)
// n1./f2
//!
// Copyright INRIA
if size(n1,'*')==0 then f=[],return,end
f=rlist(n1.*f2('den'),ones(n1).*f2('num'),f2('dt'))
endfunction
