function [stk,txt,top]=sci_toeplitz()
// Copyright INRIA
txt=[]
if rhs==1 then
  stk=list('toeplitz('+stk(top)(1)+')','0','?','?','1')
else
  stk=list('toeplitz'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0','?','?','1')
  top=top-1
end
endfunction
