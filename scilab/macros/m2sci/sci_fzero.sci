function [stk,txt,top]=sci_fzero()
// Copyright INRIA
txt=[]
  txt='deff(''y=%ev(x)'',''y='+stk(top-rhs+1)(1)+'(x)'')'
if rhs==2 then
  stk=list('fsolve('+stk(top)(1)+',%ev)','0',stk(top)(3),stk(top)(4),'1')
else 
  stk=list('fsolve('+stk(top-rhs+2)(1)+',%ev,'+stk(top-rhs+3)(1)+')','0',stk(top)(3),stk(top)(4),'1')
end
