function [stk,txt,top]=sci_spec()
RHS=[]
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
stk=list('%spec'+rhsargs(RHS),'0','?','?','?')
endfunction
