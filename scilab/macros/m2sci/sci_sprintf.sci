function [stk,txt,top]=sci_sprintf()

// Copyright INRIA
txt=[] 
RHS=[]
nam='mtlb_sprintf'
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
if lhs==1 then
  stk=list(nam+rhsargs(RHS),'0','?','1','10')
else
  stk=list()
  stk(1)=list(nam+rhsargs(RHS),'-1','?','1','10')
  if lhs>=2 then
    stk(2)=list(nam+rhsargs(RHS),'-1','1','1','10')
  end
end

