function [stk,txt,top]=sci_poly()
// Copyright INRIA
temp=gettempvar()
txt=temp+' = poly('+stk(top)(1)+',''x'')'
stk=list('coeff('+temp+',(degree('+temp+'):-1:0))','0','1','?','1')
endfunction
