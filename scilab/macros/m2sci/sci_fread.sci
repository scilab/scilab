function [stk,txt,top]=sci_fread()
// Copyright INRIA
txt=[]
RHS=[]
for k=1:rhs
  RHS=[RHS,stk(top-rhs+k)(1)]
end
if lhs==1 then
  stk=list('mtlb_fread'+rhsargs(RHS),'0','?','?','1')
else
  stk=list(list('mtlb_fread'+rhsargs(RHS),'-1','?','?','1'),..
           list('mtlb_fread'+rhsargs(RHS),'-1','1','1','1'))
end
