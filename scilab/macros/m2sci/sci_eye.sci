function [stk,txt,top]=sci_eye()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list(':','0','-1','-1','?')
  top=top+1
elseif rhs==1 then
  if stk(top)(3)=='1'&stk(top)(4)=='1' then
    stk=list('eye('+stk(top)(1)+','+stk(top)(1)+')','0',stk(top)(1),stk(top)(1),'?')
  elseif (stk(top)(3)=='1'&stk(top)(4)=='2')|(stk(top)(3)=='2'&stk(top)(4)=='1') then
    temp=gettempvar()
    txt=temp+'='+stk(top)(1)
    stk=list('eye('+temp+'(1),'+temp+'(2))','0','?','?','1','?')
  else
    set_infos('Not enough information using mtlb_eye instead of eye',1)
    stk=list('mtlb_eye('+stk(top)(1)+')','0','?','?','?')
  end
else
  stk=list('eye('+stk(top-1)(1)+','+stk(top)(1)+')','0',stk(top-1)(1),stk(top)(1),'?')
  top=top-1
end

