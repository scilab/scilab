function f=%p_l_p(p2,p1)
// f=%p_l_p(p2,p1) <=> f=p2\p1 for polynomial matrices
//!
// Copyright INRIA
[l,c]=size(p2)
if l*c <>1 then f=invr(p2)*p1,return,end
[l,c]=size(p1)
[p1,p2]=simp(p1,p2*ones(l,c))
f=tlist(['r','num','den','dt'],p1,p2,[])
endfunction
