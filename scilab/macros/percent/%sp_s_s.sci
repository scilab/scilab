function s=%sp_s_s(a,b)
// %sp_s_s - substract a scalar b to a sparse matrix a
//!
// Copyright INRIA
if size(b)==[-1 -1] then
  [m,n]=size(a)
  s=a-(b+0)*speye(m,n)
else
  s=full(a)-b
end
endfunction
