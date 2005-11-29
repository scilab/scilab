function r=%p_prod(a,flag)
// Copyright INRIA
r=[]
[m,n]=size(a);
[m,n]=size(a);
if flag=='m'|flag==-1 then
  flag=find([m,n]>1,1)
  if flag==[] then a=prod(a),return,end
end

if flag==1|flag=='r' then
  for k=1:n,r(1,k)=prod(a(:,k)),end
end
if flag==2|flag=='c' then
  for k=1:m,r(k,1)=prod(a(k,:)),end
end
endfunction
