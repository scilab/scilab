function [stk,txt,top]=sci_angle()
// Copyright INRIA
txt=[]
if isname(stk(top)(1)) then
  n=stk(top)(1)
else
  n=gettempvar(1)
  txt=n+'='+stk(top)(1)
end
stk=list('atan(imag('+n+'),real('+n+'))','0',stk(top)(3),stk(top)(4),'1')
