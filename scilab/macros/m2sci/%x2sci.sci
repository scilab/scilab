function [stk,txt,top]=%x2sci()
// Copyright INRIA
txt=[]
s1=stk(top-1)
s2=stk(top)

if s2(5)=='4' then s2(1)='bool2s('+s2(1)+')',s2(5)='1';s2(2)='0';end
if s1(5)=='4' then s1(1)='bool2s('+s1(1)+')',s1(5)='1';s1(2)='0';end

[e1,te1]=s1(1:2);
[e2,te2]=s2(1:2);
//
if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
if te1=='2'|te1=='3' then e1='('+e1+')',end

if s1(3)=='1'&s1(4)=='1' then
  stk=list(e1+' .* '+e2,'1',s2(3),s2(4),s1(5))
elseif s2(3)=='1'&s2(4)=='1' then
  stk=list(e1+' .* '+e2,'1',s1(3),s1(4),s1(5))
else
  stk=list(e1+' .* '+e2,'1',s1(3),s1(4),s1(5))
end
top=top-1
