function [stk,txt,top]=sci_rank()
// Copyright INRIA
txt=[]
if rhs==1 then
  stk=list('rank('+stk(top)(1)+')','0','1','1','1')
else
  stk=list('rank('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','1','1') 
end


