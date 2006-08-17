function [slb,u]=balreal(a)
// Copyright INRIA
[lhs,rhs]=argn(0)

if type(a)<>16 then error(91,1),end
ttp=a(1);
if ttp(1)<>'lss' then error(91,1),end
[a,b,c,d,x0,dom]=a(2:7);
if dom==[] then error(96,1),end
domain='c';
if dom<>'c' then domain='d',end
if lhs>2  then error('balreal: 1 or 2 output arguments: slb [,u]'),end
wc=lyap(a',-b*b',domain)
wo=lyap(a,-c'*c,domain)
r=chol(wo);x=r*wc*r';[u,s,v]=svd(x);s=diag(s);
ns=size(s,'*');
lli=sqrt(sqrt(s));ll=ones(ns,1)./lli
ri=inv(r)*v;r=v'*r;
a=r*a*ri;b=r*b;c=c*ri
a=diag(ll)*a*diag(lli)
b=diag(ll)*b
c=c*diag(lli)
slb=syslin(dom,a,b,c,d,diag(ll)*r*x0),
if lhs==2 then u=ri*diag(lli),end
endfunction
