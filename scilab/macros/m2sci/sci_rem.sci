function [stk,txt,top]=sci_rem()
// Copyright INRIA
txt=[]
x= stk(top-1)(1)
y= stk(top)(1)
if ~(isname(x)|isnum(x)) then
  x=gettempvar(1)
  txt=[txt;x+' = '+stk(top-1)(1)]
end  
if ~(isname(y)|isnum(y)) then
  y=gettempvar(1)
  txt=[txt;y+' = '+stk(top)(1)]
end  
e=x+'-fix('+x+'./'+y+').*'+y
if stk(top)(3)=='1'&stk(top)(4)=='1' then
  stk=list(e,'2',stk(top-1)(3),stk(top-1)(4),'1')
elseif stk(top-1)(3)=='1'&stk(top-1)(4)=='1' then
  stk=list(e,'2',stk(top)(3),stk(top)(4),'1')
elseif stk(top-1)(3)=='1'|stk(top-1)(4)=='1'
  stk=list(e,'2',stk(top-1)(3),stk(top-1)(4),'1')
elseif stk(top)(3)=='1'&stk(top)(4)=='1'
  stk=list(e,'2',stk(top)(3),stk(top)(4),'1')
else
  stk=list(e,'2',stk(top)(3),stk(top)(4),'1')
end
top=top-1
endfunction
