function a=%s_i_sp(i,j,b,a)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  a=b;
  b=j;
  [m,n]=size(a)
  a=a(:)
  a(i)=b(:)
  a=matrix(a,m,n)
end
endfunction
