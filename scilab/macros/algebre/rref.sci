function [A,jb]=rref(A,tol)
  //R = RREF(A) produces the reduced row echelon form of A.
  if A==[] then R=[],return,end
  if argn(2)<2 then
    tol=2*%eps*norm(A,'inf')*max(size(A))
  end
  [m,n]=size(A)
  k = 1;l = 1;jb = [];
  while (k <= m) & (l <= n)
    // Find value and index of largest element in the remainder of column l.
    [p,i] = max(abs(A(k:$,l))); i = i+k-1;
    if (p <= tol) // The column is negligible
      A(k:$,l) = zeros(m-k+1,1);
      l = l + 1;
    else 
      jb = [jb, l]; // Remember column index
      A([k,i],l:$) = A([i,k],l:$); //swap rows i and j
      A(k,l:$) = A(k,l:$)/A(k,l); // Normalize the pivot row

      i = [1:k-1 k+1:m]; //other rows
      A(i,l:$) = A(i,l:$) - A(i,l)*A(k,l:$); //eliminate
      k = k + 1;l = l + 1;
    end
  end
endfunction
