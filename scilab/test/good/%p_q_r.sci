function r=%p_q_r(p,r)
// r= p.\ r   polynomial./rational
//!
// Copyright INRIA
r=rlist(ones(p).*r('num'),p.*r('den'),r('dt'))
endfunction
