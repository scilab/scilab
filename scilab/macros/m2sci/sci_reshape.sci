function [stk,txt,top]=sci_reshape()
// Copyright INRIA
txt=[]
RHS=[]
for k=1:rhs
  RHS=[RHS stk(top-rhs+k)(1)]
end
RHS=rhsargs(RHS)
if rhs==2 then
  stk=list('matrix'+RHS,'0',stk(top-1)(1),stk(top)(1),'1')
else
  stk=list('matrix'+RHS,'0','?','?','?')
end
endfunction
