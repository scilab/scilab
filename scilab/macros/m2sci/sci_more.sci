function [stk,txt,top]=sci_more()
// Copyright INRIA
txt=[]
if stk(top)(1)=='off' then
  stk=list('lines(0)','0','0','0','0')
elseif stk(top)(1)=='on' then
  stk=list('lines(60)','0','0','0','0')
elseif stk(top)(5)=='1' then
  stk=list('lines('+stk(top)(1)+')','0','0','0','0')
else
  stk=list('mtlb_more('+stk(top)(1)+')','0','0','0','0')
end
 



