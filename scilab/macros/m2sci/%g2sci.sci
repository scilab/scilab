function [stk,txt,top]=%g2sci()
//
//!
// Copyright INRIA
txt=[]
s2=stk(top);s1=stk(top-1);
[e1,te1]=s1(1:2);
[e2,te2]=s2(1:2);
if s1(3)=='1'&s1(4)=='1' then
  stk=list(e1+'|'+e2,'2',s2(3),s2(4),s1(5))
elseif s2(3)=='1'&s2(4)=='1' then
  stk=list(e1+'|'+e2,'2',s1(3),s1(4),s1(5))
else
  stk=list(e1+'|'+e2,'2',s1(3),s1(4),s1(5))
end
top=top-1
endfunction
