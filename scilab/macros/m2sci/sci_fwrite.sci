function [stk,txt,top]=sci_fwrite()
// Copyright INRIA
txt=[]
RHS=[]
for k=1:rhs
  RHS=[RHS,stk(top-rhs+k)(1)]
end
stk=list('mtlb_fwrite'+rhsargs(RHS),'0','1','1','1')
