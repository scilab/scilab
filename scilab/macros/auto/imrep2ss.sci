function [sl]=imrep2ss(v,deg)
// Copyright INRIA
[lhs,rhs]=argn(0)
// hankel
[no,nv]=size(v);
n=nv/2;
ns1=no-1;n2=n-1;
l=1;
h=0*ones(n,n);
for k=1:n,h(l:l+ns1,:)=v(:,k:k+n2),l=l+no,end;
//factorization
if rhs==1 then [u,h1,v1,deg]=svd(h);else [u,h1,v1]=svd(h);end
//extraction
obs=u(:,1:deg);con=h1*v1';con=con(1:deg,:);
//shift
obstild=obs(no+1:n*no,:);obstild(n*no,deg)=0;
sl=syslin('d',obs'*obstild,con(:,1),obs(1:no,:))
endfunction
