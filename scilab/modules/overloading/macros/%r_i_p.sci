function f2=%r_i_p(i,j,f2,n)
// f2=%r_i_p(i,j,f2,p)   insertion
//
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  n=f2;f2=j
  d=ones(n);
  n(i)=f2('num'),d(i)=f2('den')
else
  d=ones(n);
  n(i,j)=f2('num'),d(i,j)=f2('den')
end
f2=rlist(n,d,f2('dt'))
endfunction
