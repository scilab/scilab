function r=is_empty(A)
// Copyright INRIA
// M2SCI function
// Checks if all A dimension are 0
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A is an empty matrix)
// V.C.

n=size(A.dims)
nb=0
for k=1:n
  if A.dims(k)==0 then
    nb=nb+1
  end
end

r=nb==n
endfunction
