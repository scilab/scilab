function [stk,txt,top]=sci_fieldnames()
// Copyright INRIA
txt=[]
if isname(stk(top)(1)) then
  s=stk(top)(1)
else
  s=gettempvar(1)
  txt=s+'='+stk(top)(1)
end
stk=list(s+'(1)(2:$)','0','?','1','10','?')
endfunction
