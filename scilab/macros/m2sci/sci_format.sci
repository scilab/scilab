function [stk,txt,top]=sci_format()
// Copyright INRIA
txt=[]
rhs1=rhs
if rhs==2 then
  fp=sci2exp('e')
  rhs=rhs-1
  top=top-1
else
  fp=sci2exp('v')
end
d=5;hex=%f;bank=%f;compact=%f;loose=%f,ratf=%f;symb=%f;unk=%f
if rhs==1 then
  f=stk(top)(1)
  select f
  case '''short''' then
    d=5
  case '''long''' then
    d=15
  case '''hex''' then
    hex=%t
  case '''+''' then
    symb=%t
  case '''bank''' then
    bank=%t
  case '''compact''' then
    compact=%t
  case '''loose''' then
    loose=%t
  case '''rat''' then
    ratf=%t
  else
    unk=%t
  end
else
  d=5
end

if unk then
  if rhs1==1 then
    stk=list('mtlb_format('+stk(top)(1)+')','0','0','0','0')
  else
    stk=list('mtlb_format('+stk(top)(1)+','+stk(top+1)(1)+')','0','0','0','0')
  end
elseif ratf|loose|compact|bank|hex then
  set_infos('No equivalent to '+f+' format. Ignored',2)
  stk=list(' ','-2','0','0','0','0')
else
  if rhs1==1 then
    stk=list('format('+string(d+1)+')','0','0','0','0')
  elseif rhs1==2 then
     stk=list('format('+fp+','+string(d+1)+')','0','0','0','0')
  else
    stk=list('format(''v'',6)','0','0','0','0')
    top=top+1
  end
end

