function h=%r_v_p(h1,h2)
// %r_v_p(h1,h2) computes (I+h1*h2)\h1. h1: rational
// h2 polynomial
//!
// Copyright INRIA
[m1,n1]=size(h1('num'))
[m2,n2]=size(h2)
if abs(n1-m2)+abs(m1-n2)<>0 then error('inconsistent dimensions'),end
h=(eye(m1,m1)+h1*h2)\h1
endfunction
