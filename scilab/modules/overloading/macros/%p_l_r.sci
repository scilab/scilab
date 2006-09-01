function f1=%p_l_r(p1,f1)
//  f1=p1\f1
//!
// Copyright INRIA
if size(p1,'*')<>1 then 
  f1=invr(p1)*f1
else
  [n1,d1]=simp(f1('num'),p1*f1('den')),
  f1=rlist(n1,d1,f1('dt'))
end
endfunction
