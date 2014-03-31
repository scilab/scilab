// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(-1)
exec SCI/modules/differential_equations/demos/levitron/levitron_gui.sci;
exec SCI/modules/differential_equations/demos/levitron/levitron.sci;


//Notations
//---------
//M:  The magnetic moment of the top
//Me: the net strength of the ring dipole
//R : the effective radius of the ring dipole (used as a normalization factor)
//A : transverse moment of inertia of the top
//C : polar  moment of inertia of the top
//m : the mass of the top
// x,y,z:    the coordinates of the center of mass of the top
//
// X=x/R, Y=y/R, Z=z/R, normalized coordinates
// a=A/(m*R^2)    normalized transverse moment of inertia of the top
// c=C/(m*R^2)    normalized polar  moment of inertia of the top
// Mc=(M*Me)/(4*%pi*m*g*R^4),   normalized magnetic moment of the top

//R=34.7d-3
//m=0.02135
//A=1.32d-6
//C=2.20d-6
//http://www.koepken.de/levitron/maglev.java

a=0.089;
c=0.139;
Mc=8.2;
//
%h=poly(0,"h");
Ph=(3*Mc*%h*(2*%h^2-3))^2-(1+%h^2)^7;
rPh=roots(Ph);
rPh=real(rPh(find(abs(imag(rPh))<0.00001& real(rPh)>0)));
mx=max(rPh);
mn=min(rPh);

h=1.74;//1.72;
Theta=0.005;
dTheta=0;
Psi=0;
dPsi=1.2;
Phi=0;
Omega_T=7.5;
dPhi=Omega_T-dPsi*cos(Theta);


q0=[0;    //X
0;    //Y
h;    //Z
Theta;
Phi;
Psi];
p0=[0;    //X'
0;    //Y'
0;    //Z'
a*dTheta;
dPsi*(a*sin(Theta)^2+c*cos(Theta)^2)+c*dPhi*cos(Theta)
c*(dPhi+dPsi*cos(Theta))]

demo_viewCode("levitron.dem.sce");
levitron_create_gui()
H=build_levitron(h);
y=[q0;p0];y0=y;y1=y;init=%t;state_changed=%f;
global y0 y1 state_changed init  Stop
update_height(1.72);
update_theta(0.28);
update_phi(0);
update_psi(0);
set_levitron(H,y);

