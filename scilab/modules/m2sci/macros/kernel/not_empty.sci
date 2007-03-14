function r=not_empty(A)
// Copyright INRIA
// M2SCI function
// Checks if no A dimension can be 0
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A can not be an empty matrix)
// V.C.

n=size(A.dims)
nb=0
for k=1:n
  if A.dims(k)>0 | A.dims(k)==SupToOne then
    nb=nb+1
  end
end

r=nb>0
endfunction
