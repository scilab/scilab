function [stk,txt,top]=sci_contour()
// Copyright INRIA
txt=[]
if stk(top)(5)=='10' then
  set_infos('contour '+stk(top)(1)+' option not yet handled',1)
  top=top-1
  rhs=rhs-1
end
if lhs==2 then
  set_infos('contour with lhs args not yet handled',2)
end
select rhs
case 1 then //contour(Z)
  x='1:size('+stk(top)(1)+',1)'
  y='1:size('+stk(top)(1)+',2)'
  z=stk(top)(1)
  stk=list('contour2d'+rhsargs([x,y,z]),'0','?','?','?')
case 2 then
  x='1:size('+stk(top-1)(1)+',1)'
  y='1:size('+stk(top-1)(1)+',2)'
  z=stk(top-1)(1)
  nz=stk(top)(1)
  stk=list('contour2d'+rhsargs([x,y,z,nz]),'0','?','?','?')
case 3 then  
  x=stk(top-2)(1)
  y=stk(top-1)(1)
  z=stk(top)(1)
  stk=list('contour2d'+rhsargs([x,y,z]),'0','?','?','?')
case 4 then
  x=stk(top-3)(1)
  y=stk(top-2)(1)
  z=stk(top-1)(1)
  stk=list('contour2d'+rhsargs([x,y,z]),'0','?','?','?')
  nz=stk(top)(1)
  stk=list('contour2d'+rhsargs([x,y,z,nz]),'0','?','?','?')
end
if lhs==2 then
  stk=list(stk,stk)
end
endfunction
