function [stk,txt,top]=sci_input()
// Copyright INRIA
txt=[]
if rhs==1 then
  stk=list('input('+stk(top)(1)+')','0','1','1','1')
else
  stk=list('input('+stk(top-1)(1)+',''s'')','0','1','1','10')
end
endfunction
