function [stk,txt,top]=sci_any()
// Copyright INRIA
txt=[]
v=stk(top-rhs+1)(1),
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	'mtlb_any('+x+') may be replaced by '
	' or('+x+')'+' if '+x+'is a vector'
	' or('+x+',1)'+' if '+x+'is a matrix'],1)
    stk=list('mtlb_any('+x+')','0','?','?','4')
  elseif m==1|n==1 then
    stk=list('or('+x+')','0','1','1','4')
  else
    stk=list('or('+x+',1)','0','1',stk(top)(4),'4')
  end 
else
  if stk(top)(1)=='1' then
    stk=list('or('+v+',1)','0','1',stk(top-1)(4),'4')
  elseif stk(top)(1)=='2' then  
    stk=list('or('+v+',2)','0',stk(top-1)(3),'1','4')
  else  
    x=stk(top)(1)
    stk=list('or('+v+','+x+')','0','?','?','4')
  end
  top=top-1
end
endfunction
