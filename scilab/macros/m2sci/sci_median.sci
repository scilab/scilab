function [stk,txt,top]=sci_median()
// Copyright INRIA
txt=[]
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	 'mtlb_median('+x+') may be replaced by '
         '  median('+x+')'+' if '+x+'is a vector'
	 '  median('+x+',1)'+' if '+x+'is a matrix'],1)
    stk=list('mtlb_median('+x+')','0','?','?','1')
  elseif m==1|n==1 then
    stk=list('median('+x+')','0','1','1','1')
  else
    stk=list('median('+x+',1)','0','1',stk(top)(4),'1')
  end
else
  x=stk(top-1)(1)
  if stk(top)(1)=='1' then
    stk=list('median('+x+',1)','0','1',stk(top-1)(4),stk(top-1)(5))
  elseif stk(top)(1)=='2' then  
    stk=list('median('+x+',2)','0',stk(top-1)(3),'1',stk(top-1)(5))
  else  
    y=stk(top)(1)
    stk=list('median('+x+','+y+')','0','?','?',stk(top-1)(5))
  end
  top=top-1
end
