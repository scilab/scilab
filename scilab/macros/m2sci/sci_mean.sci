function [stk,txt,top]=sci_mean()
// Copyright INRIA
txt=[]
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	 'mtlb_mean('+x+') may be replaced by '
	 '  mean('+x+')'+' if '+x+'is a vector'
	 '  mean('+x+',1)'+' if '+x+'is a matrix'],1)
    stk=list('mtlb_mean('+x+')','0','1','?','1')
  elseif m==1|n==1 then
    stk=list('mean('+x+')','0','1','1','1')
  else
    stk=list('mean('+x+',1)','0','1',stk(top)(4),'1')
  end
else
  if stk(top)(1)=='1' then
    stk=list('mean('+v+',1)','0','1',stk(top-1)(4),stk(top-1)(5))
  elseif stk(top)(1)=='2' then  
    stk=list('mean('+v+',2)','0',stk(top-1)(3),'1',stk(top-1)(5))
  else  
    x=stk(top)(1)
    stk=list('mean('+v+','+x+')','0','?','?',stk(top-1)(5))
  end
  top=top-1 
end
endfunction
