function f=%r_r_p(f1,p2)
// 
//!
// Copyright INRIA
if prod(size(p2)) <>1 then f=f1*invr(p2),return,end
[n1,p2]=simp(f1('num'),p2*f1('den'))
f=rlist(n1,p2,f1('dt'))
endfunction
