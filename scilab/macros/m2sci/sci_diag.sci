function [stk,txt,top]=sci_diag()
// Copyright INRIA
txt=[]
if rhs==2 then
  stk=list('diag('+stk(top-1)(1)+','+stk(top)(1)'+')','0','?','?','1')
  top=top-1
else
  if stk(top)(3)=='1' then
    stk=list('diag('+stk(top)(1)+')','0',stk(top)(4),stk(top)(4),'1')
  elseif stk(top)(4)=='1' then
    stk=list('diag('+stk(top)(1)+')','0',stk(top)(3),stk(top)(3),'1')
  else
    stk=list('diag('+stk(top)(1)+')','0','?','?','1')
  end
  
end






