// Copyright INRIA
deff('[y]=t(x)',[
'y=0'
'if x>0 then'
'  if x==1 then '
'    return'
'  else '
'    if x==2 then '
'       y=10'
'    else '
'       for u=1:x, '
'         y=y+u,'
'       end,'
'       return'
'    end'
'  end'
'else '
'  for u=1:-x, '
'    if u==4 then '
'      return,'
'    else '
'      y=u,'
'    end'
'  end'
'end'],'n')
//
if t(0)<>0 then pause,end
if t(1)<>0 then pause,end
if t(-1)<>1 then pause,end
//
t1=t;
comp(t1)
if t(0)-t1(0)<>0 then pause,end
if t(1)-t1(1)<>0 then pause,end
if t(-1)-t1(-1)<>0 then pause,end
