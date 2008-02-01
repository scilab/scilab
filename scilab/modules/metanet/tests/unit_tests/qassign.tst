//==============================================================================
// Authors : Claude GOMEZ, Serge STEER
// Scilab team
// Copyright INRIA
// Date : 01 Feb, 2008
//==============================================================================

// unit tests for qassign function
// =============================================================================

n=15;
d=100*rand(15,15);
d=d-diag(diag(d));
c=zeros(n,n);f=c;
f(2:n,1)=ones(1:n-1)';
[crit,order]=qassign(c,f,d)


