function y = linspace(d1, d2, n)
// Linearly spaced vector.
// linspace(x1, x2) generates a row vector of 100 linearly
// equally spaced points between x1 and x2.
// linspace(x1, x2, n) generates n points between x1 and x2.
// Copyright INRIA
  if argn(2) == 2 then n = 100;end
  y = [d1*ones(1,n-1)+(0:n-2)*(d2-d1)/(n-1),d2];
endfunction
