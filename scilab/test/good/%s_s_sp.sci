function s=%s_s_sp(a,b)
// %s_s_sp - substract a sparse matrix b to a scalar matrix a
//!
// Copyright INRIA
if size(a)==[-1,-1] then
  //eye+b
  [m,n]=size(b)
  s=(0+a)*speye(m,n)-b
else
  s=a-full(b)
end
endfunction
