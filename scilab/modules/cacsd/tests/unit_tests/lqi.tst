//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
A=[0 1 0;0 0 1;1 0 0];
B=[0.3 1;0 1;-0.3 0.9];
C=[1.9 1.3 1;0 1 1];
D=[0.53 -0.61;0 0];

Q_xx=0.1*eye(3,3);
R_uu=[1 0;0 2];
Q_i=eye(2,2);
Q_xu=sysdiag(0.1*eye(3,3),[1 0;0 2]);
Q_zz=sysdiag(Q_xx,Q_i);

// continuous time----------------------------------------
P=syslin("c",A,B,C,D);
[ny,nu,nx]=size(P);

Kc=lqi(P,Q_zz,R_uu);
//add states to the output to compute the regulated system
Px=syslin("c",A,B,[eye(3,3);C],[zeros(3,2);D]);
I=tf2ss(syslin("c",1/%s));

Pa=[zeros(ny,nx) eye(ny,ny)
    eye(nx,nx)   zeros(nx,ny)
    zeros(ny,nx) eye(ny,ny)]*Px*[-Kc(:,nx+1:nx+ny)*I -Kc(:,1:nx) Kc(:,nx+1:nx+ny)*I];
H=lft(Pa,eye(5,5));

p=spec(H.A);
assert_checktrue(and(real(p)<10*%eps));


assert_checktrue(size(find(abs(p)<10*%eps),"*")==2);
