function [stk,txt,top]=sci_cd()
// Copyright INRIA
txt=[]
if rhs<=0 then
  stk=list('getcwd()','0','0','0','0')
else
  stk=list('chdir('+stk(top)(1)+')','0','0','0','0')
end
