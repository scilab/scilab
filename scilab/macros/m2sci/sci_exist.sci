function [stk,txt,top]=sci_exist()
// Copyright INRIA
txt=[]

set_infos('Not enough information using mtlb_exist instead of exists',1)
stk=list('mtlb_exist('+stk(top)(1)+')','0','1','1','1')
endfunction
