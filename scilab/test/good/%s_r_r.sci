function f=%s_r_r(m,f)
// %s_r_r(m,r) <=> m/f   constant/rational
//!
// Copyright INRIA
if prod(size(f('num')))<>1 then f=m*invr(f),return,end
f=simp(rlist(m*f('den'),ones(m)*f('num'),f('dt')))
endfunction
