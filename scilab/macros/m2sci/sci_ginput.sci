function [stk,txt,top]=sci_ginput()
// Copyright INRIA
txt=[]
if rhs<1 then
  RHS='-1'
else
  RHS=stk(top)(1)
end
if lhs==1 then
    stk=list('locate('+RHS+')''','0','?','2','1')
elseif lhs==2 then
  xy=gettempvar()
  txt=[txt;xy+' = locate('+RHS+')'''];
  stk=list(list(xy+'(:,1)','-1','?','1','1'),..
      list(xy+'(:,2)+1','-1','?','1','1'))
else
  [x,y,btn]=lhsvarsnames()  
  xy=gettempvar()
  txt=[txt;['['+xy+','+btn+'] = locate('+RHS+')';
      x+' = '+xy+'(1,:)'''
      y+' = '+xy+'(2,:)'''
      btn+' = '+btn+'''+1']]
   
  stk=list(list(' ','-2','0','?','1','1'),..
      list(' ','-2','0','?','1','1'),..
      list(' ','-2','0','?','1','1'))   

end
