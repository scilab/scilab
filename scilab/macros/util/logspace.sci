function y=logspace(d1, d2, n)
// Logarithmically spaced vector.
// logspace(d1, d2) generates a row vector of 50 logarithmically
// equally spaced points between 10^d1 and 10^d2.  If d2=%pi
// then the points are between 10^d1 and %pi.
// logspace(d1, d2, n) generates n points.
// Copyright INRIA
  if argn(2) == 2
    n = 50;
  end
  n=floor(n)
  if d2==%pi
    d2 = log(%pi)/log(10);
  end
  y=10^[d1*ones(1,n-1)+(0:n-2)*(d2-d1)/(n-1),d2];
endfunction
