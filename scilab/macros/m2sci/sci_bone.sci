function [stk,txt,top]=sci_bone()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('(7*graycolormap() + hotcolormap()*[0 0 1;0 1 0;1 0 0])/8','0','32','3','1')
else
  if isname(stk(top)(1)) then
    n=stk(top)(1)
  else
    n=gettempvar(1)
    txt=n+'='+stk(top)(1)
  end
  stk=list('(7*graycolormap('+n+') + hotcolormap('+n+')*[0 0 1;0 1 0;1 0 0])/8','0',n,'3','1')
end
endfunction
