function [stk,txt,top]=sci_mat_read()
RHS=[]
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
stk=list('mat_read'+rhsargs(RHS),'0','?','?','?')
