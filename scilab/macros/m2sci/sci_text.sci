function [stk,txt,top]=sci_text()
// Copyright INRIA
txt=[]
if rhs==3 then
  str=stk(top)(1)
  x=stk(top-2)(1)
  y=stk(top-1)(1)
  stk=list('xstring'+rhsargs([x,y,str]),'0','0','0','0')
elseif rhs==0 then
  stk=list(' ','-2','0','0','0')
else
  str=stk(top)(1)
  z1=stk(top-1)(1)
  y1=stk(top-2)(1)
  x1=stk(top-3)(1)
  x=gettempvar(1)
  y=gettempvar(2)
  txt=lhsargs([x,y])+' = geom3d'+rhsargs([x1,y1,z1])
  stk=list('xstring'+rhsargs([x,y,str]),'0','0','0','0')
end



