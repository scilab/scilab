function [stk,txt,top]=sci_semilogy()
// Copyright INRIA
txt=[]
RHS=[]
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
stk=list('mtlb_semilogy'+'('+makeargs(RHS)+')','0','?','?','?')
endfunction
