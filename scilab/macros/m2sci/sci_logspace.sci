function [stk,txt,top]=sci_logspace()
// Copyright INRIA
txt=[]
if rhs==2 then
  stk=list('logspace('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','100','1')
  top=top-1
else
  stk=list('logspace'+rhsargs([stk(top-2)(1),stk(top-1)(1),stk(top)(1)]),'0','1',stk(top)(1),'1')
  top=top-2
end





