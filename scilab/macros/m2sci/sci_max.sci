function [stk,txt,top]=sci_max()
// Copyright INRIA
txt=[]
v=stk(top+1-rhs)(1)
if stk(top+1-rhs)(5)=='4' then v='bool2s('+v+')';end
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	' mtlb_max('+x+') may be replaced by '
	'    max('+x+')'+' if '+x+'is a vector'
	'    max('+x+',''r'')'+' if '+x+'is a matrix'],1)
    stk=list('mtlb_max('+x+')','0','?','?','1')
  elseif m==1|n==1 then
    stk=list('max('+x+')','0','1','1','1')
  else
    stk=list('max('+x+',''r'')','0','1',stk(top)(4),'1')
  end
elseif rhs==2 then
  stk=list('max('+v+','+stk(top)(1)+')','0','?','?','1')
else
  if stk(top)(1)=='1' then
    stk=list('max('+v+',''r'')','0','1',stk(top+1-rhs)(4),'1')
  else
    stk=list('max('+v+',''c'')','0',stk(top+1-rhs)(3),'1','1')
  end
end
if lhs<>1 then
  stk(2)='-1'
  stk=list(stk,stk)
end
endfunction
