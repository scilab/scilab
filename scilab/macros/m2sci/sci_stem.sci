function [stk,txt,top]=sci_stem()
// Copyright INRIA
txt=[]
if rhs==3|stk(top)(5)=='10' then
  style=linetype(stk(top)(1))
  top=top-1
  rhs=rhs-1
else
  style=-1
end

style=sci2exp(style)
if rhs==1 then
  x=gettempvar(1)
  if isname(stk(top)(1)) then
    y=stk(top)(1)
  else
    y=gettempvar(2)
    txt=y+' = '+stk(top)(1);
  end
  txt=[txt;
      x+' = 1:size('+y+',''*''))'
      'plot2d'+rhsargs([x,y,'-9'])]
  if stk(top)(3)=='1' then // row vector
    txt=[txt;'xsegs'+rhsargs(['[1;1]*'+x,'[1;1]*'+y,style])]
  elseif stk(top)(4)=='1' then // column vector    
    txt=[txt;'xsegs'+rhsargs(['[1;1]*'+x,'[1;1]*'+y+'''',style])]
  else   // unknown shape
    txt=[txt;'xsegs'+rhsargs(['[1;1]*'+x,'[1;1]*'+y+'(:)''',style])]
  end
elseif rhs==2 then     
  if isname(stk(top)(1)) then
    y=stk(top)(1)
  else
    y=gettempvar(2)
    txt=[txt;y+' = '+stk(top)(1)];
  end
  if isname(stk(top-1)(1)) then
    x=stk(top-1)(1)
  else
    x=gettempvar(2)
    txt=[txt;x+' = '+stk(top-1)(1)];
  end
  if stk(top)(4)=='1' then //y is a column vector 
    txt=[txt;y+' = '+y+'''']
  elseif stk(top)(3)<>'1' then //y has unknown shape
    txt=[txt;y+' = '+y(:)+'''']
  end
  if stk(top-1)(4)=='1' then //x is a column vector 
    txt=[txt;x+' = '+x+'''']
  elseif stk(top-1)(3)<>'1' then //x has unknown shape
    txt=[txt;x+' = '+x(:)+'''']
  end
  txt=[txt;
      'plot2d'+rhsargs([x,y,'-9'])
      'xsegs'+rhsargs(['[1;1]*'+x,'[1;1]*'+y,style])]
  top=top-1
end
stk=list(' ','-2','0','0','0')
