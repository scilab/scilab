function r=%sp_e(i,j,a)
// r=a(i,j) for f sparse in some special cases
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2 then
  a=j;
  a=a(:)
  r=a(i)
end
endfunction
