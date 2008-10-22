function [AA, BB, Q, Z, V]=mtlb_qz(A,B)
// Copyright INRIA
[lhs,rhs]=argn(0)
[AA, BB, Q, Z]=gschur(A,B)
if lhs==5 then
  n=size(A,1)
  LA=diag(AA)
  LB=diag(BB)
  V=zeros(n,n)
  for k=1:n
    K=kernel(A*LB(k)-B*LA(k))
    if size(K,2)<>1 then
      error('qz : pencil is not diagonalizable')
    end
    V(:,k)=K
  end
end
endfunction
