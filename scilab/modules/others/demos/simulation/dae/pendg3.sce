// Copyright INRIA

//
// Sliding pendulum, curve #3
//

mode(1)

getf("SCI/demos/simulation/dae/pendg.sci")
getf("SCI/demos/simulation/dae/pendc3.sci")

k1=20; k2=18; k3=1/2;
g=10; l=1; m=1; M=1; k=0.5;
vx0=1;
vX0=vx0-2*k3*vx0*sin(k1*sqrt(1+4*k3^2*vx0^2))/sqrt(1+4*k3^2*vx0^2)/k2;
vY0=k3*vx0^2+sin(k1*sqrt(1+4*k3^2*vx0^2))/sqrt(1+4*k3^2*vx0^2)/k2;
x0=[vX0;vY0;0]; u0=[0;0;0];
y0=[x0;u0;0];
ud0=[0;-g;0]; yd0=[u0;ud0;0];

t0=0; T=t0:0.05:20;

info=list([],0,[],[],[],0,0);
atol=[0.001;0.001;0.001;0.001;0.001;0.001;0.01];
rtol=atol;
uux0=0; uuy0=0;

sol=dassl([y0,yd0],t0,T,rtol,atol,pendg,info);

x=sol(2,:); y=sol(3,:); teta=sol(4,:); n=size(x,"*");
r=0.05;
xp=x+l*sin(teta); yp=y-l*cos(teta);
xp1=x+(l-r)*sin(teta); yp1=y-(l-r)*cos(teta);

xbasc()
xset("pixmap",1)

xmin=-1.5; xmax=1.5; ymin=-1.1; ymax=0.9;
vx=[xmin:0.01:xmax]';
vX=vx-2*k3*vx.*sin(k1*sqrt(1+4*k3^2*vx.*vx))./sqrt(1+4*k3^2*vx.*vx)/k2;
vY=k3*vx.*vx+sin(k1*sqrt(1+4*k3^2*vx.*vx))./sqrt(1+4*k3^2*vx.*vx)/k2;
plot2d(vX,vY,5,"032"," ",[xmin,ymin,xmax,ymax])
xset("wshow")

xset("alufunction",6); driver("X11")
xset("thickness",2)
for i=1:n
  plot2d([x(i);xp1(i)],[y(i);yp1(i)],1,"000")
  xfarc(xp(i)-r,yp(i)+r,2*r,2*r,0,360*64)
  xset("wshow")
  xpause(30000)
  plot2d([x(i);xp1(i)],[y(i);yp1(i)],1,"000")
  xfarc(xp(i)-r,yp(i)+r,2*r,2*r,0,360*64)
  ifin=i;
end
xset("alufunction",1); driver("Rec")
plot2d([x(ifin);xp(ifin)],[y(ifin);yp(ifin)],1,"000")
xfarc(xp(ifin)-r,yp(ifin)+r,2*r,2*r,0,360*64)
xset("wshow")

xset("pixmap",0)
xset("default")
