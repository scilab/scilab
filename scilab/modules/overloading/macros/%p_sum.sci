function a=%p_sum(a,flag)
// Copyright INRIA
[m,n]=size(a);
if flag=='m'|flag==-1 then
  flag=find([m,n]>1,1)
  if flag==[] then a=sum(a),return,end
end
if flag==1|flag=='r' then
  a=ones(1,m)*a;
elseif flag==2|flag=='c' then
  a=a*ones(n,1);
end
endfunction
