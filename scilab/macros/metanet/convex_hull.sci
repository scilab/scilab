function [nhull,ind]=convex_hull(xy)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check xy
n1=size(xy,1);
if (n1<>2) then
  error('The input array must be a 2 x n matrix')
end;
n=size(xy,2);
nn=2*n;
[nhull,iwork]=m6hullcvex(n,nn,xy);
ind=iwork(1:nhull);
endfunction
