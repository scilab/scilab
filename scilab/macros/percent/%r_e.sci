function f=%r_e(i,j,f)
// %r_e(i,j,f) extraction f(i,j) in a rational matrix
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2 then 
  f=rlist(j('num')(i),j('den')(i),j('dt'))
elseif rhs==3 then
  f=rlist(f('num')(i,j),f('den')(i,j),f('dt'))
end
if f('num')==[] then f=[],end
endfunction
