//Copyright INRIA
Eps=1.e-3
deff("xcd=f(t,xc,xd,iflag)",...
["if iflag==0 then "
"  xcd=fc(t,xc,e(t)-hd(t,xd));"
"else "
"  xcd=fd(xd,hc(t,xc));"
"end"]);

A=[-10,2,3;4,-10,6;7,8,-10];B=[1;1;1];C=[1,1,1];
Ad=[1/2,1;0,1/20];Bd=[1;1];Cd=[1,1];
deff("st=e(t)","st=sin(3*t)")
deff("xdot=fc(t,x,u)","xdot=A*x+B*u")
deff("y=hc(t,x)","y=C*x")
deff("xp=fd(x,y)","xp=Ad*x + Bd*y")
deff("u=hd(t,x)","u=Cd*x")

h=0.1;t0=0;t=0:0.1:2;
x0c=[0;0;0];x0d=[0;0];nd=2;
xcd=odedc([x0c;x0d],nd,h,t0,t,f);
if norm(xcd-odedc([x0c;x0d],nd,h,t0,t,"fcd"),1)   > Eps then pause,end


//(see default directory)
if norm(xcd-odedc([x0c;x0d],nd,h,t0,t,"fcd1"),1)  > Eps then pause,end

deff("xcd=phis(t,xc,xd,iflag)",...
["if iflag==0 then "
"  xcd=A*xc+B*xd;"
"else "
"  xcd=1-xd;"
"end"]);

t=0:0.1:30;
xcd=odedc([x0c;0],1,1,t0,t,phis);

xcd2=odedc([x0c;0],1,1,t0,t,"phis");

if norm(xcd-xcd2,1)  > Eps then pause,end
deff("xd=ff(t,x)","xd=A*x+B*u")

u=1/2;xn=ode(x0c,t0,t,ff);
//plot2d([t',t',t',t'],[(xcd(1,:))',(xcd(2,:))',(xcd(3,:))',(xcd(4,:))'])

deff("xcd=phit(t,xc,xd,iflag)",...
["if iflag==0 then "
"  xcd=[A*xc(1:3,:)+B*xc(4);xd];"
"else "
"  xcd=-xd;"
"end"]);

xcdt=odedc([x0c;1;1],1,1,t0,t,phit);

xcdt2=odedc([x0c;1;1],1,1,t0,t,"phit");
if norm(xcdt-xcdt2,1) > Eps then pause,end
//plot2d([t',t',t',t'],[(xcdt(1,:))',(xcdt(2,:))',(xcdt(3,:))',(xcdt(4,:))'])

xcdt3=odedc("adams",[x0c;1;1],1,1,t0,t,"phit");
if norm(xcdt3-xcdt2,1)  > Eps then pause,end

xcdt4=odedc("fix",[x0c;1;1],1,1,t0,t,"phit");
if norm(xcdt4-xcdt2,1) > Eps then pause,end

xcdt5=odedc("stiff",[x0c;1;1],1,1,t0,t,"phit");
if norm(xcdt5-xcdt2,1) > Eps then pause,end


