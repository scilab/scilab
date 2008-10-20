function a=%sp_sum(a,flag)
// Copyright INRIA
[m,n]=size(a);
if flag=='m'|flag==-1 then
  flag=find([m,n]>1,1)
  if flag==[] then a=sum(a),return,end
end

if flag==2|flag=='c' then
  a=sparse(a*ones(n,1))
end
if flag==1|flag=='r' then
  a=sparse(ones(1,m)*a);
end
endfunction
