function [stk,txt,top]=sci_get(nam)
txt=[] 
set_infos('Scilab get function is a partial emulation of the Matlab one',2)
RHS=[]
nam='get'
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
stk=list(nam+rhsargs(RHS),'0','1','1','1')
