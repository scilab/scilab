function f=%s_d_p(p1,p2)
// %s_d_p(M,p)=M.*p
//!
// Copyright INRIA
if size(p1,'*')==1 then
  p1=p1*ones(p2)
elseif size(p2,'*')==1 then
  p2=p2*ones(p1)
end
f=rlist(p1*poly(1,varn(p2),'c'),p2,[])
endfunction
