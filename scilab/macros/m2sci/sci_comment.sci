function [stk,txt,top]=sci_comment()
// Copyright INRIA
txt=[]
if top>=1 then 
  t=stk(top)(1)
  quote=''''
  t=strsubst(t,quote+quote,quote)
  txt='//'+part(t,2:length(t)-1)
else
// txt='//'+stk(1) 
end
stk=list(' ','-1','?','?','?')
endfunction
