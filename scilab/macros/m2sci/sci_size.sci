function [stk,txt,top]=sci_size()
// Copyright INRIA
txt=[]
if rhs==2 then
  stk=list('size('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','1','1','?')
  top=top-1
else
  if lhs==1 then
    stk=list('size('+stk(top)(1)+')','0','1','2','1','?')
  else
    stk=list(list('size('+stk(top)(1)+')','-1','1','1','?'),..
        list('size('+stk(top)(1)+')','-1','1','1','?'))
  end
end
endfunction
