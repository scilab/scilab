function a=%sp_sum(a,flag)
// Copyright INRIA
[m,n]=size(a);
if flag==2|flag=='c' then
  a=sparse(a*ones(n,1))
end
if flag==1|flag=='r' then
  a=sparse(ones(1,m)*a);
end
