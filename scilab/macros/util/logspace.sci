function y=logspace(d1, d2, n)
// Logarithmically spaced vector.
// logspace(d1, d2) generates a row vector of n logarithmically
// equally spaced points between 10^d1 and 10^d2.  If d2=%pi
// then the points are between 10^d1 and pi.
// logspace(d1, d2, n) generates n points.
// Copyright INRIA
[nargout,nargin]=argn(0);
if nargin == 2
    n = 50;
end
if d2==%pi
    d2 = log(%pi)/log(10);
end
y=exp(log(10)*[d1*ones(1,n-1)+(0:n-2)*(d2-d1)/(n-1),d2]);
