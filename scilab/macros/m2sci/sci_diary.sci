function [stk,txt,top]=sci_diary()
// Copyright INRIA
txt=[]
if rhs<=0 then
  stk=list('error(''diary toggle in not implemented'')','0','0','0','0')
else
  if conv(stk(top)(1),'l')=='off' then
    stk=list('diary(0)','0','0','0','0')
  elseif conv(stk(top)(1),'l')=='on' then
    stk=list('error(''diary on in not implemented'')','0','0','0','0')
  else
    stk=list('diary('+stk(top)(1)+')','0','0','0','0')
  end
end
endfunction
