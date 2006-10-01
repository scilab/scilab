// Copyright INRIA

// x=l(u)=u-2*k3*u*sin(k1*sqrt(1+4*k3^2*u^2))/sqrt(1+4*k3^2*u^2)/k2
// y=m(u)=k3*u^2+sin(k1*sqrt(1+4*k3^2*u^2))/sqrt(1+4*k3^2*u^2)/k2

// xx(u)=l(u)-x
function res=xx(u)
S=sqrt(1+4*k3^2*u^2);
res=u-2*k3*u*sin(k1*S)/S/k2-vvx;

function res=fx(x,y)
vvx=x; u=fsolve(uux0,xx,0.01);
S=sqrt(1+4*k3^2*u^2);
res=-S^3*k2/(-S^3*k2+2*k3*sin(k1*S)*S^2+8*k3^3*u^2*cos(k1*S)*k1*S-..
    8*k3^3*u^2*sin(k1*S));
uux0=res;

// yy(u)=m(u)-y
function res=yy(u)
S=sqrt(1+4*k3^2*u^2);
res=k3*u^2+sin(k1*S)/S/k2-vvy;

function res=fy(x,y)
vvy=y; u=fsolve(uuy0,yy,0.01);
S=sqrt(1+4*k3^2*u^2);
res=-S^3*k2/k3/u/(S^3*k2+2*cos(k1*S)*k1*k3*S-2*k3*sin(k1*S))/2;
uuy0=res;
