function [stk,txt,top]=sci_sparse()
// Copyright INRIA
txt=[]
V1=gettempvar(1)
V2=gettempvar(2)
if rhs==6 then rhs=5;top=top-1,end
if rhs==1 then
  stk=list('sparse('+stk(top)(1)+')','0',stk(top)(3),stk(top)(4),'5')
elseif rhs==2 then
  stk=list('sparse([],[],['+stk(top-1)(1)+','+stk(top)(1)+'])','0',stk(top-1)(1),stk(top)(1),'5')
  top=top-1 
elseif rhs==3 then    
  s1=stk(top-2)
  s2=stk(top-1)
  s3=stk(top)
  if s1(4)=='1'&s2(4)=='1' then
    stk=list('sparse(['+s1(1)+','+s2(1)+'],'+s3(1)+')','0','?','?','5')
  elseif or(s1(1)==vnms(:,1))&or(s2(1)==vnms(:,1)) then
    stk=list('sparse(['+s1(1)+'(:),'+s2(1)+'(:)],'+s3(1)+')','0','?','?','5')
  else
    txt=[V1+' = '+s1(1)+';'+V1+'='+V1+'(:)';
	V2+' = '+s2(1)+';'+V2+'='+V2+'(:)'];
  
    stk=list('sparse(['+V1+','+V2+'],'+s3(1)+')','0','?','?','5')
  end
  
  top=top-2
elseif rhs==5 then    
  s1=stk(top-4)
  s2=stk(top-3)
  s3=stk(top-2)
  s4=stk(top-1)
  s5=stk(top)
  if s1(4)=='1'&s2(4)=='1' then
    stk=list('sparse(['+s1(1)+','+s2(1)+'],'+s3(1)+')','0','?','?','5')
  elseif or(s1(1)==vnms(:,1))&or(s2(1)==vnms(:,1)) then
    stk=list('sparse(['+s1(1)+'(:),'+s2(1)+'(:)],'+s3(1)+',['+s4(1)+','+s5(1)+'])','0','?','?','5')  
  else
    txt=[V1+' = '+s1(1)+';'+V1+'='+V1+'(:)';
         V2+' = '+s2(1)+';'+V2+'='+V2+'(:)'];
    stk=list('sparse(['+V1+','+V2+'],'+s3(1)+',['+s4(1)+','+s5(1)+'])','0','?','?','5')
  end
  
  top=top-4 
end


