function [stk,txt,top]=sci_fprintf()

// Copyright INRIA
txt=[] 
RHS=[]
nam='mtlb_fprintf'
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
stk=list(nam+rhsargs(RHS),'0','1','1','1')
