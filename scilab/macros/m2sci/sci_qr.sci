function [stk,txt,top]=sci_qr()
// Copyright INRIA
txt=[]
if rhs==1 then
  s=stk(top)
  if lhs==1 then
    set_infos('No scilab eqivalent to qr with only one lhs arg using mtlb_qr',1)
    stk=list('mtlb_qr('+s(1)+')','0',s(3),s(4),'1')
  elseif lhs==2 then
    stk=list(list('qr('+s(1)+')','-1',s(3),s(3),'1'),..
	list('qr('+s(1)+')','-1',s(3),s(4),'1'))
  else
    r= list('qr('+s(1)+')','-1',s(3),s(4),'1')
    stk=list(r,r,r)     
  end
else  //economy size
  s=stk(top-1)
  if lhs==1 then
    set_infos('No scilab eqivalent to qr with only one lhs arg using mtlb_qr',1)
    stk=list('mtlb_qr('+s(1)+')','0',s(3),s(4),'1')
  elseif lhs==2 then
    [q,r]=lhsvarsnames()
    txt=[lhsargs([q r])+'= qr('+s(1)+')';
	 q+' = '+q+'(:,1:size('+r+',2))';
	 r+' = '+r+'(1:size('+r+',2),:)']
    
    stk=list(list(' ','-2',s(3),s(4),'1'),..
	list(' ','-2',s(4),s(4),'1'))
  else
    [q,r,e]=lhsvarsnames()    
    txt=[lhsargs([q r e])+'= qr('+s(1)+')';
	 q+' = '+q+'(:,1:size('+r+',2))'
	 r+' = '+r+'(1:size('+r+',2),:)'
         e+' = (1:size('+r+',2))*'+e]
    r= list(' ','-2',s(3),s(4),'1')
    stk=list(..
	list(' ','-2',s(3),s(4),'1'),..
	list(' ','-2',s(4),s(4),'1'),..
	list(' ','-2','1',s(4),'1'))
  end
  
end
endfunction
