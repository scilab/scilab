function a=%p_sum(a,flag)
// Copyright INRIA
[m,n]=size(a);
if flag==1|flag=='r' then
  a=ones(1,m)*a;
elseif flag==2|flag=='c' then
  a=a*ones(n,1);
end
endfunction
