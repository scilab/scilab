function [stk,txt,top]=sci_planerot()
// Copyright INRIA
txt=[]
m=stk(top)(3);n=stk(top)(4)
if lhs==1 then
  stk=list('givens('+stk(top)(1)+')','0',m,m,stk(top)(5))
else
  stk=list(..
      list('givens('+stk(top)(1)+')','-1',m,'1',stk(top)(5)),..
      list('givens('+stk(top)(1)+')','-1',m,m,stk(top)(5)))
end
