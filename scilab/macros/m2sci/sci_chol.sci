function [stk,txt,top]=sci_chol()
// Copyright INRIA
txt=[]
if rhs==1 then
  stk=list('chol('+stk(top)(1)+')','0','1','?','?')
else
  set_infos('Warning: No Scilab equivalent to [R,p]=chol(..) using mtlb_chol',0)
  stk=list('mtlb_chol('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','?','?')
end
