function [stk,txt,top]=sci_kron()
// Copyright INRIA
txt=[]
s1=stk(top-1)
s2=stk(top)
[e1,te1]=s1(1:2);
[e2,te2]=s2(1:2);
if s1(5)=='10' then e1='str2code('+e1+')''',te1='0',end
if s2(5)=='10' then e2='str2code('+e2+')''',te2='0',end
//
if te2=='2'|te2=='3' then e2='('+e2+')',end
if te1=='2'|te1=='3'  then e1='('+e1+')',end

if s1(3)=='1'&s1(4)=='1' then
  stk=list(e1+'*'+e2,'1',s2(3),s2(4),s1(5))
elseif s2(3)=='1'&s2(4)=='1' then
  stk=list(e1+'.*.'+e2,'1',s1(3),s1(4),s1(5))
else
  stk=list(e1+'.*.'+e2,'1',s1(3),s1(4),s1(5))
end
top=top-1




