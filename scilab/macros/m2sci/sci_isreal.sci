function [stk,txt,top]=sci_isreal()
// Copyright INRIA
txt=[]
if stk(top)(5)=='4' then stk(top)(1)='bool2s('+stk(top)(1)+')',end
stk=list('isreal('+stk(top)(1)+',0)','0',stk(top)(3),stk(top)(4),'4')
endfunction
