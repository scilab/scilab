function [Sl]=markp2ss(markpar,n,nout,nin)
// Given a set of n (matrix) Markov parameters stacked in the (row)-matrix 
// markpar of size nout x (n*nin) markp2ss
// returns a state-space linear system Sl (syslin list) such that with:
// A=sl(2); B=sl(3); C=sl(4);
// C*B = markpar(1:nout,1:nin),
// C*A*B =marpar(1:nout,nin+1:2*nin),....
// See also: pol2des
//!
// Copyright INRIA
nmax=maxi(size(markpar));
H=[];H(n*nout,n*nin)=0;
p=markpar;
l1=1:nout;k1=1;
H(l1,:)=p;kset=(nmax-nin+1):nmax;zeroing=0*ones(nout,nin);
for k=2:n;
    l1=l1+nout*ones(l1);
    k1=k1+nin;
    p=markpar(:,k1:nmax);p(nout,nmax)=0;
    H(l1,:)=p;
end;
[u,s,v,deg]=svd(H);
obs=u(:,1:deg);con=s*v(1:deg,:)';
obstild=obs(nout+1:n*nout,:);obstild(n*nout,deg)=0;
Sl=syslin('c',obs'*obstild,con(1:deg,1:nin),obs(1:nout,:))
endfunction
