function [stk,txt,top]=sci_schur()
// Copyright INRIA
txt=[]
s=stk(top)
if lhs==1 then
  stk=list('schur('+s(1)+')','0',s(3),s(4),'1','?')
else
  stk=list(list('schur('+s(1)+')','-1',s(3),s(4),'1','?'),..
           list('schur('+s(1)+')','-1',s(3),s(4),'1','?'))
end
endfunction
