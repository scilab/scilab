function [stk,txt,top]=sci_zeros()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list('0','0','1','1','1','?')
  top=top+1
elseif rhs==1 then
  [m,n]=checkdims(stk(top))
  if m==1&n==1 then
    stk=list('zeros('+stk(top)(1)+','+stk(top)(1)+')','0',stk(top)(1),stk(top)(1),'1','?')
  elseif m*n==2 then
    temp=gettempvar()
    txt=temp+'='+stk(top)(1)
    stk=list('zeros('+temp+'(1),'+temp+'(2))','0','?','?','1','?')
  else
    set_infos([
	'mtlb_zeros('+stk(top)(1)+') can be replaced by :'
        ' zeros('+stk(top)(1)+','+stk(top)(1)+') if '+stk(top)(1)+' is a scalar'
	' zeros(v(1),v(2),''n'') if '+stk(top)(1)+' is the vector v'],1)
    stk=list('mtlb_zeros('+stk(top)(1)+')','0','?','?','1','?')
  end
else
  stk=list('zeros('+stk(top-1)(1)+','+stk(top)(1)+')','0',stk(top-1)(1),stk(top)(1),'1','?')
  top=top-1
end


