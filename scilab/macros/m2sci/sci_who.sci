function [stk,txt,top]=sci_who()
// Copyright INRIA
txt=[]
vars=[]
for k=1:rhs
  vars=[stk(top)(1),vars]
  top=top-1
end
vars(vars=='''''')=[]
if vars==[] then
   stk=list('who','0','0','0','0')
else
  select vars(1)
  case '''global''' then
    stk=list('who(''global'')','0','0','0','0')
  case '''-file''' then
    set_infos('who('+makeargs(vars)+') untranslated',2)
    stk=list(' ','-2','0','0','1')
  else
    set_infos('who('+makeargs(vars)+') untranslated',2)
    stk=list(' ','-2','0','0','1')
  end
  
end




