function [stk,txt,top]=sci_ones()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list('1','0','1','1','1','?')
  top=top+1
elseif rhs==1 then
  [m,n]=checkdims(stk(top))
  if m==1&n==1 then
    stk=list('ones('+stk(top)(1)+','+stk(top)(1)+')','0',stk(top)(1),stk(top)(1),'1','?')
  elseif m*n==2 then
    temp=gettempvar()
    txt=temp+'='+stk(top)(1)
    stk=list('ones('+temp+'(1),'+temp+'(2))','0','?','?','1','?')
  else
    set_infos([
	'mtlb_ones('+stk(top)(1)+') can be replaced by :'
        '  ones('+stk(top)(1)+','+stk(top)(1)+') if '+stk(top)(1)+' is a scalar'
	'  ones(v(1),v(2),''n'') if '+stk(top)(1)+' is the vector v'],1)
    stk=list('mtlb_ones('+stk(top)(1)+')','0','?','?','1','?')
  end
else
  stk=list('ones('+stk(top-1)(1)+','+stk(top)(1)+')','0',stk(top-1)(1),stk(top)(1),'1','?')
  top=top-1
end


