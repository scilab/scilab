function f=%p_q_p(p1,p2)
//f= p1.\p2
//!
// Copyright INRIA
if size(p1,'*')==1 then
  p1=p1*ones(p2)
elseif size(p2,'*')==1 then
  p2=p2*ones(p1)
end
[p1,p2]=simp(p1,p2)
f=tlist(['r','num','den','dt'],p2,p1,[])
endfunction
