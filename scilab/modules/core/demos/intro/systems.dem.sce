//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

//                  SYSTEM DEFINITION
s=poly(0,"s");
h=[1/s,1/(s+1);1/s/(s+1),1/(s+2)/(s+2)]
w=tf2ss(h);
ss2tf(w)
h1=clean(ans)

//             EXAMPLE: SECOND ORDER SYSTEM ANALYSIS
sl=syslin("c",1/(s*s+0.2*s+1))
instants=0:0.05:20;
//             step response:
y=csim("step",instants,sl);
my_handle = scf(100001);clf(my_handle,"reset");
plot2d(instants',y')
//             Delayed step response
deff("[in]=u(t)","if t<3 then in=0;else in=1;end");
y1=csim(u,instants,sl);plot2d(instants',y1');
clear u;
//             Impulse response;
yi=csim("imp",instants,sl);clf();plot2d(instants',yi');
yi1=csim("step",instants,s*sl);plot2d(instants',yi1');

//              Discretization
dt=0.05;
sld=dscr(tf2ss(sl),0.05);

//               Step response
u=ones(instants);
yyy=flts(u,sld);
my_handle = scf(100001);clf(my_handle,"reset");
plot(instants,yyy);
clear u;


//              Impulse response
u=0*ones(instants);u(1)=1/dt;
yy=flts(u,sld);
my_handle = scf(100001);clf(my_handle,"reset");
plot(instants,yy);
clear u;
//            system interconnection
w1=[w,w];
clean(ss2tf(w1))
w2=[w;w];
clean(ss2tf(w2))

//               change of variable
z=poly(0,"z");
horner(h,(1-z)/(1+z))  //bilinear transform
