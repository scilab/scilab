function [stk,txt,top]=sci_gray()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('graycolormap(32)','0','32','3','1')
else
  stk=list('graycolormap('+stk(top)(1)+')','0',stk(top)(1),'3','1')
end
endfunction
