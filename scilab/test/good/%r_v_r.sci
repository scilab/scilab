function h=%r_v_r(h1,h2)
// %r_v_r(h1,h2)=(I+h1*h2)\h1. h1 and h2 rational matrices
//!
// Copyright INRIA
[h1,h2]=sysconv(h1,h2),
[m1,n1]=size(h1('num'))
[m2,n2]=size(h2('num'))
if abs(n1-m2)+abs(m1-n2)<>0 then error('inconsistent dimensions'),end
if m1*n1==1 then
  h=rlist(h1('num')*h2('den'),h1('num')*h2('num')+h1('den')*h2('den'),h1('dt'))
else
  h=(eye(m1,m1)+h1*h2)\h1
end
endfunction
