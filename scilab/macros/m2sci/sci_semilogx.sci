function [stk,txt,top]=sci_semilogx()
// Copyright INRIA
txt=[]
RHS=[]
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
stk=list('mtlb_semilogx'+'('+makeargs(RHS)+')','0','?','?','?')
