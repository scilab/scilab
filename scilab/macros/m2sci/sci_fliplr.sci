function [stk,txt,top]=sci_fliplr()
// Copyright INRIA
txt=[]
if isname(stk(top)(1)) then
  stk=list(stk(top)(1)+'(:,$:-1:1)','0',stk(top)(3),stk(top)(4),'1')
else
  v=gettempvar()
  txt=v+' = '+stk(top)(1)
  stk=list(v+'(:,$:-1:1)','0',stk(top)(3),stk(top)(4),'1')
end
endfunction
