function [U,S,V]=sva(A,tol)
// Copyright INRIA
// approximation de rang donne d'une matrice
  if A==[] then U=[],S=[],V=[],return,end
  [U,S,V]=svd(A,"e")
  if argn(2)==1 then
    tol = max(size(A)) * S(1) * %eps;
    rk = size(find(diag(S) > tol),'*');
  else
    if tol>1 then //rank given
      rk=tol
      if rk>min(size(A)) then 
	error('Requested rank is greater than matrix dimension')
      end
    else
      rk = size(find(diag(S) > tol),'*');
    end
  end
  U=U(:,1:rk);S=S(1:rk,1:rk),V=V(:,1:rk)
endfunction

    
    
