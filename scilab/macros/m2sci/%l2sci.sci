function [stk,txt,top]=%l2sci()
//  \
//!
// Copyright INRIA
txt=[]
s2=stk(top);s1=stk(top-1);top=top-1;
if s2(5)=='4' then s2(1)='bool2s('+s2(1)+')',s2(5)='1';end
if s1(5)=='4' then s1(1)='bool2s('+s1(1)+')',s1(5)='1';end

if s2(2)=='3'|s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
if  s1(2)=='3'|s1(2)=='2' then s1(1)='('+s1(1)+')',end

if part(s1(1),1)=='-' then s1(1)='('+s1(1)+')',end
if s1(3)=='1'&s1(4)=='1' then
  stk=list(s1(1)+'\'+s2(1),'1',s2(3),s2(4),s1(5))
elseif s2(3)=='1'&s2(4)=='1' then
  stk=list(s1(1)+'\'+s2(1),'1',s1(3),s1(4),s1(5))
else
  stk=list(s1(1)+'\'+s2(1),'1',s1(3),s1(4),s1(5))
end
endfunction
