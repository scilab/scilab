function [stk,txt,top]=sci_randn()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list('rand(1,''n'')','0','1','1','1','?')
elseif rhs==1 then
    [m,n]=checkdims(stk(top))
  if m==1&n==1 then
    stk=list('rand('+stk(top)(1)+','+stk(top)(1)+',''n'')','0',stk(top)(1),stk(top)(1),'1','?')
  elseif m*n==2 then
    temp=gettempvar()
    txt=[txt;temp+'='+stk(top)(1)]
    stk=list('rand('+temp+'(1),'+temp+'(2),''n'')','0','?','?','1','?')
  else
    set_infos([
	'mtlb_rand('+stk(top)(1)+') can be replaced by :'
        ' rand('+stk(top)(1)+','+stk(top)(1)+') if '+stk(top)(1)+' is a scalar'
	' rand(v(1),v(2),''n'') if '+stk(top)(1)+' is the vector v'],1)    
    stk=list('mtlb_rand('+stk(top)(1)+',''n'')','0','?','?','1','?')
  end
else
  stk=list('rand('+stk(top-1)(1)+','+stk(top)(1)+',''n'')','0',stk(top-1)(1),stk(top)(1),'1','?')
end
endfunction
