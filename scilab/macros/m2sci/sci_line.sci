function [stk,txt,top]=sci_line()
// Copyright INRIA
txt=[]
options=[]
if rhs>2 then
  top=top-rhs+2
  rhs=2
  options=[]
end
//x argument
if stk(top-1)(3)=='1'|stk(top-1)(4)=='1' then
  if isname(stk(top-1)(1)) then 
    x=stk(top-1)(1)
    txt=[txt;x+' = '+x+'(:)']
  else
    x=gettempvar(1)
    txt=[txt;x+' = '+stk(top-1)(1)+';'+x+' = '+x+'(:)']
  end
else 
  if isname(stk(top-1)(1)) then 
    x=stk(top-1)(1),
  else
    x=gettempvar(1)
    txt=[txt;x+' = '+stk(top-1)(1)]
  end
  txt=[txt;
      'if min(size('+x+'))==1 then'
      '  '+x+' = '+x+'(:)*ones(1,size('+y+',2))'
      'end']
end
//y argument
if stk(top)(3)=='1'|stk(top)(4)=='1' then
  if isname(stk(top)(1)) then 
    y=stk(top-1)(1)
    txt=[txt;y+' = '+y+'(:)']
  else
    y=gettempvar(1)
    txt=[txt;y+' = '+stk(top)(1)+';'+y+' = '+y+'(:)']
  end
else 
  if isname(stk(top)(1)) then 
    y=stk(top)(1),
  else
    y=gettempvar(1)
    txt=[txt;y+' = '+stk(top)(1)]
  end
  txt=[txt;
      'if min(size('+y+'))==1 then'
      '  '+y+' = '+y+'(:)*ones(1,size('+x+',2))'
      'end']
end

txt=[txt;'xpolys'+rhsargs([x,y,options])]
top=top-1
stk=list(' ','-2','0','0','0')




