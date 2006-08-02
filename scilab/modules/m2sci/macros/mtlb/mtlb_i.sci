function a=mtlb_i(a,i,b)
// Copyright INRIA
if size(a,1)==1 then 
  a(i)=matrix(b,1,size(b,'*'))
elseif size(a,2)==1 then 
  a(i)=matrix(b,size(b,'*'),1)
else
  a(i)=b
end
endfunction
