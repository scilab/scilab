function r=mtlb_e(a,i)
// Copyright INRIA
if type(a)==10 then
  a=strcat(a);
  r=part(a,i)
else
  r=a(i)
  if size(i,2)>1 then //index is a row vector
    if min(size(a))>1 then // a is'nt a vector
      r=matrix(r,1,size(r,'*')) //return a row vector
    end
  end
end
endfunction
