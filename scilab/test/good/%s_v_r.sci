function h=%s_v_r(h1,h2)
// %s_v_r(h1,h2) = (I+h1*h2)\h1. h1 constant h2 rational
//!
// Copyright INRIA
[m1,n1]=size(h1)
[m2,n2]=size(h2('num'))
if abs(n1-m2)+abs(m1-n2)<>0 then error('inconsistent dimensions'),end
if m1*n1==1 then
  h=h2;h('num')=h1*h2('den');h('den')=h1*h2('num')+h2('den');
else
  h=(eye(m1,m1)+h1*h2)\h1
end
endfunction
