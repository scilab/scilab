function [stk,txt,top]=sci_help()
// Copyright INRIA
txt=[]
if rhs<=0 then
  stk=list('help','0','0','0','0')
else
  stk=list('help('+stk(top)(1)+')','0','0','0','0')
end

