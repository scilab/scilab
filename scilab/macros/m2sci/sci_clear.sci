function [stk,txt,top]=sci_clear()
// Copyright INRIA
txt=[]
vars=[]
for k=1:rhs
  vars=[stk(top)(1),vars]
  top=top-1
end
vars(vars=='''''')=[]
if vars==[] then
   stk=list('clear','0','0','0','1')
else
  select vars(1)
  case '''all''' then
    stk=list('clear','0','0','0','1')
  case '''variables''' then  
    stk=list('clear','0','0','0','1')
  case '''global''' then
    stk=list('clearglobal('+makeargs(vars(2:$))+')','0','0','0','1')
  case '''functions''' then
    set_infos('clear('+makeargs(vars)+') untranslated',2)
    stk=list(' ','-2','0','0','1')
  case '''fun''' then
    set_infos('clear('+makeargs(vars)+') untranslated',2)
    stk=list(' ','-2','0','0','1')
  case '''mex''' then
    set_infos('clear('+makeargs(vars)+') untranslated',2)
    stk=list(' ','-2','0','0','1')
  else
    stk=list('clear('+makeargs(vars)+')','0','0','0','1')
  end
end
endfunction
