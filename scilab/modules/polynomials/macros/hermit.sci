function [a,u]=hermit(a)
//[A,U]=hermit(A)
//Hermite form: U is an unimodular matrix such that A*U is
//triangular. The output value of A is A*U.
//Warning: Experimental version
//!
// Copyright INRIA
[m,n]=size(a);if m<>n then error('square matrix only!'),end
[a,u]=htrianr(a)
for l=n-1:-1:1
 dl(l:n)=degree(a(l,l:n));
 for k=l+1:n
    if dl(k)>=dl(l) then
       all=a(l,l);
       if norm(coeff(all),1) > 1.d-10 then
       [r,q]=pdiv(a(l,k),a(l,l))
       if l>1 then a(1:l-1,k)=a(1:l-1,k)-a(1:l-1,l)*q;end
       a(l,k)=r
       u(:,k)=u(:,k)-u(:,l)*q
       end
    end
 end
end
endfunction
