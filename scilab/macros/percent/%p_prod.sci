function r=%p_prod(a,flag)
// Copyright INRIA
r=[]
[m,n]=size(a);
if flag==1|flag=='r' then
  for k=1:n,r(1,k)=prod(a(:,k)),end
end
if flag==2|flag=='c' then
  for k=1:m,r(k,1)=prod(a(k,:)),end
end
