function A=%sp_sort(A,flag)
// Copyright INRIA
[ij,v,mn]=spget(A);
if mn(2)==1 then
  last=find(v<0);
  first=find(v>0);
  nn=size(v,'*');
  v([1:size(first,'*'),nn-size(last,'*')+1:nn])=[sort(v(first));sort(v(last))];
  A=sparse(ij,v,mn);
elseif mn(1)==1 then
  last=find(v<0);
  first=find(v>0);
  nn=size(v,'*');
  v([1:size(first,'*'),nn-size(last,'*')+1:nn])=[sort(v(first));sort(v(last))];
  A=sparse(ij,v,mn);  
else
  error('Sorry: Only sparse VECTORS can be sorted!');
end
endfunction
