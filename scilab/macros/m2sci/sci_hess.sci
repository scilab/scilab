function [stk,txt,top]=sci_hess()
// Copyright INRIA
txt=[]
if rhs==1 then
  r=list('hess('+stk(top)(1)+')','0','1','?','?')
else
  r=list('hess('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','?','?')
end
if lhs==1 then
  stk=r
else
  r(2)='-1'
  stk=list(r,r)
end
