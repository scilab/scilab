function [stk,txt,top]=sci_lu()
// Copyright INRIA
txt=[]
s=stk(top)
if lhs==1 then
  set_infos('mtlb_lu(x) used to emulate lu(x)',1)
  stk=list('mtlb_lu('+s(1)+')','0',s(3),s(4),'1','?')
elseif lhs==2 then
  stk=list(list('lu('+s(1)+')','-1',s(3),s(4),'1','?'),..
           list('lu('+s(1)+')','-1',s(3),s(4),'1','?'))
else
  r= list('lu('+s(1)+')','-1',s(3),s(4),'1','?')   
  stk=list(r,r,r)     
end
endfunction
