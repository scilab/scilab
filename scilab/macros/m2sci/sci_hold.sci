function [stk,txt,top]=sci_hold()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list('mtlb_hold()','0','0','0','0')
else
  stk=list('mtlb_hold('+stk(top-rhs+1)(1)+')','0','0','0','0')
end
