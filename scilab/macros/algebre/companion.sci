function A = companion(p)
// Companion matrix.
// A=compan(p) is a companion matrix with 
// characteristic polynomial equal to (or proportional to) 
// p. If p is a vector of polynomials, the A matrix is block
// diagonal and block number i has characteristic polynomial
// equal to p(i).

// Copyright INRIA
if type(p) ~= 2 
  error('Compan: Input argument must be a polynomial');
end
p=p(:);
p=p';
A=[];
for pp=p;
c=coeff(pp);
c=c($:-1:1);
n = length(c);
if n <= 1
   B=[];
elseif n == 2
   B=-c(2)/c(1);
else
   c=c(:)';
   B=diag(ones(1,n-2),-1);
   B(1,:) = -c(2:n)/c(1);
end
A=sysdiag(A,B);
end
