function [stk,txt,top]=sci_pinv()
// Copyright INRIA
txt=[]
if rhs==1 then
  stk=list('pinv('+stk(top)(1)+')','0','1','1','1')
else
  stk=list('pinv('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','1','1') 
end
endfunction
