function [stk,txt,top]=sci_roots()
// Copyright INRIA
temp=gettempvar()
txt=temp+' = '+stk(top)(1)
stk=list('roots(poly('+temp+'($:-1:1),''x''))','0','?','1','1')
