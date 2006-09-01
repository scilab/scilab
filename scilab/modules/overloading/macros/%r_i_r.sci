function s1=%r_i_r(i,j,s1,s2)
// %r_i_r(i,j,s1,s2) insertion  s2(i,j)=s1
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  [s1,s2]=sysconv(j,s1)
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i)=s1('num'),
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i)=s1('den')
else
  [s1,s2]=sysconv(s1,s2)
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i,j)=s1('num'),
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i,j)=s1('den')
end
s1=rlist(n,d,s1('dt'))
endfunction
