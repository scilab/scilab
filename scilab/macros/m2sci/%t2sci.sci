function [stk,txt,top]=%t2sci()
// genere le code relatif a la transposition
//!
// Copyright INRIA
txt=[]
s2=stk(top)
if s2(2)=='1'|s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
if part(s2(1),length(s2(1)))==quote then
  s2(1)=part(s2(1),1:length(s2(1))-1)
  stk=list(s2(1),s2(2),s2(4),s2(3),s2(5))
else
  stk=list(s2(1)+quote,s2(2),s2(4),s2(3),s2(5))
end

