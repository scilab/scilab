function h=%p_v_r(h1,h2)
// h=%p_v_r(h1,h2) <=> (I+h1*h2)\h1. 
// h1 polynomial matrix
// h2 rational matrix
//!
// Copyright INRIA
[m1,n1]=size(h1)
[m2,n2]=size(h2('num'))
if abs(n1-m2)+abs(m1-n2)<>0 then error('inconsistent dimensions'),end
h=(eye(m1,m1)+h1*h2)\h1
endfunction
