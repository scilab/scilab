function f=%r_x_r(s1,s2)
// %r_x_r(r1,r2)=r1.*r2    r1,r2 rationals
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
[num,den]=simp(s1('num').*s2('num'),s1('den').*s2('den'))
f=rlist(num,den,s1('dt'))
endfunction
