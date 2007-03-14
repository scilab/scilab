function r=not_a_scalar(A)
// Copyright INRIA
// M2SCI function
// Checks if at least one dimension of A is greater than 1
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A can not be a scalar)
// V.C.

nb=0
for k=1:size(A.dims)
  if A.dims(k)>1 then
    nb=nb+1
  end
end

r=nb<>0
endfunction
