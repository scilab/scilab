// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(1)
rand("seed", 0)
rand("normal")
// Define the plant
nx=5;ny=1;nu=3;
Plant=ssrand(ny,nu,nx);
J=rand(Plant(5));Plant(5)=0*J;
[F,G,H,J]=Plant(2:5);

// define state space model of the Signal to track
nw=4;nuu=2;
A=rand(nw,nw);
A=A-max(real(spec(A)))*eye(A);
B=rand(nw,nuu);
C=2*rand(ny,nw);
D=0*rand(C*B);
xx0=0*ones(nw,1);
Model=syslin("c",A,B,C,D,xx0);

// Input to Model (t is a vector), nuu components
deff("[ut]=uu(t)","ut=[sin(3*t);cos(0.5*t)]");

// Compute Signal to track
dt=0.05;tmax=60;
instants=0:dt:tmax;
totrack=flts(uu(instants),dscr(Model,dt));    //Signal

my_handle             = scf(100001);
clf(my_handle,"reset");
plot2d(instants',totrack',axesflag=1);

halt(_("Press Return to continue ... \n"));

clf(my_handle,"reset");

//
[L,M,T]=gfrancis(Plant,Model);

//    Stabilizing the plant
K=-ppol(F,G,-0.3*ones(1,nx));

// Bigsyst= closed loop system: um --> [yplant;ymodel].
// full state gain is [K, L - K*T] * (xplant, xmodel) + M * umodel
BigA=[F+G*K,G*(L-K*T);
0*ones(nw,nx),A];
BigC=[H+J*K,J*(L-K*T);
0*ones(ny,nx),C];
BigB=[G*M;
B];
BigD=[J*M;
D];

x0=ones(nx,1);
BigX0=[x0;xx0];
Bigsyst=dscr(syslin("c",BigA,BigB,BigC,BigD,BigX0),dt);


z=flts(uu(instants),Bigsyst);
plot2d([instants',instants'],..
[totrack(1,:)',z(1,:)'], axesflag=1);
curves = gce();
captions(curves.children,["Signal to track","Computed signal"],"upper_caption");
xtitle("tracking");
