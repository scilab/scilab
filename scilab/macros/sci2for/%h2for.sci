function [stk,nwrk,txt,top]=%h2for(nwrk)
//  &
// Copyright INRIA
s2=stk(top);s1=stk(top-1);
txt=[]
[e1,te1]=s1(1:2);
[e2,te2]=s2(1:2);
//
if te2=='2'|te2=='3' then e2='('+e2+')',end
if te1=='2'|te1=='3' then e1='('+e1+')',end

if s1(4)=='1'&s1(5)=='1'&s2(4)=='1'&s2(5)=='1' then
  stk=list(e1+'.and.'+e2,'1','0',s2(4),s1(5))
else
  txt='c Warning: boolean and on matrices not yet handled'
  stk=list(e1+'.and.'+e2,'1','0',s1(4),s1(5))
end
top=top-1
endfunction
