function [stk,txt,top]=sci_norm()
// Copyright INRIA
txt=[]
if rhs==2 then
  if stk(top)(1)=='%inf' then
    stk=list('norm('+stk(top-1)(1)+',''inf'')','0','1','1','1','?')
  else
    stk=list('norm('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','1','1','?')
  end
else
  stk=list('norm('+stk(top)(1)+')','0','1','1','1','?')
end
