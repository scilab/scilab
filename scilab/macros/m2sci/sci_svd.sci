function [stk,txt,top]=sci_svd()
// Copyright INRIA
txt=[]

if rhs==1 then
  r=list('svd('+stk(top)(1)+')','0',stk(top)(3),stk(top)(4),'1')
  select lhs
  case 1 then
    stk=r;
  case 3 then
    r(2)='-1'
    stk=list(r,r,r)
  end
else
  r=list('svd('+stk(top-1)(1)+','+stk(top)(1)+')','0',stk(top-1)(3),stk(top-1)(4),'1')
  top=top-1
  select lhs
  case 1 then
    stk=r
  case 3 then
    r(2)='-1'
    stk=list(r,r,r)
  end
end

