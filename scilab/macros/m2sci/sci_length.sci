function [stk,txt,top]=sci_length()
// Copyright INRIA
txt=[]
if stk(top)(5)=='10' then
  stk=list('length('+stk(top)(1)+')','0','1','1','1')
elseif stk(top)(5)=='1' then
  stk=list('max(size('+stk(top)(1)+'))','0','1','1','1')
elseif stk(top)(5)=='4' then
  stk=list('max(size('+stk(top)(1)+'))','0','1','1','1')  
else
  set_infos('unknown arg type, using mtlb_length ',1)
  stk=list('mtlb_length('+stk(top)(1)+')','0','1','1','1')
end
endfunction
