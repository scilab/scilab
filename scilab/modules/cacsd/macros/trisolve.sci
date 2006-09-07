function [b,sexp]=trisolve(a,b,sexp)
//[x [,sexp]] = trisolve(A,b [,sexp])  symbolically solves A*x =b 
// when A and b are matrices of character strings, A being assumed to be
// upper triangular.  
//sexp : vector of common subexpressions in A, b and x.
//!
//origine F. D. S. Steer INRIA 1989
//
// Copyright INRIA
[lhs,rhs]=argn(0)
[n0,m]=size(b)
if rhs==2 then
  for n=n0:-1:1
    pivot=a(n,n)
    for k=1:m,b(n,k)=ldivf(pivot,'('+b(n,k)+')'),end
    if n==1 then return,end
    for l=1:n-1
      for k=1:m,
	b(l,k)=addf(b(l,k),mulf(mulf('-1',a(l,n)),b(n,k)))
      end
    end
  end
else
  ns=prod(size(sexp))
  for n=n0:-1:1
    pivot=a(n,n)
    for k=1:m,
      ns=ns+1
      sexp(ns)=ldivf(pivot,'('+b(n,k)+')')
      b(n,k)='%('+string(ns)+')';
    end
    if n==1 then return,end
    for l=1:n-1
      for k=1:m,
	b(l,k)=addf(b(l,k),mulf(mulf('-1',a(l,n)),b(n,k)))
      end
    end
  end
end
endfunction
