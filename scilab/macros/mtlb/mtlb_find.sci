function [i,j,v]=mtlb_find(b)
// Copyright INRIA
// Translation function for find() Matlab function
// V.C.
[lhs,rhs]=argn(0)

// Because find does not work on complex values
// And isreal does not word with Booleans
c=b
if type(b)<>4 & type(b)<>6 then
  if ~isreal(b) then
    c=abs(b)
  end
end

// One output arg
if lhs==1 then
  i=find(c)
  if size(b,1)<>1 then 
    i=i'
  end
// Two output args
elseif lhs==2 then
  [i,j]=find(c)
  if size(b,1)<>1 then 
    i=i'
    j=j'
  end
// Three output args
else
  [i,j]=find(c)
  if i<>[] then 
    v = b(i+size(b,1)*(j-1))
  else 
    v = []
  end
  if size(b,1)<>1 then 
    i=i'
    j=j'
  end
end
endfunction
