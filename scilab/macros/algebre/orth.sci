function [Q]=orth(A)
// Orthogonal basis for the span of A.
// Range(Q) = Range(A) and Q'*Q=eye
//!
// Copyright INRIA
  [X,n]=rowcomp(A);X=X';
  Q=X(:,1:n);
endfunction

