function [stk,txt,top]=sci_flag()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('ones(8,1).*.[1 0 0;1 1 1;0 0 1;0 0 0]','0','32','3','1')
else
  if isname(stk(top)(1)) then
    n=stk(top)(1)
  else
    n=gettempvar(1)
    txt=n+'='+stk(top)(1)
  end
  map=gettempvar(2)
  txt=map+'=ones(ceil('+n+'/4),1).*.[1 0 0;1 1 1;0 0 1;0 0 0]'
  stk=list(map+'(1:'+n+',:)','0',n,'3','1')
end
endfunction
