function [stk,txt,top]=sci_pink()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('sqrt((2*graycolormap() + hotcolormap())/3)','0','32','3','1')
else
  if isname(stk(top)(1)) then
    n=stk(top)(1)
  else
    n=gettempvar(1)
    txt=n+'='+stk(top)(1)
  end
  stk=list('sqrt((2*graycolormap('+n+') + hotcolormap('+n+'))/3)','0',n,'3','1')
end
