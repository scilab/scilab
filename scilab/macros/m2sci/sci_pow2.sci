function [stk,txt,top]=sci_pow2()
// Copyright INRIA
txt=[]
if rhs==1 then
  s1=stk(top)(1)
  if stk(top)(2)=='2'|stk(top)(2)=='3'|part(stk(top)(1),1)=='-' then 
    s1='('+s1+')',
  end
  stk=list('2.^'+s1,'2',stk(top)(3),stk(top)(4),'1')
else
  s2=stk(top)(1);f=stk(top)(2);
  e=stk(top-1)(2);s1=stk(top-1)(1)
  if f=='2'|f=='3' then s2='('+s2+')',end
  if e=='2'|e=='3'|part(s1,1)=='-' then 
    s1='('+s1+')',
  end
  stk=list(s1+'.* 2 .^'+s2,2,'?','?','1')
end
endfunction
