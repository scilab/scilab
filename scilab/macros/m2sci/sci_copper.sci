function [stk,txt,top]=sci_copper()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('min(ones(32,3),graycolormap()*diag([1.2500 0.7812 0.4975]))','0','32','3','1')
else
  if isname(stk(top)(1)) then
    n=stk(top)(1)
  else
    n=gettempvar(1)
    txt=n+'='+stk(top)(1)
  end
  stk=list('min(ones('+n+',3),graycolormap('+n+')*diag([1.2500 0.7812 0.4975]))','0',n,'3','1')
end
