function [stk,txt,top]=sci_mini()
// Copyright INRIA
txt=[]
if rhs==1 then
  if stk(top)(3)=='1'|stk(top)(4)=='1' then
    r=list('min('+stk(top)(1)+')','0','1','1','?')
  else
    r=list('mtlb_min('+stk(top)(1)+')','0','1','?','?')
  end
else
  r=list('min('+stk(top-1)(1)+','+stk(top)(1)+')','0','1','?','?')
end

if lhs==1 then
  stk=r
else
  r(2)='-1'
  stk=list(r,r)
end
endfunction
