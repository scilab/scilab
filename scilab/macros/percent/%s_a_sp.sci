function s=%s_a_sp(a,b)
// %s_a_sp - adds a  scalar a and a sparse matrix b
//!
// Copyright INRIA
if size(a)==[-1,-1] then
  //eye+b
  [m,n]=size(b)
  s=(0+a)*speye(m,n)+b
else
  s=a+full(b)
end
endfunction
