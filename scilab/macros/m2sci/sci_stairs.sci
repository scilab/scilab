function [stk,txt,top]=sci_stairs()
// Copyright INRIA
txt=[]
if lhs==2 then
  if rhs==1 then
    stk=list('mtlb_stairs'+rhsargs(stk(top)(1)),'0','?','1','1')
  else
    r=list('mtlb_stairs'+rhsargs([stk(top-1)(1),stk(top)(1)]),'0','?','1','1')
    stk=list(r,r)
    top=top-1
  end 
else
  if rhs==1 then
    x=gettempvar(1)
    if isname(stk(top)(1)) then
      y=stk(top)(1)
    else
      y=gettempvar(2)
      txt=y+' = '+stk(top)(1);
    end

    if stk(top)(4)=='1' then // column vector    
      txt='plot2d2'+rhsargs([sci2exp('enn'),'0',y])
    else   // unknown shape
      txt='plot2d2'+rhsargs([sci2exp('enn'),'0',y+'(:)'])
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
        'plot2d2'+rhsargs([sci2exp('gnn'),x,y])]
    top=top-1
  end
  stk=list(' ','-2','0','0','0')
end

