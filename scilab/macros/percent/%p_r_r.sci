function f=%p_r_r(m,f)
// f=%p_r_r(p1,p2) <=> f= p1*(p2^(-1)) 
// p1 polynomial matrix
// p2: rational matrix
//!
// Copyright INRIA
if prod(size(f('num')))<>1 then f=m*invr(f),return,end
f=rlist(m*f('den'),ones(m)*f('num'),f('dt'))
endfunction
