function [stk,txt,top]=%imp2sci()
//code for 1:n
//!
// Copyright INRIA
txt=[]
if op(3)=='2' then
  stk=list(stk(top-1)(1)+':'+stk(top)(1),'3','1','?','?')
  top=top-1
else
  stk=list(stk(top-2)(1)+':'+stk(top-1)(1)+':'+stk(top)(1),'3','1','?','?')
  top=top-2
end

