function f=%s_q_r(s,f)
// %s_q_r(s,f)  f= s./f
//!
// Copyright INRIA
if size(s,'*')==0 then f=[],return,end
f=rlist(f('num')./s,f('den').*ones(s),f('dt')),
endfunction
