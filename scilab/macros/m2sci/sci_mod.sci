function [stk,txt,top]=sci_mod()
// Copyright INRIA
txt=[]
// mod(x,y)
x=stk(top-1)(1);mx=stk(top-1)(3);nx=stk(top-1)(4)
y=stk(top)(1);my=stk(top)(3);ny=stk(top)(4)
if mx=='1'&nx=='1' then
  stk=list('pmodulo'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0',my,ny,'1')
elseif my=='1'&ny=='1' then
  stk=list('pmodulo'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0',mx,nx,'1')
elseif my==mx&ny==nx then
  stk=list('pmodulo'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0',mx,nx,'1')
else
  stk=list('pmodulo'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0','?','?','1')
end
