// Copyright INRIA

mode(1)

getf("SCI/demos/simulation/dae/pendule3d.sci")

m=1; g=10; l=1;

theta0=0.2; phi0=%pi/4;
x0=l*[sin(phi0)*cos(theta0);..
      cos(phi0)*cos(theta0);sin(theta0)];

// index 1

u0=[1;-1;0];
deff("r=ff(l)","r=ggpp(x0,u0,l)");
lambda0=fsolve(0,ff);
ud0=-2*lambda0/m*x0-[0;0;g];
y0=[x0;u0;lambda0];
yd0=[u0;ud0;0];
t0=0; T=t0:0.01:15;

info=list([],0,[],[],[],0,0);
atol=[0.0001;0.0001;0.0001;0.001;0.001;0.001;1];
rtol=atol;

y1=dassl([y0,yd0],t0,T,rtol,atol,index1,info);

x1=y1(2:4,:);

norm(x1(:,$),2)

xbasc()
param3d(x1(1,:),x1(2,:),x1(3,:))
