function [stk,txt,top]=sci_qz()
// Copyright INRIA
txt=[]
m=stk(top)(3)
if m=='?' then m=stk(top-1)(3),end
if lhs==1 then
  stk=list('gschur('+stk(top-1)(1)+','+stk(top)(1)+')','0',m,m,'1')
else
  s=list('gschur('+stk(top-1)(1)+','+stk(top)(1)+')','-1',m,m,'1')
  if lhs==5 then
    [AA, BB, Q, Z, V]=lhsvarsnames()
    txt=[txt;lhsargs([AA, BB, Q, Z, V])+' = mtlb_qz'+..
	    rhsargs([stk(top-1)(1),stk(top)(1)])]
    s=list(' ','-2','m','m','0')
  end
  stk=list()
  for k=1:lhs
    stk(k)=s
  end
end


