function r=is_a_scalar(A)
// Copyright INRIA
// M2SCI function
// Checks if all dimensions of A are 1
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A is a scalar)
// V.C.
if exists("A")==0 then pause;end
n=size(A.dims)
r=%T
for k=1:n
  if execstr("A.dims(k)","errcatch")<>0 then pause;end;errclear();
  if A.dims(k)<>1 then
    r=%F
    return
  end
end
endfunction
