function [g,u] = %i_bezout(a,b)
//   g = bezout(a,b) is the greatest common divisor of  a and b.   
//       a and b must contain non-negative   integer scalars.
//   [g,U] = bezout(a,b) also returns a (2x2) unimodular matrix U such that: 
//   [a,b]*U = [g,0].
//   These are useful for solving Diophantine equations and computing
//   Hermite transformations.

  it=max(inttype(a),inttype(b))
  a=iconvert(a,it);b=iconvert(b,it)
  u = [iconvert([1 0],it) a];
  v = [iconvert([0 1],it) b];
  zero=iconvert(0,it)
  while v(3)<>zero
    q = u(3)/v(3);
    t = u - v*q;
    u = v;
    v = t;
  end
  g = u(3);
  u=[u(1) -v(1);u(2) -v(2)]

endfunction
