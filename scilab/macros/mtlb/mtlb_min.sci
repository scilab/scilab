function [r,k]=mtlb_min(a,b,c)
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs==1 then //min(A)
  if ~isreal(a,0) then 
    if size(a,1)==1|size(a,2)==1 then
      [r,k]=min(abs(a))
      r=a(k)
    else
      [r,k]=min(abs(a),'r')
      r=a(k,:)
    end
  else
    a=real(a)
    if size(a,1)==1|size(a,2)==1 then
      [r,k]=min(a)
    else
      [r,k]=min(a,'r')
    end
  end
elseif rhs==3 then //min(A,[],num) with num>2
  error('not yet implemented') 
end
