function [stk,txt,top]=sci_subplot()
// Copyright INRIA
txt=[]
if rhs==2 then pause,end
if rhs==1 then
  if isnum(stk(top)(1)) then
    m=evstr(stk(top)(1))
    if type(m)==10 then  m=evstr(m),end
    p=modulo(m,10)
    n=modulo((m-p)/10,10)
    m=round((m-p-10*n)/100)
    j=int((p-1)/n)
    i=p-1-n*j
    rect=[i/n,j/m,1/n,1/m]
    i=string(i);j=string(j),n=string(n);m=string(m)
    e='xsetech('+lhsargs([i+'/'+n,j+'/'+m,'1/'+n,'1/'+m])+')'
    stk=list(e,'0','0','0','0')
  else
    stk=list('mtlb_subplot('+stk(top)(1)+')','0','0','0','0')
  end
elseif rhs==0 then
  stk=list('xsetech([0 0 1 1])','0','0','0','0')
else
  i=gettempvar(1)
  j=gettempvar(2)
  p=stk(top)(1)
  m=stk(top-2)(1)
  n=stk(top-1)(1)
  if stk(top-1)(2)=='2' then n='('+n+')',end
  if stk(top-2)(2)=='2' then m='('+m+')',end
  txt=j+' = int(('+p+'-1)/'+n+');'+i+' = '+p+'-1-'+n+'*'+j
  e='xsetech('+lhsargs([i+'/'+n,j+'/'+m,'1/'+n,'1/'+m])+')'
  stk=list(e,'0','0','0','0')
  top=top-2
end



