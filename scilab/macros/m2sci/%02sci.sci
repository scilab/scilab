function [stk,txt,top]=%02sci()
// translate .'
//!
// Copyright INRIA
txt=[]
s2=stk(top)
if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
stk=list(s2(1)+'.'+quote,'3',s2(4),s2(3),s2(5))


