function [stk,txt,top]=sci_figure()
// Copyright INRIA
if rhs==1 then
  win=stk(top)(1)
  txt='xset(''window'','+win+')'
  if and(lst(ilst+1)==['1' 'ans']) then
    stk=list(' ','-1','0','0','0')
  else
    stk=stk(top)
  end
else //figure without argument
  win='max(winsid()+1)'
  txt='xset(''window'','+win+')'
  if and(lst(ilst+1)==['1' 'ans']) then
    stk=list(' ','-1','0','0','0')
  else
    top=top+1
    stk=list('max(winsid())','0','1','1','1')
  end
end





