function [r,k]=mtlb_max(a,b,c)
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs==1 then //max(A)
  if ~isreal(a,0) then 
    if size(a,1)==1|size(a,2)==1 then
      [r,k]=max(abs(a))
      r=a(k)
    else
      [r,k]=max(abs(a),'r')
      r=a(k,:)
    end
  else
    a=real(a)
    if size(a,1)==1|size(a,2)==1 then
      [r,k]=max(a)
    else
      [r,k]=max(a,'r')
    end
  end
elseif rhs==3 then //max(A,[],num) with num>2
  error('not yet implemented') 
end
endfunction
