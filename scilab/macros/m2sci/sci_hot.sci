function [stk,txt,top]=sci_hot()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('hotcolormap(32)','0','32','3','1')
else
  stk=list('hotcolormap('+stk(top)(1)+')','0',stk(top)(1),'3','1')
end
