function [stk,txt,top]=sci_linspace()
// Copyright INRIA
txt=[]
if rhs==2 then
  stk=list('linspace('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','100','1')
  top=top-1
else
  stk=list('linspace'+rhsargs([stk(top-2)(1),stk(top-1)(1),stk(top)(1)]),'0','1',stk(top)(1),'1')
  top=top-2
end





