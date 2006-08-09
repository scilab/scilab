function [A,U,rk]=htrianr(A)
//[A,U,rk]=htrianr(a)
//triangularization of polynomial matrix A.  A is [m,n], m<=n.
//U=right unimodular basis
//the output value of A equals  A*U
//rk=normal rank of A
//Warning: there is an elimination of neglectable terms
//!
// Copyright INRIA
A=clean(A); 
[m,n]=size(A);U=eye(n,n);
l1=n+1;
for l=m:-1:maxi((m-n),1)
 l1=l1-1;
 if l1<>0 then
  Al=A(l,1:l1);
  if norm(coeff(Al),1) > 1.d-10 then
    [pg,Ul]=hrmt(Al);
    Ul=clean(Ul,1.d-10);
    A(l,1:l1)=[0*ones(1,l1-1) pg];
    U(:,1:l1)=U(:,1:l1)*Ul;
         if l>1 then
          A(1:l-1,1:l1)=A(1:l-1,1:l1)*Ul;
	end
  else
  l1=l1+1
  end
 end
end
U=clean(U,1.d-10);
k0=0;k1=0;tol=norm(coeff(A),1);
v=[];w=[];
for k=1:n
   if maxi(abs(coeff(A(:,k)))) <= sqrt(%eps)*tol then
      k0=k0+1;v=[v,k];                           else
      k1=k1+1,w=[w,k];
   end
end
ww=[v,w];
A=A(:,ww);U=U(:,ww);
rk=n-k0;
endfunction
