function [stk,txt,top]=%p2sci()
// ^
//!
// Copyright INRIA
txt=[]
s2=stk(top);s1=stk(top-1);
if s2(5)=='4' then s2(1)='bool2s('+s2(1)+')',s2(5)='1';end
if s1(5)=='4' then s1(1)='bool2s('+s1(1)+')',s1(5)='1';end

[s1,te1,t1,m1,n1]=s1(1:5);
[s2,te2,t2,m2,n2]=s2(1:5);
//
if te2=='1'|te2=='2'|te2=='3' then s2='('+s2+')',end
if te1=='2'|te1=='3' then s1='('+s1+')',end

if part(s2,1)=='-' then s2='('+s2+')',end
stk=list(s1+'^'+s2,'2',m1,n1,'1')
top=top-1


