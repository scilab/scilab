// Copyright INRIA

mode(1)

getf("SCI/demos/simulation/dae/pendule3d.sci")

m=1; g=10; l=1;

theta0=0.2; phi0=%pi/4;
x0=l*[sin(phi0)*cos(theta0);..
      cos(phi0)*cos(theta0);sin(theta0)];

// index 2

u0=[1;-1;0];
lambda0=0; ud0=[0;0;-g];
y0=[x0;u0;lambda0];
yd0=[u0;ud0;0];
t0=0; T=t0:0.01:15;

info=list([],0,[],[],[],0,0);
atol=[0.0001;0.0001;0.0001;0.001;0.001;0.001;1];
rtol=atol;

y2=dassl([y0,yd0],t0,T,rtol,atol,index2,info);

x2=y2(2:4,:);

norm(x2(:,$),2)

xbasc()
param3d(x2(1,:),x2(2,:),x2(3,:))
