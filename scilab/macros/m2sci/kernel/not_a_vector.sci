function r=not_a_vector(A)
// Copyright INRIA
// M2SCI function
// Checks if at least two dimensions of A are greater than 1
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A can not be a vector)
// V.C.

n=size(A.dims)
nb=0
for k=1:n
  if A.dims(k)>1 | A.dims(k)==SupToOne then
    nb=nb+1
  end
end

r=nb>=2
endfunction
