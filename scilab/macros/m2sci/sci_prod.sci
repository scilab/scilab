function [stk,txt,top]=sci_prod()
// Copyright INRIA
txt=[]
if stk(top-rhs+1)(5)=='4' then 
  v='bool2s('+stk(top-rhs+1)(1)+')',
else 
  v=stk(top-rhs+1)(1),
end 
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	'mtlb_prod('+x+') may be replaced by '
	'  prod('+x+')'+' if '+x+'is a vector'
	'  prod('+x+',1)'+' if '+x+'is a matrix'],1)
    stk=list('mtlb_prod('+x+')','0','?','?','1')
  elseif m==1|n==1 then
    stk=list('prod('+x+')','0','1','1','1')
  else
    stk=list('prod('+x+',1)','0','1',stk(top)(4),'1')
  end  
else
  if stk(top)(1)=='1' then
    stk=list('prod('+v+',1)','0','1',stk(top-1)(4),stk(top-1)(5))
  elseif stk(top)(1)=='2' then  
    stk=list('prod('+v+',2)','0',stk(top-1)(3),'1',stk(top-1)(5))
  else  
    x=stk(top)(1)
    stk=list('prod('+v+','+x+')','0','?','?',stk(top-1)(5))
  end
  top=top-1
end
