function [stk,txt,top]=sci_cool()
// Copyright INRIA
txt=[]
if rhs<1 then
  r=gettempvar(1)
  txt=r+'=(0:31)''/31'
  stk=list('['+r+',1-'+r+',ones('+r+')]','0','32','3','1')
else
  r=gettempvar(1)
  if isname(stk(top)(1)) then
    n=stk(top)(1)
    txt=[]
  else
    n=gettempvar(2)
    txt=n+'='+stk(top)(1)+';'
  end
  txt=[txt;
      r+'=(0:'+n+')''/'+n+';']
  stk=list('['+r+',1-'+r+',ones('+r+')]','0',n,'3','1')
end
