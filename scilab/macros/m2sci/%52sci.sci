function [stk,txt,top]=%52sci()
// genere le code relatif a la negation
//!
// Copyright INRIA
txt=[]
s2=stk(top)
if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
stk=list('~'+s2(1),'3',s2(3),s2(4),'4')
endfunction
