function s=%sp_a_s(a,b)
// %sp_a_s - adds a sparse matrix and a scalar s
//!
// Copyright INRIA
if size(b)==[-1 -1] then
  [m,n]=size(a)
  s=a+(b+0)*speye(m,n)
else
  s=full(a)+b
end
endfunction
