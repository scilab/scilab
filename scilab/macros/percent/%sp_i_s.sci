function a=%sp_i_s(i,j,b,a)
// %sp_i_s(i,j,b,a) insert sparse matrix b into full matrix a
// a(i,j)=b
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==4 then
  a(i,j)=full(b)
else
  a=b;//b=j
  a(i)=full(j)
end
