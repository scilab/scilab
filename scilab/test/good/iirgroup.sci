function [lt,grad]=iirgroup(p,r,theta,omega,wt,td)
//    p===> Lp
//    r=magnitude of poles and zeros of filters
//theta=phase   "    "   "    "    "
//omega=frequencies corresponding to filters specs.
//wt=weighting function for group delay
//!
//
// Copyright INRIA
[m,n]=size(td);if m>n,td=td';end;
[m,n]=size(omega);if m>n,omega=omega';end;
[m,n]=size(r);if n>m,r=r';m=n;end;
[m,n]=size(theta);if n>m,theta=theta';m=n;end;
m=m/2;
//
//VARIABLES LOCALES
unr=ones(r);unom=ones(omega);un=unr(1:m,:).*.unom;
n1=r.*.unom;n2=unr.*.omega;n3=theta.*.unom;n4=(unr+r.*r).*.unom;
cw=cos(omega);sw=sin(omega);ct=cos(theta);st=sin(theta);
c=cw.*.ct;s=sw.*.st;cos1=c+s;cos2=c-s;
c=sw.*.ct;s=cw.*.st;sin1=c-s;sin2=c+s;
//
//FORMES QUADRATIQUES
k1=n4-2*n1.*cos1;k2=n4-2*n1*cos2;
//
//RETARD DE GROUPE
grp=[];k3=k1(1:m,:);k4=k1(m+1:2*m,:);k5=k2(1:m,:);k6=k2(m+1:2*m,:);
c1=cos1(1:m,:);c2=cos1(m+1:2*m,:);c3=cos2(1:m,:);c4=cos2(m+1:2*m,:);
r1=n1(1:m,:);r2=n1(m+1:2*m,:);
t=(un-r2.*c2)./k4+(un-r2.*c4)./k6-(un-r1.*c1)./k3-(un-r1.*c3)./k5;
for k=t,grp=[grp sum(k)];end;
//
//GRADIENT DU RETARD DE GROUPE
k1=k1.*k1;k2=k2.*k2;
grrgrp=(n4.*cos1-2*n1)./k1+(n4.*cos2-2*n1)./k2;
grrgrp(1:m,:)=-grrgrp(1:m,:);
n5=n1.*(-n4+2*unr.*.unom);
grtgrp=(n5.*sin1)./k1-(n5.*sin2)./k2;
grtgrp(1:m,:)=-grtgrp(1:m,:);
//
//CRITERE D'ERREUR EN LE RETARD DE GROUPE ET SON GRADIENT
t=grp-td;t1=t^(2*p);t1=t1.*wt;lt=sum(t1);
t1=(t1./a)*2*p;
grad=[grrgrp*t1' grtgrp*t1'];
endfunction
