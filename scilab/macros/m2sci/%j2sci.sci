function [stk,txt,top]=%j2sci()
// 
//!
// Copyright INRIA

s2=stk(top);s1=stk(top-1);
if s2(5)=='4' then s2(1)='bool2s('+s2(1)+')',s2(5)='1';end
if s1(5)=='4' then s1(1)='bool2s('+s1(1)+')',s1(5)='1';end

txt=[]
[ss1,te1]=s1(1:2);
[ss2,te2]=s2(1:2);
//
if te2=='1'|te2=='2'|te2=='3' then ss2='('+ss2+')',end
if te1=='1'|te1=='2'|te1=='3' then ss1='('+ss1+')',end

if part(ss2,1)=='-' then ss2='('+ss2+')',end
[m2,n2]=checkdims(s2)
[m1,n1]=checkdims(s1)
if m2==1&n2==1 then
  stk=list(ss1+'.^'+ss2,'1',s1(3),s1(4),s1(5))
elseif m1==1&n1==1 then
  stk=list(ss1+'.^'+ss2,'1',s2(3),s2(4),s2(5))
elseif m1==1&m2==1 then 
  stk=list(ss1+'.^'+ss2,'1',s2(3),'?',s2(5))
elseif n1==1&n2==1 then 
  stk=list(ss1+'.^'+ss2,'1','?',s2(4),s2(5))    
else
  stk=list(ss1+'.^'+ss2,'1','?','?','?')
end
top=top-1

