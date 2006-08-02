function r=is_a_vector(A)
// Copyright INRIA
// M2SCI function
// Checks if all dimensions of A but one are 1
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A is a vector)
// V.C.

nbones=0
n=size(A.dims)
r=%F
for k=1:n
  if A.dims(k)==1 then
    nbones=nbones+1
  elseif A.dims(k)==-1 then
    return
  end
end
r=nbones==n-1
endfunction
