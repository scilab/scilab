function [stk,txt,top]=sci_colormap()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('xget(''colormap'')','0','?','3','1')
elseif stk(top)(5)=='10'|(stk(top)(3)=='1'&stk(top)(4)=='1') then
  stk=list('xset(''colormap'',defaultcmap())','0','?','3','1')
elseif  stk(top)(5)=='1' then  
  stk=list('xset(''colormap'','+stk(top)(1)+')','0','?','3','1')
else
  set_infos(['Not enough information using mtlb_colormap'
                 'instead of xset(''colormap'',..'],1)
  stk=list('mtlb_colormap('+stk(top)(1)+')','0','?','3','1')
end
 
