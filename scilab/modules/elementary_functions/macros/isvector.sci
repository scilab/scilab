function r=isvector(x)
// Copyright INRIA
// Checks if all dimensions of x but one are 1
// Input: x 
// Output: r = boolean value (true if x is a vector)
// F.B.

nbones=0
n=size(size(x),"*")
r=%F
for k=1:n
  if size(x,k)==1 then
    nbones=nbones+1
  elseif size(x,k)==0 then
return
  end
end
r=nbones==n-1
endfunction
 
