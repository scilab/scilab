function f=%p_d_r(n1,f2)
// n1./f2
//!
// Copyright INRIA
f=rlist(n1.*f2('den'),ones(n1).*f2('num'),f2('dt'))
endfunction
