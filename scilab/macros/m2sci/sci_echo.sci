function [stk,txt,top]=sci_echo()
// Copyright INRIA
RHS=[]
for k=1:rhs
  RHS=[RHS stk(top-rhs+k)(1)]
end
set_infos('//! echo '+strcat(RHS,' ')+' ignored',2)
txt=[txt;    '// echo '+strcat(RHS,' ')]
stk=list(' ','-2','0','0','1')


