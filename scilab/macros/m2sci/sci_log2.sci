function [stk,txt,top]=sci_log2()
// Copyright INRIA
txt=[]
if lhs==1 then
  stk=list('log('+stk(top)(1)+')/log(2)','0',stk(top)(3),stk(top)(4),'1')
else
  r=list('frexp('+stk(top)(1)+')','-1',stk(top)(3),stk(top)(4),'1')
  stk=list(r,r)
end




