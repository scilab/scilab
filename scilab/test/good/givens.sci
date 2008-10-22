function [u,c]=givens(x,y)
//Syntax : u=givens(xy)
//         u=givens(x,y)
//
// xy = [x;y], u=givens(xy)
// returns a 2*2 matrix u such that u*xy=[r;0].
// c is equal to u*xy
// givens(x,y)=givens([x;y])
//
//!
// Copyright INRIA
  [lhs,rhs]=argn(0);
  if rhs==2 then x=[x;y];end
  if or(size(x)<>[2 1]) then 
    error('givens: argument must be a column vector')
  end
  if x(2)<>0 then
    r = norm(x);
    u = [x'; -x(2) x(1)]/r;
    c = [r; 0];
  else
    u=eye(2,2)
    c=x
  end
endfunction
