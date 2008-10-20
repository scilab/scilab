function s1=%p_i_r(i,j,s1,s2)
//s2(i,j)=s1, s1 matrix of polynomials
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  [s1,s2]=(j,s1)
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i)=s1
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i)=ones(s1);
else
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i,j)=s1
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i,j)=ones(s1);
end
s1=rlist(n,d,s2('dt'))
endfunction
